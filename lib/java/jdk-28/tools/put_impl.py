#!/usr/bin/env python3
"""Put a java implementation block into a /lib/java/jdk-28 file.

usage: python3 lib/java/jdk-28/tools/put_impl.py <java.type.Name> <fragment-file>

The fragment holds SLeeLa members (fields, constructors, methods) written from
the JDK 28 Java Source of that type, with Java names and return types. It is
wrapped in
    < ---- BEGIN java implementation (JDK 28, SLeeLa logic) ---- >
    ...
    < ---- END java implementation ---- >
and replaces any existing implementation block. Run generate_jdk28.py
afterwards: it keeps the block and drops every stub the block now covers.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BEGIN = "  < ---- BEGIN java implementation (JDK 28, SLeeLa logic) ---- >"
END = "  < ---- END java implementation ---- >"
IMPL_RE = re.compile(r"^  < ---- BEGIN java implementation.*?^  < ---- END java implementation ---- >\n", re.S | re.M)


def main() -> int:
    if len(sys.argv) != 3:
        print(__doc__)
        return 2
    fq, frag = sys.argv[1], Path(sys.argv[2]).read_text(encoding="utf-8").rstrip("\n")
    row = next((l.split("\t") for l in (ROOT / "INDEX.tsv").read_text(encoding="utf-8").splitlines()
                if l.split("\t")[0] == fq), None)
    if row is None:
        print(f"unknown Java type {fq}")
        return 1
    path = ROOT.parent.parent.parent / row[3]
    text = path.read_text(encoding="utf-8")
    block = BEGIN + "\n" + frag + "\n" + END + "\n"
    if IMPL_RE.search(text):
        text = IMPL_RE.sub(lambda _m: block, text, count=1)
    else:
        anchor = re.search(r'^  String javaSuper = ".*";\n\n', text, re.M)
        text = text[:anchor.end()] + block + text[anchor.end():]
    path.write_text(text, encoding="utf-8")
    print(f"{fq}: implementation block written to {row[3]}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
