import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

MODULE = Path(__file__).parents[1] / "tools" / "sleelavirin_manifest.py"
SPEC = importlib.util.spec_from_file_location("sleelavirin_manifest", MODULE)
manifest = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(manifest)


class ManifestTests(unittest.TestCase):
    def test_write_and_verify(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "signatures").mkdir()
            (root / "tools").mkdir()
            (root / "README.md").write_text("documentation\n", encoding="utf-8")
            (root / "signatures" / "native-sha256.txt").write_text("# protected index\n", encoding="utf-8")
            output = root / "INTEGRITY-MANIFEST.json"
            data = manifest.build_manifest(root, output)
            output.write_text(json.dumps(data), encoding="utf-8")
            self.assertEqual(manifest.verify(root, output), 0)

    def test_tamper_detected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            sample = root / "protected-index.txt"
            sample.write_text("trusted\n", encoding="utf-8")
            output = root / "INTEGRITY-MANIFEST.json"
            output.write_text(json.dumps(manifest.build_manifest(root, output)), encoding="utf-8")
            sample.write_text("changed\n", encoding="utf-8")
            self.assertEqual(manifest.verify(root, output), 2)

    def test_unlisted_file_detected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "known.txt").write_text("known\n", encoding="utf-8")
            output = root / "INTEGRITY-MANIFEST.json"
            output.write_text(json.dumps(manifest.build_manifest(root, output)), encoding="utf-8")
            (root / "unexpected.txt").write_text("unexpected\n", encoding="utf-8")
            self.assertEqual(manifest.verify(root, output), 2)


if __name__ == "__main__":
    unittest.main()
