#!/usr/bin/env python3
"""Stage aligned EN/FR/ES Wiktionary gloss candidates for all first-batch languages.

Streams each Kaikki/Wiktextract JSONL(.gz) source once and retains only
candidate headwords. It never approves a definition or chooses a sense.
"""
from __future__ import annotations
import argparse, gzip, hashlib, json, re, unicodedata
from datetime import datetime, timezone
from pathlib import Path

EDITIONS = ("en", "fr", "es")
TARGETS = ("en", "fr", "es", "de")
POS_ALIASES = {
    "adj": "adjective", "adv": "adverb", "aux": "auxiliary",
    "noun": "noun", "verb": "verb", "pron": "pronoun",
    "det": "determiner", "prep": "preposition", "adp": "preposition",
    "postp": "postposition", "conj": "conjunction", "cconj": "conjunction",
    "sconj": "conjunction", "intj": "interjection", "num": "numeral",
    "part": "particle", "proper": "proper noun", "propn": "proper noun",
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

def load_candidates(directory: Path):
    docs, words, wanted = {}, {}, {}
    for code in TARGETS:
        path = directory / f"wordhoard-{code}.json"
        doc = json.loads(path.read_text(encoding="utf-8"))
        entries = doc.get("entries", [])
        if doc.get("candidate_count") != 1000 or len(entries) != 1000:
            raise ValueError(f"{path}: expected exactly 1,000 candidates")
        mapped = {}
        for item in entries:
            word = norm(item["word"])
            if word in mapped:
                raise ValueError(f"{path}: duplicate NFC headword {word!r}")
            mapped[word] = item
        docs[code], words[code], wanted[code] = doc, mapped, set(mapped)
    return docs, words, wanted

def hash_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()

def extract_edition(path: Path, wanted: dict[str, set[str]]):
    found = {code: {} for code in TARGETS}
    opener = gzip.open if path.suffix.casefold() == ".gz" else open
    with opener(path, "rt", encoding="utf-8") as stream:
        for line_no, line in enumerate(stream, 1):
            if not line.strip():
                continue
            try:
                entry = json.loads(line)
            except json.JSONDecodeError as exc:
                raise ValueError(f"{path}:{line_no}: invalid JSON: {exc}") from exc
            code = entry.get("lang_code")
            if code not in wanted:
                continue
            raw_word = entry.get("word")
            if not isinstance(raw_word, str):
                continue
            word = norm(raw_word)
            if word not in wanted[code]:
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
                for qid in qids_for(sense):
                    by_word = found[code].setdefault(word, {})
                    by_word.setdefault((qid, pos), []).append({
                        "glosses": glosses,
                        "tags": sorted(t for t in (sense.get("tags") or []) if isinstance(t, str)),
                        "senseid": sense.get("senseid", []),
                        "source_line": line_no,
                        "sense_index": sense_index,
                    })
    return found

def build_queue(code, doc, words, by_edition, source_meta):
    rows, words_with_matches, total_matches = [], 0, 0
    for rank, (word, candidate) in enumerate(words.items(), 1):
        wanted_pos = pos_key(candidate.get("pos"))
        groups = {}
        for edition in EDITIONS:
            raw = by_edition[edition].get(word, {})
            groups[edition] = {key: senses for key, senses in raw.items() if key[1] == wanted_pos}
        common = set(groups["en"]) & set(groups["fr"]) & set(groups["es"])
        aligned = []
        for qid, pos in sorted(common):
            total_matches += 1
            aligned.append({
                "wikidata_sense_qid": qid,
                "pos": pos,
                "definitions": {
                    edition: [sense["glosses"] for sense in groups[edition][(qid, pos)]]
                    for edition in EDITIONS
                },
                "source_evidence": {
                    edition: [{
                        "edition": edition, "source_line": sense["source_line"],
                        "sense_index": sense["sense_index"], "senseid": sense["senseid"],
                        "tags": sense["tags"],
                    } for sense in groups[edition][(qid, pos)]]
                    for edition in EDITIONS
                },
                "review_status": "required",
            })
        if aligned:
            words_with_matches += 1
        rows.append({
            "rank": rank, "word": word, "candidate_pos": candidate.get("pos"),
            "frequency_count": candidate.get("frequency_count"),
            "definition_status": "qid_pos_candidates_found" if aligned else "manual_lookup_required",
            "aligned_candidates": aligned,
            "edition_candidate_counts": {
                edition: sum(len(v) for v in by_edition[edition].get(word, {}).values())
                for edition in EDITIONS
            },
        })
    return {
        "schema": "SLeeLa FRUCHST definition review queue v2",
        "language_tag": doc["language_tag"],
        "target_wiktionary_lang_code": code,
        "frequency_candidate_file": f"languages/datasets/frequency-candidates/wordhoard-{code}.json",
        "frequency_source": doc["source"],
        "definition_sources": {
            edition: {
                "project": "Kaikki/Wiktextract extraction of Wiktionary",
                "file": source_meta[edition]["file"],
                "sha256": source_meta[edition]["sha256"],
                "size_bytes": source_meta[edition]["size_bytes"],
                "retrieved_at_utc": source_meta[edition]["retrieved_at_utc"],
                "license": "Wiktionary dual license: CC BY-SA 4.0 and GFDL; preserve attribution and verify edition notices",
            } for edition in EDITIONS
        },
        "alignment_policy": (
            "Exact NFC headword, exact target language, normalized candidate POS, "
            "and shared sense-level Wikidata QID across all three editions. Every "
            "matched sense remains a review candidate; no sense is auto-selected "
            "or approved. Multiple source senses are retained separately."
        ),
        "counts": {
            "candidate_words": len(rows),
            "words_with_qid_pos_candidates": words_with_matches,
            "words_requiring_manual_lookup": len(rows) - words_with_matches,
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
    docs, words, wanted = load_candidates(args.candidates)
    source_paths = {"en": args.en, "fr": args.fr, "es": args.es}
    retrieved = datetime.now(timezone.utc).isoformat()
    source_meta = {
        edition: {"file": str(path), "sha256": hash_file(path),
                  "size_bytes": path.stat().st_size, "retrieved_at_utc": retrieved}
        for edition, path in source_paths.items()
    }
    by_edition = {edition: extract_edition(path, wanted) for edition, path in source_paths.items()}
    args.output.mkdir(parents=True, exist_ok=True)
    for code in TARGETS:
        queue = build_queue(code, docs[code], words[code], by_edition, source_meta)
        out = args.output / f"wordhoard-{code}.review.json"
        out.write_text(json.dumps(queue, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(json.dumps({"language": code, **queue["counts"]}, sort_keys=True))

if __name__ == "__main__":
    main()
