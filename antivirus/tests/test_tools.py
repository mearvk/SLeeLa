import importlib.util
import io
import os
from pathlib import Path
import tempfile
import unittest
from unittest import mock
from contextlib import redirect_stdout, redirect_stderr

ROOT = Path(__file__).resolve().parents[1]
def load_module(name, path):
    spec = importlib.util.spec_from_file_location(name, ROOT / "tools" / path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

scan = load_module("sleelavirin_scan_test", "sleelavirin_scan.py")
importer = load_module("sleelavirin_import_test", "sleelavirin_import_clamav.py")


class ScanCoordinatorTests(unittest.TestCase):
    def test_size_parser(self):
        self.assertEqual(scan.size_arg("2k"), 2048)
        self.assertEqual(scan.size_arg("3M"), 3 * 1024 * 1024)
        with self.assertRaises(Exception):
            scan.size_arg("-1")

    def test_depth_exclusions_and_extensions(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "top.exe").write_text("top")
            (root / "ignore.cache").write_text("skip")
            (root / "one").mkdir()
            (root / "one" / "first.exe").write_text("first")
            (root / "one" / "two").mkdir()
            (root / "one" / "two" / "deep.exe").write_text("deep")
            paths, incomplete = scan.enumerate_files(
                [str(root)], 1, False, ["*.cache"], {".exe"}, 100
            )
            names = {p.name for p in paths}
            self.assertFalse(incomplete)
            self.assertIn("top.exe", names)
            self.assertIn("first.exe", names)
            self.assertNotIn("deep.exe", names)
            self.assertNotIn("ignore.cache", names)

    def test_missing_path_is_incomplete(self):
        with tempfile.TemporaryDirectory() as temp:
            paths, incomplete = scan.enumerate_files(
                [str(Path(temp) / "missing")], 8, False, [], set(), 100
            )
            self.assertEqual(paths, [])
            self.assertTrue(incomplete)

    @mock.patch.object(scan.subprocess, "run")
    def test_native_detection_and_exit_normalization(self, run):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "sample"
            path.write_text("sample")
            run.return_value = mock.Mock(returncode=1, stdout=f"DETECTED {path} : fixture", stderr="")
            verdict, _, _ = scan.native_scan("native", "db", path, scan.file_stamp(path))
            self.assertEqual(verdict, "DETECTED")

    @mock.patch.object(scan.subprocess, "run")
    def test_changed_file_is_error_not_clean(self, run):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "sample"
            path.write_text("before")
            stamp = scan.file_stamp(path)
            def mutate(*args, **kwargs):
                path.write_text("changed with different size")
                return mock.Mock(returncode=0, stdout="NO_MATCH", stderr="")
            run.side_effect = mutate
            verdict, _, message = scan.native_scan("native", "db", path, stamp)
            self.assertEqual(verdict, "ERROR")
            self.assertIn("CHANGED_DURING_SCAN", message)

    @mock.patch.object(scan.subprocess, "run")
    def test_clamav_limits_and_no_match(self, run):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "sample"
            path.write_text("sample")
            run.return_value = mock.Mock(returncode=0, stdout="", stderr="")
            verdict, _, _ = scan.clamav_scan_one(
                "clamscan", path, 1024, 4096, 4, scan.file_stamp(path)
            )
            self.assertEqual(verdict, "NO_MATCH")
            command = run.call_args.args[0]
            self.assertIn("--max-filesize=1024", command)
            self.assertIn("--max-scansize=4096", command)
            self.assertIn("--max-recursion=4", command)

    def test_quarantine_collision_and_changed_file_refusal(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            scanned = root / "scanned"
            scanned.mkdir()
            path = scanned / "sample.bin"
            path.write_text("detected")
            dest = root / "quarantine"
            stamp = scan.file_stamp(path)
            dest.mkdir()
            (dest / path.name).write_text("existing")
            target = scan.safe_quarantine(path, dest, [str(scanned)], stamp)
            self.assertTrue(target.exists())
            self.assertNotEqual(target.name, path.name)
            second = scanned / "second.bin"
            second.write_text("old")
            old_stamp = scan.file_stamp(second)
            second.write_text("changed")
            with self.assertRaises(ValueError):
                scan.safe_quarantine(second, dest, [str(scanned)], old_stamp)

    def test_quarantine_inside_scan_root_refused(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = root / "sample"
            path.write_text("sample")
            with self.assertRaises(ValueError):
                scan.safe_quarantine(path, root / "quarantine", [str(root)], scan.file_stamp(path))


class DefinitionImporterTests(unittest.TestCase):
    def test_import_deduplicates_and_skips_incompatible_records(self):
        digest = "a" * 64
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "source"
            source.mkdir()
            (source / "one.hsb").write_text(f"{digest}:12:Example.One\nnot-a-signature\n")
            (source / "two.hsb").write_text(f"{digest.upper()}:12:Example.Two\n")
            entries, skipped, errors = importer.collect_signatures(source)
            self.assertFalse(errors)
            self.assertEqual(len(entries), 1)
            self.assertEqual(len(entries[digest]), 2)
            self.assertEqual(skipped, 1)

    def test_failed_import_preserves_existing_index(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "source"
            source.mkdir()
            (source / "bad.hsb").write_text("no compatible records\n")
            output = root / "active.txt"
            output.write_text("# previous valid index\n" + "b" * 64 + " old\n")
            old = output.read_text()
            stderr = io.StringIO()
            with mock.patch("sys.argv", ["importer", "--source", str(source), "--output", str(output)]):
                with redirect_stderr(stderr):
                    result = importer.main()
            self.assertEqual(result, 2)
            self.assertEqual(output.read_text(), old)
            self.assertIn("not changed", stderr.getvalue())


    def test_successful_import_writes_provenance_manifest(self):
        digest = "c" * 64
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "source"
            source.mkdir()
            (source / "daily.hsb").write_text(f"{digest}:7:Example.Signature\n")
            output = root / "native.txt"
            with mock.patch("sys.argv", [
                "importer", "--source", str(source), "--output", str(output),
                "--source-version", "daily-test-123", "--source-url", "internal://clamav/daily-test-123"
            ]):
                result = importer.main()
            self.assertEqual(result, 0)
            manifest = Path(str(output) + ".manifest.json")
            self.assertTrue(manifest.is_file())
            import json
            data = json.loads(manifest.read_text())
            self.assertEqual(data["source_version"], "daily-test-123")
            self.assertEqual(data["signature_count"], 1)
            self.assertEqual(data["index_sha256"], __import__("hashlib").sha256(output.read_bytes()).hexdigest())

    def test_manifest_cannot_overwrite_active_index(self):
        digest = "d" * 64
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "source"
            source.mkdir()
            (source / "daily.hsb").write_text(f"{digest}:7:Example.Signature\n")
            output = root / "native.txt"
            output.write_text("# keep previous index\n" + "e" * 64 + " old\n")
            previous = output.read_text()
            errors = io.StringIO()
            with mock.patch("sys.argv", [
                "importer", "--source", str(source), "--output", str(output), "--manifest", str(output)
            ]):
                with redirect_stderr(errors):
                    result = importer.main()
            self.assertEqual(result, 2)
            self.assertEqual(output.read_text(), previous)
            self.assertIn("must differ", errors.getvalue())


if __name__ == "__main__":
    unittest.main()
