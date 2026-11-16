import gzip
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("fruchst_definition_batch.py")
SPEC = importlib.util.spec_from_file_location("fruchst_definition_batch", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)


def source_meta():
    return {
        edition: {
            "url": f"https://example.invalid/{edition}.jsonl.gz",
            "dump_revision": "2026-01-01",
            "file": edition,
            "sha256": "0" * 64,
            "size_bytes": 1,
            "retrieved_at_utc": "2026-01-01T00:00:00+00:00",
        }
        for edition in module.EDITIONS
    }


def sense(gloss, line, sense_index=1, qids=None):
    return {
        "glosses": [gloss],
        "qids": qids or [],
        "tags": [],
        "senseid": [],
        "source_line": line,
        "sense_index": sense_index,
    }


class DefinitionBatchTests(unittest.TestCase):
    def test_pos_aliases(self):
        self.assertEqual(module.pos_key("NOUN"), "noun")
        self.assertEqual(module.pos_key("ADP"), "preposition")
        self.assertEqual(module.pos_key("AUX"), "auxiliary")

    def test_qid_extraction(self):
        self.assertEqual(
            module.qids_for({"wikidata": ["Q42", "Q1", "bad", "Q42"]}),
            ["Q1", "Q42"],
        )

    def test_gzip_extraction(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "fixture.jsonl.gz"
            entry = {
                "word": "casa",
                "lang_code": "es",
                "pos": "noun",
                "senses": [{"glosses": ["house"], "wikidata": ["Q3947"]}],
            }
            with gzip.open(path, "wt", encoding="utf-8") as stream:
                stream.write(json.dumps(entry, ensure_ascii=False) + "\n")
            found = module.extract_edition(path, {"es": {"casa"}})
            self.assertIn(("Q3947", "noun"), found["es"]["casa"])

    def test_build_queue_filters_known_candidate_pos_and_keeps_review_gate(self):
        doc = {"language_tag": "es", "source": {"id": "fixture"}}
        words = {"casa": {"pos": "NOUN", "frequency_count": 5}}
        noun = sense("house", 1, qids=["Q3947"])
        verb = sense("to marry", 2, qids=["Q999"])
        senses = {("Q3947", "noun"): [noun], ("Q999", "verb"): [verb]}
        by_edition = {
            edition: {"es": {"casa": senses}}
            for edition in module.EDITIONS
        }
        queue = module.build_queue("es", doc, words, by_edition, source_meta())
        row = queue["entries"][0]
        self.assertEqual(len(row["aligned_candidates"]), 1)
        self.assertEqual(
            row["aligned_candidates"][0]["wikidata_sense_qid"], "Q3947"
        )
        self.assertEqual(row["edition_candidate_counts"], {
            "en": 1, "fr": 1, "es": 1
        })
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)
        self.assertEqual(
            queue["definition_sources"]["en"]["url"],
            "https://example.invalid/en.jsonl.gz",
        )

    def test_glosses_without_qids_are_preserved_for_manual_review(self):
        doc = {"language_tag": "es", "source": {"id": "fixture"}}
        words = {"casa": {"pos": "NOUN", "frequency_count": 5}}
        by_edition = {
            edition: {
                "es": {"casa": {("", "noun"): [sense("house", 1)]}}
            }
            for edition in module.EDITIONS
        }
        queue = module.build_queue("es", doc, words, by_edition, source_meta())
        row = queue["entries"][0]
        self.assertEqual(row["definition_status"], "edition_gloss_candidates_found")
        self.assertEqual(row["aligned_candidates"], [])
        self.assertEqual(
            row["edition_candidate_counts"], {"en": 1, "fr": 1, "es": 1}
        )
        self.assertEqual(
            row["edition_sense_candidates"]["fr"][0]["glosses"], ["house"]
        )
        self.assertEqual(queue["counts"]["words_with_edition_gloss_candidates"], 1)
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)

    def test_two_edition_qid_creates_partial_review_candidate(self):
        doc = {"language_tag": "es", "source": {"id": "fixture"}}
        words = {"casa": {"pos": "NOUN", "frequency_count": 5}}
        by_edition = {
            "en": {"es": {"casa": {("Q3947", "noun"): [sense("house", 1, qids=["Q3947"]) ]}}},
            "fr": {"es": {"casa": {("Q3947", "noun"): [sense("maison", 2, qids=["Q3947"]) ]}}},
            "es": {"es": {"casa": {("", "noun"): [sense("vivienda", 3)]}}},
        }
        queue = module.build_queue("es", doc, words, by_edition, source_meta())
        row = queue["entries"][0]
        self.assertEqual(row["aligned_candidates"], [])
        self.assertEqual(row["definition_status"], "partial_qid_review_candidates_found")
        self.assertEqual(len(row["review_alignment_candidates"]), 1)
        candidate = row["review_alignment_candidates"][0]
        self.assertEqual(candidate["qid_supporting_editions"], ["en", "fr"])
        self.assertEqual(candidate["alignment_strength"], "shared_qid_2_of_3_review_only")
        self.assertTrue(candidate["source_evidence"]["es"][0]["qid_fallback"])
        self.assertEqual(candidate["definitions"]["es"], [["vivienda"]])
        self.assertEqual(candidate["review_status"], "required")
        self.assertEqual(candidate["selection_status"], "none")
        self.assertEqual(queue["counts"]["qid_pos_candidate_triples"], 0)
        self.assertEqual(queue["counts"]["qid_pos_review_candidates"], 1)
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)

    def test_qid_in_only_one_edition_does_not_create_alignment(self):
        doc = {"language_tag": "es", "source": {"id": "fixture"}}
        words = {"casa": {"pos": "NOUN", "frequency_count": 5}}
        by_edition = {
            "en": {"es": {"casa": {("Q3947", "noun"): [sense("house", 1, qids=["Q3947"]) ]}}},
            "fr": {"es": {"casa": {("", "noun"): [sense("maison", 2)]}}},
            "es": {"es": {"casa": {("", "noun"): [sense("vivienda", 3)]}}},
        }
        queue = module.build_queue("es", doc, words, by_edition, source_meta())
        row = queue["entries"][0]
        self.assertEqual(row["review_alignment_candidates"], [])
        self.assertEqual(row["definition_status"], "edition_gloss_candidates_found")
        self.assertEqual(queue["counts"]["qid_pos_review_candidates"], 0)
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)

    def test_all_common_senses_are_preserved(self):
        doc = {"language_tag": "es", "source": {"id": "fixture"}}
        words = {"casa": {"pos": "NOUN", "frequency_count": 5}}
        senses = {
            ("Q3947", "noun"): [sense("house", 1, qids=["Q3947"])],
            ("Q41176", "noun"): [sense("home", 2, qids=["Q41176"])],
        }
        by_edition = {
            edition: {"es": {"casa": senses}}
            for edition in module.EDITIONS
        }
        queue = module.build_queue("es", doc, words, by_edition, source_meta())
        self.assertEqual(len(queue["entries"][0]["aligned_candidates"]), 2)
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)

    def test_unknown_candidate_pos_preserves_all_pos(self):
        doc = {"language_tag": "es", "source": {"id": "fixture"}}
        words = {"casa": {"pos": None, "frequency_count": 5}}
        senses = {
            ("Q3947", "noun"): [sense("house", 1, qids=["Q3947"])],
            ("Q999", "verb"): [sense("to marry", 2, qids=["Q999"])],
        }
        by_edition = {
            edition: {"es": {"casa": senses}}
            for edition in module.EDITIONS
        }
        queue = module.build_queue("es", doc, words, by_edition, source_meta())
        row = queue["entries"][0]
        self.assertFalse(row["candidate_pos_known"])
        self.assertEqual(row["edition_candidate_counts"], {
            "en": 2, "fr": 2, "es": 2
        })
        self.assertEqual(
            {candidate["pos"] for candidate in row["edition_sense_candidates"]["en"]},
            {"noun", "verb"},
        )
        self.assertEqual(len(row["aligned_candidates"]), 2)

    def test_loader_accepts_frequencywords_candidate_filename(self):
        original_targets = module.TARGETS
        original_codes = module.WIKTIONARY_LANG_CODES
        try:
            module.TARGETS = ("zh-Hans",)
            module.WIKTIONARY_LANG_CODES = {"zh-Hans": "zh"}
            with tempfile.TemporaryDirectory() as temp:
                directory = Path(temp)
                entries = [
                    {
                        "candidate_rank": rank,
                        "word": f"词{rank}",
                        "pos": None,
                        "frequency_count": 1001 - rank,
                    }
                    for rank in range(1, 1001)
                ]
                (directory / "frequencywords-zh-Hans.json").write_text(
                    json.dumps({
                        "language_tag": "zh-Hans",
                        "candidate_count": 1000,
                        "source": {"dataset": "FrequencyWords"},
                        "entries": entries,
                    }, ensure_ascii=False),
                    encoding="utf-8",
                )
                docs, words, wanted, paths = module.load_candidates(directory)
                self.assertEqual(len(words["zh-Hans"]), 1000)
                self.assertEqual(len(wanted["zh-Hans"]), 1000)
                self.assertEqual(paths["zh-Hans"].name, "frequencywords-zh-Hans.json")
                self.assertEqual(docs["zh-Hans"]["language_tag"], "zh-Hans")
        finally:
            module.TARGETS = original_targets
            module.WIKTIONARY_LANG_CODES = original_codes

    def test_explicit_wiktionary_language_code_mapping(self):
        original_targets = module.TARGETS
        original_codes = module.WIKTIONARY_LANG_CODES
        try:
            module.TARGETS = ("zh-Hans",)
            module.WIKTIONARY_LANG_CODES = {"zh-Hans": "zh"}
            with tempfile.TemporaryDirectory() as temp:
                path = Path(temp) / "fixture.jsonl"
                entry = {
                    "word": "词",
                    "lang_code": "zh",
                    "pos": "noun",
                    "senses": [{"glosses": ["word"]}],
                }
                path.write_text(json.dumps(entry, ensure_ascii=False) + "\n", encoding="utf-8")
                found = module.extract_edition(path, {"zh-Hans": {"词"}})
                self.assertIn("词", found["zh-Hans"])
                self.assertIn(("", "noun"), found["zh-Hans"]["词"])
        finally:
            module.TARGETS = original_targets
            module.WIKTIONARY_LANG_CODES = original_codes


if __name__ == "__main__":
    unittest.main()
