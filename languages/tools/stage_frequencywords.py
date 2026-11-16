#!/usr/bin/env python3
"""Stage FRUCHST frequency candidates from pinned FrequencyWords 2018 lists.

The upstream lists contain surface forms, not lemmatized entries. This script
preserves source order and counts, removes punctuation-only tokens, normalizes
headwords to NFC, and never fabricates missing ranks or words.
"""
from __future__ import annotations
import argparse
import json
import io
import sys
import zipfile
import unicodedata
import urllib.error
import urllib.request
from pathlib import Path

UPSTREAM = "https://github.com/hermitdave/FrequencyWords"
REVISION = "525f9b560de45753a5ea01069454e72e9aa541c6"
LICENSE = "CC-BY-SA-4.0"
FREQUENCY_SOURCE = "OpenSubtitles-2018"
# This workflow stages candidates only; definition approval is a separate gate.
# Explicit variety mappings only; never silently substitute a related dialect.
SOURCE_CODES = {
    "zh-Hans": "zh_cn", "hi": "hi", "ar": "ar", "bn": "bn",
    "pt": "pt", "ru": "ru", "id": "id", "ur": "ur", "ja": "ja",
    "vi": "vi", "te": "te", "tr": "tr", "tl": "tl", "fa": "fa",
    "ko": "ko", "th": "th", "it": "it", "pl": "pl", "uk": "uk",
    "ta": "ta", "ms": "ms", "nl": "nl", "ro": "ro", "af": "af",
    "si": "si",
}

def is_edge_punctuation(character: str) -> bool:
    return unicodedata.category(character)[0] in {"P", "S"}

def clean_headword(value: str) -> str:
    word = unicodedata.normalize("NFC", value.strip())
    while word and is_edge_punctuation(word[0]):
        word = word[1:]
    while word and is_edge_punctuation(word[-1]):
        word = word[:-1]
    if not word or any(ch.isspace() for ch in word) or not any(unicodedata.category(ch).startswith("L") for ch in word):
        return ""
    return word

def read_candidates(source_code: str, limit: int = 1000) -> list[dict]:
    # Most 2018 corpora are plain text; Thai is published as a ZIP archive.
    extension = ".zip" if source_code == "th" else ".txt"
    url = (
        f"https://raw.githubusercontent.com/hermitdave/FrequencyWords/{REVISION}/"
        f"content/2018/{source_code}/{source_code}_full{extension}"
    )
    request = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-FRUCHST-stager/1.0"})
    with urllib.request.urlopen(request, timeout=120) as response:
        payload = response.read()
    if extension == ".zip":
        with zipfile.ZipFile(io.BytesIO(payload)) as archive:
            member = f"{source_code}_full.txt"
            if member not in archive.namelist():
                text_members = [name for name in archive.namelist() if name.lower().endswith(".txt")]
                if len(text_members) != 1:
                    raise ValueError(f"{source_code}: expected {member} or one unambiguous TXT member in archive")
                member = text_members[0]
            content = archive.read(member).decode("utf-8-sig")
    else:
        content = payload.decode("utf-8-sig")
    candidates: list[dict] = []
    seen: set[str] = set()
    for source_rank, line in enumerate(content.splitlines(), 1):
        fields = line.rsplit(maxsplit=1)
        if len(fields) != 2:
            continue
        word = clean_headword(fields[0])
        if not word or word in seen:
            continue
        try:
            count = int(fields[1])
        except ValueError:
            continue
        if count < 0:
            continue
        seen.add(word)
        candidates.append({
            "candidate_rank": len(candidates) + 1,
            "source_rank": source_rank,
            "word": word,
            "pos": None,
            "frequency_count": count,
        })
        if len(candidates) == limit:
            break
    if len(candidates) != limit:
        raise ValueError(f"{source_code}: expected {limit} unique lexical headwords, found {len(candidates)}")
    return candidates

def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--targets", type=Path, default=Path("languages/FRUCHST_TARGETS.json"))
    parser.add_argument("--output-dir", type=Path, default=Path("languages/datasets/frequency-candidates"))
    parser.add_argument("--limit", type=int, default=1000)
    parser.add_argument("--overwrite", action="store_true")
    args = parser.parse_args()
    if args.limit != 1000:
        parser.error("FRUCHST candidate staging requires exactly 1,000 entries")
    try:
        registry = json.loads(args.targets.read_text(encoding="utf-8"))
        staged, skipped, failed = [], [], []
        for profile in registry["profiles"]:
            tag = profile["language_tag"]
            source_code = SOURCE_CODES.get(tag)
            if not source_code:
                continue
            output = args.output_dir / f"frequencywords-{tag}.json"
            if profile.get("status") == "frequency_candidates_staged" and not args.overwrite:
                skipped.append(tag)
                continue
            if output.exists() and not args.overwrite:
                skipped.append(tag)
                continue
            try:
                entries = read_candidates(source_code, args.limit)
            except (ValueError, urllib.error.URLError, TimeoutError) as exc:
                failed.append((tag, str(exc)))
                continue
            document = {
                "schema": "SLeeLa FRUCHST frequency candidate staging v1",
                "language_tag": tag,
                "language_directory": profile["directory"],
                "source": {
                    "dataset": "FrequencyWords",
                    "upstream": UPSTREAM,
                    "revision": REVISION,
                    "sample": f"content/2018/{source_code}/{source_code}_full{'.zip' if source_code == 'th' else '.txt'}",
                    "data_license": LICENSE,
                    "code_license": "MIT",
                    "frequency_source": FREQUENCY_SOURCE,
                    "attribution": [
                        "Hermit Dave / FrequencyWords — https://github.com/hermitdave/FrequencyWords",
                        "OpenSubtitles 2018 — https://opus.nlpl.eu/OpenSubtitles2018.php",
                    ],
                    "selection": (
                        "First 1,000 unique NFC-normalized lexical surface forms in source "
                        "frequency order. Punctuation-only tokens are excluded; source "
                        "line rank and raw occurrence count are retained."
                    ),
                    "limitations": [
                        "Surface forms, not lemmatized headwords.",
                        "Subtitle-domain frequency ranking; not a balanced general-purpose corpus.",
                        "No POS or sense labels are supplied by this source.",
                        "English, French, and Spanish definitions remain pending.",
                    ],
                    "license_notice": (
                        "FrequencyWords content is CC BY-SA 4.0; preserve attribution and "
                        "share-alike terms. The repository's code is MIT and does not "
                        "change the data license."
                    ),
                },
                "candidate_count": len(entries),
                "definitions_status": "pending",
                "entries": entries,
            }
            output.parent.mkdir(parents=True, exist_ok=True)
            output.write_text(json.dumps(document, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
            profile.update({
                "status": "frequency_candidates_staged",
                "entries": len(entries),
                "source_id": f"frequencywords-opensubtitles-2018-{source_code}",
                "source_revision": REVISION,
                "license_review": "CC-BY-SA-4.0 attribution recorded; definition-source review pending",
                "definitions_status": "pending",
                "notes": (
                    "1,000 unique NFC surface-form candidates staged from FrequencyWords/OpenSubtitles 2018; "
                    "English/French/Spanish sense-aligned definitions remain outstanding; "
                    "no production binary generated."
                ),
            })
            staged.append(tag)
        args.targets.write_text(json.dumps(registry, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(f"Staged {len(staged)} language sets: {', '.join(staged) or '(none)'}")
        print(f"Skipped existing sets: {', '.join(skipped) or '(none)'}")
        print(f"Source failures: {len(failed)}")
        for tag, reason in failed:
            print(f"  {tag}: {reason}")
        unsupported = [
            p["language_tag"] for p in registry["profiles"]
            if p["status"] != "frequency_candidates_staged" and p["language_tag"] not in SOURCE_CODES
        ]
        print(f"Still needing a source: {len(unsupported)} profiles")
        if unsupported:
            print("Unsupported by this source: " + ", ".join(unsupported))
    except (OSError, KeyError, ValueError, urllib.error.URLError) as exc:
        print(f"stage_frequencywords: error: {exc}", file=sys.stderr)
        return 2
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
