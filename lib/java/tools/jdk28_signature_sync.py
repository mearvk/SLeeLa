#!/usr/bin/env python3
"""Catalog JDK 28 Javadoc declarations as non-executable SLeeLa comments.

No placeholder method bodies are generated: false/0/null stubs are not Java
behavior. --check audits drift without writing. --index accepts a saved
allclasses-index.html; member pages are fetched from the official Javadoc host.
"""
from __future__ import annotations

import argparse
import html
import re
import sys
import urllib.request
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import urljoin, urlparse

INDEX_URL = "https://download.java.net/java/early_access/jdk28/docs/api/allclasses-index.html"
ROOT = Path(__file__).resolve().parents[1]
BEGIN = "// BEGIN JDK28 API SIGNATURE CATALOG (generated; do not edit)"
END = "// END JDK28 API SIGNATURE CATALOG"
VOID_TAGS = {
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "param", "source", "track", "wbr",
}
IGNORED_NAMES = {
    "module-info", "package-info", "package-summary", "module-summary",
    "package-use", "deprecated-list", "index", "overview-summary",
    "class-use", "serialized-form",
}


class SignatureParser(HTMLParser):
    """Extract text only from Javadoc div.member-signature elements."""

    def __init__(self) -> None:
        super().__init__(convert_charrefs=True)
        self.depth = 0
        self.parts: list[str] = []
        self.signatures: list[str] = []

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        classes = dict(attrs).get("class") or ""
        is_signature = tag == "div" and "member-signature" in classes.split()
        if self.depth == 0:
            if is_signature:
                self.depth = 1
                self.parts = []
            return
        if tag not in VOID_TAGS:
            self.depth += 1

    def handle_startendtag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        return

    def handle_endtag(self, tag: str) -> None:
        if self.depth == 0:
            return
        self.depth -= 1
        if self.depth == 0:
            value = re.sub(r"\s+", " ", "".join(self.parts)).strip()
            if value:
                self.signatures.append(value)
            self.parts = []

    def handle_data(self, data: str) -> None:
        if self.depth:
            self.parts.append(data)


def fetch(url: str) -> str:
    request = urllib.request.Request(
        url, headers={"User-Agent": "SLeeLa-JDK28-Signature-Catalog/3.1"}
    )
    with urllib.request.urlopen(request, timeout=60) as response:
        return response.read().decode("utf-8", errors="replace")


def index_types(document: str) -> list[tuple[str, str]]:
    found: set[tuple[str, str]] = set()
    pattern = r"""href=["']([^"']+\.html(?:#[^"']*)?)["']"""
    for raw_href in re.findall(pattern, document):
        href = html.unescape(raw_href.split("#", 1)[0])
        parsed = urlparse(urljoin(INDEX_URL, href))
        parts = parsed.path.strip("/").split("/")
        if len(parts) < 3 or not parts[-1].endswith(".html"):
            continue
        name = parts[-1][:-5]
        if name in IGNORED_NAMES or any(
            part in {"doc-files", "index-files", "class-use"} for part in parts
        ):
            continue
        package_parts = parts[1:-1]  # Omit the Javadoc module directory.
        if package_parts:
            found.add((".".join(package_parts + [name]), href))
    return sorted(found)


def extract_signatures(page: str) -> list[str]:
    parser = SignatureParser()
    parser.feed(page)
    parser.close()
    result: list[str] = []
    seen: set[str] = set()
    for raw in parser.signatures:
        signature = re.sub(r"\s+", " ", html.unescape(raw)).strip()
        if signature and signature not in seen:
            seen.add(signature)
            result.append(signature)
    return result


def render(signatures: list[str], href: str) -> str:
    lines = [
        BEGIN,
        f"// Source: {urljoin(INDEX_URL, href)}",
        "// Declarations are documentary metadata, not executable SLeeLa methods.",
        "// Java behavior remains delegated to the configured Java runtime boundary.",
    ]
    lines.extend("// JDK declaration: " + signature for signature in signatures)
    lines.append(END)
    return "\n".join(lines)


def replace_section(source: str, section: str) -> str:
    pattern = re.escape(BEGIN) + r".*?" + re.escape(END)
    if re.search(pattern, source, flags=re.S):
        return re.sub(pattern, lambda _match: section, source, count=1, flags=re.S)
    closing = source.rfind("}")
    if closing < 0:
        raise ValueError("target source has no class closing brace")
    return source[:closing].rstrip() + "\n" + section + "\n" + source[closing:]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="report drift without writing")
    parser.add_argument("--index", type=Path, help="saved allclasses-index.html")
    parser.add_argument("--limit", type=int, help="process at most N types")
    args = parser.parse_args()
    if args.limit is not None and args.limit < 1:
        parser.error("--limit must be at least 1")

    try:
        index = args.index.read_text(encoding="utf-8") if args.index else fetch(INDEX_URL)
        types = index_types(index)
        if args.limit is not None:
            types = types[:args.limit]
    except (OSError, UnicodeError, ValueError) as exc:
        print(f"ERROR: cannot read JDK API index: {exc}", file=sys.stderr)
        return 1

    checked = missing = changed = failures = 0
    for qualified, href in types:
        target = ROOT / (qualified.replace(".", "/") + ".sleela")
        if not target.is_file():
            continue
        checked += 1
        try:
            signatures = extract_signatures(fetch(urljoin(INDEX_URL, href)))
            if not signatures:
                missing += 1
                print(f"NO_DECLARED_SIGNATURES {qualified}")
                continue
            source = target.read_text(encoding="utf-8")
            updated = replace_section(source, render(signatures, href))
            if updated != source:
                changed += 1
                if not args.check:
                    target.write_text(updated, encoding="utf-8")
        except (OSError, UnicodeError, ValueError) as exc:
            failures += 1
            print(f"ERROR {qualified}: {exc}", file=sys.stderr)

    print(f"JDK 28 documented types discovered: {len(types)}")
    print(f"Existing SLeeLa type files checked: {checked}")
    print(f"Types without extracted member declarations: {missing}")
    print(f"Files {'requiring update' if args.check else 'updated'}: {changed}")
    print(f"Errors: {failures}")
    if failures:
        return 1
    return 1 if args.check and changed else 0


if __name__ == "__main__":
    raise SystemExit(main())
