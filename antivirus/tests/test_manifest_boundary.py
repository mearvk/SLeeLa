import importlib.util
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
            import json
            path.write_text(json.dumps(data), encoding="utf-8")
            self.assertEqual(manifest.verify(root, path), 2)


if __name__ == "__main__":
    unittest.main()
