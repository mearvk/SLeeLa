#!/usr/bin/env python3
# seal — integrity metalayer for Sleela / Nordshrift documents (MT-SEAL-0001).
#
# A *seal* is a tamper-evidence metalayer bound to a document. It is NOT written
# into the document's visible bytes: opening the file in vim/gedit shows nothing
# added. The seal lives in a sidecar registry (`.mt/seals/<path>.seal`) that
# editors never open, so the document text stays byte-for-byte unchanged.
#
# The seal carries a SOLVE MATRIX — derived integers naming the "machine natures
# of the code and its numbers" — and an HMAC-SHA256 binding keyed by a password.
# The seal is dormant. It reveals nothing until it is loaded through its "lip":
# a careful, neat call that supplies the exact path AND the exact password. A
# careless or wrong call reveals nothing.
#
# Usage (from repo root):
#   impl/seal/seal.py seal   <file> --pass '<password>'    # affix a dormant seal
#   impl/seal/seal.py list                                 # sealed files (no contents)
#   impl/seal/seal.py reveal <file> --pass '<password>'    # the "lip": careful load
#   impl/seal/seal.py verify <file> --pass '<password>'    # quiet check (exit code)
#
# No third-party deps: stdlib hmac + hashlib (HMAC-SHA256).

import argparse
import hashlib
import hmac
import json
import os
import re
import sys

MARKER = "@MT:"
CANON = "PMRTCSXLGAWNVDHE"          # MT-META-0001 canonical tag order
SERIES = "MT-SEAL-0001"
VERDICT_OK = "IN ORDER — careful until tine."
VERDICT_BAD = "TAMPERED — the mark is broken."
VERDICT_DENIED = "SEALED — the lip does not part. (careful, neat, and the word.)"


def repo_root():
    here = os.path.dirname(os.path.abspath(__file__))
    return os.path.abspath(os.path.join(here, os.pardir, os.pardir))


def seal_path(root, rel):
    return os.path.join(root, ".mt", "seals", rel + ".seal")


def canonical_bytes(path):
    """Canonical content: raw bytes with CRLF/CR normalized to LF, so the seal
    binds meaning, not line-ending accidents."""
    with open(path, "rb") as f:
        data = f.read()
    return data.replace(b"\r\n", b"\n").replace(b"\r", b"\n")


def solve_matrix(path):
    """Derive the solve matrix: integer rows naming the document's machine
    natures and its numbers. Deterministic; recomputed at reveal time.

    Rows:
      bytes      total canonical byte length
      lines      number of LF-delimited lines
      words      whitespace-delimited token count
      numbers    [count_of_numeric_literals, integer_sum_mod_P, max_number]
      mt         16-int metatag presence vector over CANON (MT-META-0001)
      shape      [distinct_byte_values, non_ascii_bytes]
      digest     first 8 bytes of SHA-256(content) as 8 ints (the numeric spine)
    """
    data = canonical_bytes(path)
    text = data.decode("utf-8", errors="replace")

    lines = data.split(b"\n")
    words = text.split()

    nums = [int(n) for n in re.findall(r"(?<![\w.])\d+(?![\w.])", text)]
    P = 3024  # the System depth invariant (SHEET.sheet) — the modulus of the spine
    num_row = [
        len(nums),
        (sum(nums) % P) if nums else 0,
        max(nums) if nums else 0,
    ]

    # metatag vector: 1 if the file's first @MT: marker carries that letter.
    m = re.search(re.escape(MARKER) + r"([A-Z]+)", text)
    tags = m.group(1) if m else ""
    mt_row = [1 if ch in tags else 0 for ch in CANON]

    shape_row = [len(set(data)), sum(1 for b in data if b > 127)]

    spine = list(hashlib.sha256(data).digest()[:8])

    return {
        "bytes": len(data),
        "lines": len(lines),
        "words": len(words),
        "numbers": num_row,
        "mt": mt_row,
        "shape": shape_row,
        "spine": spine,
    }


def serialize_matrix(mx):
    # stable, compact, order-fixed serialization for hashing + storage
    return json.dumps(mx, sort_keys=True, separators=(",", ":"))


def compute_binding(password, content, matrix_ser):
    key = hashlib.sha256(("MT-SEAL:" + password).encode("utf-8")).digest()
    msg = content + b"\x00" + matrix_ser.encode("utf-8")
    return hmac.new(key, msg, hashlib.sha256).hexdigest()


def cmd_seal(root, rel, password):
    abs_path = os.path.join(root, rel)
    if not os.path.isfile(abs_path):
        print(f"no such file: {rel}", file=sys.stderr)
        return 2
    content = canonical_bytes(abs_path)
    mx = solve_matrix(abs_path)
    mxs = serialize_matrix(mx)
    binding = compute_binding(password, content, mxs)

    # The seal stores the matrix + binding, NOT the password. Recovery of the
    # verdict requires re-supplying the password (the lip).
    record = {
        "series": SERIES,
        "target": rel,
        "matrix": mx,
        "binding": binding,
        "algo": "HMAC-SHA256",
        "lip": "careful, neat, and the word",
    }
    sp = seal_path(root, rel)
    os.makedirs(os.path.dirname(sp), exist_ok=True)
    with open(sp, "w") as f:
        f.write(json.dumps(record, indent=2, sort_keys=True) + "\n")
    # The document itself is untouched — invisible to vim/gedit.
    print(f"sealed  {rel}  (dormant; {SERIES})")
    print(f"        matrix rows: bytes,lines,words,numbers,mt,shape,spine")
    print(f"        the lip loads only with the exact path and word.")
    return 0


def load_seal(root, rel):
    sp = seal_path(root, rel)
    if not os.path.isfile(sp):
        return None
    with open(sp) as f:
        return json.load(f)


def cmd_list(root):
    base = os.path.join(root, ".mt", "seals")
    if not os.path.isdir(base):
        print("(no seals)")
        return 0
    found = []
    for dirpath, _dirs, files in os.walk(base):
        for fn in files:
            if fn.endswith(".seal"):
                full = os.path.join(dirpath, fn)
                rel = os.path.relpath(full, base)[:-len(".seal")]
                found.append(rel)
    for rel in sorted(found):
        print(f"sealed  {rel}")
    if not found:
        print("(no seals)")
    return 0


def check(root, rel, password):
    """Return (status, record) where status in {'ok','bad','denied','none'}."""
    rec = load_seal(root, rel)
    if rec is None:
        return "none", None
    abs_path = os.path.join(root, rel)
    if not os.path.isfile(abs_path):
        return "bad", rec
    content = canonical_bytes(abs_path)
    mx_now = solve_matrix(abs_path)
    mxs_now = serialize_matrix(mx_now)
    want = compute_binding(password, content, mxs_now)
    stored = rec.get("binding", "")

    # A wrong password OR a modified document both fail the HMAC compare. We
    # distinguish them: re-derive against the STORED matrix to see if only the
    # password is wrong (document still matches its own recorded matrix).
    if hmac.compare_digest(want, stored):
        return "ok", rec
    # Does the document still match the matrix that was sealed?
    doc_matches_stored_matrix = (mxs_now == serialize_matrix(rec.get("matrix", {})))
    if doc_matches_stored_matrix:
        # document intact, so the binding failed on the password -> denied
        return "denied", rec
    return "bad", rec


def cmd_reveal(root, rel, password):
    status, rec = check(root, rel, password)
    if status == "none":
        # dormant/absent seals reveal nothing about themselves
        print(VERDICT_DENIED)
        return 1
    if status == "ok":
        mx = rec["matrix"]
        print(VERDICT_OK)
        print(f"  target : {rec['target']}")
        print(f"  series : {rec['series']}  ({rec['algo']})")
        print(f"  solve matrix (machine natures & numbers):")
        print(f"    bytes   = {mx['bytes']}")
        print(f"    lines   = {mx['lines']}")
        print(f"    words   = {mx['words']}")
        print(f"    numbers = count {mx['numbers'][0]}, sum%3024 {mx['numbers'][1]}, max {mx['numbers'][2]}")
        print(f"    mt[{CANON}] = {''.join(str(b) for b in mx['mt'])}")
        print(f"    shape   = distinct-bytes {mx['shape'][0]}, non-ascii {mx['shape'][1]}")
        print(f"    spine   = {mx['spine']}")
        return 0
    if status == "denied":
        print(VERDICT_DENIED)
        return 1
    # bad
    print(VERDICT_BAD)
    return 3


def cmd_verify(root, rel, password):
    status, _ = check(root, rel, password)
    return {"ok": 0, "denied": 1, "none": 1, "bad": 3}[status]


def main(argv):
    ap = argparse.ArgumentParser(prog="seal", description="integrity metalayer (MT-SEAL-0001)")
    sub = ap.add_subparsers(dest="cmd", required=True)

    p_seal = sub.add_parser("seal", help="affix a dormant seal")
    p_seal.add_argument("file")
    p_seal.add_argument("--pass", dest="password", required=True)

    sub.add_parser("list", help="list sealed files (no contents)")

    p_rev = sub.add_parser("reveal", help="the lip: careful load -> verdict + matrix")
    p_rev.add_argument("file")
    p_rev.add_argument("--pass", dest="password", required=True)

    p_ver = sub.add_parser("verify", help="quiet check (exit code only)")
    p_ver.add_argument("file")
    p_ver.add_argument("--pass", dest="password", required=True)

    args = ap.parse_args(argv)
    root = repo_root()

    if args.cmd == "seal":
        rel = os.path.relpath(os.path.abspath(args.file), root)
        return cmd_seal(root, rel, args.password)
    if args.cmd == "list":
        return cmd_list(root)
    if args.cmd == "reveal":
        rel = os.path.relpath(os.path.abspath(args.file), root)
        return cmd_reveal(root, rel, args.password)
    if args.cmd == "verify":
        rel = os.path.relpath(os.path.abspath(args.file), root)
        return cmd_verify(root, rel, args.password)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
