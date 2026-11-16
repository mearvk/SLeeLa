import importlib.util
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("fruchst.py")
SPEC = importlib.util.spec_from_file_location("fruchst_production", MODULE_PATH)
module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(module)


def approved_fixture():
    sources = {
        code: {
            "url": f"https://example.invalid/{code}.jsonl.gz",
            "dump_revision": "2026-01-01",
            "sha256": "a" * 64,
            "license": "CC BY-SA 4.0",
        }
        for code in ("en", "fr", "es")
    }
    entries = []
    for rank in range(1, 1001):
        entries.append({
            "rank": rank,
            "word": f"word-{rank}",
            "frequency": 1.0,
            "definitions": {"en": "meaning", "fr": "sens", "es": "significado"},
            "review_status": "approved",
            "selected_sense": {
                "sense_id": f"Q{rank}",
                "evidence": "Matched sense evidence checked in all three editions",
                "reviewer": "reviewer-id",
                "approved_at_utc": "2026-01-01T00:00:00Z",
            },
        })
    return {
        "language_tag": "en",
        "source_id": "fixture-frequency-corpus",
        "entries": entries,
        "provenance": {
            "frequency_source": "Fixture corpus",
            "attribution": "Fixture attribution",
            "license_review": "Redistribution permitted for test fixture",
            "definition_sources": sources,
        },
    }


class ProductionGateTests(unittest.TestCase):
    def test_approved_complete_input_passes(self):
        module.validate_production_input(approved_fixture())

    def test_unreviewed_candidate_is_rejected(self):
        data = approved_fixture()
        data["entries"][0]["review_status"] = "required"
        with self.assertRaisesRegex(module.FruchstError, "human approval"):
            module.validate_production_input(data)

    def test_missing_provenance_is_rejected(self):
        data = approved_fixture()
        del data["provenance"]["definition_sources"]["fr"]
        with self.assertRaisesRegex(module.FruchstError, "definition_sources.fr"):
            module.validate_production_input(data)

    def test_missing_selected_sense_evidence_is_rejected(self):
        data = approved_fixture()
        del data["entries"][0]["selected_sense"]["evidence"]
        with self.assertRaisesRegex(module.FruchstError, "selected_sense.evidence"):
            module.validate_production_input(data)

    def test_bad_source_hash_is_rejected(self):
        data = approved_fixture()
        data["provenance"]["definition_sources"]["es"]["sha256"] = "not-a-hash"
        with self.assertRaisesRegex(module.FruchstError, "invalid SHA-256"):
            module.validate_production_input(data)


if __name__ == "__main__":
    unittest.main()
