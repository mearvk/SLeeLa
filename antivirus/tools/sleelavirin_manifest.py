#!/usr/bin/env python3
"""Create/verify SHA-256 manifests for Sleelavirin source trees and release packages.

The manifest excludes itself, build outputs, Python caches, symlinks, and ClamAV's
mutable local database cache. ClamAV snapshots have their own manifest and validator.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import sys

SCHEMA = "sleelavirin-sha256-manifest-v1"
EXCLUDED_DIRS = {"build", "dist", "__pycache__", ".pytest_cache", ".mypy_cache"}
EXCLUDED_PREFIXES = {".git"}
PROTECTED_NAMES = {
    "native-sha256.txt",
    "native-sha256.txt.manifest.json",
    "current.json",
    "manifest.json",
    "SHA256SUMS",
    "INTEGRITY-MANIFEST.json",
}


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def ignored(path, root, manifest_path):
    rel = path.relative_to(root)
    # Never follow or hash symlinks: they can point outside the inventory root
    # or change their targets between manifest creation and verification.
    if path.is_symlink():
        return True
    if path.resolve() == manifest_path.resolve() or path.name == "SHA256SUMS":
        return True
    if any(part in EXCLUDED_PREFIXES or part in EXCLUDED_DIRS for part in rel.parts):
        return True
    if "clamav-cache" in rel.parts:
        return True
    return not path.is_file()


def build_manifest(root, manifest_path):
    entries = []
    for path in sorted(root.rglob("*")):
        if ignored(path, root, manifest_path):
            continue
        rel = path.relative_to(root).as_posix()
        entries.append({
            "path": rel,
            "size": path.stat().st_size,
            "sha256": sha256(path),
            "protected": path.name in PROTECTED_NAMES or rel.startswith("signatures/"),
        })
    protected = [item for item in entries if item["protected"]]
    return {
        "schema": SCHEMA,
        "hash_algorithm": "SHA-256",
        "root": ".",
        "file_count": len(entries),
        "protected_file_count": len(protected),
        "files": entries,
        "security_note": (
            "Hashes detect changes only when compared with a trusted copy of this manifest. "
            "The manifest is not a digital signature; authenticate it independently for tamper evidence."
        ),
    }


def verify(root, manifest_path):
    try:
        expected = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        print(f"INVALID manifest: {exc}", file=sys.stderr)
        return 2
    if not isinstance(expected, dict) or expected.get("schema") != SCHEMA:
        print("INVALID unsupported manifest schema", file=sys.stderr)
        return 2
    errors = []
    records = expected.get("files")
    if not isinstance(records, list):
        print("INVALID files must be a list", file=sys.stderr)
        return 2
    expected_paths = set()
    for item in records:
        if not isinstance(item, dict) or not isinstance(item.get("path"), str):
            errors.append("malformed file record")
            continue
        rel = item["path"]
        posix = PurePosixPath(rel)
        if (not rel or ":" in rel or any(ord(ch) < 32 for ch in rel) or posix.is_absolute() or "\\" in rel
                or any(part in ("", ".", "..") for part in rel.split("/"))):
            errors.append(f"invalid/non-canonical manifest path: {rel}")
            continue
        if rel in expected_paths:
            errors.append(f"duplicate manifest path: {rel}")
            continue
        expected_paths.add(rel)
        candidate = root / rel
        if candidate.is_symlink():
            errors.append(f"symlink not allowed in manifest: {rel}")
            continue
        path = candidate.resolve()
        try:
            path.relative_to(root.resolve())
        except ValueError:
            errors.append(f"path escapes root: {rel}")
            continue
        if not path.is_file():
            errors.append(f"missing file: {rel}")
            continue
        actual_size = path.stat().st_size
        actual_hash = sha256(path)
        if actual_size != item.get("size"):
            errors.append(f"size mismatch: {rel}")
        if actual_hash != item.get("sha256"):
            errors.append(f"SHA-256 mismatch: {rel}")
    actual_paths = {
        p.relative_to(root).as_posix()
        for p in root.rglob("*")
        if not ignored(p, root, manifest_path)
    }
    for rel in sorted(actual_paths - expected_paths):
        errors.append(f"unlisted file: {rel}")
    if errors:
        for error in errors:
            print(f"INVALID {error}", file=sys.stderr)
        return 2
    print(f"VALID {manifest_path}: {len(records)} file(s), SHA-256 verified")
    return 0


def main():
    parser = argparse.ArgumentParser(description="Generate or verify a SHA-256 file-integrity manifest.")
    parser.add_argument("--root", type=Path, default=Path("."), help="directory to inventory (default: current directory)")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--write", type=Path, help="write manifest to this path")
    group.add_argument("--verify", type=Path, help="verify this manifest against current files")
    args = parser.parse_args()
    root = args.root.expanduser().resolve()
    if not root.is_dir():
        parser.error("--root must be a directory")
    manifest_path = (args.write or args.verify).expanduser()
    if not manifest_path.is_absolute():
        manifest_path = (Path.cwd() / manifest_path).resolve()
    if args.verify:
        return verify(root, manifest_path)
    manifest_path.parent.mkdir(parents=True, exist_ok=True)
    try:
        data = build_manifest(root, manifest_path)
        manifest_path.write_text(json.dumps(data, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    except OSError as exc:
        print(f"ERROR writing manifest: {exc}", file=sys.stderr)
        return 2
    print(f"WROTE {manifest_path}: {data['file_count']} file(s), {data['protected_file_count']} protected file(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
