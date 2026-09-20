#!/usr/bin/env python3
"""Standalone .ledger generator and verifier.

Builds a tamper-evident chain of records over a set of files. Each record binds
the file's SHA-256, a prev-hash link to the previous record, a UTC ISO-8601
timestamp, and a QR insignia (SVG) encoding the record identity. See LEDGER.md.

Usage:
  ledger.py build --out FILE.ledger [--insignia-dir DIR] [--root DIR] FILES...
  ledger.py verify FILE.ledger

No third-party libraries (SHA-256 via hashlib; QR via ledger/qr.py). Offline.
"""
from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import qr  # noqa: E402

GENESIS = "0" * 64
LEDGER_VERSION = 1
QR_EC = "M"

# Fixed key order used for canonical record hashing (record_hash excluded).
_CANON_KEYS = ["seq", "path", "sha256", "prev", "timestamp_utc", "timestamp_kind", "qr_svg"]


def _sha256_file(path: str) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def _sha256_bytes(b: bytes) -> str:
    return hashlib.sha256(b).hexdigest()


def _canonical(record: dict) -> bytes:
    """Compact canonical bytes of a record, keys in fixed order, no record_hash."""
    ordered = {k: record[k] for k in _CANON_KEYS}
    return json.dumps(ordered, separators=(",", ":"), ensure_ascii=False).encode("utf-8")


def record_hash(record: dict) -> str:
    return _sha256_bytes(_canonical(record))


def qr_identity(seq: int, path: str, sha: str, ts: str) -> str:
    return f"sleela-ledger:{LEDGER_VERSION}|{seq}|{path}|{sha}|{ts}"


def build(files, out_path, insignia_dir, root):
    header = {
        "ledger": "sleela",
        "version": LEDGER_VERSION,
        "algo": "sha256",
        "chain": "prev-hash",
        "genesis": GENESIS,
    }
    lines = [json.dumps(header, separators=(",", ":"))]
    prev = GENESIS
    ts = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    for seq, f in enumerate(files, start=1):
        rel = os.path.relpath(f, root) if root else f
        sha = _sha256_file(f)
        qr_rel = os.path.join(insignia_dir, rel + ".svg") if insignia_dir else ""
        rec = {
            "seq": seq,
            "path": rel,
            "sha256": sha,
            "prev": prev,
            "timestamp_utc": ts,
            "timestamp_kind": "utc-iso8601",
            "qr_svg": qr_rel,
        }
        rh = record_hash(rec)
        rec["record_hash"] = rh
        prev = rh
        # emit the QR insignia
        if insignia_dir:
            svg = qr.qr_svg(qr_identity(seq, rel, sha, ts), ec=QR_EC)
            dest = os.path.join(insignia_dir, rel + ".svg")
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            with open(dest, "w", encoding="utf-8") as fh:
                fh.write(svg)
        # ordered emission: canonical keys then record_hash
        emit = {k: rec[k] for k in _CANON_KEYS}
        emit["record_hash"] = rh
        lines.append(json.dumps(emit, separators=(",", ":"), ensure_ascii=False))
    with open(out_path, "w", encoding="utf-8") as fh:
        fh.write("\n".join(lines) + "\n")
    return len(files)


def verify(path: str) -> int:
    with open(path, "r", encoding="utf-8") as fh:
        lines = [ln for ln in fh.read().splitlines() if ln.strip()]
    if not lines:
        print("ledger: empty file")
        return 1
    header = json.loads(lines[0])
    prev = header.get("genesis", GENESIS)
    breaks = 0
    for i, ln in enumerate(lines[1:], start=1):
        rec = json.loads(ln)
        want = record_hash(rec)
        if rec.get("record_hash") != want:
            print(f"ledger: record {i} (seq {rec.get('seq')}) record_hash MISMATCH")
            breaks += 1
        if rec.get("prev") != prev:
            print(f"ledger: record {i} (seq {rec.get('seq')}) prev-link BROKEN")
            breaks += 1
        prev = rec.get("record_hash", want)
    if breaks == 0:
        print(f"ledger: OK — {len(lines) - 1} records, chain intact")
        return 0
    print(f"ledger: FAIL — {breaks} problem(s)")
    return 1


def main(argv=None):
    ap = argparse.ArgumentParser(description="SLeeLa .ledger generator/verifier")
    sub = ap.add_subparsers(dest="cmd", required=True)
    b = sub.add_parser("build")
    b.add_argument("--out", required=True)
    b.add_argument("--insignia-dir", default="")
    b.add_argument("--root", default="")
    b.add_argument("files", nargs="+")
    v = sub.add_parser("verify")
    v.add_argument("ledger")
    args = ap.parse_args(argv)
    if args.cmd == "build":
        n = build(args.files, args.out, args.insignia_dir or None, args.root or None)
        print(f"ledger: wrote {args.out} ({n} records)")
        return 0
    return verify(args.ledger)


if __name__ == "__main__":
    raise SystemExit(main())
