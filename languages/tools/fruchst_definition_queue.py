#!/usr/bin/env python3
"""Build a conservative FRUCHST definition review queue from Kaikki/Wiktextract JSONL.

The tool never selects a sense automatically. It only groups English, French,
and Spanish gloss candidates when all three editions expose the same sense QID
and POS for the target-language headword. Every result remains review-required.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
import unicodedata
from pathlib import Path
from typing import Iterable

EDITIONS = ("en", "fr", "es")
POS_ALIASES = {
    "adj": "adjective",
    "adv": "adverb",
    "aux": "auxiliary",
    "noun": "noun",
    "verb": "verb",
    "pron": "pronoun",
    "det": "determiner",
    "prep": "preposition",
    "postp": "postposition",
    "conj": "conjunction",
    "intj": "interjection",
    "num": "numeral",
    "part": "particle",
    "proper": "proper noun",
    "propn": "proper noun",
}


def norm(value: str) -> str:
    return unicodedata.normalize("NFC", value)


def pos_key(value: object) -> str:
    raw = str(value or "").strip().casefold().replace("_", " ")
    return POS_ALIASES.get(raw, raw)


def qids_for(sense: dict) -> list[str]:
    values = sense.get("wikidata", [])
    if isinstance(values, str):
        values = [values]
    if not isinstance(values, list):
        return []
    return sorted({v for v in values if isinstance(v, str) and re.fullmatch(r"Q[1-9][0-9]*", v)})


def load_words(path: Path) -> tuple[dict, dict[str, dict]]:
    doc = json.loads(path.read_text(encoding="utf-8"))
    if doc.get("candidate_count") != 1000 or len(doc.get("entries", [])) != 1000:
        raise ValueError(f"{path}: expected exactly 1,000 staged candidates")
    words = {}
    for item in doc["entries"]:
        word = norm(item["word"])
        if word in words:
            raise ValueError(f"{path}: duplicate normalized headword {word!r}")
        words[word] = item
    return doc, words


def extract_edition(path: Path, target_lang_code: str, wanted: set[str]) -> dict:
    found: dict[str, list[dict]] = {}
    with path.open("r", encoding="utf-8") as stream:
        for line_no, line in enumerate(stream, 1):
            if not line.strip():
                continue
            try:
                entry = json.loads(line)
            except json.JSONDecodeError as exc:
                raise ValueError(f"{path}:{line_no}: invalid JSON: {exc}") from exc
            if entry.get("lang_code") != target_lang_code:
                continue
            word = entry.get("word")
            if not isinstance(word, str):
                continue
            word = norm(word)
            if word not in wanted:
                continue
            pos = pos_key(entry.get("pos"))
            senses = entry.get("senses", [])
            if not isinstance(senses, list):
                continue
            for sense_index, sense in enumerate(senses, 1):
                if not isinstance(sense, dict):
                    continue
                glosses = sense.get("glosses") or sense.get("raw_glosses") or []
                if isinstance(glosses, str):
                    glosses = [glosses]
                glosses = [norm(g.strip()) for g in glosses if isinstance(g, str) and g.strip()]
                if not glosses:
                    continue
                qids = qids_for(sense)
                for qid in qids:
                    found.setdefault(word, []).append({
                        "qid": qid,
                        "pos": pos,
                        "glosses": glosses,
                        "tags": sorted(t for t in sense.get("tags", []) if isinstance(t, str)),
                        "senseid": sense.get("senseid", []),
                        "source_word": word,
                        "source_pos": entry.get("pos", ""),
                        "source_line": line_no,
                        "sense_index": sense_index,
                    })
    return found


def build_queue(candidate_path: Path, target_lang_code: str, edition_paths: dict[str, Path]) -> dict:
    candidate_doc, candidates = load_words(candidate_path)
    wanted = set(candidates)
    by_edition = {
        edition: extract_edition(path, target_lang_code, wanted)
        for edition, path in edition_paths.items()
    }
    rows = []
    aligned_total = 0
    for rank, (word, candidate) in enumerate(candidates.items(), 1):
        per_edition = {
            edition: by_edition[edition].get(word, [])
            for edition in EDITIONS
        }
        maps = {
            edition: {(sense["qid"], sense["pos"]): sense for sense in per_edition[edition]}
            for edition in EDITIONS
        }
        common = set(maps["en"]) & set(maps["fr"]) & set(maps["es"])
        aligned = []
        for qid, pos in sorted(common):
            aligned.append({
                "wikidata_sense_qid": qid,
                "pos": pos,
                "definitions": {
                    edition: maps[edition][(qid, pos)]["glosses"]
                    for edition in EDITIONS
                },
                "source_evidence": {
                    edition: {
                        "edition": edition,
                        "source_line": maps[edition][(qid, pos)]["source_line"],
                        "sense_index": maps[edition][(qid, pos)]["sense_index"],
                        "senseid": maps[edition][(qid, pos)]["senseid"],
                        "tags": maps[edition][(qid, pos)]["tags"],
                    }
                    for edition in EDITIONS
                },
                "review_status": "required",
            })
        if aligned:
            aligned_total += 1
        rows.append({
            "rank": rank,
            "word": word,
            "candidate_pos": candidate.get("pos"),
            "frequency_count": candidate.get("frequency_count"),
            "definition_status": "qid_pos_candidates_found" if aligned else "manual_lookup_required",
            "aligned_candidates": aligned,
            "edition_candidate_counts": {edition: len(per_edition[edition]) for edition in EDITIONS},
        })
    return {
        "schema": "SLeeLa FRUCHST definition review queue v1",
        "language_tag": candidate_doc["language_tag"],
        "target_wiktionary_lang_code": target_lang_code,
        "frequency_candidate_file": str(candidate_path),
        "frequency_source": candidate_doc["source"],
        "definition_sources": {
            edition: {
                "file": str(edition_paths[edition]),
                "project": "Kaikki/Wiktextract extraction of Wiktionary",
                "license": "Wiktionary dual license: CC BY-SA 4.0 and GFDL; verify edition-specific notices and attribution before redistribution",
            }
            for edition in EDITIONS
        },
        "alignment_policy": "A candidate triple is emitted only when all three editions contain the exact NFC headword, same sense-level Wikidata QID, and same normalized POS. QID/POS agreement is evidence for human review, not automatic semantic approval. No candidate is selected automatically.",
        "counts": {
            "candidate_words": len(rows),
            "words_with_qid_pos_candidates": aligned_total,
            "words_requiring_manual_lookup": len(rows) - aligned_total,
            "production_definitions_approved": 0,
        },
        "production_status": "review_required",
        "entries": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate_file", type=Path)
    parser.add_argument("target_lang_code")
    parser.add_argument("--en", required=True, type=Path, help="English-edition Wiktextract JSONL")
    parser.add_argument("--fr", required=True, type=Path, help="French-edition Wiktextract JSONL")
    parser.add_argument("--es", required=True, type=Path, help="Spanish-edition Wiktextract JSONL")
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    try:
        queue = build_queue(args.candidate_file, args.target_lang_code, {
            "en": args.en, "fr": args.fr, "es": args.es
        })
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(queue, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(json.dumps(queue["counts"], sort_keys=True))
    except (OSError, ValueError, KeyError, TypeError) as exc:
        print(f"fruchst-definition-queue: error: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
