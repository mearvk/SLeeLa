#!/usr/bin/env python3
"""Acquire and verify ClamAV databases into immutable, SHA-256-addressed snapshots.

Uses FreshClam (which validates official database signatures) instead of trusting
arbitrary direct-download URLs. Publishes a current.json pointer only after the
ClamAV engine successfully loads the staged databases.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
from datetime import datetime, timezone

SCHEMA = "sleelavirin-clamav-snapshot-v1"
DATABASE_SUFFIXES = (".cvd", ".cld")


def sha256_file(path):
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for block in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def database_files(directory):
    return sorted(
        p for p in directory.iterdir()
        if p.is_file() and p.suffix.lower() in DATABASE_SUFFIXES
    )


def build_manifest(directory, source="FreshClam / official ClamAV distribution"):
    files = database_files(directory)
    if not files:
        raise ValueError("no .cvd or .cld database files found")
    entries = []
    for path in files:
        entries.append({
            "name": path.name,
            "size_bytes": path.stat().st_size,
            "sha256": sha256_file(path),
        })
    canonical = json.dumps(entries, sort_keys=True, separators=(",", ":")).encode("utf-8")
    snapshot_id = hashlib.sha256(canonical).hexdigest()
    return {
        "schema": SCHEMA,
        "source": source,
        "verification": "FreshClam update plus successful ClamAV engine database load",
        "created_at_utc": datetime.now(timezone.utc).isoformat(),
        "snapshot_id": snapshot_id,
        "database_count": len(entries),
        "databases": entries,
        "warning": "SHA-256 records detect later changes only when this manifest or snapshot ID is independently trusted; hashes alone do not authenticate the publisher.",
    }


def verify_manifest(directory, manifest):
    errors = []
    if not isinstance(manifest, dict) or manifest.get("schema") != SCHEMA:
        return ["unsupported or malformed ClamAV snapshot manifest"]
    declared = manifest.get("databases")
    if not isinstance(declared, list) or not declared:
        return ["manifest has no database entries"]
    seen = set()
    actual = {}
    for path in database_files(directory):
        actual[path.name] = {"name": path.name, "size_bytes": path.stat().st_size, "sha256": sha256_file(path)}
    for entry in declared:
        if not isinstance(entry, dict) or not isinstance(entry.get("name"), str):
            errors.append("invalid database entry")
            continue
        name = entry["name"]
        if name in seen:
            errors.append(f"duplicate database entry: {name}")
            continue
        seen.add(name)
        if name not in actual:
            errors.append(f"database missing: {name}")
        elif actual[name] != entry:
            errors.append(f"database size/hash mismatch: {name}")
    if seen != set(actual):
        errors.append("manifest database list does not match snapshot contents")
    canonical = json.dumps(declared, sort_keys=True, separators=(",", ":")).encode("utf-8")
    expected_id = hashlib.sha256(canonical).hexdigest()
    if manifest.get("snapshot_id") != expected_id:
        errors.append("snapshot_id does not match manifest database entries")
    if manifest.get("database_count") != len(declared):
        errors.append("database_count does not match manifest")
    return errors


def atomic_json(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, name = tempfile.mkstemp(prefix=path.name + ".", suffix=".tmp", dir=str(path.parent))
    temp = Path(name)
    try:
        with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as handle:
            json.dump(data, handle, indent=2, sort_keys=True)
            handle.write("\n")
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temp, path)
    finally:
        try:
            temp.unlink()
        except OSError:
            pass


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path("antivirus/signatures/clamav-cache"),
                        help="snapshot storage root (keep generated databases out of Git)")
    parser.add_argument("--freshclam", default="freshclam", help="FreshClam executable")
    parser.add_argument("--clamscan", default="clamscan", help="ClamAV scanner executable")
    parser.add_argument("--config-file", type=Path, help="optional FreshClam configuration file")
    parser.add_argument("--timeout", type=int, default=600, help="download and validation timeout in seconds")
    parser.add_argument("--check", type=Path, help="verify an existing snapshot directory and manifest; no download")
    args = parser.parse_args()
    if args.timeout < 1:
        parser.error("--timeout must be positive")

    if args.check:
        try:
            manifest_path = args.check / "manifest.json"
            manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
            errors = verify_manifest(args.check, manifest)
        except (OSError, UnicodeError, json.JSONDecodeError) as exc:
            print(f"INVALID snapshot: {exc}", file=sys.stderr)
            return 2
        for error in errors:
            print(f"INVALID {error}", file=sys.stderr)
        if errors:
            return 2
        print(f"VALID snapshot {manifest['snapshot_id']} ({manifest['database_count']} database file(s))")
        return 0

    root = args.root.expanduser().resolve()
    snapshots = root / "snapshots"
    staging_parent = root / ".staging"
    snapshots.mkdir(parents=True, exist_ok=True)
    staging_parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix="update-", dir=str(staging_parent)))
    try:
        command = [args.freshclam, f"--datadir={stage}"]
        if args.config_file:
            command.append(f"--config-file={args.config_file.expanduser().resolve()}")
        print("Running FreshClam to acquire and verify official databases...")
        result = subprocess.run(command, check=False, timeout=args.timeout)
        if result.returncode != 0:
            print(f"ERROR: FreshClam exited with status {result.returncode}; active pointer unchanged.", file=sys.stderr)
            return 2

        files = database_files(stage)
        if not files:
            print("ERROR: FreshClam produced no .cvd/.cld database files; active pointer unchanged.", file=sys.stderr)
            return 2

        # Validate by loading the staged databases with the actual ClamAV engine.
        with tempfile.TemporaryDirectory(prefix="sleelavirin-clamav-empty-") as empty:
            check = subprocess.run(
                [args.clamscan, f"--database={stage}", "--no-summary", empty],
                check=False, timeout=args.timeout, capture_output=True, text=True,
            )
        if check.returncode != 0:
            print("ERROR: ClamAV could not load/scan with staged databases; active pointer unchanged.", file=sys.stderr)
            if check.stderr:
                print(check.stderr.strip(), file=sys.stderr)
            return 2

        manifest = build_manifest(stage)
        snapshot_id = manifest["snapshot_id"]
        final_dir = snapshots / snapshot_id
        if final_dir.exists():
            errors = verify_manifest(final_dir, json.loads((final_dir / "manifest.json").read_text(encoding="utf-8")))
            if errors:
                print("ERROR: existing snapshot ID directory failed verification; refusing to overwrite.", file=sys.stderr)
                return 2
            shutil.rmtree(stage)
        else:
            atomic_json(stage / "manifest.json", manifest)
            os.replace(stage, final_dir)

        # The pointer is the single atomic activation step. Old snapshots remain available.
        pointer = {
            "schema": SCHEMA,
            "snapshot_id": snapshot_id,
            "snapshot_path": str(final_dir),
            "manifest": str(final_dir / "manifest.json"),
            "activated_at_utc": datetime.now(timezone.utc).isoformat(),
        }
        atomic_json(root / "current.json", pointer)
        print(f"UPDATED ClamAV snapshot: {final_dir}")
        print(f"SHA-256 snapshot ID: {snapshot_id}")
        print(f"Manifest: {final_dir / 'manifest.json'}")
        print("Previous snapshots retained. Keep current.json and trusted snapshot IDs protected.")
        return 0
    except (OSError, subprocess.TimeoutExpired, json.JSONDecodeError) as exc:
        print(f"ERROR updating ClamAV databases: {exc}; active pointer unchanged where publication has not completed.", file=sys.stderr)
        return 2
    finally:
        if stage.exists():
            shutil.rmtree(stage, ignore_errors=True)


if __name__ == "__main__":
    raise SystemExit(main())
