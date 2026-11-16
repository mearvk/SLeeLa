import importlib.util
import json
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS = Path(__file__).parent
sys.path.insert(0, str(TOOLS))
MODULE_PATH = TOOLS / "merge_wikipedia_frequency.py"
SPEC = importlib.util.spec_from_file_location("merge_wikipedia_frequency", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)


def valid_document(tag="arz"):
    return {
        "schema": "SLeeLa FRUCHST Wikipedia frequency candidate staging v1",
        "language_tag": tag,
        "language_name": "Egyptian Arabic",
        "candidate_count": 1000,
        "definitions_status": "pending",
        "frequency_review_status": "review_required",
        "source": {
            "wiki_code": "arz",
            "dump_sha256": "a" * 64,
        },
        "entries": [
            {
                "candidate_rank": rank,
                "word": f"word{rank}",
                "frequency_count": 1001 - rank,
            }
            for rank in range(1, 1001)
        ],
    }


class WikipediaFrequencyMergeTests(unittest.TestCase):
    def test_validates_contiguous_unique_nfc_candidates(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "wikipedia-arz.json"
            path.write_text(json.dumps(valid_document()), encoding="utf-8")
            loaded = module.validate_document(path, "arz")
            self.assertEqual(loaded["candidate_count"], 1000)

    def test_rejects_duplicate_headwords(self):
        document = valid_document()
        document["entries"][1]["word"] = document["entries"][0]["word"]
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "wikipedia-arz.json"
            path.write_text(json.dumps(document), encoding="utf-8")
            with self.assertRaises(ValueError):
                module.validate_document(path, "arz")

    def test_aggregates_candidates_and_marks_license_review_pending(self):
        original_map = module.WIKIPEDIA_SOURCE_MAP
        try:
            module.WIKIPEDIA_SOURCE_MAP = {
                "arz": {"wiki_code": "arz", "language_name": "Egyptian Arabic"}
            }
            with tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                input_dir = root / "input"
                output_dir = root / "frequency-candidates"
                input_dir.mkdir()
                (input_dir / "wikipedia-arz.json").write_text(
                    json.dumps(valid_document()), encoding="utf-8"
                )
                registry_path = root / "registry.json"
                registry_path.write_text(json.dumps({
                    "target_count": 1,
                    "profiles": [{
                        "language_tag": "arz",
                        "directory": "egyptian-arabic",
                        "status": "pending_source_review",
                        "entries": 0,
                    }],
                }), encoding="utf-8")

                self.assertEqual(
                    module.aggregate(input_dir, output_dir, registry_path),
                    ["arz"],
                )
                registry = json.loads(registry_path.read_text(encoding="utf-8"))
                profile = registry["profiles"][0]
                self.assertEqual(profile["status"], "frequency_candidates_staged")
                self.assertEqual(profile["entries"], 1000)
                self.assertIn("license review required", profile["license_review"].lower())
                self.assertTrue((output_dir / "wikipedia-arz.json").is_file())
        finally:
            module.WIKIPEDIA_SOURCE_MAP = original_map


if __name__ == "__main__":
    unittest.main()
