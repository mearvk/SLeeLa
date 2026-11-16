import importlib.util
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("fruchst.py")
SPEC = importlib.util.spec_from_file_location("fruchst", MODULE_PATH)
fruchst = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(fruchst)


def sample_data():
    return {
        "language_tag": "fr",
        "source_id": "unit-test-fixture",
        "entries": [
            {
                "rank": i,
                "frequency": 1001 - i,
                "word": f"mot-{i:04d}" if i != 1 else "été",
                "definitions": {
                    "en": f"English definition {i}",
                    "fr": f"Définition française {i}",
                    "es": f"Definición española {i}"
                }
            }
            for i in range(1, 1001)
        ]
    }


class FruchstTests(unittest.TestCase):
    def test_round_trip_validation_and_unicode(self):
        blob = fruchst.build(sample_data())
        result = fruchst.validate(blob)
        self.assertTrue(result["valid"])
        self.assertEqual(result["entry_count"], 1000)
        self.assertEqual(result["language_tag"], "fr")

    def test_reproducible_output(self):
        data = sample_data()
        self.assertEqual(fruchst.build(data), fruchst.build(data))

    def test_rejects_corruption(self):
        blob = bytearray(fruchst.build(sample_data()))
        blob[-1] ^= 0x01
        with self.assertRaises(fruchst.FruchstError):
            fruchst.validate(bytes(blob))

    def test_rejects_wrong_entry_count(self):
        data = sample_data()
        data["entries"].pop()
        with self.assertRaises(fruchst.FruchstError):
            fruchst.build(data)

    def test_rejects_duplicate_normalized_words(self):
        data = sample_data()
        data["entries"][1]["word"] = data["entries"][0]["word"]
        with self.assertRaises(fruchst.FruchstError):
            fruchst.build(data)

    def test_rejects_missing_translation(self):
        data = sample_data()
        del data["entries"][0]["definitions"]["es"]
        with self.assertRaises(fruchst.FruchstError):
            fruchst.build(data)


if __name__ == "__main__":
    unittest.main()
