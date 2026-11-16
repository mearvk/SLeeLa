import bz2
import importlib.util
import tempfile
import unittest
from collections import Counter
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("stage_wikipedia_frequency.py")
SPEC = importlib.util.spec_from_file_location("stage_wikipedia_frequency", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)


class WikipediaFrequencyTests(unittest.TestCase):
    def test_explicit_language_mappings(self):
        self.assertEqual(module.WIKIPEDIA_SOURCE_MAP["arz"]["wiki_code"], "arz")
        self.assertEqual(module.WIKIPEDIA_SOURCE_MAP["bho"]["wiki_code"], "bh")
        self.assertEqual(module.WIKIPEDIA_SOURCE_MAP["pnb-Arab"]["wiki_code"], "pnb")
        self.assertEqual(module.WIKIPEDIA_SOURCE_MAP["pa-Guru"]["wiki_code"], "pa")
        self.assertNotIn("zh-yue", module.WIKIPEDIA_SOURCE_MAP)
        self.assertNotIn("my", module.WIKIPEDIA_SOURCE_MAP)

    def test_wikitext_cleanup(self):
        raw = (
            "The '''city''' is [[big place|large]] and {{lang|en|bright}}. "
            "<ref>citation words not counted</ref><!-- hidden --> "
            "[[Category:Cities]] [https://example.invalid external label]"
        )
        cleaned = module.strip_wikitext(raw)
        self.assertIn("large", cleaned)
        self.assertIn("external label", cleaned)
        self.assertIn("city", cleaned)
        self.assertNotIn("citation words", cleaned)
        self.assertNotIn("hidden", cleaned)
        self.assertNotIn("Category", cleaned)
        self.assertNotIn("lang", cleaned)

    def test_unicode_tokenization_and_casefold(self):
        self.assertEqual(
            module.tokenize_words("Hello HELLO café CAFÉ नमस्ते ગુજરાતી"),
            ["hello", "hello", "café", "café", "नमस्ते", "ગુજરાતી"],
        )

    def test_main_namespace_and_redirect_filtering(self):
        xml = b'''<?xml version="1.0" encoding="utf-8"?>
        <mediawiki xmlns="http://www.mediawiki.org/xml/export-0.11/">
          <page><title>Visible</title><ns>0</ns><id>1</id>
            <revision><id>10</id><text xml:space="preserve">alpha beta alpha</text></revision>
          </page>
          <page><title>Category:Hidden</title><ns>14</ns><id>2</id>
            <revision><id>11</id><text xml:space="preserve">hidden hidden hidden</text></revision>
          </page>
          <page><title>Redirect</title><ns>0</ns><id>3</id><redirect title="Visible"/>
            <revision><id>12</id><text xml:space="preserve">redirectonly</text></revision>
          </page>
        </mediawiki>'''
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "fixture.xml.bz2"
            with bz2.open(path, "wb") as stream:
                stream.write(xml)
            counts, stats = module.extract_article_frequencies(path)
        self.assertEqual(counts, Counter({"alpha": 2, "beta": 1}))
        self.assertEqual(stats["pages_seen"], 3)
        self.assertEqual(stats["main_namespace_pages"], 2)
        self.assertEqual(stats["redirect_pages_skipped"], 1)
        self.assertEqual(stats["pages_with_text"], 1)

    def test_candidate_rank_is_deterministic(self):
        candidates = module.make_candidates(Counter({"zeta": 2, "alpha": 3, "beta": 3}), 2)
        self.assertEqual([item["word"] for item in candidates], ["alpha", "beta"])
        self.assertEqual([item["candidate_rank"] for item in candidates], [1, 2])
        self.assertEqual([item["frequency_count"] for item in candidates], [3, 3])

    def test_candidate_builder_requires_enough_unique_words(self):
        with self.assertRaises(ValueError):
            module.make_candidates(Counter({"one": 1}), 2)


if __name__ == "__main__":
    unittest.main()
