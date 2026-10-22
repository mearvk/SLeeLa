#!/usr/bin/env python3
"""Verify signed Sleelavirin release metadata and every artifact it names.

The public key must be obtained and trusted independently of the release bundle.
"""
import argparse
import hashlib
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path, PurePosixPath

SCHEMA = "sleelavirin-release-manifest-v1"
HEX_SHA256 = re.compile(r"^[0-9a-fA-F]{64}$")


def fail(message, code=1):
    print("INVALID: " + message, file=sys.stderr)
    return code


def verify_manifest(root, manifest_path):
    try:
        data = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        return fail(f"cannot read release manifest: {exc}")
    if not isinstance(data, dict) or data.get("schema") != SCHEMA:
        return fail("unsupported release manifest schema")
    artifacts = data.get("artifacts")
    if not isinstance(artifacts, list) or not artifacts:
        return fail("artifacts must be a non-empty list")
    root = root.resolve()
    seen = set()
    for item in artifacts:
        if not isinstance(item, dict):
            return fail("malformed artifact record")
        rel, digest, size = item.get("path"), item.get("sha256"), item.get("size")
        if not isinstance(rel, str) or not rel:
            return fail("artifact path must be a non-empty string")
        posix = PurePosixPath(rel)
        if posix.is_absolute() or "\\" in rel or any(p in ("", ".", "..") for p in rel.split("/")):
            return fail(f"non-canonical artifact path: {rel}")
        if rel in seen:
            return fail(f"duplicate artifact path: {rel}")
        seen.add(rel)
        if not isinstance(digest, str) or not HEX_SHA256.fullmatch(digest):
            return fail(f"invalid SHA-256 value for {rel}")
        if not isinstance(size, int) or isinstance(size, bool) or size < 0:
            return fail(f"invalid size for {rel}")
        candidate = root / rel
        if candidate.is_symlink():
            return fail(f"symlink artifact is not allowed: {rel}")
        path = candidate.resolve()
        try:
            path.relative_to(root)
        except ValueError:
            return fail(f"artifact escapes release root: {rel}")
        if not path.is_file():
            return fail(f"missing artifact: {rel}")
        if path.stat().st_size != size:
            return fail(f"size mismatch: {rel}")
        digest_actual = hashlib.sha256()
        try:
            with path.open("rb") as stream:
                for chunk in iter(lambda: stream.read(1024 * 1024), b""):
                    digest_actual.update(chunk)
        except OSError as exc:
            return fail(f"cannot read artifact {rel}: {exc}", 2)
        if digest_actual.hexdigest().lower() != digest.lower():
            return fail(f"SHA-256 mismatch: {rel}")
    print(f"VALID: signed release manifest contents match {len(artifacts)} artifact(s)")
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--public-key", required=True, type=Path)
    parser.add_argument("--signature", required=True, type=Path)
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--root", type=Path, default=Path("."))
    parser.add_argument("--openssl", default="openssl")
    args = parser.parse_args()
    for label, path in (("public key", args.public_key), ("signature", args.signature),
                        ("manifest", args.manifest), ("root directory", args.root)):
        if not path.exists() or (label == "root directory" and not path.is_dir()):
            return fail(f"missing {label}: {path}", 2)
    openssl = shutil.which(args.openssl)
    if not openssl:
        return fail(f"OpenSSL executable not found: {args.openssl}", 2)
    try:
        result = subprocess.run(
            [openssl, "dgst", "-sha256", "-verify", str(args.public_key),
             "-signature", str(args.signature), str(args.manifest)],
            check=False, capture_output=True, text=True, timeout=30,
        )
    except (OSError, subprocess.TimeoutExpired) as exc:
        return fail(f"signature verification could not complete: {exc}", 2)
    if result.returncode != 0 or "Verified OK" not in result.stdout:
        return fail("release manifest signature verification failed")
    return verify_manifest(args.root, args.manifest)


if __name__ == "__main__":
    raise SystemExit(main())
