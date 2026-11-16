#!/usr/bin/env python3
"""Stage Wikipedia article-body frequency candidates for mapped FRUCHST languages.

The corpus is encyclopedic, not conversational. Output is an unapproved 1,000-word
frequency candidate file with source checksums and attribution metadata.
"""
from __future__ import annotations

import argparse
import bz2
import hashlib
import html
import json
import re
import sys
import unicodedata
import xml.etree.ElementTree as ET
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

WIKIPEDIA_SOURCE_MAP = {
    "arz": {"wiki_code": "arz", "language_name": "Egyptian Arabic"},
    "mr": {"wiki_code": "mr", "language_name": "Marathi"},
    "ha": {"wiki_code": "ha", "language_name": "Hausa"},
    "pnb-Arab": {"wiki_code": "pnb", "language_name": "Western Punjabi",
        "mapping_note": "The Western Punjabi Wikipedia uses the pnb project code and Shahmukhi script."},
    "sw": {"wiki_code": "sw", "language_name": "Swahili"},
    "jv": {"wiki_code": "jv", "language_name": "Javanese"},
    "gu": {"wiki_code": "gu", "language_name": "Gujarati"},
    "am": {"wiki_code": "am", "language_name": "Amharic"},
    "kn": {"wiki_code": "kn", "language_name": "Kannada"},
    "bho": {"wiki_code": "bh", "language_name": "Bhojpuri",
        "mapping_note": "Wikimedia's legacy bh subdomain hosts the Bhojpuri Wikipedia; Wiktionary uses bho."},
    "yo": {"wiki_code": "yo", "language_name": "Yoruba"},
    "ln": {"wiki_code": "ln", "language_name": "Lingala"},
    "ary": {"wiki_code": "ary", "language_name": "Moroccan Arabic"},
    "or": {"wiki_code": "or", "language_name": "Odia"},
    "sd": {"wiki_code": "sd", "language_name": "Sindhi"},
    "pa-Guru": {"wiki_code": "pa", "language_name": "Punjabi (Gurmukhi)",
        "mapping_note": "The Punjabi Wikipedia uses the pa project code and Gurmukhi script; verify script in review."},
    "su": {"wiki_code": "su", "language_name": "Sundanese"},
    "ig": {"wiki_code": "ig", "language_name": "Igbo"},
    "ne": {"wiki_code": "ne", "language_name": "Nepali"},
    "uz": {"wiki_code": "uz", "language_name": "Uzbek"},
    "zu": {"wiki_code": "zu", "language_name": "Zulu"},
    "skr": {"wiki_code": "skr", "language_name": "Saraiki"},
    "so": {"wiki_code": "so", "language_name": "Somali"},
    "as": {"wiki_code": "as", "language_name": "Assamese"},
    "ceb": {"wiki_code": "ceb", "language_name": "Cebuano",
        "mapping_note": "Cebuano Wikipedia has substantial automated article coverage; review for bot-generated corpus bias."},
    "mag": {"wiki_code": "mag", "language_name": "Magahi"},
    "xh": {"wiki_code": "xh", "language_name": "Xhosa"},
    "mai": {"wiki_code": "mai", "language_name": "Maithili"},
}
LICENSE = "CC-BY-SA-4.0"
WIKIMEDIA_TERMS = "https://foundation.wikimedia.org/wiki/Policy:Terms_of_Use"
DUMP_FORMAT_DOC = "https://meta.wikimedia.org/wiki/Data_dumps/Dump_format"
WIKIPEDIA_REUSE = "https://foundation.wikimedia.org/wiki/Legal:Wikimedia_Developer_App_Guidelines"


def local_name(tag: str) -> str:
    return tag.rsplit("}", 1)[-1]


def strip_nested(text: str, opening: str, closing: str) -> str:
    """Remove nested MediaWiki templates/tables without a fragile regex."""
    output = []
    i = 0
    while i < len(text):
        if not text.startswith(opening, i):
            output.append(text[i])
            i += 1
            continue
        depth, j = 1, i + len(opening)
        while j < len(text) and depth:
            if text.startswith(opening, j):
                depth += 1
                j += len(opening)
            elif text.startswith(closing, j):
                depth -= 1
                j += len(closing)
            else:
                j += 1
        if depth:
            output.append(text[i])
            i += 1
        else:
            output.append(" ")
            i = j
    return "".join(output)


def strip_wikitext(raw: str) -> str:
    text = html.unescape(raw)
    text = re.sub(r"<!--.*?-->", " ", text, flags=re.DOTALL)
    text = re.sub(r"<ref\b[^>]*>.*?</ref\s*>", " ", text, flags=re.I | re.S)
    text = re.sub(r"<ref\b[^>]*/\s*>", " ", text, flags=re.I)
    text = re.sub(r"<(gallery|math|nowiki|syntaxhighlight)\b[^>]*>.*?</\1\s*>",
                  " ", text, flags=re.I | re.S)
    text = strip_nested(text, "{{", "}}")
    text = strip_nested(text, "{|", "|}")
    def link_label(match):
        body = match.group(1).strip()
        target, sep, label = body.partition("|")
        if target.casefold().startswith(("category:", "file:", "image:", "media:")):
            return " "
        return (label if sep else target.split("#", 1)[0]).replace("_", " ").strip()
    previous = None
    while previous != text:
        previous = text
        text = re.sub(r"\[\[([^\[\]]+)\]\]", link_label, text)
    text = re.sub(r"\[(?:https?:)?//[^\s\]]+(?:\s+([^\]]+))?\]",
                  lambda m: m.group(1) or " ", text)
    text = re.sub(r"https?://\S+", " ", text)
    text = re.sub(r"</?[A-Za-z][^>]*>", " ", text)
    text = text.replace("'''", " ").replace("''", " ")
    text = re.sub(r"(?m)^\s*={1,6}\s*(.*?)\s*={1,6}\s*$", r" \1 ", text)
    return re.sub(r"\s+", " ", text).strip()


def tokenize_words(text: str) -> list[str]:
    """Tokenize Unicode letter/mark runs for space-delimited languages."""
    result, current, has_letter = [], [], False
    for char in unicodedata.normalize("NFC", text).casefold():
        category = unicodedata.category(char)
        if category.startswith("L"):
            current.append(char)
            has_letter = True
        elif category.startswith("M") and has_letter:
            current.append(char)
        else:
            if has_letter:
                word = unicodedata.normalize("NFC", "".join(current))
                if word:
                    result.append(word)
            current, has_letter = [], False
    if has_letter:
        word = unicodedata.normalize("NFC", "".join(current))
        if word:
            result.append(word)
    return result


def digest_file(path: Path, algorithm: str) -> str:
    digest = hashlib.new(algorithm)
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def page_child(page: ET.Element, field: str):
    for child in page:
        if local_name(child.tag) == field:
            return child
    return None


def extract_article_frequencies(path: Path):
    counts = Counter()
    stats = {
        "pages_seen": 0, "main_namespace_pages": 0,
        "redirect_pages_skipped": 0, "pages_with_text": 0,
        "tokens_counted": 0, "unique_tokens": 0,
    }
    with bz2.open(path, "rb") as stream:
        context = ET.iterparse(stream, events=("start", "end"))
        _, root = next(context)
        for event, element in context:
            if event != "end" or local_name(element.tag) != "page":
                continue
            stats["pages_seen"] += 1
            namespace = page_child(element, "ns")
            if namespace is None or (namespace.text or "").strip() != "0":
                element.clear()
                root.clear()
                continue
            stats["main_namespace_pages"] += 1
            if page_child(element, "redirect") is not None:
                stats["redirect_pages_skipped"] += 1
                element.clear()
                root.clear()
                continue
            revision = page_child(element, "revision")
            text_node = None
            if revision is not None:
                for node in revision.iter():
                    if local_name(node.tag) == "text":
                        text_node = node
                        break
            if text_node is not None and text_node.text:
                stats["pages_with_text"] += 1
                tokens = tokenize_words(strip_wikitext(text_node.text))
                counts.update(tokens)
                stats["tokens_counted"] += len(tokens)
            element.clear()
            root.clear()
    stats["unique_tokens"] = len(counts)
    return counts, stats


def make_candidates(counts: Counter[str], limit: int = 1000):
    ranked = sorted(counts.items(), key=lambda item: (-item[1], item[0]))
    if len(ranked) < limit:
        raise ValueError(f"only {len(ranked)} unique lexical tokens; {limit} required")
    return [{
        "candidate_rank": rank,
        "source_rank": rank,
        "word": word,
        "pos": None,
        "frequency_count": int(count),
    } for rank, (word, count) in enumerate(ranked[:limit], 1)]


def build_document(language_tag: str, source_file: Path, source_url: str,
                   expected_sha1: str | None = None, limit: int = 1000):
    mapping = WIKIPEDIA_SOURCE_MAP[language_tag]
    sha1 = digest_file(source_file, "sha1")
    if expected_sha1 and sha1.casefold() != expected_sha1.casefold():
        raise ValueError(f"SHA-1 mismatch: expected {expected_sha1}, found {sha1}")
    sha256 = digest_file(source_file, "sha256")
    counts, stats = extract_article_frequencies(source_file)
    entries = make_candidates(counts, limit)
    wiki_code = mapping["wiki_code"]
    project_url = f"https://{wiki_code}.wikipedia.org/"
    source = {
        "dataset": "Wikimedia Wikipedia XML pages-articles dump",
        "upstream": "Wikimedia dumps",
        "wiki_code": wiki_code,
        "language_name": mapping["language_name"],
        "project_url": project_url,
        "dump_url": source_url,
        "dump_file": source_url.rsplit("/", 1)[-1],
        "local_download_name": source_file.name,
        "dump_sha1": sha1,
        "dump_sha256": sha256,
        "dump_size_bytes": source_file.stat().st_size,
        "retrieved_at_utc": datetime.now(timezone.utc).isoformat(),
        "frequency_source": "Current-revision main-namespace article-body token frequency",
        "tokenization": "NFC + casefold + Unicode letter/combining-mark runs after conservative wikitext cleanup",
        "corpus_statistics": stats,
        "data_license": LICENSE,
        "additional_license": "Wikimedia text is also dual-licensed under GFDL; imported pages can carry additional notices.",
        "license_review": "CC BY-SA 4.0/GFDL source identified; verify imported-content notices, attribution and ShareAlike requirements before binary redistribution.",
        "attribution": [
            f"Wikipedia contributors, {wiki_code} edition — {project_url}",
            f"Wikimedia XML dump — {source_url}",
            WIKIPEDIA_REUSE,
        ],
        "license_notice": "Preserve attribution, indicate modifications, and provide adapted material under the required ShareAlike license; check per-page notices.",
        "mapping_note": mapping.get("mapping_note"),
        "selection": "Top 1,000 unique lexical surface tokens by raw count in current-revision main-namespace article text. Redirects and non-main namespaces excluded; templates, refs, tables and markup conservatively stripped.",
        "limitations": [
            "Encyclopedia-domain corpus, not a balanced conversational or spoken-language corpus.",
            "Article topics, names, editorial style, bot-generated content and markup cleanup can bias ranks.",
            "Surface forms only; no lemmatization, POS tagging or sense annotation.",
            "Native-speaker and corpus-bias review required.",
            "Imported articles can have additional license and attribution notices.",
        ],
        "source_references": {
            "terms": WIKIMEDIA_TERMS,
            "dump_format": DUMP_FORMAT_DOC,
            "reuse_guidelines": WIKIPEDIA_REUSE,
        },
    }
    return {
        "schema": "SLeeLa FRUCHST Wikipedia frequency candidate staging v1",
        "language_tag": language_tag,
        "language_name": mapping["language_name"],
        "candidate_count": len(entries),
        "definitions_status": "pending",
        "frequency_review_status": "review_required",
        "source": source,
        "entries": entries,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--language-tag", choices=sorted(WIKIPEDIA_SOURCE_MAP), required=True)
    parser.add_argument("--source-file", type=Path, required=True)
    parser.add_argument("--source-url", required=True)
    parser.add_argument("--sha1")
    parser.add_argument("--output-file", type=Path, required=True)
    parser.add_argument("--limit", type=int, default=1000)
    args = parser.parse_args()
    if args.limit != 1000:
        parser.error("FRUCHST requires exactly 1,000 entries")
    try:
        document = build_document(args.language_tag, args.source_file,
                                  args.source_url, args.sha1, args.limit)
        args.output_file.parent.mkdir(parents=True, exist_ok=True)
        args.output_file.write_text(json.dumps(document, ensure_ascii=False, indent=2) + "\n",
                                     encoding="utf-8")
        print(json.dumps({
            "language": args.language_tag,
            "wiki_code": WIKIPEDIA_SOURCE_MAP[args.language_tag]["wiki_code"],
            "candidate_count": document["candidate_count"],
            **document["source"]["corpus_statistics"],
            "sha256": document["source"]["dump_sha256"],
            "output": str(args.output_file),
        }, sort_keys=True))
    except (OSError, ValueError, ET.ParseError, EOFError) as exc:
        print(f"stage_wikipedia_frequency: error: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
