"""Command-line contract of the `sleela` compiler/runtime.

Covers: version/syntax gating (SL-META-0001 section 4.4), the syntax 3.1
fixtures, subcommand error handling, the fail-closed SHA-256 execution gate,
and the persistent artifact format (`compile`, `validate-artifact`, `run`).
Fixtures are the ones committed under impl/tests/; they are copied into scratch
directories because the compiler writes `docs/keyword-supporters/` next to the
files it checks.
"""
import os
import random
import shutil
import unittest
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import regression_support as rs

TESTS = rs.ROOT / "impl" / "tests"
WORKERS = max(2, min(8, os.cpu_count() or 2))


def parallel(fn, items):
    with ThreadPoolExecutor(max_workers=WORKERS) as pool:
        return list(pool.map(fn, items))


class NeedsCompiler(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if rs.sleela_binary() is None:
            raise unittest.SkipTest("no sleela binary")


def run_fixture(directory: Path, name: str, mode: str = "run"):
    return rs.run_sleela([mode, name], cwd=directory)


class VersionGating(NeedsCompiler):
    """impl/tests/version/run_version_tests.sh, as a table with exit-code assertions."""
    ACCEPTED = ["syntax_1_0", "syntax_1_2", "syntax_1_3", "syntax_1_4", "syntax_3_0", "syntax_3_1", "syntax_3_01",
                "syntax_missing", "material_2_5", "memory_2_5", "wide_integers", "int_double_widening",
                "determine_2_6", "pure_3_0", "gasoline_3_0"]
    REJECTED = ["syntax_0_9", "too_new", "minor_ahead", "material_too_early", "memory_too_early",
                "determine_too_early", "pure_too_early", "gasoline_too_early", "malformed"]

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.dir = rs.fresh_dir("version")
        for f in (TESTS / "version").glob("*.sleela"):
            shutil.copy(f, cls.dir / f.name)

    def check_all(self, names):
        results = parallel(lambda n: (n, run_fixture(self.dir, n + ".sleela")), names)
        return dict(results)

    def test_supported_editions_and_markers_are_accepted(self):
        for name, cp in self.check_all(self.ACCEPTED).items():
            self.assertEqual(cp.returncode, 0, f"{name} must be accepted:\n{rs.diagnostics(cp)}")

    def test_out_of_range_and_premature_features_are_rejected_with_a_reason(self):
        for name, cp in self.check_all(self.REJECTED).items():
            self.assertNotEqual(cp.returncode, 0, f"{name} must be rejected")
            self.assertTrue(rs.diagnostics(cp).strip(), f"{name} rejected without a diagnostic")
            self.assertEqual(rs.program_output(cp), [], f"{name} must not execute any code")

    def test_range_diagnostics_name_the_bounds(self):
        low = rs.diagnostics(run_fixture(self.dir, "syntax_0_9.sleela"))
        high = rs.diagnostics(run_fixture(self.dir, "minor_ahead.sleela"))
        self.assertIn("1.0", low)
        self.assertIn("3.1", high)

    def test_version_command_states_the_supported_range(self):
        cp = rs.run_sleela(["version"], cwd=self.dir)
        self.assertEqual(cp.returncode, 0)
        banner = cp.stdout
        self.assertIn("SLeeLa", banner)
        self.assertIn("supported source syntax 1.0", banner)
        self.assertIn("3.1", banner)

    def test_unknown_pragma_edition_does_not_change_the_result_of_check(self):
        cp = run_fixture(self.dir, "syntax_1_3.sleela", "check")
        self.assertEqual(cp.returncode, 0)
        self.assertIn("syntax_1_3.sleela: ok", cp.stdout)
        self.assertIn("1.3", cp.stdout)


class Syntax31Fixtures(NeedsCompiler):
    """impl/tests/syntax31: char literals, array initialisers, multi-dimensional
    arrays, labelled statements, finally on abrupt completion, and the 3.0
    gates that must reject them."""
    REJECT = {
        "gate_length_in_3_0": ("check", "the array length member requires #sleela 3.1"),
        "gate_nd_in_3_0": ("check", "multi-dimensional array creation requires #sleela 3.1"),
        "bad_nd_sized_after_unsized": ("check", "array dimension expression after an empty"),
        "bad_nd_negative": ("run", "negative array size"),
        "bad_length_assign": ("check", "the array length is read-only"),
        "bad_length_incr": ("check", "the array length is read-only"),
        "gate_char_in_3_0": ("check", "character literal requires #sleela 3.1"),
        "gate_label_in_3_0": ("check", "labeled statement requires #sleela 3.1"),
        "gate_arrayinit_in_3_0": ("check", "array initializer requires #sleela 3.1"),
        "bad_label": ("check", "break to undefined label 'nowhere'"),
    }

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.src = TESTS / "syntax31"

    def fixture_dir(self, label):
        d = rs.fresh_dir(label)
        for f in self.src.glob("*.sleela"):
            shutil.copy(f, d / f.name)
        return d

    def test_every_fixture_is_covered_by_this_table_or_has_expected_output(self):
        names = {p.stem for p in self.src.glob("*.sleela")}
        covered = set(self.REJECT) | {p.name[: -len(".expected.txt")] for p in self.src.glob("*.expected.txt")}
        self.assertEqual(names - covered, set(), "new syntax31 fixture without an expectation")

    def test_programs_produce_their_expected_output(self):
        names = sorted(p.name[: -len(".expected.txt")] for p in self.src.glob("*.expected.txt"))
        self.assertGreaterEqual(len(names), 5)

        def one(name):
            d = self.fixture_dir("s31-" + name)
            cp = run_fixture(d, name + ".sleela")
            return name, cp, (self.src / f"{name}.expected.txt").read_text().splitlines()
        for name, cp, want in parallel(one, names):
            self.assertEqual(cp.returncode, 0, f"{name}: {rs.diagnostics(cp)}")
            self.assertEqual(rs.program_output(cp), want, name)

    def test_gates_and_malformed_programs_are_rejected_with_the_documented_message(self):
        def one(item):
            name, (mode, message) = item
            d = self.fixture_dir("s31g-" + name)
            return name, message, run_fixture(d, name + ".sleela", mode)
        for name, message, cp in parallel(one, sorted(self.REJECT.items())):
            self.assertNotEqual(cp.returncode, 0, f"{name} must be rejected")
            self.assertIn(message, cp.stdout + cp.stderr, name)


class CommandLineErrors(NeedsCompiler):
    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.dir = rs.fresh_dir("cli")
        rs.write_program(cls.dir, "Ok.sleela", '#sleela 3.1\nclass Ok { void main() { print("hi"); } }')
        rs.write_program(cls.dir, "Syn.sleela", '#sleela 3.1\nclass Syn { void main() { print("hi" } }')
        rs.write_program(cls.dir, "Sem.sleela", "#sleela 3.1\nclass Sem { void main() { print(undefinedVar); } }")
        rs.write_program(cls.dir, "NoMain.sleela", "#sleela 3.1\nclass NoMain { void f() { } }")
        rs.write_program(cls.dir, "Side.sleela", '#sleela 3.1\nclass Side { void main() { print("SIDE-EFFECT"); } }')

    def test_no_arguments_and_unknown_subcommand_are_usage_errors(self):
        self.assertEqual(rs.run_sleela([], cwd=self.dir).returncode, 2)
        self.assertEqual(rs.run_sleela(["frobnicate"], cwd=self.dir).returncode, 2)

    def test_missing_input_file_is_reported(self):
        for mode in ("run", "check"):
            cp = rs.run_sleela([mode, "does-not-exist.sleela"], cwd=self.dir)
            self.assertEqual(cp.returncode, 1)
            self.assertIn("cannot open", cp.stderr)

    def test_syntax_error_reports_position(self):
        for mode in ("run", "check"):
            cp = rs.run_sleela([mode, "Syn.sleela"], cwd=self.dir)
            self.assertEqual(cp.returncode, 1)
            self.assertRegex(rs.diagnostics(cp), r"Syn\.sleela: Syntax error \(line 2, col \d+\): expected '\)'")
            self.assertEqual(rs.program_output(cp), [])

    def test_semantic_errors_and_missing_main_are_rejected_before_running(self):
        cp = rs.run_sleela(["run", "Sem.sleela"], cwd=self.dir)
        self.assertEqual(cp.returncode, 1)
        self.assertIn("use of undeclared variable 'undefinedVar'", rs.diagnostics(cp))
        cp = rs.run_sleela(["run", "NoMain.sleela"], cwd=self.dir)
        self.assertEqual(cp.returncode, 1)
        self.assertIn("no 'main' method found", rs.diagnostics(cp))

    def test_check_validates_without_executing(self):
        cp = rs.run_sleela(["check", "Side.sleela"], cwd=self.dir)
        self.assertEqual(cp.returncode, 0)
        self.assertIn("Side.sleela: ok", cp.stdout)
        self.assertNotIn("SIDE-EFFECT", cp.stdout)
        self.assertEqual(rs.run_sleela(["check", "Sem.sleela"], cwd=self.dir).returncode, 1)

    def test_execution_is_deterministic(self):
        runs = [rs.program_output(rs.run_sleela(["run", "Side.sleela"], cwd=self.dir)) for _ in range(3)]
        self.assertEqual(runs[0], ["SIDE-EFFECT"])
        self.assertEqual(runs[0], runs[1])
        self.assertEqual(runs[1], runs[2])


class ExecutionGate(NeedsCompiler):
    """The compiler must refuse to build, run or check anything unless the
    trusted SHA-256 manifest verifies (fail closed)."""

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.dir = rs.fresh_dir("gate")
        rs.write_program(cls.dir, "G.sleela", '#sleela 3.1\nclass G { void main() { print("RAN"); } }')
        import json
        good = json.loads(rs.fast_manifest().read_text())
        bad = json.loads(json.dumps(good))
        bad["files"][0]["sha256"] = "0" * 64
        missing = json.loads(json.dumps(good))
        missing["files"].append({"path": "no/such/file.txt", "sha256": "0" * 64})
        outside = json.loads(json.dumps(good))
        outside["files"].append({"path": "../../../../etc/passwd", "sha256": "0" * 64})
        cls.manifests = {}
        for label, doc in (("tampered", bad), ("missing-file", missing), ("outside-root", outside),
                           ("empty", {"algorithm": "sha256", "files": []}),
                           ("wrong-algorithm", {"algorithm": "md5", "files": good["files"]})):
            p = cls.dir / f"{label}.json"
            p.write_text(json.dumps(doc))
            cls.manifests[label] = p
        (cls.dir / "garbage.json").write_text("{not json")
        cls.manifests["garbage"] = cls.dir / "garbage.json"
        cls.manifests["absent"] = cls.dir / "absent.json"

    def test_bad_manifests_block_run_check_and_compile(self):
        for label, manifest in self.manifests.items():
            env = rs.cli_env(manifest)
            for args in (["run", "G.sleela"], ["check", "G.sleela"], ["compile", "G.sleela", "-o", "G.out"]):
                cp = rs.run_sleela(args, cwd=self.dir, env=env)
                self.assertNotEqual(cp.returncode, 0, f"{label}: {args[0]} must fail closed")
                self.assertNotIn("RAN", cp.stdout, f"{label}: program must not run")
                self.assertFalse((self.dir / "G.out").exists(), f"{label}: no artifact may be produced")

    def test_tampered_manifest_names_the_offending_file(self):
        cp = rs.run_sleela(["run", "G.sleela"], cwd=self.dir, env=rs.cli_env(self.manifests["tampered"]))
        self.assertIn("SHA-256 mismatch: SHEET.sheet", cp.stdout + cp.stderr)

    def test_valid_manifest_allows_execution(self):
        cp = rs.run_sleela(["run", "G.sleela"], cwd=self.dir)
        self.assertEqual(cp.returncode, 0)
        self.assertEqual(rs.program_output(cp), ["RAN"])

    def test_version_does_not_require_the_gate(self):
        cp = rs.run_sleela(["version"], cwd=self.dir, env=rs.cli_env(self.manifests["tampered"]))
        self.assertEqual(cp.returncode, 0)


class Artifacts(NeedsCompiler):
    SOURCE = '#sleela 1.3\nclass Art { void main() { print("ART-OK"); } }\n'

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.dir = rs.fresh_dir("artifact")
        rs.write_program(cls.dir, "art.sleela", cls.SOURCE)
        cp = rs.run_sleela(["compile", "art.sleela", "-o", "art.out"], cwd=cls.dir)
        assert cp.returncode == 0, cp.stderr
        cls.blob = (cls.dir / "art.out").read_bytes()

    def validate(self, data: bytes, name: str = "probe.out"):
        p = self.dir / name
        p.write_bytes(data)
        return rs.run_sleela(["validate-artifact", name], cwd=self.dir)

    def test_artifact_has_the_documented_header(self):
        self.assertTrue(self.blob.startswith(b"SLEELA-ART\x01"))
        self.assertEqual(self.blob[11:15], b"\x06\x00\x00\x00")      # artifact format 6
        self.assertEqual(self.blob[15:19], b"\x04\x03\x02\x01")      # endianness marker
        self.assertIn(b"ART-OK", self.blob)

    def test_valid_artifact_validates_and_runs_like_the_source(self):
        cp = rs.run_sleela(["validate-artifact", "art.out"], cwd=self.dir)
        self.assertEqual(cp.returncode, 0)
        self.assertIn("artifact ABI ok", cp.stdout)
        self.assertIn("format 6", cp.stdout)
        from_artifact = rs.program_output(rs.run_sleela(["run", "art.out"], cwd=self.dir))
        from_source = rs.program_output(rs.run_sleela(["run", "art.sleela"], cwd=self.dir))
        self.assertEqual(from_artifact, ["ART-OK"])
        self.assertEqual(from_artifact, from_source)

    def test_compilation_is_deterministic(self):
        again = rs.run_sleela(["compile", "art.sleela", "-o", "art2.out"], cwd=self.dir)
        self.assertEqual(again.returncode, 0)
        self.assertEqual((self.dir / "art2.out").read_bytes(), self.blob)

    def test_every_truncation_is_rejected(self):
        def one(n):
            return n, self.validate(self.blob[:n], f"trunc-{n}.out").returncode
        bad = [n for n, rc in parallel(one, range(len(self.blob))) if rc == 0]
        self.assertEqual(bad, [], f"truncated artifacts accepted at lengths {bad[:10]}")

    def test_every_single_byte_corruption_of_the_header_is_rejected(self):
        header = 53        # magic, version, format, endianness, section table (see the header test above)

        def one(i):
            m = bytearray(self.blob); m[i] ^= 0xFF
            return i, self.validate(bytes(m), f"hdr-{i}.out").returncode
        accepted = [i for i, rc in parallel(one, range(header)) if rc == 0]
        self.assertEqual(accepted, [], f"corrupted header bytes accepted: {accepted}")

    def test_arbitrary_corruption_never_crashes_the_validator(self):
        rng = random.Random(0xA27)
        cases = []
        for _ in range(300):
            m = bytearray(self.blob)
            for _ in range(rng.randint(1, 4)):
                m[rng.randrange(len(m))] = rng.randrange(256)
            cases.append(bytes(m))
        cases += [b"", b"\x00", b"SLEELA-ART", b"\xff" * 300, self.blob * 2, self.blob + b"\x00" * 64]

        def one(item):
            idx, data = item
            return self.validate(data, f"fz-{idx}.out").returncode
        codes = parallel(one, list(enumerate(cases)))
        self.assertTrue(all(c in (0, 1) for c in codes), f"validator crashed or hung: {sorted(set(codes))}")

    def test_corrupt_artifact_does_not_execute(self):
        m = bytearray(self.blob)
        m[0] ^= 0xFF
        (self.dir / "bad.out").write_bytes(bytes(m))
        cp = rs.run_sleela(["run", "bad.out"], cwd=self.dir)
        self.assertNotEqual(cp.returncode, 0)
        self.assertNotIn("ART-OK", cp.stdout)

    def test_compile_failure_leaves_no_artifact(self):
        rs.write_program(self.dir, "broken.sleela", "#sleela 3.1\nclass B { void main() { print( } }")
        cp = rs.run_sleela(["compile", "broken.sleela", "-o", "broken.out"], cwd=self.dir)
        self.assertNotEqual(cp.returncode, 0)
        self.assertFalse((self.dir / "broken.out").exists())

    def test_trailing_bytes_are_ignored(self):
        """CHARACTERIZATION: validate-artifact checks the declared structure and
        does not reject bytes appended after it."""
        self.assertEqual(self.validate(self.blob + b"\x00").returncode, 0)


if __name__ == "__main__":
    unittest.main()
