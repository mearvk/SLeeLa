#!/usr/bin/env python3
"""Validate Wikipedia worker artifacts and update FRUCHST source provenance."""
from __future__ import annotations

import argparse
import json
import shutil
import sys
import unicodedata
from pathlib import Path

from stage_wikipedia_frequency import WIKIPEDIA_SOURCE_MAP


def validate_document(path: Path, expected_tag: str) -> dict:
    document = json.loads(path.read_text(encoding="utf-8"))
    if document.get("language_tag") != expected_tag or document.get("candidate_count") != 1000:
        raise ValueError(f"{path}: language tag or candidate count mismatch")
    entries = document.get("entries")
    if not isinstance(entries, list) or len(entries) != 1000:
        raise ValueError(f"{path}: expected exactly 1,000 entries")
    words = []
    for rank, entry in enumerate(entries, 1):
        if not isinstance(entry, dict) or entry.get("candidate_rank") != rank:
            raise ValueError(f"{path}: invalid rank sequence at {rank}")
        word = entry.get("word")
        if not isinstance(word, str) or not word or unicodedata.normalize("NFC", word) != word:
            raise ValueError(f"{path}: invalid or non-NFC headword at {rank}")
        if word != word.casefold():
            raise ValueError(f"{path}: non-casefolded headword at {rank}")
        count = entry.get("frequency_count")
        if isinstance(count, bool) or not isinstance(count, int) or count < 1:
            raise ValueError(f"{path}: invalid frequency count at {rank}")
        words.append(word)
    if len(set(words)) != 1000:
        raise ValueError(f"{path}: duplicate headwords")
    return document


def aggregate(input_dir: Path, output_dir: Path, registry_path: Path):
    docs = {}
    for tag in sorted(WIKIPEDIA_SOURCE_MAP):
        source = input_dir / f"wikipedia-{tag}.json"
        if not source.is_file():
            raise ValueError(f"missing worker output for {tag}: {source}")
        docs[tag] = validate_document(source, tag)

    registry = json.loads(registry_path.read_text(encoding="utf-8"))
    profiles = {item["language_tag"]: item for item in registry["profiles"]}
    output_dir.mkdir(parents=True, exist_ok=True)
    for tag, doc in docs.items():
        if tag not in profiles:
            raise ValueError(f"target registry has no profile for {tag}")
        profile = profiles[tag]
        if profile.get("status") == "frequency_candidates_staged":
            raise ValueError(f"{tag}: a frequency source is already staged")
        shutil.copyfile(input_dir / f"wikipedia-{tag}.json",
                        output_dir / f"wikipedia-{tag}.json")
        source = doc["source"]
        profile.update({
            "status": "frequency_candidates_staged",
            "entries": 1000,
            "source_id": f"wikipedia-xml-dump-{source['wiki_code']}",
            "source_revision": source["dump_sha256"],
            "license_review": "Wikimedia CC-BY-SA-4.0/GFDL noted; per-page imported-content, attribution and ShareAlike review required before binary release",
            "definitions_status": "pending",
            "notes": (
                "1,000 encyclopedia-domain surface-form candidates from "
                f"Wikipedia {source['wiki_code']} current-revision article text; "
                "not conversational frequency; native-speaker, script, corpus-bias "
                "and license review pending; no production binary generated."
            ),
        })
    registry_path.write_text(json.dumps(registry, ensure_ascii=False, indent=2) + "\n",
                             encoding="utf-8")
    return list(docs)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input-dir", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, default=Path("languages/datasets/frequency-candidates"))
    parser.add_argument("--registry", type=Path, default=Path("languages/FRUCHST_TARGETS.json"))
    args = parser.parse_args()
    try:
        tags = aggregate(args.input_dir, args.output_dir, args.registry)
        print(f"Validated and staged {len(tags)} Wikipedia candidate files.")
        print(", ".join(tags))
    except (OSError, KeyError, ValueError, json.JSONDecodeError) as exc:
        print(f"merge_wikipedia_frequency: error: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
