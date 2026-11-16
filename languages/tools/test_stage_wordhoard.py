import importlib.util
import io
import sys
import unittest
from pathlib import Path
from unittest.mock import patch

MODULE_PATH = Path(__file__).with_name("stage_wordhoard.py")
SPEC = importlib.util.spec_from_file_location("stage_wordhoard", MODULE_PATH)
stage = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = stage
SPEC.loader.exec_module(stage)


class FakeResponse:
    def __init__(self, content):
        self.content = content.encode("utf-8")

    def __enter__(self):
        return self

    def __exit__(self, *args):
        return False

    def read(self):
        return self.content


class StageWordhoardTests(unittest.TestCase):
    def test_selects_unique_normalized_headwords_in_rank_order(self):
        rows = ["lemma,pos,frequency_rank,frequency_count,cefr_estimate"]
        rows.append("café,NOUN,1,100,A1")
        rows.append("cafe\u0301,VERB,2,90,A1")
        for i in range(3, 1002):
            rows.append(f"word{i},NOUN,{i},{2000-i},A2")
        csv_text = "\n".join(rows) + "\n"
        with patch.object(stage.urllib.request, "urlopen", return_value=FakeResponse(csv_text)):
            entries = stage.read_candidates("en", 1000)
        self.assertEqual(len(entries), 1000)
        self.assertEqual(entries[0]["word"], "café")
        self.assertEqual(entries[0]["source_rank"], 1)
        self.assertEqual(entries[1]["candidate_rank"], 2)
        self.assertEqual(len({item["word"] for item in entries}), 1000)

    def test_rejects_unknown_language(self):
        with self.assertRaises(ValueError):
            stage.read_candidates("xx", 1000)

    def test_rejects_insufficient_unique_rows(self):
        csv_text = "lemma,pos,frequency_rank,frequency_count\na,NOUN,1,1\n"
        with patch.object(stage.urllib.request, "urlopen", return_value=FakeResponse(csv_text)):
            with self.assertRaises(ValueError):
                stage.read_candidates("en", 1000)


if __name__ == "__main__":
    unittest.main()
