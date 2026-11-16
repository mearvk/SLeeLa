#!/usr/bin/env python3
"""Focused regression tests for partial-QID FRUCHST review candidates.

This file is intentionally independent of the existing batch test module so it
can be run directly during the alignment-engine change.
"""
import importlib.util
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("fruchst_definition_batch.py")
SPEC = importlib.util.spec_from_file_location("fruchst_definition_batch", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)


def meta():
    return {
        edition: {
            "url": f"https://example.invalid/{edition}.jsonl.gz",
            "dump_revision": "fixture",
            "file": edition,
            "sha256": "0" * 64,
            "size_bytes": 1,
            "retrieved_at_utc": "2026-01-01T00:00:00+00:00",
        }
        for edition in module.EDITIONS
    }


def sense(gloss, line, qids=None):
    return {
        "glosses": [gloss], "qids": qids or [], "tags": [],
        "senseid": [], "source_line": line, "sense_index": 1,
    }


class PartialQidAlignmentTests(unittest.TestCase):
    def test_two_editions_anchor_third_edition_fallback_for_review(self):
        by_edition = {
            "en": {"es": {"casa": {("Q3947", "noun"): [sense("house", 1, ["Q3947"])]}}},
            "fr": {"es": {"casa": {("Q3947", "noun"): [sense("maison", 2, ["Q3947"])]}}},
            "es": {"es": {"casa": {("", "noun"): [sense("vivienda", 3)]}}},
        }
        queue = module.build_queue(
            "es", {"language_tag": "es", "source": {}},
            {"casa": {"pos": "noun"}}, by_edition, meta(),
        )
        row = queue["entries"][0]
        self.assertEqual(row["aligned_candidates"], [])
        self.assertEqual(row["definition_status"], "partial_qid_review_candidates_found")
        self.assertEqual(len(row["review_alignment_candidates"]), 1)
        candidate = row["review_alignment_candidates"][0]
        self.assertEqual(candidate["qid_support_count"], 2)
        self.assertEqual(candidate["qid_supporting_editions"], ["en", "fr"])
        self.assertTrue(candidate["source_evidence"]["es"][0]["qid_fallback"])
        self.assertEqual(candidate["review_status"], "required")
        self.assertEqual(candidate["selection_status"], "none")
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)

    def test_single_edition_qid_is_not_alignment_evidence(self):
        by_edition = {
            "en": {"es": {"casa": {("Q3947", "noun"): [sense("house", 1, ["Q3947"])]}}},
            "fr": {"es": {"casa": {("", "noun"): [sense("maison", 2)]}}},
            "es": {"es": {"casa": {("", "noun"): [sense("vivienda", 3)]}}},
        }
        queue = module.build_queue(
            "es", {"language_tag": "es", "source": {}},
            {"casa": {"pos": "noun"}}, by_edition, meta(),
        )
        row = queue["entries"][0]
        self.assertEqual(row["review_alignment_candidates"], [])
        self.assertEqual(queue["counts"]["qid_pos_review_candidates"], 0)
        self.assertEqual(queue["counts"]["production_definitions_approved"], 0)


if __name__ == "__main__":
    unittest.main()
