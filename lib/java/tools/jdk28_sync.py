#!/usr/bin/env python3
"""Add missing SLeeLa source envelopes for types in the official JDK 28 Javadoc."""
from __future__ import annotations

import argparse
import html
import re
import sys
import urllib.request
from pathlib import Path
from urllib.parse import urljoin, urlparse

INDEX_URL = "https://download.java.net/java/early_access/jdk28/docs/api/allclasses-index.html"
API_ROOT_PATH = urlparse(INDEX_URL).path.rsplit("/", 1)[0] + "/"
ROOT = Path(__file__).resolve().parents[1]
IGNORED_NAMES = {
    "module-info", "package-info", "package-summary", "module-summary",
    "package-use", "deprecated-list", "index", "overview-summary",
    "class-use", "serialized-form",
}


def fetch_index() -> str:
    request = urllib.request.Request(
        INDEX_URL, headers={"User-Agent": "SLeeLa-JDK28-API-Synchronizer/2.0"}
    )
    with urllib.request.urlopen(request, timeout=60) as response:
        return response.read().decode("utf-8", errors="replace")


def extract_types(document: str) -> list[tuple[str, str]]:
    """Return (qualified Java name, SLeeLa-relative path) pairs."""
    pattern = r"""href=["']([^"']+\.html(?:#[^"']*)?)["']"""
    found: set[tuple[str, str]] = set()
    for raw_href in re.findall(pattern, document):
        href = html.unescape(raw_href.split("#", 1)[0])
        parsed = urlparse(urljoin(INDEX_URL, href))
        if not parsed.path.startswith(API_ROOT_PATH):
            continue
        relative_doc = parsed.path[len(API_ROOT_PATH):].strip("/")
        parts = relative_doc.split("/")
        # Javadoc paths are <module>/<package path>/<Type>.html.
        if len(parts) < 3 or not parts[-1].endswith(".html"):
            continue
        filename = parts[-1][:-5]
        package_parts = parts[1:-1]
        if not package_parts or filename in IGNORED_NAMES:
            continue
        if any(part in {"doc-files", "index-files", "class-use"} for part in parts):
            continue
        qualified = ".".join(package_parts + [filename])
        # Keep nested type dots in the filename: Map.Entry.sleela.
        relative = "/".join(package_parts + [filename + ".sleela"])
        found.add((qualified, relative))
    return sorted(found)


def envelope(qualified: str, relative: str) -> str:
    simple = qualified.rsplit(".", 1)[-1]
    return f'''/*
 * lib/java/{relative}
 * SLeeLa Java SE/JDK 28 source envelope.
 * Definition: Native SLeeLa source declaration for the Java API type {qualified}.
 *
 * Generated from the official JDK 28 API specification. This is a source
 * contract; Java behavior is supplied through the SLeeLa Java runtime boundary.
 *
 * Additive-only generated output. Existing files are preserved.
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
    parser.add_argument("--index", type=Path, help="Use a saved allclasses-index.html")
    args = parser.parse_args()

    try:
        document = args.index.read_text(encoding="utf-8") if args.index else fetch_index()
    except (OSError, UnicodeError) as exc:
        print(f"ERROR: cannot read JDK API index: {exc}", file=sys.stderr)
        return 1

    types = extract_types(document)
    created = skipped = 0
    for qualified, relative in types:
        target = ROOT / relative
        if target.exists():
            skipped += 1
            continue
        if not args.dry_run:
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(envelope(qualified, relative), encoding="utf-8")
        created += 1

    print(f"JDK 28 documented API types discovered: {len(types)}")
    print(f"New SLeeLa files {'would be ' if args.dry_run else ''}created: {created}")
    print(f"Existing SLeeLa files preserved: {skipped}")
    print(f"Source: {INDEX_URL}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
