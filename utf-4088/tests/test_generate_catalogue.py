import tempfile
import unittest
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))

from generate_catalogue import (
    HEADER, RECORD, CAPACITY, MAX_BYTES, generate, input_states,
    make_glyph, pack_input,
)


class CatalogueTests(unittest.TestCase):
    def test_header_and_record_sizes(self):
        self.assertEqual(HEADER.size, 28)
        self.assertEqual(RECORD.size, 16)
        self.assertEqual(HEADER.size + CAPACITY * RECORD.size, 16_777_244)
        self.assertLess(HEADER.size + CAPACITY * RECORD.size, MAX_BYTES)

    def test_input_packing_and_bounds(self):
        packed = pack_input(7, 11, 50, 255)
        self.assertEqual(packed, 7 | (11 << 3) | (49 << 7) | (255 << 13))
        with self.assertRaises(ValueError):
            pack_input(8, 0, 1, 0)
        with self.assertRaises(ValueError):
            pack_input(0, 0, 51, 0)

    def test_traversal_count_and_stability(self):
        states = list(input_states(4))
        self.assertEqual(states, [(0, 0, 1, 0), (0, 0, 1, 1),
                                  (0, 0, 1, 2), (0, 0, 1, 3)])
        self.assertEqual(make_glyph(pack_input(*states[0]), 0),
                         make_glyph(pack_input(*states[0]), 0))

    def test_small_catalogue_round_trip_metadata(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "small.bin"
            summary = Path(temp) / "summary.json"
            result = generate(path, summary, 1000, MAX_BYTES)
            self.assertEqual(result["entry_count"], 1000)
            self.assertEqual(result["file_bytes"], 28 + 1000 * 16)
            self.assertEqual(result["unique_glyph_count"], 1000)
            self.assertTrue(result["under_size_limit"])
            self.assertEqual(len(result["sha256"]), 64)
            self.assertTrue(summary.exists())

    def test_size_limit_rejected_before_writing(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "too-large.bin"
            with self.assertRaises(ValueError):
                generate(path, None, 100, 100)
            self.assertFalse(path.exists())


if __name__ == "__main__":
    unittest.main()
