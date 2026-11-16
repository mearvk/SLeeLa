#!/usr/bin/env python3
"""Wikidata Lexemes (CC0) as an independent English-definition source.

Streams the Wikidata lexeme JSON dump (one entity per line inside a JSON
array) once. A FRUCHST headword matches a lexeme when it equals the lexeme's
lemma or one of its form representations in a mapped language (exactly, or
with optional marks removed). For each matching lexeme sense, the English
evidence is the sense's English gloss, and/or the English lexemes it is linked
to by "translation" (P5972) statements. The sense's "item for this sense"
(P5137) QID is kept for later cross-edition alignment.
"""
from __future__ import annotations

import bz2
import gzip
import json
from pathlib import Path

from fruchst_definition_batch import norm, wiktionary_codes
from fruchst_close_variants import ALEF_TAGS, strip_key
from fruchst_english_segments import script_of

SOURCE = {
    "project": "Wikidata Lexemes",
    "url": "https://dumps.wikimedia.org/wikidatawiki/entities/latest-lexemes.json.gz",
    "license": "CC0 1.0 (Wikidata structured data); attribution not required but recorded",
}
# Representation language keys beyond the Wiktionary codes.
EXTRA_KEYS = {
    "zh-Hans": ("zh", "zh-hans", "cmn", "cmn-hans"),
    "pa-Guru": ("pa", "pa-guru"),
    "uz": ("uz", "uz-cyrl"),
    "prs": ("prs", "fa", "fa-af"),
    "ms": ("ms", "zsm"),
    "tl": ("tl", "fil"),
}
LEXICAL_CATEGORIES = {
    "Q1084": "noun", "Q24905": "verb", "Q34698": "adjective", "Q380057": "adverb",
    "Q36224": "pronoun", "Q4833830": "preposition", "Q36484": "conjunction",
    "Q83034": "interjection", "Q63116": "numeral", "Q147276": "proper noun",
}
MAX_SENSES_PER_WORD = 20


def opener(path: Path):
    suffix = path.suffix.casefold()
    if suffix == ".gz":
        return gzip.open(path, "rt", encoding="utf-8")
    if suffix == ".bz2":
        return bz2.open(path, "rt", encoding="utf-8")
    return open(path, encoding="utf-8")


def entity_lines(path: Path):
    with opener(path) as stream:
        for line in stream:
            line = line.strip().rstrip(",")
            if not line or line in ("[", "]"):
                continue
            yield json.loads(line)


def statement_ids(claims: dict, prop: str) -> list[str]:
    out = []
    for claim in (claims or {}).get(prop, []) or []:
        value = (((claim or {}).get("mainsnak") or {}).get("datavalue") or {}).get("value")
        if isinstance(value, dict) and isinstance(value.get("id"), str):
            out.append(value["id"])
    return out


def scan_lexemes(path: Path, words_by_tag: dict[str, set[str]]) -> dict[tuple[str, str], list[dict]]:
    """Return (tag, word) -> English sense records from matching lexemes."""
    keymap: dict[str, list[str]] = {}
    for tag in words_by_tag:
        for key in set(wiktionary_codes(tag)) | set(EXTRA_KEYS.get(tag, ())):
            keymap.setdefault(key.casefold(), []).append(tag)
    exact: dict[tuple[str, str], list[str]] = {}
    loose: dict[tuple[str, str], list[str]] = {}
    for tag, words in words_by_tag.items():
        for word in words:
            exact.setdefault((tag, word), []).append(word)
            loose.setdefault((tag, strip_key(word, tag in ALEF_TAGS)), []).append(word)

    english: dict[str, tuple[str, str | None]] = {}       # English sense id -> (lemma, gloss)
    hits: dict[tuple[str, str], list[dict]] = {}
    for entity in entity_lines(path):
        if entity.get("type") != "lexeme":
            continue
        lemmas = entity.get("lemmas") or {}
        senses = entity.get("senses") or []
        if "en" in lemmas and entity.get("language") == "Q1860":
            for sense in senses:
                gloss = ((sense.get("glosses") or {}).get("en") or {}).get("value")
                english[sense.get("id")] = (norm(lemmas["en"]["value"]), norm(gloss) if gloss else None)
        if not senses:
            continue
        reps = [(k, v.get("value"), "lemma", []) for k, v in lemmas.items() if isinstance(v, dict)]
        for form in entity.get("forms") or []:
            for k, v in (form.get("representations") or {}).items():
                if isinstance(v, dict):
                    reps.append((k, v.get("value"), "form", form.get("grammaticalFeatures") or []))
        matched: dict[tuple[str, str], tuple[str, str, list]] = {}
        for key, value, kind, features in reps:
            if not isinstance(value, str):
                continue
            tags = keymap.get(key.casefold()) or keymap.get(key.split("-")[0].casefold()) or []
            value = norm(value)
            for tag in tags:
                for word in exact.get((tag, value), []):
                    prior = matched.get((tag, word))
                    if prior is None or (prior[0] != "lemma" and kind == "lemma") or prior[1] != "exact":
                        matched[(tag, word)] = (kind, "exact", features)
                for word in loose.get((tag, strip_key(value, tag in ALEF_TAGS)), []):
                    matched.setdefault((tag, word), (kind, "optional_marks", features))
        if not matched:
            continue
        lemma_reps = [(k, norm(v["value"])) for k, v in lemmas.items() if isinstance(v, dict) and v.get("value")]

        def lemma_for(tag: str, script_sample: str) -> str | None:
            """The lemma spelling in the matched profile's own language key and script."""
            own = [v for k, v in lemma_reps if tag in (keymap.get(k.casefold()) or [])]
            same_script = [v for v in own if script_of(v) == script_of(script_sample)]
            return (same_script or own or [v for _, v in lemma_reps] or [None])[0]
        pos = LEXICAL_CATEGORIES.get(entity.get("lexicalCategory"))
        sense_records = []
        for sense in senses:
            gloss = ((sense.get("glosses") or {}).get("en") or {}).get("value")
            claims = sense.get("claims") or {}
            record = {"lexeme": entity.get("id"), "sense": sense.get("id"), "pos": pos,
                      "lexical_category": entity.get("lexicalCategory"), "gloss": norm(gloss) if gloss else None,
                      "items": statement_ids(claims, "P5137"), "translation_senses": statement_ids(claims, "P5972")}
            if record["gloss"] or record["translation_senses"]:
                sense_records.append(record)
        for (tag, word), (kind, how, features) in matched.items():
            bucket = hits.setdefault((tag, word), [])
            for record in sense_records:
                if len(bucket) < MAX_SENSES_PER_WORD:
                    bucket.append({**record, "lemma": lemma_for(tag, word), "match": kind, "match_how": how,
                                   **({"grammatical_features": features} if kind == "form" else {})})

    # Resolve translation links to English lexeme senses.
    for records in hits.values():
        for record in records:
            linked = []
            for sid in record.pop("translation_senses"):
                if sid in english:
                    lemma, gloss = english[sid]
                    linked.append({"english_lemma": lemma, "english_gloss": gloss, "english_sense": sid})
            if linked:
                record["english_translations"] = linked
    for key in list(hits):
        hits[key] = [r for r in hits[key] if r.get("gloss") or r.get("english_translations")]
        if not hits[key]:
            del hits[key]
    return hits
