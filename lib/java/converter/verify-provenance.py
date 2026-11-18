#!/usr/bin/env python3
"""Verify a native Java-converter provenance manifest without trusting its hashes."""
import hashlib
import json
import pathlib
import sys

def fail(message):
    print(f"provenance: FAIL: {message}", file=sys.stderr)
    raise SystemExit(1)

def digest(path):
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()

def safe_relative(value):
    p = pathlib.PurePosixPath(value)
    return not p.is_absolute() and bool(value) and all(part not in ("", ".", "..") for part in p.parts)

if len(sys.argv) != 5:
    fail("usage: verify-provenance.py MANIFEST REPO_ROOT SOURCE_ROOT OUTPUT_ROOT")
manifest_path, repo_root, source_root, output_root = map(pathlib.Path, sys.argv[1:])
try:
    data = json.loads(manifest_path.read_text(encoding="utf-8"))
except Exception as exc:
    fail(f"cannot parse manifest: {exc}")
if data.get("format") != "sleela-java-converter-provenance-v1":
    fail("unsupported manifest format")

def verify_entries(label, entries, base):
    if not isinstance(entries, list):
        fail(f"{label} must be an array")
    paths = [entry.get("path") for entry in entries]
    if paths != sorted(paths) or len(paths) != len(set(paths)):
        fail(f"{label} paths are not unique and lexically sorted")
    for entry in entries:
        rel = entry.get("path")
        expected = entry.get("sha256")
        if not isinstance(rel, str) or not safe_relative(rel):
            fail(f"unsafe {label} path: {rel!r}")
        path = base.joinpath(*pathlib.PurePosixPath(rel).parts)
        if path.is_symlink() or not path.is_file():
            fail(f"missing or unsafe {label} file: {rel}")
        if not isinstance(expected, str) or len(expected) != 64 or digest(path) != expected:
            fail(f"SHA-256 mismatch: {label}/{rel}")

model = data.get("model", {})
if model.get("path") != "impl/subjects/conversion/conversion.model":
    fail("unexpected model path")
if digest(repo_root / model["path"]) != model.get("sha256"):
    fail("model SHA-256 mismatch")
verify_entries("converterSources", data.get("converterSources"), repo_root)
verify_entries("inputs", data.get("inputs"), source_root)
verify_entries("outputs", data.get("outputs"), output_root)
print("provenance: PASS")
