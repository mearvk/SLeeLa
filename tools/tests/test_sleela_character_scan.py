#!/usr/bin/env python3
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "sleela-character-scan.py"
SPEC = importlib.util.spec_from_file_location("sleela_character_scan", SCRIPT)
scanner = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(scanner)


class CharacterScanTests(unittest.TestCase):
    def test_hybrid_manifest_and_capacity(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "SLEELA-CHARSET.conf").write_text("mode=hybrid\ncapacity=256\n", encoding="utf-8")
            (root / "generator.cpp").write_text("int make_character() { return 0; }\n", encoding="utf-8")
            (root / "characters.json").write_text(json.dumps({"entries": [{"id": 1}, {"id": 2}]}), encoding="utf-8")
            result = scanner.scan(root)
            self.assertEqual(result["mode"], "hybrid")
            self.assertEqual(result["signature"], "C256")
            self.assertEqual(result["literal_record_count_estimate"], 2)
            self.assertFalse(result["executed_source"])

    def test_auto_detects_procedural(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "generator.cpp").write_text("// generator\n", encoding="utf-8")
            self.assertEqual(scanner.scan(root)["mode"], "procedural")

    def test_rejects_capacity_overflow(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "SLEELA-CHARSET.conf").write_text("capacity=1048577\n", encoding="utf-8")
            with self.assertRaises(ValueError):
                scanner.scan(root)


if __name__ == "__main__":
    unittest.main()
