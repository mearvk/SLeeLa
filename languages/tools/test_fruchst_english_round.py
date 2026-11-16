import gzip
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("fruchst_english_round.py")
SPEC = importlib.util.spec_from_file_location("fruchst_english_round", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)
import fruchst_lexemes as lexeme_module  # noqa: E402  (importable once the module above set sys.path)


def review(tag, words, senses_by_word):
    entries = []
    for rank, word in enumerate(words, 1):
        en = senses_by_word.get(word, [])
        entries.append({
            "rank": rank, "word": word, "candidate_pos": None, "frequency_count": 10 - rank,
            "definition_status": "manual_lookup_required", "aligned_candidates": [],
            "edition_sense_candidates": {"en": en, "fr": [], "es": []},
            "edition_candidate_counts": {"en": len(en), "fr": 0, "es": 0},
        })
    return {"language_tag": tag, "target_language_mapping_note": None,
            "definition_sources": {"en": {"url": "https://example.invalid/en.jsonl.gz"}}, "entries": entries}


def rsense(gloss, line, index=1, pos="noun", tags=(), code=None):
    return {"wikidata_sense_qids": [], "pos": pos, "glosses": [gloss], "tags": list(tags), "senseid": [],
            "source_line": line, "sense_index": index, "source_lang_code": code}


class CleanGlossTests(unittest.TestCase):
    def test_wiki_link_markup_is_cleaned_for_display(self):
        text, issues = module.clean_gloss(
            "[[Appendix:Swahili_noun_classes#N class|n class_((IX))]] inflected form of -a")
        self.assertEqual(text, "n class (IX) inflected form of -a")
        self.assertIn("wiki_link_markup", issues)

    def test_stray_bracket(self):
        text, issues = module.clean_gloss("concave part of a [[butter] churn")
        self.assertEqual(text, "concave part of a butter churn")
        self.assertTrue(issues)

    def test_pointer_heuristic(self):
        self.assertTrue(module.is_pointer_text("plural of mtu"))
        self.assertTrue(module.is_pointer_text("feminine singular of هٰذَا (hāḏā)"))
        self.assertFalse(module.is_pointer_text("To form a mental picture of, to perceive"))
        self.assertFalse(module.is_pointer_text("time"))


class EnglishRoundTests(unittest.TestCase):
    def run_round(self, dump_entries, reviews, variants=None):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "en.jsonl.gz"
            with gzip.open(path, "wt", encoding="utf-8") as stream:
                for entry in dump_entries:
                    stream.write(json.dumps(entry, ensure_ascii=False) + "\n")
            scanner = module.Scanner(reviews, variants or {}, module.english_ranks(reviews))
            scanner.first_pass(path)
            wanted = module.pointer_requests(scanner)
            requested = {c: set(w) for c, w in wanted.items()}
            for _ in range(module.MAX_HOPS):
                if not wanted:
                    break
                scanner.lemma_pass(path, wanted)
                wanted = module.next_hop_requests(scanner, requested)
                for c, w in wanted.items():
                    requested.setdefault(c, set()).update(w)
            return {tag: module.build_profile(Path(f"{tag}.review.json"), doc, scanner, None)
                    for tag, doc in reviews.items()}

    def test_form_of_resolved_corroboration_and_translation_only(self):
        dump = [
            # line 1: lemma هٰذَا (page هذا)
            {"word": "هذا", "lang_code": "ar", "pos": "pron",
             "senses": [{"glosses": ["this (masculine singular)"]}]},
            # line 2: هذه as a form of هٰذَا
            {"word": "هذه", "lang_code": "ar", "pos": "pron",
             "forms": [{"form": "hāḏihī", "tags": ["romanization"]}],
             "senses": [{"glosses": ["feminine singular of هٰذَا (hāḏā)"], "tags": ["form-of"],
                         "form_of": [{"word": "هٰذَا"}]}]},
            # line 3: من with a rare noun sense and the preposition
            {"word": "من", "lang_code": "ar", "pos": "noun",
             "senses": [{"glosses": ["favor, benefit"], "tags": ["rare"]}]},
            {"word": "من", "lang_code": "ar", "pos": "prep",
             "senses": [{"glosses": ["from, of"],
                         "examples": [{"text": "مِنَ البَيْت", "english": "from the house"}]}]},
            # English entries listing the Arabic words in translation tables
            {"word": "from", "lang_code": "en", "pos": "prep", "senses": [{"glosses": ["Away from"]}],
             "translations": [{"lang_code": "ar", "word": "مِن", "sense": "with the source or provenance of"}]},
            {"word": "time", "lang_code": "en", "pos": "noun",
             "senses": [{"glosses": ["The inevitable progression into the future"],
                         "translations": [{"code": "ar", "word": "وَقْت", "roman": "waqt"}]}]},
        ]
        ar = review("ar", ["هذه", "من", "وقت"], {
            "هذه": [rsense("feminine singular of هٰذَا (hāḏā)", 2, pos="pronoun", tags=["form-of"], code="ar")],
            "من": [rsense("favor, benefit", 3, tags=["rare"], code="ar"),
                   rsense("from, of", 4, pos="preposition", code="ar")],
        })
        en = review("en", ["from", "time"], {})
        out = self.run_round(dump, {"ar": ar, "en": en})["ar"]
        rows = {r["word"]: r for r in out["entries"]}

        hadhihi = rows["هذه"]
        self.assertEqual(hadhihi["english_status"], "lemma_of_form_found")
        self.assertEqual(hadhihi["lead_candidate"]["text"], "this (masculine singular)")
        self.assertEqual(hadhihi["definitions"][0]["lemma"], "هذا")
        self.assertEqual(hadhihi["romanizations"], ["hāḏihī"])

        min_ = rows["من"]
        self.assertEqual(min_["lead_candidate"]["text"], "from, of")
        self.assertEqual(min_["definitions"][0]["corroborated_by_translation_tables"], ["from"])
        self.assertEqual(min_["definitions"][0]["examples"][0]["english"], "from the house")
        self.assertEqual(min_["definitions"][1]["register"], ["rare"])

        waqt = rows["وقت"]
        self.assertEqual(waqt["english_status"], "translation_equivalent_found")
        self.assertTrue(waqt["definitions"][0]["text"].startswith("time"))
        self.assertEqual(waqt["definitions"][0]["evidence"]["match"], "optional_marks")
        self.assertEqual(waqt["lead_candidate"]["review_status"], "required")

        counts = out["counts"]
        self.assertEqual(counts["words_with_any_english_definition"], 3)
        self.assertEqual(counts["words_gaining_english_definition"], 1)
        self.assertEqual(counts["pointer_senses_resolved"], 1)
        self.assertEqual(counts["production_definitions_approved"], 0)

    def test_two_hop_pointer_and_hanja(self):
        dump = [
            {"word": "go", "lang_code": "pl", "pos": "pron",
             "senses": [{"glosses": ["genitive of on"], "tags": ["form-of"], "form_of": [{"word": "on"}]}]},
            {"word": "on", "lang_code": "pl", "pos": "pron",
             "senses": [{"glosses": ["alternative form of jego"], "tags": ["alt-of"], "alt_of": [{"word": "jego"}]}]},
            {"word": "jego", "lang_code": "pl", "pos": "pron", "senses": [{"glosses": ["him"]}]},
            {"word": "왜", "lang_code": "ko", "pos": "syllable", "senses": [{"glosses": ["娃"]}]},
            {"word": "娃", "lang_code": "ko", "pos": "character", "senses": [{"glosses": ["pretty, beautiful"]}]},
        ]
        pl = review("pl", ["go"], {"go": [rsense("genitive of on", 1, pos="pronoun", tags=["form-of"], code="pl")]})
        ko = review("ko", ["왜"], {"왜": [rsense("娃", 4, pos="syllable", code="ko")]})
        out = self.run_round(dump, {"pl": pl, "ko": ko})
        go = out["pl"]["entries"][0]
        self.assertEqual(go["lead_candidate"]["text"], "him")
        self.assertEqual(go["definitions"][0]["hops"], 2)
        wae = out["ko"]["entries"][0]
        self.assertEqual(wae["lead_candidate"]["text"], "pretty, beautiful")
        self.assertIn("hanja_reading_gloss", wae["gloss_notes"][0]["issues"])

    def test_review_glosses_are_not_modified(self):
        ar = review("ar", ["هذه"], {"هذه": [rsense("feminine singular of هٰذَا (hāḏā)", 1, tags=["form-of"])]})
        before = json.dumps(ar, sort_keys=True)
        self.run_round([], {"ar": ar})
        self.assertEqual(json.dumps(ar, sort_keys=True), before)


    def test_rule_based_keys_and_flags(self):
        dump = [
            {"word": "당신", "lang_code": "ko", "pos": "pron", "senses": [{"glosses": ["you (honorific)"]}]},
            {"word": "그거", "lang_code": "ko", "pos": "pron", "senses": [{"glosses": ["that thing"]}]},
            {"word": "ഞാൻ", "lang_code": "ml", "pos": "pron", "senses": [{"glosses": ["I"]}]},
            {"word": "что", "lang_code": "ru", "pos": "pron", "senses": [{"glosses": ["what"]}]},
        ]
        filler = [chr(0xAC00 + 28 * i) for i in range(25)]
        ko = review("ko", ["당신이", "그건", "있는", "사람", "you"] + filler, {"사람": [rsense("person", 9, code="ko")]})
        ml = review("ml", ["ഞാന്"], {})
        uk = review("uk", ["что", "так"], {"так": [rsense("so", 9, pos="adverb", code="uk")]})
        out = self.run_round(dump, {"ko": ko, "ml": ml, "uk": uk})
        rows = {r["word"]: r for r in out["ko"]["entries"]}
        self.assertEqual(rows["당신이"]["lead_candidate"]["text"], "you (honorific)")
        self.assertEqual(rows["당신이"]["definitions"][0]["affix"], "이")
        self.assertEqual(rows["그건"]["definitions"][0]["variant_method"], "contracted_particle")
        self.assertEqual(rows["있는"]["definitions"], [])
        self.assertEqual(rows["you"]["flags"].get("script_mismatch"), "LATIN")
        self.assertEqual(out["ml"]["entries"][0]["lead_candidate"]["text"], "I")
        self.assertEqual(out["ml"]["entries"][0]["definitions"][0]["variant_method"], "chillu_normalized")
        chto = out["uk"]["entries"][0]
        self.assertEqual(chto["definitions"], [])
        self.assertEqual(chto["flags"]["found_only_in_other_languages"], ["ru"])
        self.assertEqual(out["uk"]["counts"]["other_language_codes"], {"ru": 1})

    def test_wikidata_lexemes(self):
        lexemes = [
            {"type": "lexeme", "id": "L1", "language": "Q9610", "lexicalCategory": "Q1084",
             "lemmas": {"bn": {"language": "bn", "value": "বই"}},
             "forms": [{"id": "L1-F2", "representations": {"bn": {"language": "bn", "value": "বইটি"}},
                        "grammaticalFeatures": ["Q110786"]}],
             "senses": [{"id": "L1-S1", "glosses": {"en": {"language": "en", "value": "book"}},
                         "claims": {"P5137": [{"mainsnak": {"datavalue": {"value": {"id": "Q571"}}}}]}},
                        {"id": "L1-S2", "glosses": {"bn": {"language": "bn", "value": "x"}},
                         "claims": {"P5972": [{"mainsnak": {"datavalue": {"value": {"id": "L9-S1"}}}}]}}]},
            {"type": "lexeme", "id": "L9", "language": "Q1860", "lexicalCategory": "Q1084",
             "lemmas": {"en": {"language": "en", "value": "volume"}},
             "senses": [{"id": "L9-S1", "glosses": {"en": {"language": "en", "value": "a single book"}}}]},
        ]
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "lexemes.json.gz"
            with gzip.open(path, "wt", encoding="utf-8") as stream:
                stream.write("[\n" + ",\n".join(json.dumps(x, ensure_ascii=False) for x in lexemes) + "\n]\n")
            hits = lexeme_module.scan_lexemes(path, {"bn": {"বই", "বইটি"}})
        book = hits[("bn", "বই")]
        self.assertEqual(book[0]["gloss"], "book")
        self.assertEqual(book[0]["items"], ["Q571"])
        self.assertEqual(book[1]["english_translations"][0]["english_lemma"], "volume")
        self.assertEqual(hits[("bn", "বইটি")][0]["match"], "form")

        bn = review("bn", ["বই", "বইটি"], {})
        scanner = module.Scanner({"bn": bn}, {}, {})
        scanner.lexemes = hits
        out = module.build_profile(Path("bn.review.json"), bn, scanner, None)
        rows = {r["word"]: r for r in out["entries"]}
        self.assertEqual(rows["বই"]["english_status"], "lexeme_sense_found")
        self.assertEqual(rows["বই"]["lead_candidate"]["text"], "book")
        self.assertEqual(rows["বইটি"]["definitions"][0]["display"], "form of বই: book")


if __name__ == "__main__":
    unittest.main()
