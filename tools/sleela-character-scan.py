#!/usr/bin/env python3
"""Inspect a SLeeLa character-library source folder without executing its code."""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

MANIFEST = "SLEELA-CHARSET.conf"
SOURCE_EXTS = {".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".py", ".rs", ".java", ".sleela"}
DATA_EXTS = {".csv", ".tsv", ".json", ".jsonl", ".ndjson", ".txt"}
IGNORED_DIRS = {".git", "build", "dist", "target", "__pycache__", ".venv", "node_modules"}
VALID_MODES = {"auto", "literal", "procedural", "hybrid"}


def read_manifest(path: Path) -> dict[str, str]:
    values: dict[str, str] = {}
    if not path.is_file():
        return values
    for line_no, raw in enumerate(path.read_text(encoding="utf-8-sig").splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = (part.strip() for part in line.split("=", 1))
        if key:
            values[key.lower()] = value
    mode = values.get("mode", "auto").lower()
    if mode not in VALID_MODES:
        raise ValueError(f"{path}:{line_no}: mode must be one of {', '.join(sorted(VALID_MODES))}")
    values["mode"] = mode
    capacity = values.get("capacity", "1048576")
    if not capacity.isdecimal() or not 1 <= int(capacity) <= 1048576:
        raise ValueError(f"{path}: capacity must be an integer in the range 1..1048576")
    values["capacity"] = str(int(capacity))
    return values


def count_records(path: Path) -> int:
    """Count explicit literal records when safely recognizable; never run generators."""
    try:
        raw = path.read_text(encoding="utf-8-sig")
    except (UnicodeError, OSError):
        return 0
    if path.suffix.lower() == ".json":
        try:
            obj = json.loads(raw)
            if isinstance(obj, list):
                return len(obj)
            if isinstance(obj, dict):
                for key in ("entries", "characters", "records", "codepoints", "items"):
                    if isinstance(obj.get(key), list):
                        return len(obj[key])
                return 1 if obj else 0
            return 0
        except json.JSONDecodeError:
            return 0
    lines = [line for line in raw.splitlines() if line.strip() and not line.lstrip().startswith(("#", "//"))]
    if path.suffix.lower() in {".csv", ".tsv"} and lines:
        return max(0, len(lines) - 1)  # first non-comment line is treated as the header
    return len(lines)


def scan(root: Path) -> dict:
    root = root.expanduser().resolve(strict=True)
    if not root.is_dir():
        raise ValueError(f"not a directory: {root}")
    manifest = read_manifest(root / MANIFEST)
    files: list[Path] = []
    for path in root.rglob("*"):
        if not path.is_file():
            continue
        rel = path.relative_to(root)
        if any(part in IGNORED_DIRS or part.startswith(".") for part in rel.parts[:-1]):
            continue
        if path.name == MANIFEST:
            continue
        files.append(path)
    files.sort(key=lambda p: p.relative_to(root).as_posix().casefold())

    source_files = [p for p in files if p.suffix.lower() in SOURCE_EXTS]
    data_files = [p for p in files if p.suffix.lower() in DATA_EXTS]
    mode = manifest.get("mode", "auto")
    if mode == "auto":
        mode = "hybrid" if source_files and data_files else "procedural" if source_files else "literal" if data_files else "unknown"
    capacity = int(manifest.get("capacity", "1048576"))
    records = sum(count_records(p) for p in data_files)
    return {
        "root": str(root),
        "manifest": str(root / MANIFEST) if (root / MANIFEST).is_file() else None,
        "mode": mode,
        "capacity": capacity,
        "signature": f"C{capacity}",
        "source_file_count": len(source_files),
        "literal_data_file_count": len(data_files),
        "literal_record_count_estimate": records,
        "population_is_verified": False,
        "executed_source": False,
        "source_files": [p.relative_to(root).as_posix() for p in source_files],
        "literal_data_files": [p.relative_to(root).as_posix() for p in data_files],
        "generator": manifest.get("generator"),
        "catalogue": manifest.get("catalogue"),
        "notes": [
            "Scanning is read-only and never imports or executes source files.",
            "C<n> is declared capacity, not actual population.",
            "Literal record count is a best-effort estimate; validate the library's own schema before treating it as authoritative.",
            "Procedural libraries may generate a character only for a supplied input; this scan does not call the generator."
        ]
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Scan a SLeeLa character-library source folder.")
    parser.add_argument("folder", help="folder to inspect, e.g. utf-4088")
    parser.add_argument("--json", action="store_true", help="emit the complete report as JSON")
    args = parser.parse_args(argv)
    try:
        report = scan(Path(args.folder))
    except (OSError, ValueError) as exc:
        print(f"sleela-character-scan: error: {exc}", file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps(report, indent=2, ensure_ascii=False))
    else:
        print(f"Character library : {report['root']}")
        print(f"Mode              : {report['mode']}")
        print(f"Signature         : {report['signature']} (capacity, not population)")
        print(f"Source files      : {report['source_file_count']}")
        print(f"Literal data files: {report['literal_data_file_count']}")
        print(f"Literal records*  : {report['literal_record_count_estimate']}")
        print("Source executed   : no")
        for note in report["notes"]:
            print(f"Note              : {note}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
