#!/usr/bin/env python3
"""Import only compatible whole-file SHA-256 records from extracted ClamAV .hsb text.

This is not a CVD/CLD decoder and never modifies the active index when input
cannot produce a valid non-empty candidate database.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import sys
import tempfile
from datetime import datetime, timezone

SHA256 = re.compile(r"^[0-9a-fA-F]{64}$")


def validate_database(path):
    errors, count = [], 0
    try:
        with path.open("r", encoding="utf-8") as handle:
            for number, raw in enumerate(handle, 1):
                line = raw.strip()
                if not line or line.startswith("#"):
                    continue
                digest = line.split(None, 1)[0]
                if not SHA256.fullmatch(digest):
                    errors.append(f"{path}:{number}: expected a 64-character SHA-256 digest")
                else:
                    count += 1
    except OSError as exc:
        return [f"cannot read {path}: {exc}"], 0
    return errors, count


def collect_signatures(source):
    entries, skipped = {}, 0
    paths = sorted(p for p in source.rglob("*") if p.is_file() and p.suffix.lower() == ".hsb")
    if not paths:
        return entries, skipped, [f"no extracted .hsb files found under {source}"]
    errors = []
    for path in paths:
        try:
            lines = path.read_text(encoding="utf-8", errors="strict").splitlines()
        except (OSError, UnicodeError) as exc:
            errors.append(f"cannot read {path}: {exc}")
            continue
        for line_no, raw in enumerate(lines, 1):
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            fields = line.split(":")
            if len(fields) < 3 or not SHA256.fullmatch(fields[0].strip()):
                skipped += 1
                continue
            try:
                size = int(fields[1])
                if size < 0:
                    raise ValueError()
            except ValueError:
                skipped += 1
                continue
            label = ":".join(fields[2:]).strip() or "clamav-import"
            entries.setdefault(fields[0].lower(), set()).add(label)
    if errors:
        return entries, skipped, errors
    if not entries:
        errors.append("no compatible whole-file SHA-256 .hsb records found")
    return entries, skipped, errors


def write_atomic(path, content):
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, temp_name = tempfile.mkstemp(prefix=path.name + ".", suffix=".tmp", dir=str(path.parent))
    try:
        with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as handle:
            handle.write(content)
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temp_name, path)
    except Exception:
        try:
            os.unlink(temp_name)
        except OSError:
            pass
        raise


def main():
    parser = argparse.ArgumentParser(description="Import compatible extracted ClamAV SHA-256 .hsb records.")
    parser.add_argument("--source", type=Path, help="directory of extracted plain-text .hsb files")
    parser.add_argument("--output", type=Path, help="native SHA-256 index destination")
    parser.add_argument("--check", type=Path, help="validate an existing native index")
    parser.add_argument("--source-version", default="unspecified", help="ClamAV database/version identifier")
    parser.add_argument("--source-url", default="", help="origin URL or internal artifact identifier")
    parser.add_argument("--manifest", type=Path, help="provenance JSON path (default: OUTPUT.manifest.json)")
    args = parser.parse_args()
    if args.check:
        errors, count = validate_database(args.check)
        for error in errors:
            print(f"INVALID {error}", file=sys.stderr)
        if errors:
            return 2
        print(f"VALID {args.check}: {count} signature(s)")
        return 0
    if not args.source or not args.output:
        parser.error("provide --source and --output, or use --check")
    source = args.source.expanduser()
    output = args.output.expanduser()
    if not source.is_dir():
        parser.error("source must be a directory of extracted text signature files")
    try:
        entries, skipped, errors = collect_signatures(source)
    except OSError as exc:
        print(f"ERROR reading source tree: {exc}", file=sys.stderr)
        return 2
    if errors:
        for error in errors:
            print(f"ERROR: {error}", file=sys.stderr)
        print("Active index was not changed.", file=sys.stderr)
        return 2

    lines = [
        "# Imported compatible whole-file SHA-256 records from extracted ClamAV .hsb files.",
        "# This subset does not represent all ClamAV detections.",
    ]
    for digest in sorted(entries):
        lines.append(f"{digest} {', '.join(sorted(entries[digest]))}")
    candidate = "\n".join(lines) + "\n"
    # Validate a same-directory temporary candidate before replacing the active index.
    output.parent.mkdir(parents=True, exist_ok=True)
    fd, temp_name = tempfile.mkstemp(prefix=output.name + ".", suffix=".candidate", dir=str(output.parent))
    temp_path = Path(temp_name)
    try:
        with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as handle:
            handle.write(candidate)
            handle.flush()
            os.fsync(handle.fileno())
        errors, count = validate_database(temp_path)
        if errors or count != len(entries):
            for error in errors:
                print(f"ERROR validating candidate: {error}", file=sys.stderr)
            print("Candidate index invalid; active index was not changed.", file=sys.stderr)
            return 2
        digest = hashlib.sha256(candidate.encode("utf-8")).hexdigest()
        manifest_path = args.manifest.expanduser() if args.manifest else Path(str(output) + ".manifest.json")
        manifest = {
            "schema": "sleelavirin-native-index-manifest-v1",
            "source_engine": "ClamAV",
            "source_version": args.source_version,
            "source_url": args.source_url or None,
            "source_directory": str(source.resolve()),
            "imported_at_utc": datetime.now(timezone.utc).isoformat(),
            "format": "native whole-file SHA-256 digest plus optional label",
            "signature_count": count,
            "skipped_record_count": skipped,
            "index_sha256": digest,
            "coverage_note": "Compatible extracted .hsb subset only; not equivalent to the full ClamAV signature database.",
        }
        # Atomic index replacement preserves the prior valid index on all earlier failures.
        os.replace(temp_path, output)
        write_atomic(manifest_path, json.dumps(manifest, indent=2, sort_keys=True) + "\n")
    except OSError as exc:
        print(f"ERROR writing candidate/manifest: {exc}", file=sys.stderr)
        print("If index replacement already completed, verify its adjacent manifest before deployment.", file=sys.stderr)
        return 2
    finally:
        try:
            temp_path.unlink()
        except OSError:
            pass
    print(f"IMPORTED {count} unique SHA-256 signatures -> {output}")
    print(f"PROVENANCE {manifest_path}")
    print(f"SKIPPED {skipped} unsupported/invalid records")
    print("NOTE: this is a compatible subset, not a complete ClamAV database.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
