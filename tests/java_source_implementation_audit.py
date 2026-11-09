#!/usr/bin/env python3
"""Measure implementation depth in SLeeLa Java counterpart source files.

This is a conservative source-shape audit, not a Java parser or proof of
behavioral equivalence. It distinguishes real method bodies from metadata
accessors and generic Java dispatch placeholders so implementation work can
be prioritized using evidence.
"""
from __future__ import annotations

import argparse
import json
import re
from collections import Counter
from pathlib import Path

METHOD_START = re.compile(
    r"(?m)^\s*(?:(?:public|protected|private|static|final|abstract|native|"
    r"synchronized|override|virtual)\s+)*"
    r"[A-Za-z_$][\w.$<>?, \[\]]*\s+([A-Za-z_$][\w$]*)\s*"
    r"\([^;{}]*\)\s*\{"
)
DISPATCH = re.compile(
    r'return\s+"java\.(?:invoke|static|construct):[^"]*"\s*;',
    re.S,
)
TRIVIAL_ACCESSOR = re.compile(
    r"^(?:\s*(?:return\s+\w+\s*;|\w+\s*=\s*\w+\s*;|"
    r"return\s+this\.\w+\s*;)\s*)+$",
    re.S,
)
CONSTANT_RETURN = re.compile(
    r"^\s*return\s+(?:null|true|false|[-+]?\d+(?:\.\d+)?|"
    r'""|".*?")\s*;\s*$',
    re.S,
)


def _method_bodies(source: str):
    """Yield (name, body) using brace matching that skips strings/comments."""
    pos = 0
    while True:
        match = METHOD_START.search(source, pos)
        if not match:
            return
        name = match.group(1)
        i = match.end() - 1
        depth = 1
        body_start = i + 1
        quote = None
        escaped = False
        line_comment = False
        block_comment = False
        while i + 1 < len(source) and depth:
            i += 1
            ch = source[i]
            nxt = source[i + 1] if i + 1 < len(source) else ""
            if line_comment:
                if ch == "\n":
                    line_comment = False
                continue
            if block_comment:
                if ch == "*" and nxt == "/":
                    block_comment = False
                    i += 1
                continue
            if quote:
                if escaped:
                    escaped = False
                elif ch == "\\":
                    escaped = True
                elif ch == quote:
                    quote = None
                continue
            if ch == "/" and nxt == "/":
                line_comment = True
                i += 1
            elif ch == "/" and nxt == "*":
                block_comment = True
                i += 1
            elif ch in ('"', "'"):
                quote = ch
            elif ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
        if depth == 0:
            yield name, source[body_start:i]
            pos = i + 1
        else:
            return


def classify_method(name: str, body: str) -> str:
    stripped = body.strip()
    if not stripped:
        return "empty"
    if DISPATCH.search(body):
        return "generic-dispatch"
    if TRIVIAL_ACCESSOR.fullmatch(stripped):
        return "accessor-or-field-copy"
    if CONSTANT_RETURN.fullmatch(stripped):
        return "constant-placeholder"
    return "substantive-candidate"


def analyze_text(source: str) -> dict:
    methods = []
    for name, body in _method_bodies(source):
        methods.append({"name": name, "classification": classify_method(name, body)})
    counts = Counter(item["classification"] for item in methods)
    return {
        "method_count": len(methods),
        "classifications": dict(sorted(counts.items())),
        "methods": methods,
    }


def analyze_tree(root: Path) -> dict:
    base = root / "lib" / "java"
    files = sorted(base.rglob("*.sleela"))
    totals = Counter()
    file_rows = []
    for path in files:
        result = analyze_text(path.read_text(encoding="utf-8", errors="replace"))
        totals.update(result["classifications"])
        file_rows.append({
            "path": path.relative_to(root).as_posix(),
            "method_count": result["method_count"],
            "classifications": result["classifications"],
        })
    method_count = sum(totals.values())
    return {
        "schema": "sleela-java-source-implementation-audit-v1",
        "scope": "lib/java/**/*.sleela",
        "file_count": len(files),
        "method_count": method_count,
        "classifications": dict(sorted(totals.items())),
        "substantive_candidate_files": [
            row["path"] for row in file_rows
            if row["classifications"].get("substantive-candidate", 0)
        ],
        "files": file_rows,
        "limitations": [
            "Conservative lexical recognizer; not a complete SLeeLa parser.",
            "A substantive-looking body is not proof of correct Java behavior.",
            "Native calls and runtime dispatch require separate implementation evidence.",
            "Generated metadata accessors are not counted as Java API behavior.",
        ],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--json", type=Path, help="write the full machine-readable report")
    parser.add_argument("--top", type=int, default=25, help="show this many least-implemented files")
    args = parser.parse_args()
    report = analyze_tree(args.root.resolve())
    if args.json:
        args.json.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"SLeeLa Java source implementation inventory")
    print(f"Files scanned: {report['file_count']}")
    print(f"Method bodies recognized: {report['method_count']}")
    for category, count in report["classifications"].items():
        print(f"  {category}: {count}")
    ranked = sorted(
        report["files"],
        key=lambda row: (
            row["classifications"].get("substantive-candidate", 0),
            -row["method_count"],
            row["path"],
        ),
    )
    print(f"\nLowest implementation-depth candidates (up to {args.top}):")
    for row in ranked[:max(args.top, 0)]:
        print(f"  {row['path']}: {row['classifications'] or {'no-method-bodies': 1}}")
    if args.json:
        print(f"JSON report: {args.json}")
    print("NOTE: classifications are triage evidence, not behavioral qualification.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
