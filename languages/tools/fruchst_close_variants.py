#!/usr/bin/env python3
"""Find close English definitions for FRUCHST headwords that have none.

For each headword whose review queue has no English Wiktionary sense, look for
a close variant in English Wiktionary (same target language) and record that
variant's English glosses as a *close* definition. Methods, closest first:

  case_variant          same word, different letter case
  orthographic_variant  same word after removing optional marks: Latin/Cyrillic/
                        Greek diacritics, Arabic short vowels and tatweel, and
                        unifying Arabic-script letter variants (ي/ى/ی, ك/ک, أ/إ/آ/ا, ة/ه)
  inflected_form        the word is listed in another entry's inflection table
                        ("forms"); the close definition is that lemma's gloss
  transliteration       Uzbek only: official Cyrillic-to-Latin conversion
  clitic_stripped       Arabic only: a leading و/ف/ب/ل/ك and/or ال removed

Every match keeps the variant word, method, source line and tags. A close
definition is not the headword's own definition; all rows stay review-required.
"""
from __future__ import annotations

import argparse
import gzip
import json
import sys
import unicodedata
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from fruchst_definition_batch import SOURCE_CATALOG, TARGETS, norm, pos_key, wiktionary_codes  # noqa: E402

METHOD_ORDER = ("case_variant", "orthographic_variant", "inflected_form", "transliteration", "clitic_stripped")
SKIP_FORM_TAGS = {"romanization", "transliteration", "table-tags", "inflection-template", "class", "canonical"}
MAX_MATCHES = 20

ARABIC_MARKS = {chr(c) for c in range(0x064B, 0x0660)} | {"\u0670", "\u0640"}
ARABIC_LETTERS = str.maketrans({
    "ى": "ي", "ی": "ي", "ې": "ي", "ك": "ک", "أ": "ا", "إ": "ا", "آ": "ا", "ٱ": "ا", "ة": "ه", "ۀ": "ه",
})
STRIP_SCRIPTS = ("LATIN", "CYRILLIC", "GREEK")
APOSTROPHES = str.maketrans({"'": "ʻ", "‘": "ʻ", "’": "ʼ", "`": "ʻ", "ʹ": "ʻ"})

UZ_CYR = {
    "а": "a", "б": "b", "в": "v", "г": "g", "ғ": "gʻ", "д": "d", "е": "e", "ё": "yo", "ж": "j",
    "з": "z", "и": "i", "й": "y", "к": "k", "қ": "q", "л": "l", "м": "m", "н": "n", "о": "o",
    "п": "p", "р": "r", "с": "s", "т": "t", "у": "u", "ў": "oʻ", "ф": "f", "х": "x", "ҳ": "h",
    "ц": "ts", "ч": "ch", "ш": "sh", "ъ": "ʼ", "ь": "", "э": "e", "ю": "yu", "я": "ya",
}
UZ_VOWELS = set("аеёиоуўэюя")
AR_PREFIXES = ("وال", "فال", "بال", "كال", "لل", "ال", "و", "ف", "ب", "ل", "ك")


def strip_key(word: str) -> str:
    """Case-fold and remove optional marks, without touching abugida vowel signs."""
    out = []
    for ch in unicodedata.normalize("NFD", word.casefold()):
        if ch in ARABIC_MARKS:
            continue
        if unicodedata.category(ch) == "Mn" and out:
            base = unicodedata.name(out[-1], "")
            if base.startswith(STRIP_SCRIPTS):
                continue
        out.append(ch)
    return unicodedata.normalize("NFC", "".join(out)).translate(ARABIC_LETTERS).translate(APOSTROPHES)


def uz_latin(word: str) -> str:
    """Uzbek Cyrillic to Latin (1995 official alphabet); 'е' is 'ye' word-initially and after vowels."""
    out = []
    for i, ch in enumerate(word.lower()):
        if ch == "е" and (i == 0 or word.lower()[i - 1] in UZ_VOWELS):
            out.append("ye")
        else:
            out.append(UZ_CYR.get(ch, ch))
    return "".join(out)


def ar_stems(word: str) -> list[str]:
    stems = []
    for prefix in AR_PREFIXES:
        if word.startswith(prefix) and len(word) - len(prefix) >= 2:
            stems.append(word[len(prefix):])
    return stems


def sense_summaries(entry: dict) -> list[dict]:
    out = []
    for index, sense in enumerate(entry.get("senses") or [], 1):
        if not isinstance(sense, dict):
            continue
        glosses = [norm(g.strip()) for g in sense.get("glosses") or [] if isinstance(g, str) and g.strip()]
        if not glosses:
            continue
        item = {"sense_index": index, "glosses": glosses}
        tags = sorted(t for t in sense.get("tags") or [] if isinstance(t, str))
        if tags:
            item["tags"] = tags
        form_of = [f.get("word") for f in sense.get("form_of") or [] if isinstance(f, dict) and f.get("word")]
        if form_of:
            item["form_of"] = form_of
        out.append(item)
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--reviews", type=Path, required=True)
    parser.add_argument("--en", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    reviews = {}
    for path in sorted(args.reviews.glob("*.review.json")):
        doc = json.loads(path.read_text(encoding="utf-8"))
        if doc["language_tag"] in TARGETS:
            reviews[path] = doc

    # Lookup keys: (wiktionary_code, key) -> [(tag, headword, method)]
    exact_keys: dict[tuple, list] = {}
    strip_keys: dict[tuple, list] = {}
    missing: dict[str, list[str]] = {}
    for doc in reviews.values():
        tag = doc["language_tag"]
        words = [e["word"] for e in doc["entries"] if not e["edition_sense_candidates"]["en"]]
        if tag == "en":
            words = []
        missing[tag] = words
        for code in wiktionary_codes(tag):
            for word in words:
                exact_keys.setdefault((code, word.casefold()), []).append((tag, word, "case_variant"))
                strip_keys.setdefault((code, strip_key(word)), []).append((tag, word, "orthographic_variant"))
                if tag == "uz":
                    strip_keys.setdefault((code, strip_key(uz_latin(word))), []).append((tag, word, "transliteration"))
                if tag == "ar":
                    for stem in ar_stems(word):
                        strip_keys.setdefault((code, strip_key(stem)), []).append((tag, word, "clitic_stripped"))
    codes = {code for code, _ in exact_keys}
    form_keys = {k: [(t, w, "inflected_form") for t, w, _ in v] for k, v in exact_keys.items()}

    matches: dict[tuple[str, str], list[dict]] = {}

    def add(tag, word, method, entry, line_no, extra=None):
        bucket = matches.setdefault((tag, word), [])
        if len(bucket) >= MAX_MATCHES:
            return
        senses = sense_summaries(entry)
        if not senses:
            return
        record = {"method": method, "variant": norm(entry["word"]), "pos": pos_key(entry.get("pos")),
                  "lang_code": entry.get("lang_code"), "source_line": line_no, "english_senses": senses}
        if extra:
            record.update(extra)
        if record not in bucket:
            bucket.append(record)

    opener = gzip.open if args.en.suffix.casefold() == ".gz" else open
    with opener(args.en, "rt", encoding="utf-8") as stream:
        for line_no, line in enumerate(stream, 1):
            entry = json.loads(line) if line.strip() else {}
            code = entry.get("lang_code")
            word = entry.get("word")
            if code not in codes or not isinstance(word, str):
                continue
            word = norm(word)
            for tag, head, method in exact_keys.get((code, word.casefold()), []):
                if head != word:
                    add(tag, head, method, entry, line_no)
            seen_exact = {(t, h) for t, h, _ in exact_keys.get((code, word.casefold()), [])}
            for tag, head, method in strip_keys.get((code, strip_key(word)), []):
                if method == "orthographic_variant" and (tag, head) in seen_exact:
                    continue
                add(tag, head, method, entry, line_no)
            for form in entry.get("forms") or []:
                if not isinstance(form, dict) or not isinstance(form.get("form"), str):
                    continue
                tags = set(form.get("tags") or [])
                if tags & SKIP_FORM_TAGS:
                    continue
                surface = norm(form["form"].strip())
                extra = {"form_tags": sorted(tags), "listed_form": surface}
                exact_hits = form_keys.get((code, surface.casefold()), [])
                for tag, head, method in exact_hits:
                    add(tag, head, method, entry, line_no, extra)
                exact_heads = {(t, h) for t, h, _ in exact_hits}
                for tag, head, via in strip_keys.get((code, strip_key(surface)), []):
                    if (tag, head) not in exact_heads:
                        add(tag, head, "inflected_form", entry, line_no, {**extra, "via": via})

    args.output.mkdir(parents=True, exist_ok=True)
    rank = {m: i for i, m in enumerate(METHOD_ORDER)}
    for path, doc in reviews.items():
        tag = doc["language_tag"]
        rows, by_method = [], {m: 0 for m in METHOD_ORDER}
        missing_set = set(missing[tag])
        for entry in doc["entries"]:
            if entry["word"] not in missing_set:
                continue
            found = sorted(matches.get((tag, entry["word"]), []), key=lambda r: (rank[r["method"]], r["source_line"]))
            best = found[0]["method"] if found else None
            if best:
                by_method[best] += 1
            rows.append({"rank": entry["rank"], "word": entry["word"],
                         "status": "close_definition_found" if found else "no_close_variant",
                         "closest_method": best, "close_matches": found, "review_status": "required"})
        counts = {
            "candidate_words": len(doc["entries"]),
            "words_without_english": len(rows),
            "words_with_close_definition": sum(1 for r in rows if r["close_matches"]),
            "by_closest_method": by_method,
            "production_definitions_approved": 0,
        }
        out = {
            "schema": "SLeeLa FRUCHST close-variant English definitions v1",
            "language_tag": tag,
            "review_queue_file": path.as_posix(),
            "wiktionary_codes": list(wiktionary_codes(tag)),
            "method_order": list(METHOD_ORDER),
            "policy": (
                "Only headwords with no English Wiktionary sense are listed. A close definition is the "
                "English gloss of a variant (case, optional marks, inflection-table form, Uzbek "
                "transliteration, or Arabic clitic removal) in the same Wiktionary language. It is not "
                "the headword's own definition; nothing is selected or approved."
            ),
            "source": {**SOURCE_CATALOG["en"],
                       "license": "Wiktionary dual license: CC BY-SA 4.0 and GFDL; preserve attribution"},
            "counts": counts,
            "production_status": "review_required",
            "entries": rows,
        }
        target = args.output / path.name.replace(".review.json", ".variants.json")
        target.write_text(json.dumps(out, ensure_ascii=False, separators=(",", ":")) + "\n", encoding="utf-8")
        print(json.dumps({"language": tag, **counts}, ensure_ascii=False, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
