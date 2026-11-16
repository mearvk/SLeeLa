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


def sense_boxes(items: list[dict]) -> list[dict]:
    """Group translation items into English Wiktionary sense boxes with fr and es side by side."""
    boxes: dict[tuple, dict] = {}
    for item in items:
        key = (item["english_pos"], item["translation_sense"], item["source_line"])
        box = boxes.setdefault(key, {"pos": key[0], "sense": key[1], "source_line": key[2], "fr": [], "es": []})
        value = {"word": item["word"], "tags": item["tags"]} if item["tags"] else item["word"]
        if value not in box[item["edition_target"]]:
            box[item["edition_target"]].append(value)
    return list(boxes.values())


def lookup_for(english: str, wanted_pos: set, translations: dict, glosses: dict) -> dict:
    """Lookup record for one English word, limited to the POS its pivots need.

    If some pivot POS has no matching English entry, every POS is kept so that
    nothing usable is dropped; pivots record per-edition pos_match flags.
    """
    boxes = sense_boxes(translations.get(english, []))
    edition_glosses = {e: list(glosses[e].get(english, [])) for e in TARGET_EDITIONS}
    # Filter each source list separately, and only when every wanted POS is
    # present in it; a pivot with unknown POS (None) keeps everything.
    if wanted_pos and all(wanted_pos):
        if all(wanted_pos <= {b["pos"] for b in boxes if b[e]} for e in TARGET_EDITIONS):
            boxes = [b for b in boxes if b["pos"] in wanted_pos]
        for e, items in edition_glosses.items():
            if wanted_pos <= {g["english_pos"] for g in items}:
                edition_glosses[e] = [g for g in items if g["english_pos"] in wanted_pos]
    return {
        "translation_senses": boxes,
        "glosses": {e: [{"pos": g["english_pos"], "glosses": g["glosses"], "source_line": g["source_line"],
                         "sense_index": g["sense_index"]} for g in v] for e, v in edition_glosses.items()},
    }


def build(review: dict, pivots: dict, translations: dict, glosses: dict, review_file: Path, sources: dict) -> dict:
    needed: dict[str, set] = {}
    for entry in review["entries"]:
        for pivot in pivots[entry["word"]]:
            needed.setdefault(pivot["english"], set()).add(pivot["pos"])
    lookup = {}
    for english, pos_set in sorted(needed.items()):
        record = lookup_for(english, pos_set, translations, glosses)
        if record["translation_senses"] or any(record["glosses"].values()):
            lookup[english] = record

    def has(record: dict, edition: str, pos: str | None) -> tuple[bool, bool]:
        found = [b["pos"] for b in record["translation_senses"] if b[edition]]
        found += [g["pos"] for g in record["glosses"][edition]]
        return bool(found), bool(pos) and pos in found

    rows = []
    counts = {"candidate_words": 0, "words_with_english_pivot": 0, "words_with_fr_equivalent": 0,
              "words_with_es_equivalent": 0, "words_with_fr_and_es_equivalents": 0,
              "words_with_fr_and_es_pos_matched": 0, "english_lookup_words": len(lookup),
              "production_definitions_approved": 0}
    for entry in review["entries"]:
        counts["candidate_words"] += 1
        word_pivots, any_ed, matched_ed = [], {e: False for e in TARGET_EDITIONS}, {e: False for e in TARGET_EDITIONS}
        for pivot in pivots[entry["word"]]:
            record = lookup.get(pivot["english"])
            flags = {}
            for edition in TARGET_EDITIONS:
                found, matched = has(record, edition, pivot["pos"]) if record else (False, False)
                flags[edition] = matched if found else None
                any_ed[edition] |= found
                matched_ed[edition] |= matched
            word_pivots.append({**pivot, "pos_match": flags})
        both = all(any_ed.values())
        counts["words_with_english_pivot"] += bool(word_pivots)
        counts["words_with_fr_equivalent"] += any_ed["fr"]
        counts["words_with_es_equivalent"] += any_ed["es"]
        counts["words_with_fr_and_es_equivalents"] += both
        counts["words_with_fr_and_es_pos_matched"] += all(matched_ed.values())
        rows.append({
            "rank": entry["rank"],
            "word": entry["word"],
            "status": ("no_english_pivot" if not word_pivots else
                       "fr_es_equivalents_found" if both else
                       "partial_equivalents" if any(any_ed.values()) else "no_equivalents_found"),
            "english_pivots": word_pivots,
            "review_status": "required",
        })
    return {
        "schema": "SLeeLa FRUCHST English-pivot equivalents v2",
        "language_tag": review["language_tag"],
        "review_queue_file": review_file.as_posix(),
        "method": (
            "English word(s) taken from each English Wiktionary gloss (form-of senses and "
            "wikitext glosses skipped; parentheticals removed; split on ';' and ','; leading "
            "to/a/an/the removed; 1-3 words kept). Each English word is looked up once in "
            "english_lookup: translation_senses are English Wiktionary translation boxes with "
            "their French and Spanish words side by side; glosses are the French and Spanish "
            "Wiktionary definitions of the English word. Only POS needed by a pivot are kept "
            "unless a pivot POS has no match, in which case all POS are kept. Each pivot "
            "carries pos_match per edition (true, false, or null when nothing was found). "
            "No equivalent is selected or approved."
        ),
        "sources": sources,
        "counts": counts,
        "production_status": "review_required",
        "entries": rows,
        "english_lookup": lookup,
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
        out.write_text(json.dumps(doc, ensure_ascii=False, separators=(",", ":")) + "\n", encoding="utf-8")
        print(json.dumps({"language": review["language_tag"], "file": out.name, **doc["counts"]}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
