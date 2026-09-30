#!/usr/bin/env python3
"""
SLeeLa JDK 28 API synchronizer.

Downloads the official JDK 28 API "all classes and interfaces" index and creates
one SLeeLa source envelope per documented API type under lib/java.

IMPORTANT: Existing .sleela files are never overwritten. The synchronizer is
additive-only by design.
"""

from __future__ import annotations

import argparse
import html
import re
import sys
import urllib.request
from pathlib import Path
from urllib.parse import urljoin, urlparse

INDEX_URL = "https://download.java.net/java/early_access/jdk28/docs/api/allclasses-index.html"
ROOT = Path(__file__).resolve().parents[1]


def fetch_index() -> str:
    request = urllib.request.Request(
        INDEX_URL,
        headers={"User-Agent": "SLeeLa-JDK28-API-Synchronizer/1.0"},
    )
    with urllib.request.urlopen(request, timeout=60) as response:
        return response.read().decode("utf-8", errors="replace")


def extract_types(document: str) -> list[tuple[str, str]]:
    # Javadoc's all-classes index links directly to type pages. We deliberately
    # use hrefs instead of display text so generic type parameters do not matter.
    hrefs = re.findall(r'href=["\']([^"\']+\.html(?:#[^"\']*)?)["\']', document)
    found: set[tuple[str, str]] = set()

    for raw_href in hrefs:
        href = html.unescape(raw_href.split("#", 1)[0])
        parsed = urlparse(urljoin(INDEX_URL, href))
        path = parsed.path.strip("/")

        # Expected form: <module>/<package>/<Type>.html
        parts = path.split("/")
        if len(parts) < 3 or not parts[-1].endswith(".html"):
            continue

        filename = parts[-1][:-5]
        package_parts = parts[1:-1]
        if not package_parts or filename in {
            "module-info", "package-info", "package-summary", "module-summary",
            "package-use", "deprecated-list", "index", "overview-summary",
        }:
            continue

        # The index can contain links to member pages and documentation pages.
        if filename in {"class-use", "serialized-form"}:
            continue
        if any(p in {"doc-files", "index-files"} for p in parts):
            continue

        package_name = ".".join(package_parts)
        qualified = f"{package_name}.{filename}"
        found.add((qualified, "/".join(package_parts + [filename + ".sleela"])))

    return sorted(found)


def envelope(qualified: str) -> str:
    simple = qualified.rsplit(".", 1)[-1]
    return f'''/*
 * lib/java/{qualified.replace(".", "/")}.sleela
 * SLeeLa Java SE/JDK 28 source envelope.
 * Definition: Native SLeeLa source declaration for the Java API type {qualified}.
 *
 * Generated from the official JDK 28 API specification. The declaration is
 * intentionally a source contract; Java behavior is supplied through the
 * SLeeLa Java conformance boundary.
 *
 * This file is additive-only generated output. Existing files are preserved.
 */
#sleela 1.3

class {simple} {{
  String javaType = "{qualified}";
  String typeName() {{ return javaType; }}
  boolean available() {{ return javaType != ""; }}
  String construct() {{ return "java.construct:" + javaType; }}
  String invoke(String operation) {{ return "java.invoke:" + javaType + ":" + operation; }}
  String invokeStatic(String operation) {{ return "java.static:" + javaType + ":" + operation; }}
}}
'''


def main() -> int:
    parser = argparse.ArgumentParser(description="Add missing JDK 28 API SLeeLa envelopes.")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--index", type=Path, help="Use a previously downloaded allclasses-index.html")
    args = parser.parse_args()

    document = args.index.read_text(encoding="utf-8") if args.index else fetch_index()
    types = extract_types(document)

    created = 0
    skipped = 0
    for qualified, relative in types:
        target = ROOT / relative
        target.parent.mkdir(parents=True, exist_ok=True)

        if target.exists():
            skipped += 1
            continue

        if not args.dry_run:
            target.write_text(envelope(qualified), encoding="utf-8")
        created += 1

    print(f"JDK 28 documented API types discovered: {len(types)}")
    print(f"New SLeeLa files {'would be ' if args.dry_run else ''}created: {created}")
    print(f"Existing SLeeLa files preserved: {skipped}")
    print(f"Source: {INDEX_URL}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
