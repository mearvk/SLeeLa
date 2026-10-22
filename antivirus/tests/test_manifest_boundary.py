import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "manifest_boundary_test", ROOT / "tools" / "sleelavirin_manifest.py"
)
manifest = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(manifest)


class ManifestBoundaryTests(unittest.TestCase):
    def test_parent_path_is_rejected(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp) / "root"
            root.mkdir()
            outside = Path(temp) / "outside.txt"
            outside.write_text("fixture", encoding="utf-8")
            data = {
                "schema": manifest.SCHEMA,
                "files": [{
                    "path": "../outside.txt",
                    "size": outside.stat().st_size,
                    "sha256": manifest.sha256(outside),
                }],
            }
            path = root / "manifest.json"
            path.write_text(json.dumps(data), encoding="utf-8")
            self.assertEqual(manifest.verify(root, path), 2)

    def test_manifest_builder_omits_symlinks(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp) / "root"
            root.mkdir()
            target = Path(temp) / "target.txt"
            target.write_text("fixture", encoding="utf-8")
            link = root / "link.txt"
            try:
                link.symlink_to(target)
            except (OSError, NotImplementedError):
                self.skipTest("symlink creation is unavailable")
            data = manifest.build_manifest(root, root / "manifest.json")
            self.assertNotIn("link.txt", {item["path"] for item in data["files"]})

    def test_manifest_verifier_rejects_symlink_record(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp) / "root"
            root.mkdir()
            target = Path(temp) / "target.txt"
            target.write_text("fixture", encoding="utf-8")
            link = root / "link.txt"
            try:
                link.symlink_to(target)
            except (OSError, NotImplementedError):
                self.skipTest("symlink creation is unavailable")
            data = {
                "schema": manifest.SCHEMA,
                "files": [{
                    "path": "link.txt",
                    "size": target.stat().st_size,
                    "sha256": manifest.sha256(target),
                }],
            }
            path = root / "manifest.json"
            path.write_text(json.dumps(data), encoding="utf-8")
            self.assertEqual(manifest.verify(root, path), 2)


if __name__ == "__main__":
    unittest.main()
