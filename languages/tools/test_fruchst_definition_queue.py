import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("fruchst_definition_queue.py")
SPEC = importlib.util.spec_from_file_location("fruchst_definition_queue", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)


class DefinitionQueueTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.candidate = self.root / "candidates.json"
        doc = {
            "language_tag": "en",
            "candidate_count": 1000,
            "source": {"dataset": "fixture"},
            "entries": [
                {"candidate_rank": i, "word": "casa" if i == 1 else f"word-{i}",
                 "pos": "NOUN", "frequency_count": 1001-i}
                for i in range(1, 1001)
            ],
        }
        self.candidate.write_text(json.dumps(doc), encoding="utf-8")
        self.paths = {}
        for edition, gloss in (("en", "A house"), ("fr", "Une maison"), ("es", "Una vivienda")):
            path = self.root / f"{edition}.jsonl"
            entry = {
                "word": "casa", "lang_code": "es", "pos": "noun",
                "senses": [{"glosses": [gloss], "wikidata": ["Q3947"]}],
            }
            path.write_text(json.dumps(entry) + "\n", encoding="utf-8")
            self.paths[edition] = path

    def tearDown(self):
        self.tmp.cleanup()

    def test_requires_shared_qid_and_pos_and_keeps_review_gate(self):
        queue = module.build_queue(self.candidate, "es", self.paths)
        first = queue["entries"][0]
        self.assertEqual(first["definition_status"], "qid_pos_candidates_found")
        self.assertEqual(first["aligned_candidates"][0]["wikidata_sense_qid"], "Q3947")
        self.assertEqual(first["aligned_candidates"][0]["definitions"]["fr"], ["Une maison"])
        self.assertEqual(first["aligned_candidates"][0]["review_status"], "required")
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)

    def test_no_common_qid_means_manual_review(self):
        for edition, path in self.paths.items():
            entry = json.loads(path.read_text(encoding="utf-8"))
            entry["senses"][0]["wikidata"] = {"en": ["Q101"], "fr": ["Q102"], "es": ["Q103"]}[edition]
            path.write_text(json.dumps(entry) + "\n", encoding="utf-8")
        queue = module.build_queue(self.candidate, "es", self.paths)
        self.assertEqual(queue["entries"][0]["definition_status"], "manual_lookup_required")
        self.assertEqual(queue["entries"][0]["aligned_candidates"], [])

    def test_preserves_multiple_senses_with_same_qid_and_pos(self):
        for edition, path in self.paths.items():
            entry = json.loads(path.read_text(encoding="utf-8"))
            extra = dict(entry["senses"][0])
            extra["glosses"] = [f"Alternative gloss {edition}"]
            entry["senses"].append(extra)
            path.write_text(json.dumps(entry) + "\n", encoding="utf-8")
        queue = module.build_queue(self.candidate, "es", self.paths)
        # Two senses in each edition produce all eight review combinations.
        self.assertEqual(len(queue["entries"][0]["aligned_candidates"]), 8)
        self.assertTrue(all(x["review_status"] == "required" for x in queue["entries"][0]["aligned_candidates"]))

    def test_rejects_wrong_candidate_count(self):
        doc = json.loads(self.candidate.read_text(encoding="utf-8"))
        doc["entries"].pop()
        self.candidate.write_text(json.dumps(doc), encoding="utf-8")
        with self.assertRaises(ValueError):
            module.build_queue(self.candidate, "es", self.paths)


if __name__ == "__main__":
    unittest.main()
