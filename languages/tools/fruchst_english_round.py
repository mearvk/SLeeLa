#!/usr/bin/env python3
"""English round: add every English definition English Wiktionary can supply.

Add-only companion to the review queues. It never edits or removes a review
gloss; it writes one ``<stem>.english.json`` per profile that collects, ranks
and explains all English definition evidence for each headword:

  tier 1  own_sense              the headword's own English Wiktionary sense
  tier 2  lemma_of_form          a form-of / alt-of / synonym-of sense resolved
                                 to the lemma's own definitions (up to 2 hops),
                                 and Korean hanja-reading senses resolved to the
                                 hanja character's definition
  tier 3  lexeme_sense           Wikidata Lexemes (CC0): English gloss or English
                                 translation link of a lexeme whose lemma or
                                 form is the headword (optional --lexemes)
  tier 4  form_of_gloss          a pointer gloss whose lemma could not be found,
                                 kept verbatim (add-only)
  tier 5  close_variant          the close-variant pass (definition-variants/),
                                 with its own form-of senses resolved as well,
                                 plus rule-based keys for words still without
                                 English (Malayalam chillu spelling, Korean
                                 particles, Amharic prefixes)
  tier 6  translation_equivalent English Wiktionary translation tables that list
                                 the headword (exact or with optional marks)

Within tier 1, senses corroborated by a translation table (the English word
that lists the headword also appears in the gloss) rank first; obsolete,
archaic, rare and similar senses rank last; otherwise Wiktionary order holds.
The first ranked definition is recorded as ``lead_candidate``. It is a
reviewer's starting point only: nothing is selected or approved here.

Gloss-quality notes (wiki-markup remnants, duplicates, unresolved pointers,
non-English glosses) are recorded with a cleaned display text; the original
glosses stay exactly as staged in the review queue.
"""
from __future__ import annotations

import argparse
import gzip
import json
import re
import sys
import unicodedata
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from fruchst_definition_batch import TARGETS, norm, pos_key, wiktionary_codes  # noqa: E402
from fruchst_close_variants import ALEF_CODES, ALEF_TAGS, strip_key, uz_latin  # noqa: E402
from fruchst_english_segments import profile_scripts, script_mismatch, segment_keys  # noqa: E402
import fruchst_lexemes  # noqa: E402

SCHEMA = "SLeeLa FRUCHST English round v1"
TIERS = ("own_sense", "lemma_of_form", "lexeme_sense", "form_of_gloss", "close_variant", "translation_equivalent")
MAX_HOPS = 2
MAX_LEMMA_SENSES = 6
MAX_TRANSLATIONS = 40
MAX_EXAMPLES = 2
MAX_DEFINITIONS = 40  # added (non-review) definitions per word

# Translation tables use the ISO code of the variety listed; Mandarin rows are
# "cmn", while English Wiktionary entries file Chinese under "zh".
TRANSLATION_CODES = {"zh-Hans": ("zh", "cmn")}
# Korean senses whose gloss is only a hanja character are resolved through
# that character's Korean, then Chinese, entry.
HANJA_CODES = ("ko", "zh")
POINTER_TAGS = {"form-of", "alt-of", "synonym-of"}
DEMOTED_TAGS = {
    "obsolete", "archaic", "rare", "dated", "historical", "nonstandard",
    "misspelling", "proscribed", "uncommon", "nonce-word", "hapax", "error-misspelling",
}
CAUTION_TAGS = {"dialectal", "regional", "slang", "vulgar", "derogatory", "offensive", "colloquial", "informal"}
POINTER_WORDS = re.compile(
    r"\b(?:plural|singular|feminine|masculine|neuter|dual|past|present|passive|alternative|"
    r"alternate|obsolete|archaic|nonstandard|misspelling|romanization|transliteration|"
    r"abbreviation|initialism|acronym|contraction|diminutive|augmentative|superlative|"
    r"comparative|inflection|genitive|dative|accusative|nominative|vocative|locative|"
    r"instrumental|ablative|imperative|participle|gerund|verbal noun|infinitive|"
    r"conjugated|inflected|variant|spelling|form|clipping|ellipsis|synonym|stem|"
    r"person|degree|tense|mood|aspect)\b",
    re.I,
)
NON_POINTER_START = {
    "to", "a", "an", "the", "one", "used", "someone", "something", "any", "that", "which",
    "denotes", "denoting", "indicates", "indicating", "expresses", "expressing", "refers", "referring",
    "attached", "marks", "marking", "forms", "forming", "makes",
}
HAN_RE = re.compile(r"^[\u2E80-\u2FFF\u3400-\u4DBF\u4E00-\u9FFF\uF900-\uFAFF\U00020000-\U0003FFFF]+$")


# ---------------------------------------------------------------- gloss text

def clean_gloss(text: str) -> tuple[str, list[str]]:
    """Return a display text with wiki-markup remnants removed, and the issues found."""
    issues = []
    out = text
    if re.search(r"\bTemplate:", out):
        issues.append("unexpanded_template")
    if "[[" in out or "]]" in out:
        issues.append("wiki_link_markup")
        out = re.sub(r"\[\[(?:[^\]|]*\|)?([^\]]*)\]\]", r"\1", out)
        out = out.replace("[[", "").replace("]]", "")
        if out.count("[") != out.count("]"):
            out = out.replace("[", "").replace("]", "")
    if re.search(r"_\(\(.*?\)\)", out):
        issues.append("template_label_markup")
        out = re.sub(r"_\(\((.*?)\)\)", r" (\1)", out)
    if re.search(r"<[a-z/][^>]*>|&[a-z]+;", out):
        issues.append("html_markup")
        out = re.sub(r"<[^>]+>", "", out)
        out = re.sub(r"&nbsp;", " ", out)
    squeezed = re.sub(r"\s+", " ", out).strip()
    squeezed = re.sub(r"\s+([,;:.)])", r"\1", squeezed).replace("( ", "(")
    if squeezed != out and "whitespace" not in issues and not issues:
        issues.append("whitespace")
    return squeezed, issues


def is_pointer_text(text: str) -> bool:
    """Heuristic fallback: does the gloss only point at another word ("plural of X")?

    Used only when the dump gives no form_of/alt_of link. Parenthesised notes
    and anything after a colon are ignored, the part before "of"/"for" must be
    a short label naming a grammatical relation, and "form of address" is a
    real sense.
    """
    head = re.split(r":", re.sub(r"\([^()]*\)", " ", text), maxsplit=1)[0]
    head = re.sub(r"\s+", " ", head).strip()
    match = re.match(r"^(.{1,80}?)\s+(?:of|for)\s+\S", head)
    if not match:
        return False
    prefix = match.group(1)
    words = prefix.split()
    if (not words or words[0].casefold() in NON_POINTER_START or len(words) > 7
            or re.search(r"[,;]", prefix) or re.search(r"\bform of address\b", head, re.I)):
        return False
    return bool(POINTER_WORDS.search(prefix))


def text_targets(text: str) -> list[str]:
    """Pointer target named in the gloss text: the first word after "of"/"for"."""
    head = re.split(r":", re.sub(r"\([^()]*\)", " ", text), maxsplit=1)[0]
    match = re.search(r"\s(?:of|for)\s+([^\s,;(/+]+)", head)
    if not match:
        return []
    word = match.group(1).strip(".,;:\"'“”‘’")
    return [norm(word)] if word else []


LEADING_FILLER = re.compile(r"^(?:to|a|an|the|or|and|be|being|one's|oneself)\s+", re.I)


def equivalents(text: str) -> set[str]:
    """The English equivalents a gloss lists: its comma/semicolon segments,
    without parenthesised notes and leading "to", "a", "the", "or"."""
    body = re.sub(r"\([^()]*\)", " ", text)
    body = body.split(":", 1)[1] if ":" in body and len(body.split(":", 1)[1].strip()) > 0 else body
    out = set()
    for seg in re.split(r"[,;/]| or ", body):
        seg = re.sub(r"\s+", " ", seg).strip(" .!?\"'").casefold()
        while seg:
            stripped = LEADING_FILLER.sub("", seg)
            if stripped == seg:
                break
            seg = stripped
        if seg:
            out.add(seg)
    return out


def demotion(tags) -> int:
    return 1 if set(tags or []) & DEMOTED_TAGS else 0


# ---------------------------------------------------------------- dump access

def opener(path: Path):
    return gzip.open(path, "rt", encoding="utf-8") if path.suffix.casefold() == ".gz" else open(path, encoding="utf-8")


def gloss_list(sense: dict) -> list[str]:
    glosses = sense.get("glosses") or sense.get("raw_glosses") or []
    if isinstance(glosses, str):
        glosses = [glosses]
    return [norm(g.strip()) for g in glosses if isinstance(g, str) and g.strip()]


def pointer_targets(sense: dict) -> list[str]:
    out = []
    for key in ("form_of", "alt_of"):
        for item in sense.get(key) or []:
            if isinstance(item, dict) and isinstance(item.get("word"), str) and item["word"].strip():
                word = norm(item["word"].strip())
                if word not in out:
                    out.append(word)
    return out


def examples_of(sense: dict) -> list[dict]:
    out = []
    for ex in sense.get("examples") or []:
        if not isinstance(ex, dict):
            continue
        text = ex.get("text")
        english = ex.get("english") or ex.get("translation")
        if not isinstance(text, str) or not isinstance(english, str) or not english.strip():
            continue
        item = {"text": norm(text.strip()), "english": norm(english.strip())}
        if isinstance(ex.get("roman"), str) and ex["roman"].strip():
            item["roman"] = norm(ex["roman"].strip())
        out.append(item)
        if len(out) >= MAX_EXAMPLES:
            break
    return out


def romanizations_of(entry: dict) -> list[str]:
    out = []
    for form in entry.get("forms") or []:
        if not isinstance(form, dict) or not isinstance(form.get("form"), str):
            continue
        tags = set(form.get("tags") or [])
        if "romanization" in tags and not tags & {"inflection-template", "table-tags"}:
            value = norm(form["form"].strip())
            if value and value not in out:
                out.append(value)
        if len(out) >= 3:
            break
    return out


def lemma_senses(entry: dict) -> list[dict]:
    out = []
    for index, sense in enumerate(entry.get("senses") or [], 1):
        if not isinstance(sense, dict):
            continue
        glosses = gloss_list(sense)
        if not glosses:
            continue
        tags = sorted(t for t in sense.get("tags") or [] if isinstance(t, str))
        out.append({"sense_index": index, "glosses": glosses, "tags": tags,
                    "pointer_targets": pointer_targets(sense), "examples": examples_of(sense)})
    return out


def translation_items(entry: dict):
    for item in entry.get("translations") or []:
        yield item, None
    for index, sense in enumerate(entry.get("senses") or [], 1):
        if isinstance(sense, dict):
            for item in sense.get("translations") or []:
                yield item, (index, gloss_list(sense))


# ---------------------------------------------------------------- scanning

class Scanner:
    """Collects all dump evidence for every profile in at most 1 + MAX_HOPS passes."""

    def __init__(self, reviews: dict, variants: dict, english_rank: dict[str, int]):
        self.reviews = reviews
        self.variants = variants
        self.english_rank = english_rank
        self.head_index: dict[tuple[str, str], list[str]] = {}       # (code, word) -> tags
        self.trans_exact: dict[tuple[str, str], list[tuple]] = {}
        self.trans_strip: dict[tuple[str, str], list[tuple]] = {}
        self.extras: dict[tuple[str, int, int], dict] = {}           # (tag, line, sense) -> extras
        self.romans: dict[tuple[str, str], list[str]] = {}
        self.translations: dict[tuple[str, str], list[dict]] = {}
        self.lemmas: dict[tuple[str, str], list[dict]] = {}          # (code, key) -> entries
        self.lexemes: dict[tuple[str, str], list[dict]] = {}         # (tag, word) -> lexeme senses
        self.lexeme_source = None
        self.scripts = {tag: profile_scripts(e["word"] for e in doc["entries"]) for tag, doc in reviews.items()}
        for tag, doc in reviews.items():
            codes = wiktionary_codes(tag)
            tcodes = TRANSLATION_CODES.get(tag, codes)
            for entry in doc["entries"]:
                word = entry["word"]
                for code in codes:
                    self.head_index.setdefault((code, word), []).append(tag)
                if tag == "en":
                    continue
                keys = [word] + ([uz_latin(word)] if tag == "uz" else [])
                for code in tcodes:
                    for key in keys:
                        self.trans_exact.setdefault((code, key), []).append((tag, word))
                        self.trans_strip.setdefault((code, strip_key(key, tag in ALEF_TAGS)), []).append((tag, word))
        self.trans_codes = {code for code, _ in self.trans_exact}
        # Words with no own English sense and no close variant: rule-based
        # segment keys, and which other languages list the same spelling.
        self.missing: dict[str, list[str]] = {}
        self.seg_index: dict[tuple[str, str], list[tuple]] = {}
        self.segments: dict[tuple[str, str], list[dict]] = {}
        self.other_langs: dict[tuple[str, str], set[str]] = {}
        for tag, doc in reviews.items():
            vrows = {r["word"]: r for r in variants.get(tag, {}).get("entries", [])}
            for entry in doc["entries"]:
                word = entry["word"]
                if entry["edition_sense_candidates"].get("en") or vrows.get(word, {}).get("close_matches"):
                    continue
                self.missing.setdefault(word, []).append(tag)
                for key in segment_keys(tag, word):
                    for code in wiktionary_codes(tag):
                        self.seg_index.setdefault((code, key["stem"]), []).append((tag, word, key))

    # pass 1 ------------------------------------------------------------
    def first_pass(self, path: Path) -> None:
        with opener(path) as stream:
            for line_no, line in enumerate(stream, 1):
                if not line.strip():
                    continue
                entry = json.loads(line)
                code, word = entry.get("lang_code"), entry.get("word")
                if not isinstance(word, str):
                    continue
                word = norm(word)
                for tag in self.head_index.get((code, word), ()):
                    self._record_head(tag, word, entry, line_no)
                for tag in self.missing.get(word, ()):
                    if code not in wiktionary_codes(tag):
                        self.other_langs.setdefault((tag, word), set()).add(code)
                for tag, head, key in self.seg_index.get((code, word), ()):
                    self._record_segment(tag, head, key, entry, line_no)
                if code == "en" and ('"translations"' in line):
                    self._record_translations(word, entry, line_no)

    def _record_segment(self, tag, head, key, entry, line_no):
        pos = pos_key(entry.get("pos"))
        if key["allowed_pos"] is not None and pos not in key["allowed_pos"]:
            return
        senses = [s for s in lemma_senses(entry) if not s["pointer_targets"] and not is_pointer_text(s["glosses"][-1])]
        if not senses:
            return
        bucket = self.segments.setdefault((tag, head), [])
        if len(bucket) < 12:
            bucket.append({"stem": key["stem"], "method": key["method"], "affix": key["affix"],
                           "label": key["label"], "pos": pos, "source_line": line_no,
                           "lang_code": entry.get("lang_code"), "senses": senses[:MAX_LEMMA_SENSES]})

    def _record_head(self, tag, word, entry, line_no):
        romans = romanizations_of(entry)
        bucket = self.romans.setdefault((tag, word), [])
        for r in romans:
            if r not in bucket:
                bucket.append(r)
        for index, sense in enumerate(entry.get("senses") or [], 1):
            if not isinstance(sense, dict):
                continue
            item = {"pointer_targets": pointer_targets(sense), "examples": examples_of(sense)}
            for key in ("qualifier", "head_nr"):
                if isinstance(sense.get(key), str):
                    item[key] = sense[key]
            raw = sense.get("raw_glosses")
            if isinstance(raw, list) and raw and raw != sense.get("glosses"):
                item["raw_glosses"] = [norm(g) for g in raw if isinstance(g, str)]
            self.extras[(tag, line_no, index)] = item

    def _record_translations(self, english, entry, line_no):
        pos = pos_key(entry.get("pos"))
        for item, sense_ctx in translation_items(entry):
            if not isinstance(item, dict):
                continue
            code = item.get("lang_code") or item.get("code")
            target = item.get("word")
            if not isinstance(code, str) or not isinstance(target, str) or not target.strip():
                continue
            if code not in self.trans_codes:
                continue
            target = norm(target.strip())
            exact = self.trans_exact.get((code, target), [])
            hits = [(t, h, "exact") for t, h in exact]
            seen = {(t, h) for t, h in exact}
            for t, h in self.trans_strip.get((code, strip_key(target, code in ALEF_CODES)), []):
                if (t, h) not in seen:
                    hits.append((t, h, "optional_marks"))
                    seen.add((t, h))
            for tag, head, match in hits:
                bucket = self.translations.setdefault((tag, head), [])
                if len(bucket) >= MAX_TRANSLATIONS * 3:
                    continue
                record = {
                    "english_word": english,
                    "english_pos": pos,
                    "translation_sense": item.get("sense") if isinstance(item.get("sense"), str) else None,
                    "listed_form": target,
                    "match": match,
                    "translation_lang_code": code,
                    "source_line": line_no,
                }
                if sense_ctx:
                    record["english_sense_index"] = sense_ctx[0]
                    record["english_sense_glosses"] = sense_ctx[1]
                for key in ("roman", "english", "alt"):
                    if isinstance(item.get(key), str) and item[key].strip():
                        record["translation_" + key] = norm(item[key].strip())
                tags = sorted(t for t in item.get("tags") or [] if isinstance(t, str))
                if tags:
                    record["tags"] = tags
                if record not in bucket:
                    bucket.append(record)

    # lemma passes ------------------------------------------------------
    def lemma_pass(self, path: Path, wanted: dict[str, set[str]]) -> None:
        """Load every entry whose (code, exact or loose key) is wanted."""
        wanted = {code: words for code, words in wanted.items() if isinstance(code, str) and words}
        loose = {code: {strip_key(w, code in ALEF_CODES, True) for w in words} for code, words in wanted.items()}
        with opener(path) as stream:
            for line_no, line in enumerate(stream, 1):
                if not line.strip():
                    continue
                entry = json.loads(line)
                code, word = entry.get("lang_code"), entry.get("word")
                if code not in wanted or not isinstance(word, str):
                    continue
                word = norm(word)
                key = strip_key(word, code in ALEF_CODES, True)
                if word not in wanted[code] and key not in loose[code]:
                    continue
                senses = lemma_senses(entry)
                if not senses:
                    continue
                record = {"word": word, "pos": pos_key(entry.get("pos")), "lang_code": code,
                          "source_line": line_no, "senses": senses}
                for k in {(code, word), (code, "~" + key)}:
                    bucket = self.lemmas.setdefault(k, [])
                    if record not in bucket:
                        bucket.append(record)

    def lemma_lookup(self, code: str, word: str) -> tuple[list[dict], str]:
        exact = self.lemmas.get((code, word), [])
        if exact:
            return exact, "exact"
        loose = self.lemmas.get((code, "~" + strip_key(word, code in ALEF_CODES, True)), [])
        return loose, "optional_marks"


# ---------------------------------------------------------------- building

def pointer_requests(scanner: Scanner) -> dict[str, set[str]]:
    wanted: dict[str, set[str]] = {}
    for tag, doc in scanner.reviews.items():
        for entry in doc["entries"]:
            for sense in entry["edition_sense_candidates"].get("en", []):
                code = sense.get("source_lang_code") or wiktionary_codes(tag)[0]
                extra = scanner.extras.get((tag, sense["source_line"], sense["sense_index"]), {})
                targets = extra.get("pointer_targets") or (
                    text_targets(clean_gloss(sense["glosses"][-1])[0])
                    if is_pointer_text(clean_gloss(sense["glosses"][-1])[0]) else [])
                for target in targets:
                    wanted.setdefault(code, set()).add(target)
                if tag == "ko":
                    for g in sense["glosses"]:
                        if HAN_RE.match(g.strip()):
                            for c in HANJA_CODES:
                                wanted.setdefault(c, set()).add(g.strip())
        for row in scanner.variants.get(tag, {}).get("entries", []):
            for match in row.get("close_matches", []):
                for s in match.get("english_senses", []):
                    for target in s.get("form_of", []):
                        wanted.setdefault(match.get("lang_code"), set()).add(norm(target))
    return wanted


def next_hop_requests(scanner: Scanner, already: dict[str, set[str]]) -> dict[str, set[str]]:
    wanted: dict[str, set[str]] = {}
    for (code, key), records in scanner.lemmas.items():
        if key.startswith("~"):
            continue
        for record in records:
            if any(not s["pointer_targets"] and not is_pointer_text(s["glosses"][-1]) for s in record["senses"]):
                continue
            for s in record["senses"]:
                for target in s["pointer_targets"]:
                    if target not in already.get(code, set()):
                        wanted.setdefault(code, set()).add(target)
    return wanted


def resolve(scanner: Scanner, code: str, target: str, want_pos: str | None, hops: int = 1, trail=()) -> list[dict]:
    """Substantive senses of a pointer target, following at most MAX_HOPS pointers."""
    records, how = scanner.lemma_lookup(code, target)
    same = [r for r in records if want_pos and r["pos"] == want_pos]
    records = same or records
    out = []
    for record in records:
        substantive = [s for s in record["senses"]
                       if not s["pointer_targets"] and not is_pointer_text(s["glosses"][-1])]
        if substantive:
            substantive.sort(key=lambda s: (demotion(s["tags"]), s["sense_index"]))
            for s in substantive[:MAX_LEMMA_SENSES]:
                out.append({"lemma": record["word"], "lemma_pos": record["pos"], "lemma_lang_code": code,
                            "lemma_match": how, "pos_matched": bool(want_pos) and record["pos"] == want_pos,
                            "hops": hops, "via": list(trail), "source_line": record["source_line"],
                            "sense_index": s["sense_index"], "glosses": s["glosses"], "tags": s["tags"],
                            "examples": s["examples"]})
        elif hops < MAX_HOPS:
            for s in record["senses"]:
                for nxt in s["pointer_targets"]:
                    if nxt != target and nxt not in trail:
                        out += resolve(scanner, code, nxt, record["pos"], hops + 1, trail + (record["word"],))
    return out


def definition(tier, text_glosses, pos, evidence, tags=(), examples=(), **extra) -> dict:
    display, issues = clean_gloss(text_glosses[-1])
    item = {"tier": TIERS.index(tier) + 1, "method": tier, "text": display, "pos": pos}
    if len(text_glosses) > 1:
        item["context"] = [clean_gloss(g)[0] for g in text_glosses[:-1]]
    if issues:
        item["cleaned_from"] = text_glosses[-1]
    if tags:
        item["tags"] = list(tags)
        cautions = sorted(set(tags) & (DEMOTED_TAGS | CAUTION_TAGS))
        if cautions:
            item["register"] = cautions
    if examples:
        item["examples"] = list(examples)
    item.update(extra)
    item["evidence"] = evidence
    return item


def build_entry(tag: str, entry: dict, scanner: Scanner, variant_row: dict | None, mapping_note) -> tuple[dict, dict]:
    word = entry["word"]
    senses = entry["edition_sense_candidates"].get("en", [])
    trans = scanner.translations.get((tag, word), [])
    trans.sort(key=lambda t: (t["match"] != "exact", scanner.english_rank.get(t["english_word"], 10**6),
                              t["source_line"]))
    trans = trans[:MAX_TRANSLATIONS]
    lexeme_senses = scanner.lexemes.get((tag, word), [])
    lexeme_words = {link["english_lemma"] for r in lexeme_senses for link in r.get("english_translations", [])}
    trans_words = sorted({t["english_word"] for t in trans} | lexeme_words,
                         key=lambda w: scanner.english_rank.get(w, 10**6))

    defs, notes, stats = [], [], {"pointer_resolved": 0, "pointer_unresolved": 0, "cleaned": 0, "duplicates": 0}
    seen_text: dict[tuple[str, str], int] = {}

    def push(item):
        key = (item["text"].casefold(), item.get("pos") or "")
        if key in seen_text:
            stats["duplicates"] += 1
            prior = defs[seen_text[key]]
            prior.setdefault("also_supported_by", []).append(item["evidence"])
            return
        seen_text[key] = len(defs)
        defs.append(item)

    # tier 1 and 2 -------------------------------------------------------
    own = []
    for order, sense in enumerate(senses):
        code = sense.get("source_lang_code") or wiktionary_codes(tag)[0]
        extra = scanner.extras.get((tag, sense["source_line"], sense["sense_index"]), {})
        glosses = sense["glosses"]
        last = glosses[-1]
        display, issues = clean_gloss(last)
        ev = {"source": "enwiktionary", "source_line": sense["source_line"], "sense_index": sense["sense_index"],
              "lang_code": code}
        if issues:
            stats["cleaned"] += 1
            notes.append({"source_line": sense["source_line"], "sense_index": sense["sense_index"],
                          "issues": issues, "original": last, "display": display})
        targets = list(extra.get("pointer_targets", []))
        pointer = bool(targets) or bool(set(sense.get("tags") or []) & POINTER_TAGS) or is_pointer_text(display)
        relation = clean_gloss(" ".join(glosses))[0]
        if pointer and not targets:
            targets = text_targets(display) or text_targets(relation)
        hanja = tag == "ko" and HAN_RE.match(last.strip())
        if hanja:
            notes.append({"source_line": sense["source_line"], "sense_index": sense["sense_index"],
                          "issues": ["hanja_reading_gloss"], "original": last})
            resolved = []
            for c in HANJA_CODES:
                resolved = resolve(scanner, c, last.strip(), None)
                if resolved:
                    break
            for r in resolved:
                push(definition("lemma_of_form", r["glosses"], r["lemma_pos"],
                                {**ev, "lemma_source_line": r["source_line"], "lemma_sense_index": r["sense_index"]},
                                r["tags"], r["examples"], relation=f"hanja {last.strip()}", lemma=r["lemma"],
                                lemma_lang_code=r["lemma_lang_code"], hops=r["hops"],
                                display=f"{display} (hanja): {clean_gloss(r['glosses'][-1])[0]}"))
            stats["pointer_resolved" if resolved else "pointer_unresolved"] += 1
            if not resolved:
                push(definition("form_of_gloss", glosses, sense.get("pos"), ev, sense.get("tags") or [],
                                relation=f"hanja {last.strip()}"))
            continue
        if pointer:
            resolved = []
            for target in targets:
                resolved += resolve(scanner, code, target, sense.get("pos"))
            if resolved:
                stats["pointer_resolved"] += 1
            else:
                stats["pointer_unresolved"] += 1
                notes.append({"source_line": sense["source_line"], "sense_index": sense["sense_index"],
                              "issues": ["pointer_gloss_unresolved"], "original": last,
                              "pointer_targets": targets})
                push(definition("form_of_gloss", glosses, sense.get("pos"), ev, sense.get("tags") or [],
                                relation=relation, pointer_targets=targets))
            for r in resolved:
                lemma_gloss = clean_gloss(r["glosses"][-1])[0]
                push(definition("lemma_of_form", r["glosses"], r["lemma_pos"],
                                {**ev, "lemma_source_line": r["source_line"], "lemma_sense_index": r["sense_index"]},
                                r["tags"], r["examples"], relation=relation, lemma=r["lemma"],
                                lemma_lang_code=r["lemma_lang_code"], lemma_match=r["lemma_match"],
                                pos_matched=r["pos_matched"], hops=r["hops"],
                                **({"via": r["via"]} if r["via"] else {}),
                                display=f"{relation}: {lemma_gloss}"))
            continue
        tags = sense.get("tags") or []
        extras = {}
        for k in ("qualifier", "raw_glosses"):
            if k in extra:
                extras[k] = extra[k]
        if code != wiktionary_codes(tag)[0] and mapping_note:
            extras["caution"] = f"gloss is filed under Wiktionary '{code}'; confirm it is valid for '{tag}'"
        own.append(definition("own_sense", glosses, sense.get("pos"), ev, tags, extra.get("examples", []), **extras))

    # Rank own senses and resolved lemmas by translation-table corroboration:
    # an English word whose translation table lists this headword and which is
    # itself one of the equivalents a gloss lists. More frequent English
    # corroborators rank higher; circular glosses (gloss == headword) and
    # obsolete/archaic/rare senses rank last; Wiktionary order breaks ties.
    def corroborate(item, exclude=frozenset()):
        eq = equivalents(item["text"])
        hits = [w for w in trans_words if w.casefold() in eq and w not in exclude]
        if hits:
            item["corroborated_by_translation_tables"] = hits[:8]
        if item["text"].strip(" .").casefold() == word.casefold():
            item["circular_gloss"] = True
        return hits

    def rank_key(item, position):
        hits = item.get("corroborated_by_translation_tables") or []
        best = min((scanner.english_rank.get(w, 10**6) for w in hits), default=10**7)
        return (bool(item.get("circular_gloss")), not hits, demotion(item.get("tags")), best, position)

    lemma_defs = defs[:]
    defs.clear(); seen_text.clear()
    for item in own + lemma_defs:
        corroborate(item)
    own = [d for _, d in sorted(enumerate(own), key=lambda p: rank_key(p[1], p[0]))]
    lemma_defs.sort(key=lambda d: d["tier"])
    tier2 = [d for d in lemma_defs if d["method"] == "lemma_of_form"]
    rest = [d for d in lemma_defs if d["method"] != "lemma_of_form"]
    tier2 = [d for _, d in sorted(enumerate(tier2), key=lambda p: rank_key(p[1], p[0]))]
    for item in own + tier2 + rest:
        push(item)

    # tier 3: Wikidata Lexemes -------------------------------------------
    for r in lexeme_senses:
        links = r.get("english_translations", [])
        if r.get("gloss"):
            text = r["gloss"]
        else:
            text = ", ".join(dict.fromkeys(link["english_lemma"] for link in links))
        ev = {"source": "wikidata-lexemes", "lexeme": r["lexeme"], "sense": r["sense"], "match": r["match"],
              "match_how": r["match_how"]}
        extra_l = {"lexeme_lemma": r["lemma"]}
        if r.get("items"):
            extra_l["wikidata_items"] = r["items"]
        if links:
            extra_l["english_translations"] = links
        if r["match"] == "form":
            extra_l["display"] = f"form of {r['lemma']}: {text}"
            if r.get("grammatical_features"):
                extra_l["grammatical_features"] = r["grammatical_features"]
        if r["match_how"] != "exact":
            extra_l["caution"] = "matched with optional marks removed; confirm it is the same word"
        item = definition("lexeme_sense", [text], r["pos"], ev, **extra_l)
        # A sense's own translation links are not independent corroboration.
        corroborate(item, exclude={link["english_lemma"] for link in links})
        push(item)

    # tier 3 -------------------------------------------------------------
    if variant_row:
        for match in variant_row.get("close_matches", []):
            mev = {"source": "definition-variants", "variant": match.get("variant"), "variant_method": match["method"],
                   "source_line": match.get("source_line"), "lang_code": match.get("lang_code")}
            for s in match.get("english_senses", []):
                ev = {**mev, "sense_index": s.get("sense_index")}
                extra_v = {"variant": match.get("variant"), "variant_method": match["method"]}
                if match.get("caution"):
                    extra_v["caution"] = match["caution"]
                resolved = []
                for target in s.get("form_of") or []:
                    resolved += resolve(scanner, match.get("lang_code"), norm(target), match.get("pos"))
                for r in resolved:
                    push(definition("close_variant", r["glosses"], r["lemma_pos"],
                                    {**ev, "lemma_source_line": r["source_line"]}, r["tags"], r["examples"],
                                    relation=clean_gloss(s["glosses"][-1])[0], lemma=r["lemma"], **extra_v))
                if resolved:
                    continue
                if s.get("form_of") or is_pointer_text(clean_gloss(s["glosses"][-1])[0]):
                    extra_v["pointer_gloss"] = True
                push(definition("close_variant", s["glosses"], match.get("pos"), ev, s.get("tags") or [], **extra_v))

    # rule-based keys (Malayalam chillu, Korean particles, Amharic prefixes)
    for seg in scanner.segments.get((tag, word), []):
        for s in seg["senses"]:
            ev = {"source": "enwiktionary", "source_line": seg["source_line"], "sense_index": s["sense_index"],
                  "lang_code": seg["lang_code"], "variant": seg["stem"], "variant_method": seg["method"]}
            extra_s = {"variant": seg["stem"], "variant_method": seg["method"]}
            gloss = clean_gloss(s["glosses"][-1])[0]
            if seg["affix"]:
                extra_s["affix"] = seg["affix"]
                extra_s["affix_meaning"] = seg["label"]
                extra_s["display"] = f"{seg['stem']} ({gloss}) + {seg['affix']} ({seg['label']})"
                extra_s["caution"] = "segmented by rule; confirm the stem and affix"
            push(definition("close_variant", s["glosses"], seg["pos"], ev, s["tags"], s["examples"], **extra_s))

    # tier 6 -------------------------------------------------------------
    for t in trans:
        label = t.get("translation_sense") or (t.get("english_sense_glosses") or [None])[-1]
        text = t["english_word"] if not label else f"{t['english_word']} ({label})"
        ev = {"source": "enwiktionary-translations", "english_word": t["english_word"],
              "english_pos": t["english_pos"], "source_line": t["source_line"], "listed_form": t["listed_form"],
              "match": t["match"], "translation_lang_code": t["translation_lang_code"]}
        if t.get("english_sense_index"):
            ev["english_sense_index"] = t["english_sense_index"]
        extra_t = {k: t[k] for k in ("translation_roman", "translation_english", "tags") if k in t}
        if t["match"] != "exact":
            extra_t["caution"] = "listed with optional marks (e.g. vowel points or accents); confirm it is the same word"
        push(definition("translation_equivalent", [text], t["english_pos"], ev, **extra_t))

    defs.sort(key=lambda d: (d["tier"], bool(d.get("pointer_gloss"))))
    # Review-derived senses (own_sense, form_of_gloss) are never dropped; the
    # cap applies only to the evidence this pass adds.
    kept, added = [], 0
    for d in defs:
        if d["method"] in ("own_sense", "form_of_gloss"):
            kept.append(d)
        elif added < MAX_DEFINITIONS:
            kept.append(d)
            added += 1
    defs = kept
    tiers_present = sorted({d["tier"] for d in defs})
    status = ("no_english_definition_found" if not defs else TIERS[tiers_present[0] - 1] + "_found")
    lead = None
    if defs:
        pool = [(i, d) for i, d in enumerate(defs) if d["tier"] <= 3] or list(enumerate(defs))
        _, best = min(pool, key=lambda p: rank_key(p[1], p[0])[:4] + (p[1]["tier"], p[0]))
        lead = {k: best[k] for k in ("tier", "method", "text", "pos") if k in best}
        if "display" in best:
            lead["display"] = best["display"]
        if best.get("corroborated_by_translation_tables"):
            lead["corroborated_by"] = best["corroborated_by_translation_tables"][:3]
        lead["review_status"] = "required"
    flags = {}
    odd_script = script_mismatch(word, scanner.scripts.get(tag, {}))
    if odd_script:
        flags["script_mismatch"] = odd_script
    if not defs and scanner.other_langs.get((tag, word)):
        flags["found_only_in_other_languages"] = sorted(scanner.other_langs[(tag, word)])
    row = {
        "rank": entry["rank"],
        "word": word,
        "frequency_count": entry.get("frequency_count"),
        "candidate_pos": entry.get("candidate_pos"),
        "english_status": status,
        "lead_candidate": lead,
        "romanizations": scanner.romans.get((tag, word), []),
        "translation_table_english_words": trans_words[:15],
        "definitions": defs,
        "flags": flags,
        "gloss_notes": notes,
        "review_status": "required",
    }
    stats["tiers"] = tiers_present
    return row, stats


def build_profile(path: Path, doc: dict, scanner: Scanner, variants_path: Path | None) -> dict:
    tag = doc["language_tag"]
    vdoc = scanner.variants.get(tag, {})
    vrows = {r["word"]: r for r in vdoc.get("entries", [])}
    rows = []
    counts = {
        "candidate_words": len(doc["entries"]),
        "baseline_words_with_own_english_sense": 0,
        "baseline_words_with_substantive_own_sense": 0,
        "words_with_any_english_definition": 0,
        "words_by_best_tier": {t: 0 for t in TIERS},
        "words_without_english_definition": 0,
        "words_gaining_english_definition": 0,
        "words_gaining_substantive_definition": 0,
        "pointer_senses_resolved": 0,
        "pointer_senses_unresolved": 0,
        "glosses_cleaned_for_display": 0,
        "duplicate_definitions_merged": 0,
        "definitions_total": 0,
        "words_with_translation_corroborated_lead": 0,
        "words_with_examples": 0,
        "words_with_romanization": 0,
        "words_with_wikidata_lexeme_sense": 0,
        "words_with_rule_based_key_definition": 0,
        "words_flagged_script_mismatch": 0,
        "words_found_only_in_other_languages": 0,
        "other_language_codes": {},
        "production_definitions_approved": 0,
    }
    for entry in doc["entries"]:
        row, stats = build_entry(tag, entry, scanner, vrows.get(entry["word"]), doc.get("target_language_mapping_note"))
        rows.append(row)
        had = bool(entry["edition_sense_candidates"].get("en"))
        own_sub = 1 in stats["tiers"]
        counts["baseline_words_with_own_english_sense"] += had
        counts["baseline_words_with_substantive_own_sense"] += own_sub
        if row["definitions"]:
            counts["words_with_any_english_definition"] += 1
            counts["words_by_best_tier"][TIERS[stats["tiers"][0] - 1]] += 1
            if not had:
                counts["words_gaining_english_definition"] += 1
            if not own_sub:
                counts["words_gaining_substantive_definition"] += 1
        else:
            counts["words_without_english_definition"] += 1
        counts["pointer_senses_resolved"] += stats["pointer_resolved"]
        counts["pointer_senses_unresolved"] += stats["pointer_unresolved"]
        counts["glosses_cleaned_for_display"] += stats["cleaned"]
        counts["duplicate_definitions_merged"] += stats["duplicates"]
        counts["definitions_total"] += len(row["definitions"])
        if row["lead_candidate"] and row["lead_candidate"].get("corroborated_by"):
            counts["words_with_translation_corroborated_lead"] += 1
        counts["words_with_examples"] += any(d.get("examples") for d in row["definitions"])
        counts["words_with_romanization"] += bool(row["romanizations"])
        counts["words_with_wikidata_lexeme_sense"] += any(d["method"] == "lexeme_sense" for d in row["definitions"])
        counts["words_with_rule_based_key_definition"] += any(
            d.get("variant_method") in ("chillu_normalized", "particle_stripped", "contracted_particle", "prefix_stripped")
            for d in row["definitions"])
        counts["words_flagged_script_mismatch"] += "script_mismatch" in row["flags"]
        others = row["flags"].get("found_only_in_other_languages")
        if others:
            counts["words_found_only_in_other_languages"] += 1
            for code in others:
                counts["other_language_codes"][code] = counts["other_language_codes"].get(code, 0) + 1
    top = sorted(counts["other_language_codes"].items(), key=lambda kv: -kv[1])[:10]
    counts["other_language_codes"] = dict(top)
    return {
        "schema": SCHEMA,
        "language_tag": tag,
        "review_queue_file": path.as_posix(),
        "variants_file": variants_path.as_posix() if variants_path else None,
        "target_language_mapping_note": doc.get("target_language_mapping_note"),
        "tier_order": list(TIERS),
        "policy": (
            "Add-only. Every review-queue gloss is kept unchanged; this file adds the English evidence "
            "English Wiktionary and Wikidata Lexemes supply: resolved form-of lemmas, lexeme senses, close "
            "variants and rule-based keys, translation-table equivalents, usage examples with English "
            "translations, and romanizations. Words with no English are flagged when their script is rare "
            "in the profile or when only other languages have an entry for the spelling. Definitions are "
            "ranked (own sense corroborated by a translation table, then own sense in Wiktionary order "
            "with obsolete/archaic/rare senses last, then resolved lemma, close variant, translation "
            "equivalent). The lead candidate is a reviewer's starting point; nothing is selected or approved."
        ),
        "source": {**doc["definition_sources"]["en"]},
        "lexeme_source": scanner.lexeme_source,
        "counts": counts,
        "production_status": "review_required",
        "entries": rows,
    }


def english_ranks(reviews: dict) -> dict[str, int]:
    doc = reviews.get("en")
    if not doc:
        return {}
    return {e["word"]: e["rank"] for e in doc["entries"]}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--reviews", type=Path, required=True)
    parser.add_argument("--variants", type=Path, required=True)
    parser.add_argument("--en", type=Path, required=True)
    parser.add_argument("--lexemes", type=Path, help="Wikidata lexeme JSON dump (.json.gz/.bz2), optional")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    reviews, paths, vpaths, variants = {}, {}, {}, {}
    for path in sorted(args.reviews.glob("*.review.json")):
        doc = json.loads(path.read_text(encoding="utf-8"))
        if doc["language_tag"] in TARGETS:
            reviews[doc["language_tag"]] = doc
            paths[doc["language_tag"]] = path
            vpath = args.variants / path.name.replace(".review.json", ".variants.json")
            if vpath.is_file():
                variants[doc["language_tag"]] = json.loads(vpath.read_text(encoding="utf-8"))
                vpaths[doc["language_tag"]] = vpath

    scanner = Scanner(reviews, variants, english_ranks(reviews))
    if args.lexemes:
        scanner.lexeme_source = {**fruchst_lexemes.SOURCE, "file": args.lexemes.name,
                                 "size_bytes": args.lexemes.stat().st_size}
        scanner.lexemes = fruchst_lexemes.scan_lexemes(
            args.lexemes, {tag: {e["word"] for e in doc["entries"]} for tag, doc in reviews.items()})
    scanner.first_pass(args.en)
    wanted = pointer_requests(scanner)
    requested = {c: set(w) for c, w in wanted.items()}
    for hop in range(MAX_HOPS):
        if not wanted:
            break
        scanner.lemma_pass(args.en, wanted)
        wanted = next_hop_requests(scanner, requested)
        for c, w in wanted.items():
            requested.setdefault(c, set()).update(w)

    args.output.mkdir(parents=True, exist_ok=True)
    for tag, doc in reviews.items():
        out = build_profile(paths[tag], doc, scanner, vpaths.get(tag))
        target = args.output / paths[tag].name.replace(".review.json", ".english.json")
        target.write_text(json.dumps(out, ensure_ascii=False, separators=(",", ":")) + "\n", encoding="utf-8")
        print(json.dumps({"language": tag, **out["counts"]}, ensure_ascii=False, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
