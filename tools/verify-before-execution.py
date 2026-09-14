#!/usr/bin/env python3
"""Fail-closed SHA-256 verification gate for SLeeLa build/run/diagnostic paths.

Manifest format: JSON object with an optional `algorithm` (sha256) and a
`files` array. Each file entry contains `path` and `sha256`.

Example:
{
  "algorithm": "sha256",
  "files": [
    {"path": "impl/frontend/driver.cpp", "sha256": "..."}
  ]
}
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys


def verify(manifest_path: Path, root: Path) -> int:
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except Exception as exc:
        print(f"verification: cannot read manifest: {exc}", file=sys.stderr)
        return 2

    if str(manifest.get("algorithm", "sha256")).lower() != "sha256":
        print("verification: manifest algorithm must be sha256", file=sys.stderr)
        return 2

    files = manifest.get("files")
    if not isinstance(files, list) or not files:
        print("verification: manifest contains no files", file=sys.stderr)
        return 2

    failures = 0
    root = root.resolve()
    for entry in files:
        if not isinstance(entry, dict):
            print("verification: invalid manifest entry", file=sys.stderr)
            failures += 1
            continue
        rel = entry.get("path")
        expected = str(entry.get("sha256", "")).lower()
        if not isinstance(rel, str) or not expected:
            print("verification: manifest entry requires path and sha256", file=sys.stderr)
            failures += 1
            continue
        path = (root / rel).resolve()
        try:
            path.relative_to(root)
        except ValueError:
            print(f"verification: rejected path outside root: {rel}", file=sys.stderr)
            failures += 1
            continue
        if not path.is_file():
            print(f"verification: missing file: {rel}", file=sys.stderr)
            failures += 1
            continue
        digest = hashlib.sha256()
        try:
            with path.open("rb") as stream:
                for chunk in iter(lambda: stream.read(1024 * 1024), b""):
                    digest.update(chunk)
        except OSError as exc:
            print(f"verification: cannot read {rel}: {exc}", file=sys.stderr)
            failures += 1
            continue
        actual = digest.hexdigest()
        if actual != expected:
            print(f"verification: SHA-256 mismatch: {rel}", file=sys.stderr)
            print(f"  expected: {expected}", file=sys.stderr)
            print(f"  actual:   {actual}", file=sys.stderr)
            failures += 1

    if failures:
        print(f"verification: FAILED ({failures} item(s))", file=sys.stderr)
        return 1
    print(f"verification: PASS ({len(files)} item(s))")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Verify SLeeLa files before build or execution.")
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--root", required=True, type=Path)
    args = parser.parse_args()
    return verify(args.manifest, args.root)


if __name__ == "__main__":
    raise SystemExit(main())
