#!/usr/bin/env python3
"""Find French and Spanish equivalents for FRUCHST headwords through English.

For every headword in each definition review queue:
  1. take the English word(s) from its English Wiktionary glosses
     (the headword itself for the English profile);
  2. look each English word up in English Wiktionary and keep its French and
     Spanish translation-table entries (with the translation sense label);
  3. as secondary evidence, keep the French and Spanish Wiktionary glosses of
     that same English word.

Nothing is selected or approved. The pivot can drift between senses, so each
equivalent records the English word, both POS values, and source lines.
"""
from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import re
import sys
import unicodedata
from datetime import datetime, timezone
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from fruchst_definition_batch import SOURCE_CATALOG, pos_key  # noqa: E402

TARGET_EDITIONS = ("fr", "es")
SKIP_TAGS = {"form-of", "alt-of", "abbreviation", "initialism", "acronym", "misspelling"}
FORM_OF = re.compile(
    r"\b(plural|singular|inflection|form|spelling|participle|tense|case|infinitive|"
    r"genitive|dative|accusative|locative|comparative|superlative|romanization|"
    r"transliteration|abbreviation) of\b",
    re.IGNORECASE,
)
PIVOT_OK = re.compile(r"^[A-Za-z][A-Za-z'\-]*(?: [A-Za-z][A-Za-z'\-]*){0,2}$")
LEADING = re.compile(r"^(?:to|a|an|the)\s+", re.IGNORECASE)


def norm(value: str) -> str:
    return unicodedata.normalize("NFC", value)


def pivots_from_gloss(gloss: str) -> list[str]:
    """Extract short English equivalents from one gloss; [] if it is not a simple equivalent."""
    if "[[" in gloss or "{{" in gloss or FORM_OF.search(gloss):
        return []
    text = gloss
    while True:
        stripped = re.sub(r"\([^()]*\)", "", text)
        if stripped == text:
            break
        text = stripped
    out = []
    for piece in re.split(r"[;,]", text):
        piece = LEADING.sub("", piece.strip().rstrip(".!?:").strip())
        piece = re.sub(r"\s+", " ", piece)
        if PIVOT_OK.match(piece):
            out.append(piece)
    return out


def collect_pivots(review: dict) -> dict[str, list[dict]]:
    """Map each queue headword to its English pivot records."""
    result = {}
    english_profile = review["language_tag"] == "en"
    for entry in review["entries"]:
        records, seen = [], set()
        if english_profile:
            records.append({"english": entry["word"], "pos": pos_key(entry.get("candidate_pos")),
                            "method": "headword_is_english"})
        else:
            for sense in entry["edition_sense_candidates"]["en"]:
                if SKIP_TAGS & set(sense.get("tags") or []):
                    continue
                gloss = (sense.get("glosses") or [""])[-1]
                for english in pivots_from_gloss(gloss):
                    key = (english, sense.get("pos"))
                    if key in seen:
                        continue
                    seen.add(key)
                    records.append({
                        "english": english,
                        "pos": sense.get("pos"),
                        "method": "english_gloss",
                        "gloss": gloss,
                        "source_line": sense.get("source_line"),
                        "sense_index": sense.get("sense_index"),
                    })
        result[entry["word"]] = records
    return result


def opener(path: Path):
    return gzip.open(path, "rt", encoding="utf-8") if path.suffix.casefold() == ".gz" else path.open(encoding="utf-8")


def translation_items(entry: dict):
    yield from entry.get("translations") or []
    for sense in entry.get("senses") or []:
        if isinstance(sense, dict):
            yield from sense.get("translations") or []


def scan_english_translations(path: Path, wanted: set[str]) -> dict[str, list[dict]]:
    """English Wiktionary: English entries -> fr/es translation-table items."""
    found: dict[str, list[dict]] = {}
    with opener(path) as stream:
        for line_no, line in enumerate(stream, 1):
            if '"en"' not in line:
                continue
            entry = json.loads(line)
            word = entry.get("word")
            if entry.get("lang_code") != "en" or not isinstance(word, str) or norm(word) not in wanted:
                continue
            pos = pos_key(entry.get("pos"))
            for item in translation_items(entry):
                if not isinstance(item, dict):
                    continue
                code = item.get("lang_code") or item.get("code")
                target = item.get("word")
                if code not in TARGET_EDITIONS or not isinstance(target, str) or not target.strip():
                    continue
                found.setdefault(norm(word), []).append({
                    "edition_target": code,
                    "english_pos": pos,
                    "word": norm(target.strip()),
                    "translation_sense": item.get("sense"),
                    "tags": sorted(t for t in item.get("tags") or [] if isinstance(t, str)),
                    "source": "enwiktionary-translations",
                    "source_line": line_no,
                })
    return found


def scan_edition_glosses(path: Path, wanted: set[str]) -> dict[str, list[dict]]:
    """French/Spanish Wiktionary: glosses written for English-language entries."""
    found: dict[str, list[dict]] = {}
    with opener(path) as stream:
        for line_no, line in enumerate(stream, 1):
            if '"en"' not in line:
                continue
            entry = json.loads(line)
            word = entry.get("word")
            if entry.get("lang_code") != "en" or not isinstance(word, str) or norm(word) not in wanted:
                continue
            pos = pos_key(entry.get("pos"))
            for index, sense in enumerate(entry.get("senses") or [], 1):
                if not isinstance(sense, dict):
                    continue
                glosses = [norm(g.strip()) for g in sense.get("glosses") or [] if isinstance(g, str) and g.strip()]
                if glosses:
                    found.setdefault(norm(word), []).append({
                        "english_pos": pos, "glosses": glosses,
                        "source_line": line_no, "sense_index": index,
                    })
    return found


def hash_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def pos_filtered(items: list[dict], pos: str | None) -> tuple[list[dict], bool]:
    """Prefer items whose English POS matches the source sense; fall back to all, flagged."""
    if pos:
        matched = [i for i in items if i["english_pos"] == pos]
        if matched:
            return matched, True
    return items, False


def build(review: dict, pivots: dict, translations: dict, glosses: dict, review_file: Path, sources: dict) -> dict:
    rows = []
    counts = {"candidate_words": 0, "words_with_english_pivot": 0, "words_with_fr_equivalent": 0,
              "words_with_es_equivalent": 0, "words_with_fr_and_es_equivalents": 0,
              "words_with_fr_and_es_pos_matched": 0, "production_definitions_approved": 0}
    for entry in review["entries"]:
        counts["candidate_words"] += 1
        word_pivots = pivots[entry["word"]]
        equivalents = {e: [] for e in TARGET_EDITIONS}
        edition_glosses = {e: [] for e in TARGET_EDITIONS}
        matched_any = {e: False for e in TARGET_EDITIONS}
        seen = set()
        for pivot in word_pivots:
            english = pivot["english"]
            items, tr_match = pos_filtered(translations.get(english, []), pivot["pos"])
            for item in items:
                key = (item["edition_target"], english, item["word"], item["translation_sense"])
                if key in seen:
                    continue
                seen.add(key)
                equivalents[item["edition_target"]].append({
                    "english": english, "source_pos": pivot["pos"], "pos_match": tr_match,
                    **{k: v for k, v in item.items() if k != "edition_target"},
                })
                matched_any[item["edition_target"]] |= tr_match
            for edition in TARGET_EDITIONS:
                items, gl_match = pos_filtered(glosses[edition].get(english, []), pivot["pos"])
                for item in items:
                    key = (edition, "gloss", english, item["source_line"], item["sense_index"])
                    if key in seen:
                        continue
                    seen.add(key)
                    edition_glosses[edition].append({"english": english, "source_pos": pivot["pos"],
                                                     "pos_match": gl_match, **item})
                    matched_any[edition] |= gl_match
        has = {e: bool(equivalents[e] or edition_glosses[e]) for e in TARGET_EDITIONS}
        counts["words_with_english_pivot"] += bool(word_pivots)
        counts["words_with_fr_equivalent"] += has["fr"]
        counts["words_with_es_equivalent"] += has["es"]
        counts["words_with_fr_and_es_equivalents"] += has["fr"] and has["es"]
        counts["words_with_fr_and_es_pos_matched"] += has["fr"] and has["es"] and all(matched_any.values())
        rows.append({
            "rank": entry["rank"],
            "word": entry["word"],
            "status": ("no_english_pivot" if not word_pivots else
                       "fr_es_equivalents_found" if has["fr"] and has["es"] else
                       "partial_equivalents" if has["fr"] or has["es"] else "no_equivalents_found"),
            "english_pivots": word_pivots,
            "equivalents": equivalents,
            "edition_glosses_of_english": edition_glosses,
            "review_status": "required",
        })
    return {
        "schema": "SLeeLa FRUCHST English-pivot equivalents v1",
        "language_tag": review["language_tag"],
        "review_queue_file": review_file.as_posix(),
        "method": (
            "English word(s) taken from each English Wiktionary gloss (form-of senses and "
            "wikitext glosses skipped; parentheticals removed; split on ';' and ','; leading "
            "to/a/an/the removed; 1-3 words kept). Each English word is looked up in English "
            "Wiktionary for French and Spanish translation-table entries, and in French and "
            "Spanish Wiktionary for their glosses of the English word. Items whose English POS "
            "matches the source sense POS are preferred; otherwise all are kept with "
            "pos_match=false. No equivalent is selected or approved."
        ),
        "sources": sources,
        "counts": counts,
        "production_status": "review_required",
        "entries": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--reviews", type=Path, required=True)
    parser.add_argument("--en", type=Path, required=True)
    parser.add_argument("--fr", type=Path, required=True)
    parser.add_argument("--es", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    review_files = sorted(args.reviews.glob("*.review.json"))
    if not review_files:
        print(f"no review queues in {args.reviews}", file=sys.stderr)
        return 2
    reviews = {p: json.loads(p.read_text(encoding="utf-8")) for p in review_files}
    pivots = {p: collect_pivots(r) for p, r in reviews.items()}
    wanted = {rec["english"] for per in pivots.values() for recs in per.values() for rec in recs}

    retrieved = datetime.now(timezone.utc).isoformat()
    paths = {"en": args.en, "fr": args.fr, "es": args.es}
    sources = {
        e: {**SOURCE_CATALOG[e], "sha256": hash_file(p), "size_bytes": p.stat().st_size,
            "retrieved_at_utc": retrieved,
            "license": "Wiktionary dual license: CC BY-SA 4.0 and GFDL; preserve attribution"}
        for e, p in paths.items()
    }
    translations = scan_english_translations(args.en, wanted)
    glosses = {e: scan_edition_glosses(paths[e], wanted) for e in TARGET_EDITIONS}

    args.output.mkdir(parents=True, exist_ok=True)
    for path, review in reviews.items():
        doc = build(review, pivots[path], translations, glosses, path, sources)
        out = args.output / path.name.replace(".review.json", ".pivot.json")
        out.write_text(json.dumps(doc, ensure_ascii=False, indent=1) + "\n", encoding="utf-8")
        print(json.dumps({"language": review["language_tag"], "file": out.name, **doc["counts"]}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
