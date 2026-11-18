#!/usr/bin/env python3
"""Independently verify CONVERSION-PROVENANCE.json from sleelactl java-convert."""
import hashlib
import json
import pathlib
import sys

def fail(message):
    print(f"javaconv provenance: FAIL: {message}", file=sys.stderr)
    raise SystemExit(1)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def file_sha(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()

def safe(rel):
    p = pathlib.PurePosixPath(rel)
    return bool(rel) and not p.is_absolute() and all(x not in ("", ".", "..") for x in p.parts)

if len(sys.argv) != 5:
    fail("usage: verify-provenance.py MANIFEST REPO_ROOT SOURCE_ROOT OUTPUT_ROOT")
manifest, repo, src, out = map(pathlib.Path, sys.argv[1:])
try:
    data = json.loads(manifest.read_text(encoding="utf-8"))
except Exception as exc:
    fail(f"cannot parse manifest: {exc}")
if data.get("schema") != "sleelactl.javaconv.provenance.v1":
    fail("unsupported manifest schema")

def verify_list(label, entries, root):
    if not isinstance(entries, list):
        fail(f"{label} is not an array")
    names = [x.get("path") for x in entries]
    if names != sorted(names) or len(names) != len(set(names)):
        fail(f"{label} paths are not unique and sorted")
    fingerprint = bytearray()
    for item in entries:
        rel, expected = item.get("path"), item.get("sha256")
        if not isinstance(rel, str) or not safe(rel):
            fail(f"unsafe {label} path: {rel!r}")
        path = root.joinpath(*pathlib.PurePosixPath(rel).parts)
        if path.is_symlink() or not path.is_file():
            fail(f"missing or unsafe {label} file: {rel}")
        actual = file_sha(path)
        if actual != expected:
            fail(f"SHA-256 mismatch: {label}/{rel}")
        fingerprint.extend(rel.encode()); fingerprint.append(0)
        fingerprint.extend(actual.encode()); fingerprint.append(10)
    return sha(bytes(fingerprint))

model = data.get("model", {})
if not isinstance(model, dict) or not isinstance(model.get("sha256"), str):
    fail("model digest missing")
# The converter records the exact model digest; its configured path may be outside
# the repository, so the default shared model is the verifiable repository contract.
model_path = repo / "impl/subjects/conversion/conversion.model"
if file_sha(model_path) != model["sha256"]:
    fail("shared conversion model digest mismatch")
converter_fp = verify_list("converterSources", data.get("converterSources"), repo)
if converter_fp != data.get("converterSourceSha256"):
    fail("converter source aggregate mismatch")
input_fp = verify_list("inputs", data.get("inputs"), src)
if input_fp != data.get("inputSnapshotSha256"):
    fail("input snapshot aggregate mismatch")
output_fp = verify_list("outputs", data.get("outputs"), out)
if output_fp != data.get("outputSnapshotSha256"):
    fail("output snapshot aggregate mismatch")
print("javaconv provenance: PASS")
