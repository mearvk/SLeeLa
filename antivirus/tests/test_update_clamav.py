import importlib.util
from pathlib import Path
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "sleelavirin_update_clamav_test", ROOT / "tools" / "sleelavirin_update_clamav.py"
)
updater = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(updater)


class ClamAVSnapshotTests(unittest.TestCase):
    def test_manifest_verifies_exact_database_bytes(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "daily.cvd").write_bytes(b"test database bytes")
            manifest = updater.build_manifest(root)
            self.assertEqual(updater.verify_manifest(root, manifest), [])
            (root / "daily.cvd").write_bytes(b"changed database bytes")
            self.assertTrue(any("mismatch" in error for error in updater.verify_manifest(root, manifest)))

    def test_manifest_detects_missing_or_extra_database(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "main.cld").write_bytes(b"main")
            manifest = updater.build_manifest(root)
            (root / "daily.cvd").write_bytes(b"extra")
            self.assertTrue(updater.verify_manifest(root, manifest))

    def test_snapshot_id_is_deterministic_for_same_database_bytes(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "daily.cvd").write_bytes(b"stable")
            first = updater.build_manifest(root)
            second = updater.build_manifest(root)
            self.assertEqual(first["snapshot_id"], second["snapshot_id"])


if __name__ == "__main__":
    unittest.main()
