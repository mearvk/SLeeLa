#!/usr/bin/env python3
"""Collect the .sleela sources backing a SleelaDoc corpus into a flat tree.

Reads a TOPICS.index (one "package<TAB>Unit" line per documented unit, as
written by `sleela docs --generate`) and, for each entry, finds the matching
<Unit>.sleela under lib/<package>/ and copies it to <out>/<package>/<Unit>.sleela.

This mirrors the corpus exactly: the same package scope the doc generator used
(the java compat mirror / early_access are already absent from TOPICS.index),
so the bundled source set matches the doc set one-to-one.
"""
from __future__ import annotations
import argparse
import shutil
import sys
from pathlib import Path


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--lib", required=True, type=Path, help="the /lib root")
    ap.add_argument("--topics", required=True, type=Path, help="TOPICS.index")
    ap.add_argument("--out", required=True, type=Path, help="destination sources/ dir")
    args = ap.parse_args()

    if not args.topics.is_file():
        print(f"collect-sources: no TOPICS.index at {args.topics}", file=sys.stderr)
        return 1

    # Build a quick index of every <name>.sleela under each top-level package so
    # we resolve a unit to its real nested path (e.g. vm/src/sleela/SLVmVersion).
    lib = args.lib.resolve()
    by_pkg_name: dict[tuple[str, str], Path] = {}
    for p in lib.rglob("*.sleela"):
        rel = p.relative_to(lib)
        if not rel.parts:
            continue
        pkg = rel.parts[0]
        by_pkg_name.setdefault((pkg, p.stem), p)  # first wins; stable by rglob order

    copied = 0
    missing = 0
    for line in args.topics.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or "\t" not in line:
            continue
        pkg, unit = line.split("\t", 1)
        src = by_pkg_name.get((pkg, unit))
        if src is None:
            # Fall back to a direct search within the package subtree.
            cands = list((lib / pkg).rglob(unit + ".sleela"))
            src = cands[0] if cands else None
        if src is None:
            missing += 1
            print(f"collect-sources: WARN no source for {pkg}/{unit}", file=sys.stderr)
            continue
        dest = args.out / pkg / (unit + ".sleela")
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(src, dest)
        copied += 1

    print(f"collect-sources: copied {copied} source(s)"
          + (f", {missing} missing" if missing else ""))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
