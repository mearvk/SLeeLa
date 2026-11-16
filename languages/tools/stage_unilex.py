#!/usr/bin/env python3
"""Stage FRUCHST frequency candidates from pinned Unicode unilex frequency tables.

unilex (https://github.com/unicode-org/unilex) publishes per-language word-form
frequency tables computed from Google Corpus Crawler web/scripture crawls.
Values are scaled to occurrences per billion tokens. The tables are sorted
alphabetically, so this script re-ranks them by frequency.

Rules (mirrors stage_frequencywords.py, never fabricates data):
  * every source file is verified against its pinned git blob SHA-1;
  * headwords are NFC-normalized; punctuation-only / letterless tokens dropped;
  * case variants of one form (e.g. "Na" / "na") are merged: counts are summed
    and the most frequent surface form is kept as the headword;
  * ranking is frequency descending, ties broken by NFC code-point order;
  * a source with fewer than 1,000 usable forms is reported, not padded;
  * only explicit, documented variety mappings are used. A macrolanguage table
    is never silently assigned to one of its member varieties.

Usage (unilex must be reachable or checked out locally at the pinned revision):
  git clone --filter=blob:none --sparse https://github.com/unicode-org/unilex.git
  python3 languages/tools/stage_unilex.py --unilex-dir unilex
"""
from __future__ import annotations
import argparse
import hashlib
import json
import sys
import unicodedata
import urllib.request
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from stage_frequencywords import clean_headword  # noqa: E402

UPSTREAM = "https://github.com/unicode-org/unilex"
REVISION = "b3a9ae27a0c29eac217bb15a75c6f8d8efb15d17"
LICENSE = "Unicode-DFS-2016"
RAW = f"https://raw.githubusercontent.com/unicode-org/unilex/{REVISION}/data/frequency/{{code}}.txt"

# FRUCHST language_tag -> (unilex file code, mapping note). Explicit only.
SOURCE_CODES: dict[str, tuple[str, str]] = {
    "pcm": ("pcm", "exact ISO 639-3 match"),
    "mr": ("mr", "exact match"),
    "ha": ("ha", "exact match"),
    "sw": ("sw", "unilex 'sw' (Swahili macrolanguage); content is Standard Swahili"),
    "jv": ("jv", "exact match"),
    "gu": ("gu", "exact match"),
    "am": ("am", "exact match"),
    "kn": ("kn", "exact match"),
    "yo": ("yo", "exact match"),
    "my": ("my", "exact match"),
    "or": ("or", "exact match"),
    "pa-Guru": ("pa", "unilex 'pa' verified Gurmukhi-script (Eastern Punjabi); not used for pnb-Arab"),
    "su": ("su", "exact match"),
    "ig": ("ig", "exact match"),
    "prs": ("fa-AF", "CLDR fa-AF is Dari (Afghan Persian), ISO 639-3 prs"),
    "uz": ("uz", "unilex 'uz' is Cyrillic-script Uzbek of Uzbekistan (Northern Uzbek, uzn); modern official orthography is Latin"),
    "so": ("so", "exact match"),
    "ceb": ("ceb", "exact match"),
    "mag": ("mag", "exact match"),
    "km": ("km", "exact match"),
    "fuv": ("fuv", "exact ISO 639-3 match"),
    "mai": ("mai", "exact match"),
}

# Git blob SHA-1 of each source file at REVISION (provenance pin).
BLOB_SHA1 = {
    "am": "961ee6bd88147a5c5a87a8057c445faea1b8a6d4",
    "ceb": "7d693f5a52f93840c4c633d190aed06adb3dd5d2",
    "fa-AF": "c4deef4b52911d4490b2da092876a77359443ebb",
    "fuv": "ebb5de217392541df09665946908c2d0754205fa",
    "gu": "31870929338a7301ab5ae08b6be7348ccbd6d46d",
    "ha": "c3fa2acf427bb074128b92c0cd9b9cc08e190b06",
    "ig": "ea59a1b3e1854a5604a3691126c1b5427d3301dc",
    "jv": "b39e1d4cac3aca8bb3c8b29a23fe50e759b3c992",
    "km": "1e6a711c0c4818bc598e85b3b7bebb13ec1e3720",
    "kn": "0f2c1edd974b8627eaf3c1f6320adbf2a2757368",
    "mag": "f3c689473eb96a2c67c468e3f86cd6b1fd014e36",
    "mai": "29b1c6c1b63627233f2131a9b7bbf707e2617a4d",
    "mr": "84c819046bfa980dd2f565c3467e5702f4d3661c",
    "my": "14f1910c2c995023261a684d309d7f37019c33ba",
    "or": "9a517b6496791db8e6ea7e4e986d7856a6403354",
    "pa": "b79bbd8bfa20f1fa90856c0425b39bd3539ea6b3",
    "pcm": "40ca04a7304c1039ee4e51a18ee40a36fca3931d",
    "so": "2a4b6935aee8c091dd5308c5435a9e2e2d5645ae",
    "su": "e0fad9f8faa69a3b7cbd3c1c292ce45c0efc7519",
    "sw": "7447f896e2fe192073b17b1ffc7c332a29ddb60d",
    "uz": "ca17fad106203725e49f0fe6f923aa350d3db3d2",
    "yo": "f823eb5fbd65f270be49095365310af586d4d84f",
}

# Variety profiles that unilex only covers at macrolanguage level.
AMBIGUOUS = {
    "pbu": "unilex has only macrolanguage 'ps' (Pashto); Northern vs Southern variety is undocumented",
    "pst": "unilex has only macrolanguage 'ps' (Pashto); Northern vs Southern variety is undocumented",
}

DOMAIN_NOTE = {
    "kn": "corpus is small (~108k tokens) and Bible-heavy",
    "or": "corpus is small (~169k tokens) and Bible-heavy",
    "uz": "corpus is small (~119k tokens) and Bible-heavy; Cyrillic script",
    "jv": "corpus is small (~172k tokens)",
    "su": "corpus is small (~167k tokens)",
    "mag": "corpus is small (~189k tokens)",
    "mai": "corpus is small (~208k tokens)",
    "yo": "corpus is small (~265k tokens)",
    "pcm": "corpus is small (~299k tokens)",
}


def git_blob_sha1(payload: bytes) -> str:
    return hashlib.sha1(b"blob %d\0" % len(payload) + payload).hexdigest()


def load_source(code: str, unilex_dir: Path | None) -> bytes:
    if unilex_dir is not None:
        return (unilex_dir / "data" / "frequency" / f"{code}.txt").read_bytes()
    request = urllib.request.Request(RAW.format(code=code), headers={"User-Agent": "SLeeLa-FRUCHST-stager/1.0"})
    with urllib.request.urlopen(request, timeout=120) as response:
        return response.read()


def parse(payload: bytes) -> tuple[int | None, list[tuple[str, int]]]:
    corpus_size = None
    rows: list[tuple[str, int]] = []
    header: list[str] | None = None
    for line in payload.decode("utf-8-sig").splitlines():
        if line.startswith("# Corpus-Size:"):
            corpus_size = int(line.split(":", 1)[1].strip())
            continue
        if not line.strip() or line.startswith("#"):
            continue
        fields = line.split("\t")
        if header is None:
            header = fields
            if header[:2] != ["Form", "Frequency"]:
                raise ValueError(f"unexpected unilex header {header!r}")
            continue
        rows.append((fields[0], int(fields[1])))
    return corpus_size, rows


def rank(rows: list[tuple[str, int]], limit: int) -> tuple[list[dict], int, int]:
    groups: dict[str, dict] = {}
    for form, freq in rows:
        word = clean_headword(form)
        if not word or freq <= 0:
            continue
        key = word.lower()
        group = groups.setdefault(key, {"forms": {}, "total": 0})
        group["forms"][word] = group["forms"].get(word, 0) + freq
        group["total"] += freq
    ranked = []
    for group in groups.values():
        head = sorted(group["forms"].items(), key=lambda kv: (-kv[1], kv[0]))[0][0]
        ranked.append((group["total"], head, sorted(group["forms"])))
    ranked.sort(key=lambda r: (-r[0], r[1]))
    if len(ranked) < limit:
        return [], len(ranked), 0
    cutoff = ranked[limit - 1][0]
    ties = sum(1 for r in ranked if r[0] == cutoff)
    entries = [
        {
            "candidate_rank": i,
            "source_rank": i,
            "word": head,
            "pos": None,
            "frequency_count": total,
            "source_forms": forms,
        }
        for i, (total, head, forms) in enumerate(ranked[:limit], 1)
    ]
    return entries, len(ranked), ties


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--targets", type=Path, default=Path("languages/FRUCHST_TARGETS.json"))
    parser.add_argument("--output-dir", type=Path, default=Path("languages/datasets/frequency-candidates"))
    parser.add_argument("--unilex-dir", type=Path, help="local unilex checkout at the pinned revision")
    parser.add_argument("--overwrite", action="store_true")
    args = parser.parse_args()
    limit = 1000
    registry = json.loads(args.targets.read_text(encoding="utf-8"))
    staged, skipped, short, failed = [], [], [], []
    for profile in registry["profiles"]:
        tag = profile["language_tag"]
        if tag in AMBIGUOUS and profile.get("status") != "frequency_candidates_staged":
            profile["notes"] = f"Blocked: {AMBIGUOUS[tag]}. Needs a variety-specific frequency source."
            continue
        if tag not in SOURCE_CODES:
            continue
        code, mapping = SOURCE_CODES[tag]
        output = args.output_dir / f"unilex-{tag}.json"
        if (profile.get("status") == "frequency_candidates_staged" or output.exists()) and not args.overwrite:
            skipped.append(tag)
            continue
        try:
            payload = load_source(code, args.unilex_dir)
        except OSError as exc:
            failed.append((tag, str(exc)))
            continue
        blob = git_blob_sha1(payload)
        pinned = BLOB_SHA1.get(code)
        if pinned != blob:
            failed.append((tag, f"blob SHA-1 {blob} does not match pinned {pinned}"))
            continue
        corpus_size, rows = parse(payload)
        entries, usable, ties = rank(rows, limit)
        if not entries:
            short.append((tag, usable))
            profile["notes"] = (
                f"Blocked: unilex {code}.txt at {REVISION[:12]} has only {usable} usable word forms "
                f"(corpus {corpus_size} tokens); 1,000 required. Not padded. Needs a larger corpus."
            )
            continue
        limitations = [
            "Surface forms, not lemmatized headwords; case variants merged.",
            "Web/scripture crawl (Google Corpus Crawler); not a balanced general-purpose corpus; may contain foreign-language noise.",
            "Proper nouns (people, places, organizations) are not filtered and appear in the list.",
            "No POS or sense labels are supplied by this source.",
            "English, French, and Spanish definitions remain pending.",
        ]
        if tag in DOMAIN_NOTE:
            limitations.insert(1, DOMAIN_NOTE[tag].capitalize() + ".")
        if ties > 1:
            limitations.append(f"{ties} forms share the frequency at rank 1,000; cut by code-point order.")
        document = {
            "schema": "SLeeLa FRUCHST frequency candidate staging v1",
            "language_tag": tag,
            "language_directory": profile["directory"],
            "source": {
                "dataset": "unilex",
                "upstream": UPSTREAM,
                "revision": REVISION,
                "sample": f"data/frequency/{code}.txt",
                "git_blob_sha1": blob,
                "sha256": hashlib.sha256(payload).hexdigest(),
                "corpus_size_tokens": corpus_size,
                "usable_forms": usable,
                "variety_mapping": mapping,
                "data_license": LICENSE,
                "code_license": "Unicode-3.0",
                "frequency_source": "Google Corpus Crawler (via unilex)",
                "frequency_unit": "occurrences per billion tokens (scaled by unilex)",
                "attribution": [
                    "Unicode, Inc. / unilex — https://github.com/unicode-org/unilex",
                    "Google Corpus Crawler — https://github.com/google/corpuscrawler",
                ],
                "selection": (
                    "Forms NFC-normalized and stripped of edge punctuation; letterless tokens dropped; "
                    "case variants merged (summed, most frequent form kept); ranked by frequency "
                    "descending with code-point tie-break; first 1,000 kept. source_rank equals the "
                    "re-ranked position because the upstream file is alphabetical."
                ),
                "limitations": limitations,
                "license_notice": (
                    "unilex data files are distributed under the Unicode Data Files license "
                    "(SPDX Unicode-DFS-2016, file header) and the repository LICENSE (Unicode License v3). "
                    "Both are permissive and require the copyright and permission notice to be retained."
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
            "source_id": f"unilex-{code}",
            "source_revision": REVISION,
            "license_review": "Unicode-DFS-2016 notice recorded; definition-source review pending",
            "definitions_status": "pending",
            "notes": (
                "1,000 unique NFC surface-form candidates staged from unicode-org/unilex "
                f"({mapping}); English/French/Spanish sense-aligned definitions remain outstanding; "
                "no production binary generated."
            ),
        })
        staged.append(tag)
    args.targets.write_text(json.dumps(registry, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"Staged {len(staged)}: {', '.join(staged) or '(none)'}")
    print(f"Skipped existing: {', '.join(skipped) or '(none)'}")
    print(f"Too few forms: {', '.join(f'{t} ({n})' for t, n in short) or '(none)'}")
    print(f"Ambiguous variety: {', '.join(AMBIGUOUS)}")
    for tag, reason in failed:
        print(f"  failed {tag}: {reason}")
    remaining = [p["language_tag"] for p in registry["profiles"] if p["status"] != "frequency_candidates_staged"]
    print(f"Still needing a source: {len(remaining)}: {', '.join(remaining)}")
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
