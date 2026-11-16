#!/usr/bin/env python3
"""Stage EN/FR/ES Wiktionary gloss candidates for every staged FRUCHST language.

Streams each Kaikki/Wiktextract JSONL(.gz) source once and retains all exact
NFC headword/POS gloss candidates. Wikidata QIDs are optional alignment hints;
this tool never approves a definition or chooses a sense.
"""
from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import re
import unicodedata
from datetime import datetime, timezone
from pathlib import Path

EDITIONS = ("en", "fr", "es")
SOURCE_CATALOG = {
    "en": {
        "url": "https://kaikki.org/dictionary/raw-wiktextract-data.jsonl.gz",
        "dump_revision": "2026-10-01",
    },
    "fr": {
        "url": "https://kaikki.org/frwiktionary/raw-wiktextract-data.jsonl.gz",
        "dump_revision": "2026-10-01",
    },
    "es": {
        "url": "https://kaikki.org/eswiktionary/raw-wiktextract-data.jsonl.gz",
        "dump_revision": "2026-11-01",
    },
}

# Candidate file tags map explicitly to the language codes used by Wiktionary.
# zh-Hans is sourced from FrequencyWords' zh_cn (Simplified Chinese) list, while
# Wiktionary uses the broader "zh" language code; this mapping is therefore
# disclosed in every resulting queue and requires script review.
TARGETS = (
    "en", "zh-Hans", "hi", "es", "ar", "fr", "bn", "pt", "ru", "id",
    "ur", "de", "ja", "vi", "te", "tr", "tl", "ta", "fa", "ko", "th",
    "it", "pl", "uk", "ml", "ms", "nl", "ro", "kk", "si", "af",
    "arz", "mr", "ha", "pnb-Arab", "sw", "jv", "gu", "am", "kn", "bho",
    "yo", "ln", "ary", "or", "sd", "pa-Guru", "su", "ig", "ne", "uz",
    "zu", "skr", "so", "as", "ceb", "mag", "xh", "mai",
)
WIKTIONARY_LANG_CODES = {
    "en": "en",
    "zh-Hans": "zh",
    "hi": "hi",
    "es": "es",
    "ar": "ar",
    "fr": "fr",
    "bn": "bn",
    "pt": "pt",
    "ru": "ru",
    "id": "id",
    "ur": "ur",
    "de": "de",
    "ja": "ja",
    "vi": "vi",
    "te": "te",
    "tr": "tr",
    "tl": "tl",
    "ta": "ta",
    "fa": "fa",
    "ko": "ko",
    "th": "th",
    "it": "it",
    "pl": "pl",
    "uk": "uk",
    "ml": "ml",
    "ms": "ms",
    "nl": "nl",
    "ro": "ro",
    "kk": "kk",
    "si": "si",
    "af": "af",
    "arz": "arz",
    "mr": "mr",
    "ha": "ha",
    "pnb-Arab": "pnb",
    "sw": "sw",
    "jv": "jv",
    "gu": "gu",
    "am": "am",
    "kn": "kn",
    "bho": "bho",
    "yo": "yo",
    "ln": "ln",
    "ary": "ary",
    "or": "or",
    "sd": "sd",
    "pa-Guru": "pa",
    "su": "su",
    "ig": "ig",
    "ne": "ne",
    "uz": "uz",
    "zu": "zu",
    "skr": "skr",
    "so": "so",
    "as": "as",
    "ceb": "ceb",
    "mag": "mag",
    "xh": "xh",
    "mai": "mai",
}
TARGET_MAPPING_NOTES = {
    "zh-Hans": (
        "Wiktionary entries use the broad 'zh' code; the candidate word list "
        "is the Simplified Chinese zh_cn frequency set. Verify script/variety "
        "for every selected sense before approval."
    ),
    "pnb-Arab": (
        "Wiktionary uses the pnb language code. Verify Western Punjabi identity "
        "and Shahmukhi script for every selected sense."
    ),
    "pa-Guru": (
        "Wiktionary uses the broad pa code. Verify Eastern Punjabi identity "
        "and Gurmukhi script for every selected sense."
    ),
    "bho": (
        "The frequency corpus is from Wikimedia's bh Wikipedia project, mapped "
        "to Bhojpuri. Verify every headword's language identity."
    ),
}
POS_ALIASES = {
    "adj": "adjective",
    "adv": "adverb",
    "aux": "auxiliary",
    "noun": "noun",
    "verb": "verb",
    "pron": "pronoun",
    "det": "determiner",
    "prep": "preposition",
    "adp": "preposition",
    "postp": "postposition",
    "conj": "conjunction",
    "cconj": "conjunction",
    "sconj": "conjunction",
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
    return sorted({
        value for value in values
        if isinstance(value, str) and re.fullmatch(r"Q[1-9][0-9]*", value)
    })


def candidate_path_for(directory: Path, language_tag: str) -> Path:
    """Find exactly one known candidate file for a registered language tag."""
    matches = [
        path for prefix in ("wordhoard", "frequencywords", "wikipedia")
        if (path := directory / f"{prefix}-{language_tag}.json").is_file()
    ]
    if len(matches) != 1:
        raise ValueError(
            f"{directory}: expected exactly one wordhoard/frequencywords file "
            f"for {language_tag}, found {len(matches)}"
        )
    return matches[0]


def load_candidates(directory: Path):
    docs, words, wanted, candidate_paths = {}, {}, {}, {}
    if set(WIKTIONARY_LANG_CODES) != set(TARGETS):
        raise ValueError("TARGETS and WIKTIONARY_LANG_CODES must cover the same tags")

    for language_tag in TARGETS:
        path = candidate_path_for(directory, language_tag)
        doc = json.loads(path.read_text(encoding="utf-8"))
        entries = doc.get("entries", [])
        if doc.get("language_tag") != language_tag:
            raise ValueError(f"{path}: language_tag does not match {language_tag}")
        if doc.get("candidate_count") != 1000 or len(entries) != 1000:
            raise ValueError(f"{path}: expected exactly 1,000 candidates")

        mapped = {}
        for expected_rank, item in enumerate(entries, 1):
            if not isinstance(item, dict):
                raise ValueError(f"{path}: candidate {expected_rank} is not an object")
            if item.get("candidate_rank") != expected_rank:
                raise ValueError(f"{path}: candidate ranks must be contiguous 1..1000")
            raw_word = item.get("word")
            if not isinstance(raw_word, str) or not raw_word.strip():
                raise ValueError(f"{path}: candidate {expected_rank} has an invalid word")
            word = norm(raw_word)
            if word != raw_word:
                raise ValueError(f"{path}: candidate {expected_rank} is not NFC-normalized")
            if word in mapped:
                raise ValueError(f"{path}: duplicate NFC headword {word!r}")
            mapped[word] = item

        docs[language_tag] = doc
        words[language_tag] = mapped
        wanted[language_tag] = set(mapped)
        candidate_paths[language_tag] = path

    return docs, words, wanted, candidate_paths


def hash_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def extract_edition(path: Path, wanted: dict[str, set[str]]):
    """Extract target-language entries by explicit Wiktionary language code."""
    tags_by_wiktionary_code = {}
    for language_tag, wiktionary_code in WIKTIONARY_LANG_CODES.items():
        tags_by_wiktionary_code.setdefault(wiktionary_code, []).append(language_tag)

    found = {language_tag: {} for language_tag in TARGETS}
    opener = gzip.open if path.suffix.casefold() == ".gz" else open
    with opener(path, "rt", encoding="utf-8") as stream:
        for line_no, line in enumerate(stream, 1):
            if not line.strip():
                continue
            try:
                entry = json.loads(line)
            except json.JSONDecodeError as exc:
                raise ValueError(f"{path}:{line_no}: invalid JSON: {exc}") from exc

            matching_tags = tags_by_wiktionary_code.get(entry.get("lang_code"), ())
            if not matching_tags:
                continue
            raw_word = entry.get("word")
            if not isinstance(raw_word, str):
                continue
            word = norm(raw_word)
            tags = [tag for tag in matching_tags if word in wanted[tag]]
            if not tags:
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
                glosses = [
                    norm(gloss.strip())
                    for gloss in glosses
                    if isinstance(gloss, str) and gloss.strip()
                ]
                if not glosses:
                    continue

                qids = qids_for(sense)
                record = {
                    "glosses": glosses,
                    "qids": qids,
                    "tags": sorted(
                        tag for tag in (sense.get("tags") or [])
                        if isinstance(tag, str)
                    ),
                    "senseid": sense.get("senseid", []),
                    "source_line": line_no,
                    "sense_index": sense_index,
                }
                for language_tag in tags:
                    by_word = found[language_tag].setdefault(word, {})
                    if qids:
                        for qid in qids:
                            by_word.setdefault((qid, pos), []).append(record)
                    else:
                        # QIDs are optional metadata; never discard an otherwise
                        # usable gloss just because Wiktextract has no QID.
                        by_word.setdefault(("", pos), []).append(record)
    return found


def build_queue(
    language_tag,
    doc,
    words,
    by_edition,
    source_meta,
    candidate_file=None,
):
    rows, words_with_matches, total_matches = [], 0, 0
    target_wiktionary_code = WIKTIONARY_LANG_CODES[language_tag]
    if candidate_file is None:
        candidate_file = Path(f"languages/datasets/frequency-candidates/wordhoard-{language_tag}.json")

    for rank, (word, candidate) in enumerate(words.items(), 1):
        wanted_pos = pos_key(candidate.get("pos"))
        groups = {}
        for edition in EDITIONS:
            raw = by_edition[edition].get(language_tag, {}).get(word, {})
            # A null POS from the frequency source means "unknown", not a
            # license to discard every sense. Retain all POS values in that case.
            groups[edition] = {
                key: senses for key, senses in raw.items()
                if not wanted_pos or key[1] == wanted_pos
            }

        common = (
            set(groups["en"]) & set(groups["fr"]) & set(groups["es"])
        )
        common = {
            (qid, pos) for qid, pos in common
            if qid and (not wanted_pos or pos == wanted_pos)
        }

        aligned = []
        for qid, pos in sorted(common):
            total_matches += 1
            aligned.append({
                "wikidata_sense_qid": qid,
                "pos": pos,
                "definitions": {
                    edition: [
                        sense["glosses"]
                        for sense in groups[edition][(qid, pos)]
                    ]
                    for edition in EDITIONS
                },
                "source_evidence": {
                    edition: [{
                        "edition": edition,
                        "source_line": sense["source_line"],
                        "sense_index": sense["sense_index"],
                        "senseid": sense["senseid"],
                        "tags": sense["tags"],
                    } for sense in groups[edition][(qid, pos)]]
                    for edition in EDITIONS
                },
                "review_status": "required",
            })

        edition_candidates = {}
        for edition in EDITIONS:
            candidates = []
            for (qid, pos), senses in sorted(groups[edition].items()):
                for sense in senses:
                    candidates.append({
                        "wikidata_sense_qids": [qid] if qid else [],
                        "pos": pos,
                        "glosses": sense["glosses"],
                        "tags": sense["tags"],
                        "senseid": sense["senseid"],
                        "source_line": sense["source_line"],
                        "sense_index": sense["sense_index"],
                    })
            edition_candidates[edition] = candidates

        if aligned:
            words_with_matches += 1
        rows.append({
            "rank": rank,
            "word": word,
            "candidate_pos": candidate.get("pos"),
            "candidate_pos_known": bool(wanted_pos),
            "frequency_count": candidate.get("frequency_count"),
            "definition_status": (
                "qid_pos_candidates_found" if aligned else
                "edition_gloss_candidates_found"
                if all(edition_candidates[edition] for edition in EDITIONS) else
                "manual_lookup_required"
            ),
            "aligned_candidates": aligned,
            "edition_sense_candidates": edition_candidates,
            "edition_candidate_counts": {
                edition: len(edition_candidates[edition])
                for edition in EDITIONS
            },
        })

    words_with_all_edition_glosses = sum(
        1 for row in rows
        if all(row["edition_sense_candidates"][edition] for edition in EDITIONS)
    )
    return {
        "schema": "SLeeLa FRUCHST definition review queue v4",
        "language_tag": doc["language_tag"],
        "target_wiktionary_lang_code": target_wiktionary_code,
        "target_language_mapping_note": TARGET_MAPPING_NOTES.get(language_tag),
        "frequency_candidate_file": candidate_file.as_posix(),
        "frequency_source": doc["source"],
        "definition_sources": {
            edition: {
                "project": "Kaikki/Wiktextract extraction of Wiktionary",
                "url": source_meta[edition]["url"],
                "dump_revision": source_meta[edition]["dump_revision"],
                "file": source_meta[edition]["file"],
                "sha256": source_meta[edition]["sha256"],
                "size_bytes": source_meta[edition]["size_bytes"],
                "retrieved_at_utc": source_meta[edition]["retrieved_at_utc"],
                "license": (
                    "Wiktionary dual license: CC BY-SA 4.0 and GFDL; "
                    "preserve attribution and verify edition notices"
                ),
            } for edition in EDITIONS
        },
        "alignment_policy": (
            "Exact NFC headword and explicit target-language-to-Wiktionary-code mapping. "
            "Candidate POS is applied when supplied; if the frequency source has no POS, "
            "all matching POS senses are retained. All edition glosses are preserved "
            "independently, including senses without Wikidata QIDs. Shared sense-level "
            "QIDs create optional cross-edition alignment candidates only. Every "
            "candidate remains for human review; no sense is auto-selected or approved."
        ),
        "counts": {
            "candidate_words": len(rows),
            "words_with_qid_pos_candidates": words_with_matches,
            "words_with_edition_gloss_candidates": words_with_all_edition_glosses,
            "words_requiring_manual_lookup": len(rows) - words_with_all_edition_glosses,
            "qid_pos_candidate_triples": total_matches,
            "production_definitions_approved": 0,
        },
        "production_status": "review_required",
        "entries": rows,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--candidates", type=Path, required=True)
    parser.add_argument("--en", type=Path, required=True)
    parser.add_argument("--fr", type=Path, required=True)
    parser.add_argument("--es", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    docs, words, wanted, candidate_paths = load_candidates(args.candidates)
    source_paths = {"en": args.en, "fr": args.fr, "es": args.es}
    retrieved = datetime.now(timezone.utc).isoformat()
    source_meta = {
        edition: {
            **SOURCE_CATALOG[edition],
            "file": str(path),
            "sha256": hash_file(path),
            "size_bytes": path.stat().st_size,
            "retrieved_at_utc": retrieved,
        }
        for edition, path in source_paths.items()
    }

    by_edition = {
        edition: extract_edition(path, wanted)
        for edition, path in source_paths.items()
    }
    args.output.mkdir(parents=True, exist_ok=True)
    for language_tag in TARGETS:
        candidate_file = candidate_paths[language_tag]
        queue = build_queue(
            language_tag,
            docs[language_tag],
            words[language_tag],
            by_edition,
            source_meta,
            candidate_file,
        )
        output_path = args.output / f"{candidate_file.stem}.review.json"
        output_path.write_text(
            json.dumps(queue, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
        )
        print(json.dumps({
            "language": language_tag,
            "candidate_file": candidate_file.name,
            **queue["counts"],
        }, sort_keys=True))


if __name__ == "__main__":
    main()
