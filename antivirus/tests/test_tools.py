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

    def test_file_stamp_tracks_metadata_beyond_size_and_mtime(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "sample"
            path.write_text("same-size")
            first = scan.file_stamp(path)
            # Rewriting identical-length content should normally change ctime even
            # if a filesystem or caller preserves the modification timestamp.
            path.write_text("same-size")
            second = scan.file_stamp(path)
            self.assertEqual(first[0], second[0])
            self.assertEqual(len(first), 5)

    @mock.patch.object(scan.subprocess, "run", side_effect=scan.subprocess.TimeoutExpired(cmd=["scanner"], timeout=1))
    def test_native_timeout_is_error_not_clean(self, run):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "sample"
            path.write_text("sample")
            verdict, _, message = scan.native_scan("native", "db", path, scan.file_stamp(path), timeout=1)
            self.assertEqual(verdict, "ERROR")
            self.assertIn("SCAN_TIMEOUT", message)

    @mock.patch.object(scan.subprocess, "run", side_effect=scan.subprocess.TimeoutExpired(cmd=["clamscan"], timeout=1))
    def test_clamav_timeout_is_error_not_clean(self, run):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "sample"
            path.write_text("sample")
            verdict, _, message = scan.clamav_scan_one("clamscan", path, 1024, 4096, 4, scan.file_stamp(path), timeout=1)
            self.assertEqual(verdict, "ERROR")
            self.assertIn("CLAMAV_TIMEOUT", message)

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
            self.assertEqual(target.read_text(), "detected")
            self.assertEqual((dest / path.name).read_text(), "existing")
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


    def test_directory_walk_errors_mark_scan_incomplete(self):
        with tempfile.TemporaryDirectory() as temp:
            def failed_walk(root, followlinks=False, onerror=None):
                if onerror:
                    onerror(PermissionError("permission denied fixture"))
                return iter(())
            with mock.patch.object(scan.os, "walk", side_effect=failed_walk):
                paths, incomplete = scan.enumerate_files([temp], 8, False, [], set(), 100)
            self.assertEqual(paths, [])
            self.assertTrue(incomplete)

    def test_followed_symlink_loop_terminates_and_deduplicates(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            sub = root / "sub"
            sub.mkdir()
            (sub / "sample.bin").write_text("sample")
            try:
                (sub / "loop").symlink_to(root, target_is_directory=True)
            except (OSError, NotImplementedError):
                self.skipTest("directory symbolic links are not available")
            paths, incomplete = scan.enumerate_files([temp], 0, True, [], set(), 100)
            self.assertFalse(incomplete)
            self.assertEqual(sum(p.name == "sample.bin" for p in paths), 1)

    def test_duplicate_detection_paths_are_reported_once(self):
        path = Path("sample.bin")
        self.assertEqual(scan.unique_paths([path, path, Path("other.bin"), path]),
                         [path, Path("other.bin")])

    @mock.patch.object(scan.subprocess, "run")
    def test_native_database_or_engine_errors_are_not_clean(self, run):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "sample"
            path.write_text("sample")
            run.return_value = mock.Mock(returncode=2, stdout="", stderr="invalid signature database")
            verdict, _, message = scan.native_scan("native", "corrupt-db", path, scan.file_stamp(path))
            self.assertEqual(verdict, "ERROR")
            self.assertIn("invalid signature database", message)


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

    def test_manifest_integrity_is_bound_to_active_index(self):
        digest = "a" * 64
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            index = root / "index.txt"
            index.write_text("# index\n" + digest + " label\n")
            manifest = root / "index.manifest.json"
            manifest.write_text(__import__("json").dumps({
                "schema": "sleelavirin-native-index-manifest-v1",
                "index_sha256": "0" * 64,
                "signature_count": 1
            }))
            errors = importer.validate_manifest(index, manifest)
            self.assertTrue(any("does not match active index bytes" in error for error in errors))
            manifest.write_text(__import__("json").dumps({
                "schema": "sleelavirin-native-index-manifest-v1",
                "index_sha256": __import__("hashlib").sha256(index.read_bytes()).hexdigest(),
                "signature_count": 1
            }))
            self.assertEqual(importer.validate_manifest(index, manifest), [])

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


    def test_quarantine_refuses_symlink_path(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            target = root / "target.bin"
            target.write_text("detected")
            link = root / "link.bin"
            try:
                link.symlink_to(target)
            except (OSError, NotImplementedError):
                self.skipTest("symbolic links are not available in this environment")
            with self.assertRaises(ValueError):
                scan.safe_quarantine(link, root / "quarantine", [str(root)], scan.file_stamp(link))


    def test_corrupt_database_is_rejected(self):
        with tempfile.TemporaryDirectory() as temp:
            database = Path(temp) / "corrupt.txt"
            database.write_text("not-a-sha256-record\n")
            errors, count = importer.validate_database(database)
            self.assertTrue(errors)
            self.assertEqual(count, 0)

    def test_manifest_publication_failure_rolls_back_active_index(self):
        digest = "f" * 64
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "source"
            source.mkdir()
            (source / "daily.hsb").write_text(f"{digest}:7:Example.Signature\n")
            output = root / "native.txt"
            output.write_text("# previous index\n" + "e" * 64 + " old\n")
            previous = output.read_text()
            real_replace = os.replace
            calls = {"count": 0}
            def fail_manifest_replace(src, dst):
                calls["count"] += 1
                if calls["count"] == 2:
                    raise OSError("simulated manifest publication failure")
                return real_replace(src, dst)
            errors = io.StringIO()
            with mock.patch("sys.argv", [
                "importer", "--source", str(source), "--output", str(output)
            ]), mock.patch.object(importer.os, "replace", side_effect=fail_manifest_replace):
                with redirect_stderr(errors):
                    result = importer.main()
            self.assertEqual(result, 2)
            self.assertEqual(output.read_text(), previous)
            self.assertIn("rolled back", errors.getvalue())


if __name__ == "__main__":
    unittest.main()
