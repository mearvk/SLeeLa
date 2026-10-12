#!/usr/bin/env python3
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "sleela-character-scan.py"
SPEC = importlib.util.spec_from_file_location("sleela_character_scan", SCRIPT)
scanner = importlib.util.module_from_spec(SPEC)
assert SPEC and SPEC.loader
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

    def test_scanner_never_executes_procedural_source(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            marker = root / "executed.txt"
            (root / "SLEELA-CHARSET.conf").write_text(
                "mode=hybrid\ncapacity=8\ngenerator=generator.py\n", encoding="utf-8"
            )
            (root / "generator.py").write_text(
                "from pathlib import Path\nPath(__file__).with_name('executed.txt').write_text('yes')\n",
                encoding="utf-8",
            )
            report = scanner.scan(root)
            self.assertFalse(report["executed_source"])
            self.assertFalse(marker.exists())

    def test_auto_detects_procedural(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "generator.cpp").write_text("// generator\n", encoding="utf-8")
            self.assertEqual(scanner.scan(root)["mode"], "procedural")

    def test_rejects_noncanonical_and_out_of_range_capacity(self):
        for value in ("0", "01", "-1", "1048577", "not-a-number"):
            with self.subTest(value=value), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                (root / "SLEELA-CHARSET.conf").write_text(
                    f"capacity={value}\n", encoding="utf-8"
                )
                with self.assertRaises(ValueError):
                    scanner.scan(root)

    def test_default_capacity_is_explicit_but_not_verified_population(self):
        with tempfile.TemporaryDirectory() as temp:
            result = scanner.scan(Path(temp))
            self.assertEqual(result["capacity"], 1048576)
            self.assertEqual(result["signature"], "C1048576")
            self.assertFalse(result["population_is_verified"])


if __name__ == "__main__":
    unittest.main()
