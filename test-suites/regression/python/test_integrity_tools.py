"""Regression tests for the SHA-256 execution gate tooling and for repository
invariants that other tooling depends on.

  tools/verify-before-execution.py   fail-closed verifier (exit 0 pass, 1 fail, 2 unreadable)
  tools/generate-sha256-manifest.py  manifest generator
  security/sha256-manifest.json      the committed trust root
  lib/LIBRARY.SYMBOLS.md / test-suites/test-library-inventory.sh   /lib inventory

The verifier is the only thing standing between a tampered source file and
`sleela run`, so its rejection paths are exercised exhaustively.
"""
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

import regression_support as rs

VERIFY = rs.ROOT / "tools" / "verify-before-execution.py"
GENERATE = rs.ROOT / "tools" / "generate-sha256-manifest.py"
COMMITTED = rs.ROOT / "security" / "sha256-manifest.json"
EMPTY_SHA256 = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"


def verify(manifest: Path, root: Path):
    return subprocess.run([sys.executable, str(VERIFY), "--manifest", str(manifest), "--root", str(root)],
                          capture_output=True, text=True, timeout=300)


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


class VerifierRejectionPaths(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="verify-", dir=str(rs.BUILD) if rs.BUILD.exists() else None)
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name) / "root"
        (self.root / "sub").mkdir(parents=True)
        (self.root / "a.txt").write_bytes(b"alpha\n")
        (self.root / "sub" / "b.bin").write_bytes(bytes(range(256)) * 4)
        (self.root / "empty").write_bytes(b"")
        self.manifest = Path(self.tmp.name) / "m.json"

    def write_manifest(self, files, **extra):
        doc = {"algorithm": "sha256", "files": files}
        doc.update(extra)
        self.manifest.write_text(json.dumps(doc))

    def good_entries(self):
        return [{"path": "a.txt", "sha256": sha(b"alpha\n")},
                {"path": "sub/b.bin", "sha256": sha(bytes(range(256)) * 4)},
                {"path": "empty", "sha256": EMPTY_SHA256}]

    def test_correct_manifest_passes(self):
        self.write_manifest(self.good_entries())
        r = verify(self.manifest, self.root)
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertIn("PASS (3 item(s))", r.stdout)

    def test_hex_digest_case_is_irrelevant(self):
        e = self.good_entries()
        e[0]["sha256"] = e[0]["sha256"].upper()
        self.write_manifest(e)
        self.assertEqual(verify(self.manifest, self.root).returncode, 0)

    def test_algorithm_defaults_to_sha256_when_omitted(self):
        self.manifest.write_text(json.dumps({"files": self.good_entries()}))
        self.assertEqual(verify(self.manifest, self.root).returncode, 0)

    def test_a_one_byte_change_is_detected_and_named(self):
        self.write_manifest(self.good_entries())
        (self.root / "a.txt").write_bytes(b"alphb\n")
        r = verify(self.manifest, self.root)
        self.assertEqual(r.returncode, 1)
        self.assertIn("SHA-256 mismatch: a.txt", r.stderr)
        self.assertIn("expected: " + sha(b"alpha\n"), r.stderr)
        self.assertIn("actual:   " + sha(b"alphb\n"), r.stderr)
        self.assertIn("FAILED (1 item(s))", r.stderr)

    def test_appended_and_truncated_content_is_detected(self):
        self.write_manifest(self.good_entries())
        data = (self.root / "sub" / "b.bin").read_bytes()
        for mutated in (data + b"\0", data[:-1], b"", data[:10] + bytes([data[10] ^ 1]) + data[11:]):
            (self.root / "sub" / "b.bin").write_bytes(mutated)
            self.assertEqual(verify(self.manifest, self.root).returncode, 1)
        (self.root / "sub" / "b.bin").write_bytes(data)
        self.assertEqual(verify(self.manifest, self.root).returncode, 0)

    def test_every_failing_entry_is_counted(self):
        self.write_manifest(self.good_entries())
        (self.root / "a.txt").write_bytes(b"x")
        (self.root / "empty").write_bytes(b"x")
        r = verify(self.manifest, self.root)
        self.assertEqual(r.returncode, 1)
        self.assertIn("FAILED (2 item(s))", r.stderr)

    def test_missing_file_fails(self):
        self.write_manifest(self.good_entries() + [{"path": "gone.txt", "sha256": EMPTY_SHA256}])
        r = verify(self.manifest, self.root)
        self.assertEqual(r.returncode, 1)
        self.assertIn("missing file: gone.txt", r.stderr)

    def test_a_directory_is_not_a_file(self):
        self.write_manifest([{"path": "sub", "sha256": EMPTY_SHA256}])
        self.assertEqual(verify(self.manifest, self.root).returncode, 1)

    def test_paths_that_escape_the_root_are_rejected(self):
        outside = Path(self.tmp.name) / "outside.txt"
        outside.write_bytes(b"secret")
        (self.root / "link").symlink_to(outside)
        for path in ("../outside.txt", "sub/../../outside.txt", str(outside), "link"):
            self.write_manifest([{"path": path, "sha256": sha(b"secret")}])      # digest is correct: still refused
            r = verify(self.manifest, self.root)
            self.assertEqual(r.returncode, 1, path)
            self.assertIn("rejected path outside root", r.stderr, path)

    def test_a_symlink_that_stays_inside_the_root_is_followed(self):
        (self.root / "alias").symlink_to(self.root / "a.txt")
        self.write_manifest([{"path": "alias", "sha256": sha(b"alpha\n")}])
        self.assertEqual(verify(self.manifest, self.root).returncode, 0)

    def test_unreadable_or_invalid_manifests_exit_2(self):
        r = verify(Path(self.tmp.name) / "absent.json", self.root)
        self.assertEqual(r.returncode, 2)
        self.manifest.write_text("{not json")
        self.assertEqual(verify(self.manifest, self.root).returncode, 2)
        self.write_manifest([])
        self.assertEqual(verify(self.manifest, self.root).returncode, 2)
        self.manifest.write_text(json.dumps({"algorithm": "sha256"}))
        self.assertEqual(verify(self.manifest, self.root).returncode, 2)
        self.write_manifest(self.good_entries(), algorithm="md5")
        self.assertEqual(verify(self.manifest, self.root).returncode, 2)
        self.manifest.write_text(json.dumps({"files": "a.txt"}))
        self.assertEqual(verify(self.manifest, self.root).returncode, 2)

    def test_malformed_entries_fail_rather_than_being_skipped(self):
        for entry in ("a.txt", 5, None, {"path": "a.txt"}, {"sha256": EMPTY_SHA256}, {"path": 3, "sha256": EMPTY_SHA256},
                      {"path": "a.txt", "sha256": ""}):
            self.write_manifest(self.good_entries() + [entry])
            r = verify(self.manifest, self.root)
            self.assertEqual(r.returncode, 1, repr(entry))

    def test_large_files_are_hashed_completely(self):
        big = (b"0123456789abcdef" * 1024) * 3 * 1024 + b"tail"                  # 48 MiB + 4 spans several read chunks
        (self.root / "big.bin").write_bytes(big)
        self.write_manifest([{"path": "big.bin", "sha256": sha(big)}])
        self.assertEqual(verify(self.manifest, self.root).returncode, 0)
        (self.root / "big.bin").write_bytes(big[:-1] + b"X")
        self.assertEqual(verify(self.manifest, self.root).returncode, 1)


class ManifestGenerator(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = rs.fresh_dir("gen")
        cls.out = cls.dir / "m.json"
        r = subprocess.run([sys.executable, str(GENERATE), "--root", str(rs.ROOT), "--output", str(cls.out)],
                           capture_output=True, text=True, timeout=300)
        assert r.returncode == 0, r.stderr
        cls.doc = json.loads(cls.out.read_text())
        cls.paths = [f["path"] for f in cls.doc["files"]]

    def test_format(self):
        self.assertEqual(self.doc["algorithm"], "sha256")
        self.assertGreater(len(self.paths), 1000)
        self.assertEqual(len(self.paths), len(set(self.paths)), "duplicate entries")
        for f in self.doc["files"]:
            self.assertRegex(f["sha256"], r"^[0-9a-f]{64}$")
            self.assertFalse(f["path"].startswith("/") or ".." in Path(f["path"]).parts, f["path"])
            self.assertNotIn("\\", f["path"])

    def test_generation_is_deterministic(self):
        again = self.dir / "again.json"
        subprocess.run([sys.executable, str(GENERATE), "--root", str(rs.ROOT), "--output", str(again)],
                       check=True, capture_output=True, timeout=300)
        self.assertEqual(again.read_bytes(), self.out.read_bytes())

    def test_stdout_mode_matches_file_mode(self):
        r = subprocess.run([sys.executable, str(GENERATE), "--root", str(rs.ROOT), "--stdout"],
                           capture_output=True, text=True, timeout=300)
        self.assertEqual(r.returncode, 0)
        self.assertEqual(json.loads(r.stdout), self.doc)

    def test_trusted_computing_base_is_covered(self):
        must = ["impl/core/sleela_core.c", "impl/core/sleela_utf16.c", "impl/frontend/lexer.cpp",
                "impl/frontend/compiler.cpp", "SHEET.sheet", "tools/sleela-native-drive.py"]
        for m in must:
            self.assertIn(m, self.paths)
        self.assertTrue(any(p.startswith("lib/") and p.endswith(".sleela") for p in self.paths))
        self.assertEqual([p for p in self.paths if p.startswith("bash/")],
                         ["bash/pixel_terminal.cpp", "bash/pixel_terminal.hpp"], "the vendored bash tree is out; only its 2 SLeeLa files are in")
        self.assertFalse(any(p.startswith((".git/", "impl/build/")) for p in self.paths))
        # user programs are deliberately not covered
        self.assertFalse(any(p.startswith(("examples/", "tutorial/")) for p in self.paths))

    def test_every_entry_matches_its_file_and_the_gate_accepts_the_manifest(self):
        r = verify(self.out, rs.ROOT)
        self.assertEqual(r.returncode, 0, r.stderr[-800:])
        self.assertIn(f"PASS ({len(self.paths)} item(s))", r.stdout)

    def test_generator_on_a_miniature_tree(self):
        root = self.dir / "mini"
        for rel, data in {"impl/core/x.c": b"int x;\n", "impl/frontend/y.cpp": b"int y;\n", "lib/A/B.sleela": b"class B {}\n",
                          "SHEET.sheet": b"sheet\n", "bash/vendored.c": b"nope\n", "impl/core/build/gen.c": b"gen\n", "lib/A/.build/x.sleela": b"class X {}\n",
                          "impl/core/notes.txt": b"ignored\n"}.items():
            p = root / rel
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_bytes(data)
        out = self.dir / "mini.json"
        r = subprocess.run([sys.executable, str(GENERATE), "--root", str(root), "--output", str(out)],
                           capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 0, r.stderr)
        paths = {f["path"]: f["sha256"] for f in json.loads(out.read_text())["files"]}
        self.assertEqual(paths["impl/core/x.c"], sha(b"int x;\n"))
        self.assertIn("lib/A/B.sleela", paths)
        self.assertIn("SHEET.sheet", paths)
        self.assertNotIn("bash/vendored.c", paths)
        self.assertNotIn("impl/core/build/gen.c", paths)          # generated output is never trusted source
        self.assertNotIn("lib/A/.build/x.sleela", paths)
        self.assertNotIn("impl/core/notes.txt", paths)
        self.assertEqual(verify(out, root).returncode, 0)
        (root / "lib/A/B.sleela").write_bytes(b"class B { }\n")            # edit after generation
        self.assertEqual(verify(out, root).returncode, 1)


class CommittedTrustRoot(unittest.TestCase):
    def test_committed_manifest_matches_the_committed_tree(self):
        """security/sha256-manifest.json must describe the tree it ships with.
        If this fails, `impl/Makefile verify-security` and every prebuilt
        `working-binaries/*` invocation refuse to run on a fresh clone.
        Fix: python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json"""
        self.assertTrue(COMMITTED.is_file())
        r = verify(COMMITTED, rs.ROOT)
        mismatched = re.findall(r"SHA-256 mismatch: (\S+)", r.stderr)
        missing = re.findall(r"missing file: (\S+)", r.stderr)
        self.assertEqual(r.returncode, 0,
                         f"{len(mismatched)} file(s) differ from the committed manifest, {len(missing)} missing:\n  "
                         + "\n  ".join((mismatched + missing)[:20]))

    def test_committed_manifest_is_well_formed_and_covers_the_gate_itself(self):
        doc = json.loads(COMMITTED.read_text())
        self.assertEqual(doc["algorithm"], "sha256")
        paths = [f["path"] for f in doc["files"]]
        self.assertEqual(len(paths), len(set(paths)))
        for must in ("impl/core/sleela_core.c", "impl/frontend/driver.cpp", "SHEET.sheet"):
            self.assertIn(must, paths)
        for f in doc["files"]:
            self.assertRegex(f["sha256"], r"^[0-9a-f]{64}$")

    def test_every_manifest_entry_exists(self):
        doc = json.loads(COMMITTED.read_text())
        missing = [f["path"] for f in doc["files"] if not (rs.ROOT / f["path"]).is_file()]
        self.assertEqual(missing, [])


class LibraryInventory(unittest.TestCase):
    """/lib is counted four independent ways; they must agree."""

    @classmethod
    def setUpClass(cls):
        lib = rs.ROOT / "lib"
        packages, sources, facades = set(), 0, 0
        for dirpath, _dirs, files in os.walk(lib):
            for name in files:
                if not name.endswith(".sleela"):
                    continue
                rel = Path(dirpath, name).relative_to(lib)
                packages.add(rel.parts[0])
                if name == "SLPackage.sleela":
                    facades += 1
                else:
                    sources += 1
        cls.fs = {"packages": len(packages), "sources": sources, "facades": facades, "symbols": sources + facades}

    def test_generator_counts_equal_the_filesystem(self):
        r = subprocess.run([sys.executable, str(rs.ROOT / "tools" / "generate-library-symbols.py"), "--print-counts"],
                           capture_output=True, text=True, timeout=120)
        self.assertEqual(r.returncode, 0)
        got = dict((k, int(v)) for k, v in re.findall(r"(\w+)=(\d+)", r.stdout))
        self.assertEqual(got, self.fs)

    def test_hardcoded_expectations_in_the_inventory_script_are_current(self):
        text = (rs.ROOT / "test-suites" / "test-library-inventory.sh").read_text()
        want = {k.lower(): int(v) for k, v in re.findall(r"^EXPECTED_(\w+)=(\d+)", text, re.M)}
        self.assertEqual(want, self.fs, "update EXPECTED_* in test-suites/test-library-inventory.sh")

    def test_symbols_manifest_header_and_rows_match(self):
        text = (rs.ROOT / "lib" / "LIBRARY.SYMBOLS.md").read_text()
        rows = [ln.split("\t") for ln in text.splitlines() if ln.count("\t") == 3 and ln.split("\t")[2].startswith("lib/")]
        self.assertEqual(len(rows), self.fs["symbols"])
        self.assertEqual(sum(1 for r in rows if r[3] == "facade"), self.fs["facades"])
        self.assertEqual(len({r[2] for r in rows}), len(rows), "duplicate paths in LIBRARY.SYMBOLS.md")
        for r in rows[:: max(1, len(rows) // 400)]:                                  # spot-check 400 paths exist
            self.assertTrue((rs.ROOT / r[2]).is_file(), r[2])
        self.assertIn(f"library-source-files: {self.fs['sources']}", text)

    def test_index_headline_revision_states_the_current_counts(self):
        text = (rs.ROOT / "lib" / "LIBRARY.INDEX.md").read_text()
        first = next(ln for ln in text.splitlines() if ln.startswith("> Revision"))
        numbers = [int(n) for n in re.findall(r"\*\*(\d+)\*\*", first)]
        self.assertIn(self.fs["sources"], numbers, first[:200])
        self.assertIn(self.fs["symbols"], numbers, first[:200])


class RepositoryToolingHygiene(unittest.TestCase):
    def test_every_python_tool_compiles(self):
        bad = []
        for p in sorted((rs.ROOT / "tools").rglob("*.py")):
            try:
                compile(p.read_text(encoding="utf-8"), str(p), "exec")
            except (SyntaxError, UnicodeDecodeError) as e:
                bad.append(f"{p.relative_to(rs.ROOT)}: {e}")
        self.assertEqual(bad, [])


class CompilerSelection(unittest.TestCase):
    """Regression (was HARNESS-RELATIVE-SLEELA-BIN): a relative $SLEELA_BIN was
    returned as given, and the CLI suites run the compiler from scratch
    directories, so nearly every CLI test failed with FileNotFoundError. A
    missing $SLEELA_BIN silently fell back to a different compiler."""

    def setUp(self):
        self.saved = os.environ.get("SLEELA_BIN")
        self.cwd = os.getcwd()

    def tearDown(self):
        os.chdir(self.cwd)
        if self.saved is None:
            os.environ.pop("SLEELA_BIN", None)
        else:
            os.environ["SLEELA_BIN"] = self.saved

    def test_relative_path_is_resolved_to_an_absolute_one(self):
        with tempfile.TemporaryDirectory() as tmp:
            exe = Path(tmp) / "bin" / "fake-sleela"
            exe.parent.mkdir()
            exe.write_text("#!/bin/sh\nexit 0\n")
            exe.chmod(0o755)
            os.chdir(tmp)
            os.environ["SLEELA_BIN"] = "bin/fake-sleela"
            got = rs.sleela_binary()
            self.assertTrue(got.is_absolute(), got)
            self.assertEqual(got, exe.resolve())

    def test_explicit_path_that_is_not_an_executable_is_an_error(self):
        with tempfile.TemporaryDirectory() as tmp:
            not_exec = Path(tmp) / "plain.txt"
            not_exec.write_text("x")
            not_exec.chmod(0o644)
            for value in (str(Path(tmp) / "missing"), str(not_exec), tmp):
                os.environ["SLEELA_BIN"] = value
                with self.assertRaises(rs.CompilerSelectionError, msg=value):
                    rs.sleela_binary()

    def test_runner_refuses_to_start_with_a_bad_sleela_bin(self):
        env = dict(os.environ, SLEELA_BIN="/nonexistent/sleela")
        r = subprocess.run([sys.executable, str(rs.HERE / "run_regression.py"), "--only", "integrity"],
                           capture_output=True, text=True, env=env, timeout=120)
        self.assertEqual(r.returncode, 2, r.stdout + r.stderr)
        self.assertIn("SLEELA_BIN", r.stderr)


if __name__ == "__main__":
    unittest.main()
