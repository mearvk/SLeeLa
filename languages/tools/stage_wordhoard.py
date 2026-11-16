#!/usr/bin/env python3
"""Stage reproducible FRUCHST frequency candidates from wordhoard samples."""

from __future__ import annotations

import argparse
import csv
import json
import sys
import unicodedata
import urllib.error
import urllib.request
from pathlib import Path

UPSTREAM = "https://github.com/natema/wordhoard"
REVISION = "1bc5730e8d6e682c416c03680b7cb8c6c7ca8cd0"
LANGUAGES = {
    "en": "english",
    "fr": "french",
    "es": "spanish",
    "de": "standard-german",
}
LICENSE = "CC-BY-SA-4.0"


def read_candidates(language: str, limit: int = 1000) -> list[dict]:
    if language not in LANGUAGES:
        raise ValueError(f"unsupported first-batch language: {language}")
    url = (
        f"https://raw.githubusercontent.com/natema/wordhoard/{REVISION}/"
        f"samples/{language}.csv"
    )
    request = urllib.request.Request(
        url, headers={"User-Agent": "SLeeLa-FRUCHST-stager/1.0"}
    )
    with urllib.request.urlopen(request, timeout=60) as response:
        text = response.read().decode("utf-8-sig")

    candidates: list[dict] = []
    seen: set[str] = set()
    for row in csv.DictReader(text.splitlines()):
        word = unicodedata.normalize("NFC", (row.get("lemma") or "").strip())
        if not word or word in seen:
            continue
        try:
            source_rank = int(row["frequency_rank"])
            frequency_count = int(row["frequency_count"])
        except (KeyError, TypeError, ValueError) as exc:
            raise ValueError("upstream row has invalid frequency fields") from exc
        seen.add(word)
        candidates.append(
            {
                "candidate_rank": len(candidates) + 1,
                "source_rank": source_rank,
                "word": word,
                "pos": row.get("pos") or None,
                "frequency_count": frequency_count,
                "cefr_estimate": row.get("cefr_estimate") or None,
            }
        )
        if len(candidates) == limit:
            break
    if len(candidates) != limit:
        raise ValueError(
            f"{language}: expected {limit} unique headwords, got {len(candidates)}"
        )
    return candidates


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("language", choices=sorted(LANGUAGES))
    parser.add_argument("output", type=Path)
    parser.add_argument("--limit", type=int, default=1000)
    args = parser.parse_args()
    if args.limit != 1000:
        parser.error("production staging requires exactly 1,000 candidates")

    try:
        entries = read_candidates(args.language, args.limit)
        document = {
            "schema": "SLeeLa FRUCHST frequency candidate staging v1",
            "language_tag": args.language,
            "language_directory": LANGUAGES[args.language],
            "source": {
                "dataset": "wordhoard",
                "upstream": UPSTREAM,
                "revision": REVISION,
                "sample": f"samples/{args.language}.csv",
                "data_license": LICENSE,
                "frequency_source": (
                    "OpenSubtitles-2018 via hermitdave/FrequencyWords"
                ),
                "selection": (
                    "First 1,000 unique NFC-normalized lemmas in source "
                    "frequency_rank order; duplicate lemmas across POS rows "
                    "collapse to their first/highest-frequency row."
                ),
                "attribution": [
                    "wordhoard contributors — https://github.com/natema/wordhoard",
                    "OpenSubtitles-2018 via hermitdave/FrequencyWords",
                    "spaCy — https://spacy.io",
                    "Wiktionary contributors via Kaikki/Wiktextract",
                ],
                "license_notice": (
                    "Dataset CC BY-SA 4.0; preserve attribution and "
                    "share-alike terms. Upstream code is MIT, not the data."
                ),
                "limitations": [
                    "Subtitle-domain frequency list, not a balanced corpus.",
                    "Raw occurrence counts are not comparable across languages.",
                    "This file stages headwords only; definitions remain pending.",
                ],
            },
            "candidate_count": len(entries),
            "definitions_status": "pending",
            "entries": entries,
        }
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(
            json.dumps(document, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
        )
        print(
            f"staged {len(entries)} unique {args.language} headwords "
            f"from wordhoard {REVISION}"
        )
    except (OSError, ValueError, urllib.error.URLError) as exc:
        print(f"stage_wordhoard: error: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
