#!/usr/bin/env python3
"""Rule-based lookup keys and script checks for the FRUCHST English round.

These rules only propose a dictionary headword to look up; the definition
always comes from that headword's own English Wiktionary entry, and every
segmented match records the rule, the stem and the stripped affix.

  chillu_normalized    Malayalam: word-final consonant + virama written in the
                       pre-Unicode-5.1 way (ന്) is the atomic chillu (ൻ) that
                       Wiktionary uses. Not a segmentation; no caution.
  particle_stripped    Korean: a trailing case/topic/other particle is removed
                       (당신이 -> 당신 + 이); the stem must be a noun, pronoun,
                       proper noun or numeral entry.
  contracted_particle  Korean: a final ㄴ (topic) or ㄹ (object) contracted onto
                       the last syllable is removed (그건 -> 그거 + -ㄴ).
  prefix_stripped      Amharic: a leading preposition prefix (የ, በ, ለ, ከ, እንደ) is
                       removed; the stem must be a noun, proper noun, adjective
                       or pronoun entry.
"""
from __future__ import annotations

import unicodedata

NOMINAL_POS = {"noun", "pronoun", "proper noun", "numeral", "name"}

ML_CHILLU = {"ണ്": "ൺ", "ന്": "ൻ", "ര്": "ർ", "ല്": "ൽ", "ള്": "ൾ", "ക്": "ൿ"}

KO_PARTICLES = (
    ("에서는", "at/in (topic)"), ("에게서", "from (a person)"), ("한테서", "from (a person)"),
    ("으로는", "by/toward (topic)"), ("이라고", "quotative"), ("에서", "at/in/from (location)"),
    ("에게", "to (a person)"), ("한테", "to (a person)"), ("으로", "by/toward/as"), ("까지", "until/up to"),
    ("부터", "from (starting point)"), ("보다", "than"), ("처럼", "like"), ("마다", "every"),
    ("이나", "or/as many as"), ("이랑", "with/and"), ("라고", "quotative"), ("이야", "vocative/copula"),
    ("은", "topic marker"), ("는", "topic marker"), ("이", "subject marker"), ("가", "subject marker"),
    ("을", "object marker"), ("를", "object marker"), ("의", "possessive marker"), ("에", "at/to"),
    ("도", "also/too"), ("로", "by/toward/as"), ("와", "and/with"), ("과", "and/with"),
    ("만", "only"), ("야", "vocative/copula"), ("랑", "with/and"), ("요", "polite ending"),
)
# Hangul syllable = 0xAC00 + (initial*21 + medial)*28 + final; final 4 = ㄴ, 8 = ㄹ.
KO_CONTRACTED_FINALS = {4: ("-ㄴ", "contracted topic marker"), 8: ("-ㄹ", "contracted object marker")}

AM_PREFIXES = (("እንደ", "like/as"), ("የ", "of (genitive)"), ("በ", "in/by/with"), ("ለ", "for/to"), ("ከ", "from/than"))
AM_STEM_POS = NOMINAL_POS | {"adjective"}


def segment_keys(tag: str, word: str) -> list[dict]:
    """Candidate dictionary headwords for a word with no English entry."""
    out = []
    if tag == "ml":
        for virama, chillu in ML_CHILLU.items():
            if word.endswith(virama) and len(word) > len(virama):
                out.append({"stem": word[: -len(virama)] + chillu, "method": "chillu_normalized",
                            "affix": None, "label": None, "allowed_pos": None})
                break
    elif tag == "ko":
        for particle, label in KO_PARTICLES:
            if word.endswith(particle) and len(word) > len(particle):
                out.append({"stem": word[: -len(particle)], "method": "particle_stripped",
                            "affix": particle, "label": label, "allowed_pos": NOMINAL_POS})
        last = word[-1]
        code = ord(last) - 0xAC00
        if 0 <= code < 11172 and code % 28 in KO_CONTRACTED_FINALS:
            base = chr(0xAC00 + code - code % 28)
            out.append({"stem": word[:-1] + base, "method": "contracted_particle",
                        "affix": KO_CONTRACTED_FINALS[code % 28][0],
                        "label": KO_CONTRACTED_FINALS[code % 28][1], "allowed_pos": NOMINAL_POS})
    elif tag == "am":
        for prefix, label in AM_PREFIXES:
            if word.startswith(prefix) and len(word) - len(prefix) >= 2:
                out.append({"stem": word[len(prefix):], "method": "prefix_stripped",
                            "affix": prefix, "label": label, "allowed_pos": AM_STEM_POS})
                break
    return [k for k in out if k["stem"] and k["stem"] != word]


def script_of(word: str) -> str | None:
    """Unicode script group of the first letter (Japanese and Korean kana/Han grouped)."""
    for ch in word:
        if not ch.isalpha():
            continue
        name = unicodedata.name(ch, "")
        if not name:
            return None
        first = name.split()[0]
        if first in {"CJK", "HIRAGANA", "KATAKANA"}:
            return "HAN-KANA"
        if first == "HANGUL":
            return "HANGUL"
        return first
    return None


def profile_scripts(words) -> dict[str, float]:
    counts: dict[str, int] = {}
    total = 0
    for word in words:
        script = script_of(word)
        if script:
            counts[script] = counts.get(script, 0) + 1
            total += 1
    return {s: c / total for s, c in counts.items()} if total else {}


def script_mismatch(word: str, shares: dict[str, float]) -> str | None:
    """Return the word's script when it differs from the profile's dominant script.

    Han characters inside a Hangul profile are expected (hanja) and not flagged.
    """
    script = script_of(word)
    if script is None or not shares:
        return None
    dominant = max(shares, key=shares.get)
    if script == dominant or shares[dominant] < 0.5:
        return None
    if script == "HAN-KANA" and dominant == "HANGUL":
        return None
    return script
