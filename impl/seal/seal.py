#!/usr/bin/env python3
# seal — integrity metalayer for Sleela / Nordshrift documents (MT-SEAL-0001).
#
# A *seal* is a tamper-evidence metalayer bound to a document. By default it is
# NOT written into the document's visible bytes: opening the file in vim/gedit
# shows nothing added. The seal lives in a sidecar registry
# (`.mt/seals/<path>.seal`) that editors never open, so the document text stays
# byte-for-byte unchanged.
#
# The seal carries a SOLVE MATRIX — derived integers naming the "machine natures
# of the code and its numbers" — and one or more HMAC-SHA256 bindings, each
# keyed by a marksman's password. The seal is dormant. It reveals nothing until
# it is loaded through its "lip": a careful, neat call that supplies the exact
# path AND the word(s). A careless or wrong call reveals nothing.
#
# Capabilities:
#   * M-of-N marksmen  — seal with several words + a threshold; IN ORDER only
#                        when at least M distinct marksmen agree.
#   * trailer mode     — for appendable/compiled artifacts, embed the seal as a
#                        trailing block that is EXCLUDED from its own digest.
#   * JSON output      — reveal/verify --json for verification pipelines.
#
# Usage (from repo root):
#   seal   <file> --pass W [--pass W2 ...] [--threshold M] [--trailer]
#   list
#   reveal <file> --pass W [--pass W2 ...] [--json]
#   verify <file> --pass W [--pass W2 ...] [--json]   # exit code (+ optional JSON)
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

# Trailer fencing. The digest is computed over everything BEFORE the begin line
# (plus the trailing newline that precedes it is dropped), so the trailer never
# seals itself. Chosen to be inert as a comment in md/# files.
TRAILER_BEGIN = "<!-- MT-SEAL-0001:BEGIN"
TRAILER_END = "MT-SEAL-0001:END -->"


def repo_root():
    here = os.path.dirname(os.path.abspath(__file__))
    return os.path.abspath(os.path.join(here, os.pardir, os.pardir))


def seal_path(root, rel):
    return os.path.join(root, ".mt", "seals", rel + ".seal")


def read_bytes(path):
    with open(path, "rb") as f:
        return f.read()


def strip_trailer(data):
    """Return (body_bytes, trailer_record_or_None). The body is the document
    with any MT-SEAL trailer removed — this is what the digest binds."""
    text = data.decode("utf-8", errors="replace")
    idx = text.rfind(TRAILER_BEGIN)
    if idx == -1:
        return data, None
    body_text = text[:idx]
    # drop the single separating newline we insert before the trailer, so the
    # body recovered here is byte-identical to the body that was sealed.
    if body_text.endswith("\n"):
        body_text = body_text[:-1]
    tail = text[idx:]
    m = re.search(re.escape(TRAILER_BEGIN) + r"\s*(\{.*\})\s*" + re.escape(TRAILER_END),
                  tail, re.DOTALL)
    rec = None
    if m:
        try:
            rec = json.loads(m.group(1))
        except Exception:
            rec = None
    return body_text.encode("utf-8"), rec


def canonical_bytes_from(data):
    """Canonical content: CRLF/CR normalized to LF, so the seal binds meaning,
    not line-ending accidents. `data` must already be trailer-stripped."""
    return data.replace(b"\r\n", b"\n").replace(b"\r", b"\n")


def load_document(path):
    """Return (canonical_body_bytes, trailer_record_or_None)."""
    raw = read_bytes(path)
    body, trailer = strip_trailer(raw)
    return canonical_bytes_from(body), trailer


def solve_matrix_from(content):
    """Derive the solve matrix from canonical body bytes. Deterministic."""
    text = content.decode("utf-8", errors="replace")
    lines = content.split(b"\n")
    words = text.split()

    nums = [int(n) for n in re.findall(r"(?<![\w.])\d+(?![\w.])", text)]
    P = 3024  # the System depth invariant (SHEET.sheet)
    num_row = [len(nums), (sum(nums) % P) if nums else 0, max(nums) if nums else 0]

    m = re.search(re.escape(MARKER) + r"([A-Z]+)", text)
    tags = m.group(1) if m else ""
    mt_row = [1 if ch in tags else 0 for ch in CANON]

    shape_row = [len(set(content)), sum(1 for b in content if b > 127)]
    spine = list(hashlib.sha256(content).digest()[:8])

    return {
        "bytes": len(content),
        "lines": len(lines),
        "words": len(words),
        "numbers": num_row,
        "mt": mt_row,
        "shape": shape_row,
        "spine": spine,
    }


def serialize_matrix(mx):
    return json.dumps(mx, sort_keys=True, separators=(",", ":"))


def marksman_id(password):
    """A public, non-reversing label for a marksman, so reveal can report WHICH
    words matched without ever storing the words."""
    return hashlib.sha256(("MT-SEAL-ID:" + password).encode("utf-8")).hexdigest()[:12]


def compute_binding(password, content, matrix_ser):
    key = hashlib.sha256(("MT-SEAL:" + password).encode("utf-8")).digest()
    msg = content + b"\x00" + matrix_ser.encode("utf-8")
    return hmac.new(key, msg, hashlib.sha256).hexdigest()


def build_record(rel, content, passwords, threshold):
    mx = solve_matrix_from(content)
    mxs = serialize_matrix(mx)
    marksmen = []
    seen = set()
    for pw in passwords:
        mid = marksman_id(pw)
        if mid in seen:
            continue  # a marksman counts once
        seen.add(mid)
        marksmen.append({"id": mid, "binding": compute_binding(pw, content, mxs)})
    n = len(marksmen)
    m = threshold if threshold is not None else n
    m = max(1, min(m, n))
    return {
        "series": SERIES,
        "target": rel,
        "matrix": mx,
        "algo": "HMAC-SHA256",
        "threshold": m,
        "marksmen": marksmen,
        "lip": "careful, neat, and the word",
    }


def cmd_seal(root, rel, passwords, threshold, trailer):
    abs_path = os.path.join(root, rel)
    if not os.path.isfile(abs_path):
        print(f"no such file: {rel}", file=sys.stderr)
        return 2

    if trailer:
        # The sealed body is EXACTLY what reveal will recover after it strips
        # the trailer: the trailer-stripped, canonical, one-trailing-newline-
        # removed body. Bind over that, then write body + "\n" + trailer, so a
        # subsequent reveal recovers byte-identical body (idempotent, and the
        # trailer never seals itself).
        raw = read_bytes(abs_path)
        body, _ = strip_trailer(raw)                    # drops trailer + 1 newline
        content = canonical_bytes_from(body)            # what reveal binds
        record = build_record(rel, content, passwords, threshold)
        body_text = content.decode("utf-8", errors="replace")
        blob = json.dumps(record, sort_keys=True, separators=(",", ":"))
        new_text = body_text + "\n" + TRAILER_BEGIN + " " + blob + " " + TRAILER_END + "\n"
        with open(abs_path, "wb") as f:
            f.write(new_text.encode("utf-8"))
        where = "trailer (digest-excluded, embedded in the artifact)"
    else:
        content, _existing = load_document(abs_path)
        record = build_record(rel, content, passwords, threshold)
        sp = seal_path(root, rel)
        os.makedirs(os.path.dirname(sp), exist_ok=True)
        with open(sp, "w") as f:
            f.write(json.dumps(record, indent=2, sort_keys=True) + "\n")
        where = "sidecar (invisible to vim/gedit)"

    print(f"sealed  {rel}  (dormant; {SERIES})")
    print(f"        marksmen: {len(record['marksmen'])}  threshold: {record['threshold']}-of-{len(record['marksmen'])}")
    print(f"        store: {where}")
    print(f"        the lip loads only with the exact path and word(s).")
    return 0


def load_seal(root, rel, trailer_record):
    if trailer_record is not None:
        return trailer_record
    sp = seal_path(root, rel)
    if not os.path.isfile(sp):
        return None
    with open(sp) as f:
        return json.load(f)


def normalize_marksmen(rec):
    """Support both the M-of-N shape and the legacy single-binding shape."""
    if "marksmen" in rec:
        return rec["marksmen"], rec.get("threshold", len(rec["marksmen"]))
    # legacy: one binding, threshold 1, unknown id
    if "binding" in rec:
        return [{"id": "legacy", "binding": rec["binding"]}], 1
    return [], 1


def cmd_list(root):
    base = os.path.join(root, ".mt", "seals")
    found = []
    if os.path.isdir(base):
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


def check(root, rel, passwords):
    """Return (status, rec, tally). status in {ok,bad,denied,none}.
    tally = {matched, threshold, total, matched_ids}."""
    abs_path = os.path.join(root, rel)
    trailer_record = None
    content = None
    if os.path.isfile(abs_path):
        content, trailer_record = load_document(abs_path)

    rec = load_seal(root, rel, trailer_record)
    if rec is None:
        return "none", None, None
    if content is None:
        return "bad", rec, None

    marksmen, threshold = normalize_marksmen(rec)
    mxs_now = serialize_matrix(solve_matrix_from(content))

    # How many supplied words match a distinct marksman binding (vs current doc)?
    matched_ids = set()
    for pw in passwords:
        want = compute_binding(pw, content, mxs_now)
        for mk in marksmen:
            if mk["id"] in matched_ids:
                continue
            if hmac.compare_digest(want, mk["binding"]):
                matched_ids.add(mk["id"])
                break
    tally = {
        "matched": len(matched_ids),
        "threshold": threshold,
        "total": len(marksmen),
        "matched_ids": sorted(matched_ids),
    }

    if len(matched_ids) >= threshold:
        return "ok", rec, tally

    # Not enough marksmen. Distinguish "wrong words" from "tampered document":
    # does the doc still match the matrix that was sealed?
    doc_matches_stored = (mxs_now == serialize_matrix(rec.get("matrix", {})))
    if doc_matches_stored:
        return "denied", rec, tally
    return "bad", rec, tally


def integrity_of(root, rel):
    """Passwordless integrity check for CI: does the sealed document still match
    the solve matrix that was recorded in its seal? Returns one of
    {'intact','tampered','missing-doc','no-seal'}. This catches tampering
    WITHOUT any password, since the stored matrix pins the document's shape."""
    abs_path = os.path.join(root, rel)
    trailer_record = None
    content = None
    if os.path.isfile(abs_path):
        content, trailer_record = load_document(abs_path)
    rec = load_seal(root, rel, trailer_record)
    if rec is None:
        return "no-seal"
    if content is None:
        return "missing-doc"
    mxs_now = serialize_matrix(solve_matrix_from(content))
    if mxs_now == serialize_matrix(rec.get("matrix", {})):
        return "intact"
    return "tampered"


def sealed_targets(root):
    """Every sealed document known to the repo: sidecar seals plus any file
    carrying an embedded trailer seal."""
    targets = set()
    base = os.path.join(root, ".mt", "seals")
    if os.path.isdir(base):
        for dirpath, _dirs, files in os.walk(base):
            for fn in files:
                if fn.endswith(".seal"):
                    full = os.path.join(dirpath, fn)
                    targets.add(os.path.relpath(full, base)[:-len(".seal")])
    # trailer-sealed files anywhere in the tree
    for dirpath, dirs, files in os.walk(root):
        if ".git" in dirs:
            dirs.remove(".git")
        for fn in files:
            full = os.path.join(dirpath, fn)
            try:
                with open(full, "rb") as f:
                    head = f.read()
            except Exception:
                continue
            # require a real, complete trailer (BEGIN {json} END), not just a
            # mention of the token (e.g. this tool's own source defines it).
            try:
                htext = head.decode("utf-8", errors="replace")
            except Exception:
                continue
            if re.search(re.escape(TRAILER_BEGIN) + r"\s*\{.*\}\s*"
                         + re.escape(TRAILER_END), htext, re.DOTALL):
                targets.add(os.path.relpath(full, root))
    return sorted(targets)


def cmd_audit(root, as_json):
    """CI gate: verify every sealed document is intact (no password needed).
    Exit 0 if all intact; exit 3 if any tampered or its document is missing."""
    results = []
    rc = 0
    for rel in sealed_targets(root):
        st = integrity_of(root, rel)
        results.append({"target": rel, "integrity": st})
        if st in ("tampered", "missing-doc"):
            rc = 3
    if as_json:
        print(json.dumps({"series": SERIES, "audit": results,
                          "ok": rc == 0}, indent=2, sort_keys=True))
    else:
        if not results:
            print("(no seals to audit)")
        for r in results:
            flag = {"intact": "ok      ", "tampered": "TAMPERED",
                    "missing-doc": "MISSING ", "no-seal": "no-seal "}[r["integrity"]]
            print(f"{flag} {r['target']}")
        print(f"audit: {'ALL IN ORDER' if rc == 0 else 'INTEGRITY FAILURE'}")
    return rc


def emit_json(status, rec, tally):
    out = {
        "verdict": {"ok": "in-order", "bad": "tampered",
                    "denied": "sealed", "none": "sealed"}[status],
        "series": SERIES,
    }
    if rec is not None:
        out["target"] = rec.get("target")
        out["algo"] = rec.get("algo")
    if tally is not None:
        out["marksmen"] = tally
    if status == "ok" and rec is not None:
        out["matrix"] = rec["matrix"]
    print(json.dumps(out, indent=2, sort_keys=True))


def cmd_reveal(root, rel, passwords, as_json):
    status, rec, tally = check(root, rel, passwords)
    if as_json:
        emit_json(status, rec, tally)
        return {"ok": 0, "denied": 1, "none": 1, "bad": 3}[status]

    if status == "none":
        print(VERDICT_DENIED)
        return 1
    if status == "ok":
        mx = rec["matrix"]
        print(VERDICT_OK)
        print(f"  target    : {rec['target']}")
        print(f"  series    : {rec['series']}  ({rec['algo']})")
        if tally:
            print(f"  marksmen  : {tally['matched']}-of-{tally['total']} agreed "
                  f"(threshold {tally['threshold']})")
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
        if tally and tally["matched"] > 0:
            print(f"  ({tally['matched']}-of-{tally['total']} agreed; "
                  f"{tally['threshold']} required — not enough marksmen.)")
        return 1
    print(VERDICT_BAD)
    return 3


def cmd_verify(root, rel, passwords, as_json):
    status, rec, tally = check(root, rel, passwords)
    if as_json:
        emit_json(status, rec, tally)
    return {"ok": 0, "denied": 1, "none": 1, "bad": 3}[status]


def main(argv):
    ap = argparse.ArgumentParser(prog="seal", description="integrity metalayer (MT-SEAL-0001)")
    sub = ap.add_subparsers(dest="cmd", required=True)

    p_seal = sub.add_parser("seal", help="affix a dormant seal")
    p_seal.add_argument("file")
    p_seal.add_argument("--pass", dest="passwords", action="append", required=True,
                        help="a marksman's word; repeat for M-of-N")
    p_seal.add_argument("--threshold", type=int, default=None,
                        help="M: how many marksmen must agree (default: all N)")
    p_seal.add_argument("--trailer", action="store_true",
                        help="embed a digest-excluded trailer instead of a sidecar")

    sub.add_parser("list", help="list sealed files (no contents)")

    p_aud = sub.add_parser("audit", help="CI gate: passwordless integrity of all seals")
    p_aud.add_argument("--json", action="store_true")

    p_rev = sub.add_parser("reveal", help="the lip: careful load -> verdict + matrix")
    p_rev.add_argument("file")
    p_rev.add_argument("--pass", dest="passwords", action="append", required=True)
    p_rev.add_argument("--json", action="store_true")

    p_ver = sub.add_parser("verify", help="quiet check (exit code; optional --json)")
    p_ver.add_argument("file")
    p_ver.add_argument("--pass", dest="passwords", action="append", required=True)
    p_ver.add_argument("--json", action="store_true")

    args = ap.parse_args(argv)
    root = repo_root()

    if args.cmd == "list":
        return cmd_list(root)
    if args.cmd == "audit":
        return cmd_audit(root, args.json)

    rel = os.path.relpath(os.path.abspath(args.file), root)
    if args.cmd == "seal":
        return cmd_seal(root, rel, args.passwords, args.threshold, args.trailer)
    if args.cmd == "reveal":
        return cmd_reveal(root, rel, args.passwords, args.json)
    if args.cmd == "verify":
        return cmd_verify(root, rel, args.passwords, args.json)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
