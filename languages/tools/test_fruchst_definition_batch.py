import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("fruchst_definition_batch.py")
SPEC = importlib.util.spec_from_file_location("fruchst_definition_batch", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)

class DefinitionBatchTests(unittest.TestCase):
    def test_pos_aliases(self):
        self.assertEqual(module.pos_key("NOUN"), "noun")
        self.assertEqual(module.pos_key("ADP"), "preposition")
        self.assertEqual(module.pos_key("AUX"), "auxiliary")

    def test_qid_extraction(self):
        self.assertEqual(module.qids_for({"wikidata": ["Q42", "Q1", "bad", "Q42"]}), ["Q1", "Q42"])

    def test_gzip_extraction(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "fixture.jsonl.gz"
            import gzip
            entry = {"word":"casa","lang_code":"es","pos":"noun",
                     "senses":[{"glosses":["house"],"wikidata":["Q3947"]}]}
            with gzip.open(path, "wt", encoding="utf-8") as stream:
                stream.write(json.dumps(entry, ensure_ascii=False) + "\n")
            found = module.extract_edition(path, {"es":{"casa"},"en":set(),"fr":set(),"de":set()})
            self.assertIn(("Q3947","noun"), found["es"]["casa"])

    def test_build_queue_filters_candidate_pos_and_keeps_review_gate(self):
        doc = {"language_tag":"es","source":{"id":"fixture"}}
        words = {"casa":{"pos":"NOUN","frequency_count":5}}
        noun = {"glosses":["house"],"source_line":1,"sense_index":1,"senseid":[],"tags":[]}
        verb = {"glosses":["to marry"],"source_line":2,"sense_index":1,"senseid":[],"tags":[]}
        senses = {("Q3947","noun"):[noun],("Q999","verb"):[verb]}
        by_edition = {edition:{"casa":senses} for edition in module.EDITIONS}
        source_meta = {edition:{"url":f"https://example.invalid/{edition}.jsonl.gz",
                               "dump_revision":"2026-01-01","file":edition,"sha256":"0"*64,
                               "size_bytes":1,"retrieved_at_utc":"2026-01-01T00:00:00+00:00"}
                       for edition in module.EDITIONS}
        queue = module.build_queue("es",doc,words,by_edition,source_meta)
        self.assertEqual(len(queue["entries"][0]["aligned_candidates"]),1)
        self.assertEqual(queue["entries"][0]["aligned_candidates"][0]["wikidata_sense_qid"],"Q3947")
        self.assertEqual(queue["counts"]["production_definitions_approved"],0)
        self.assertEqual(queue["definition_sources"]["en"]["url"],"https://example.invalid/en.jsonl.gz")
        self.assertEqual(queue["definition_sources"]["en"]["dump_revision"],"2026-01-01")

    def test_glosses_without_qids_are_preserved_for_manual_review(self):
        doc = {"language_tag":"es","source":{"id":"fixture"}}
        words = {"casa":{"pos":"NOUN","frequency_count":5}}
        sense = {"glosses":["house"],"source_line":1,"sense_index":1,"senseid":[],"tags":[]}
        by_edition = {edition:{"casa":{("", "noun"):[sense]}} for edition in module.EDITIONS}
        source_meta = {edition:{"url":f"https://example.invalid/{edition}.jsonl.gz",
                               "dump_revision":"2026-01-01","file":edition,"sha256":"0"*64,
                               "size_bytes":1,"retrieved_at_utc":"2026-01-01T00:00:00+00:00"}
                       for edition in module.EDITIONS}
        queue = module.build_queue("es",doc,words,by_edition,source_meta)
        row = queue["entries"][0]
        self.assertEqual(row["definition_status"],"edition_gloss_candidates_found")
        self.assertEqual(row["aligned_candidates"],[])
        self.assertEqual(row["edition_candidate_counts"],{"en":1,"fr":1,"es":1})
        self.assertEqual(row["edition_sense_candidates"]["fr"][0]["glosses"],["house"])
        self.assertEqual(queue["counts"]["words_with_edition_gloss_candidates"],1)
        self.assertEqual(queue["counts"]["production_definitions_approved"],0)

    def test_all_common_senses_are_preserved(self):
        doc = {"language_tag":"es","source":{"id":"fixture"}}
        words = {"casa":{"pos":"NOUN","frequency_count":5}}
        first = {"glosses":["house"],"source_line":1,"sense_index":1,"senseid":[],"tags":[]}
        second = {"glosses":["home"],"source_line":2,"sense_index":2,"senseid":[],"tags":[]}
        senses = {("Q3947","noun"):[first],("Q41176","noun"):[second]}
        by_edition = {edition:{"casa":senses} for edition in module.EDITIONS}
        source_meta = {edition:{"url":f"https://example.invalid/{edition}.jsonl.gz",
                               "dump_revision":"2026-01-01","file":edition,"sha256":"0"*64,
                               "size_bytes":1,"retrieved_at_utc":"2026-01-01T00:00:00+00:00"}
                       for edition in module.EDITIONS}
        queue = module.build_queue("es",doc,words,by_edition,source_meta)
        self.assertEqual(len(queue["entries"][0]["aligned_candidates"]),2)
        self.assertEqual(queue["counts"]["production_definitions_approved"],0)


if __name__ == "__main__":
    unittest.main()
