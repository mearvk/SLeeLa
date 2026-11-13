#!/usr/bin/env python3
"""Drop the exact-signature stubs that a JDK 28 implementation block now covers.

usage: python3 lib/java/jdk-28/tools/drop_stubs.py <java.type.Name> [...]

generate_jdk28.py does this for a regenerated file, but it also rewrites
thousands of unrelated files (the committed tree has moved on since it last
ran), so this tool performs only the stub-dropping step on the named files.

A stub is a Java-signature entry in the `java constructors` or `java
signatures` block: optional annotation lines followed by one declaration line.
It is dropped when the implementation block declares a member with the same
name, the same parameter count and compatible parameter types. A parameter
type matches when the normalized spellings are equal, or when the stub erased
a reference type to `Any` and the implementation names a class type for it
(`java.lang.Throwable`, `Throwable`). Stubs for other overloads stay. A field in the `java declarations` block (for
example `static Any TRUE = null;`) is dropped when the implementation declares
the same field name.
Idempotent.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]          # lib/java/jdk-28
REPO = ROOT.parents[2]

IMPL_RE = re.compile(r"^  < ---- BEGIN java implementation.*?^  < ---- END java implementation ---- >\n", re.S | re.M)
BLOCK_RE = re.compile(r"(^  < ---- BEGIN java (?:constructors|signatures)[^\n]*\n)(.*?)(^  < ---- END java (?:constructors|signatures) ---- >\n)", re.S | re.M)
MODS = {"public", "protected", "private", "static", "final", "abstract", "synchronized", "native", "default", "strictfp"}
PRIMS = {"int", "long", "short", "byte", "char", "boolean", "double", "float", "void", "String", "Any"}


def norm(t: str) -> str:
    t = t.strip()
    t = re.sub(r"\b(?:java|javax)\.[a-z0-9_.]*\.", "", t)      # drop package prefixes
    return re.sub(r"\s+", "", t)


def split_params(s: str) -> list[str]:
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur)
    types = []
    for p in out:
        p = re.sub(r"@\w+(?:\([^)]*\))?\s*", "", p).strip()
        p = re.sub(r"\bfinal\s+", "", p)
        toks = p.rsplit(None, 1)
        types.append(norm(toks[0] if len(toks) == 2 else p))
    return types


HEADER = re.compile(r"^  ((?:(?:public|protected|private|static|final|abstract|synchronized|native|default)\s+)*)"
                    r"(?:<[^>]*>\s+)?(?:[\w.<>\[\],?]+\s+)?(\w+)\(([^)]*)\)\s*(?:throws [\w., ]+)?\s*\{", re.M)


def members(block: str, cls: str) -> list[tuple[str, list[str]]]:
    found = []
    for m in HEADER.finditer(block):
        name = m.group(2)
        if name in ("if", "while", "for", "switch", "catch", "return", "new", "super", "this", "synchronized"):
            continue
        found.append((name, split_params(m.group(3))))
    return found


def covers(impl_types: list[str], stub_types: list[str]) -> bool:
    if len(impl_types) != len(stub_types):
        return False
    for i, s in zip(impl_types, stub_types):
        if i == s:
            continue
        if s == "Any" and i not in PRIMS and not i.endswith("[]"):
            continue
        return False
    return True


FIELD = re.compile(r"^  (?:(?:public|protected|private|static|final)\s+)*[\w.<>\[\]]+\s+(\w+)\s*(?:=[^;\n]*)?;", re.M)
DECL_RE = re.compile(r"(^  < ---- BEGIN java declarations[^\n]*\n)(.*?)(^  < ---- END java declarations ---- >\n)", re.S | re.M)


def process(text: str, cls: str) -> tuple[str, int]:
    impl = IMPL_RE.search(text)
    if not impl:
        return text, 0
    text, n = drop_members(text, members(impl.group(0), cls))
    # an API-declared field (static Any TRUE = null;) that the implementation declares itself
    impl = IMPL_RE.search(text)
    names = {m.group(1) for m in FIELD.finditer(impl.group(0))}

    def fix(m: re.Match) -> str:
        nonlocal n
        out = []
        for ln in m.group(2).splitlines(keepends=True):
            fm = FIELD.match(ln)
            if fm and fm.group(1) in names:
                n += 1
                continue
            out.append(ln)
        return m.group(1) + "".join(out) + m.group(3)

    return DECL_RE.sub(fix, text), n


def drop_members(text: str, impl_members: list) -> tuple[str, int]:
    """Drop every stub covered by one of impl_members [(name, [param types])]."""
    dropped = 0

    def fix(m: re.Match) -> str:
        nonlocal dropped
        head, body, tail = m.group(1), m.group(2), m.group(3)
        out, keep_from = [], 0
        pending = []                      # annotation lines awaiting their declaration
        for ln in body.splitlines(keepends=True):
            s = ln.strip()
            if s.startswith("@") or (not s) or s.startswith("<"):
                pending.append(ln)
                continue
            hm = HEADER.match(ln)
            drop = False
            if hm:
                name, types = hm.group(2), split_params(hm.group(3))
                drop = any(n == name and covers(t, types) for n, t in impl_members)
            if drop:
                dropped += 1
                # keep comment/blank lines, drop annotations that belonged to the stub
                out.extend(p for p in pending if not p.strip().startswith("@"))
            else:
                out.extend(pending)
                out.append(ln)
            pending = []
        out.extend(pending)
        return head + "".join(out) + tail

    return BLOCK_RE.sub(fix, text), dropped


def main() -> int:
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    index = {}
    for line in (ROOT / "INDEX.tsv").read_text(encoding="utf-8").splitlines():
        f = line.split("\t")
        if len(f) > 3 and not line.startswith("#"):
            index[f[0]] = f[3]
    rc = 0
    for fq in sys.argv[1:]:
        rel = index.get(fq)
        if rel is None:
            print(f"unknown Java type {fq}")
            rc = 1
            continue
        path = REPO / rel
        text = path.read_text(encoding="utf-8")
        new, n = process(text, fq.split(".")[-1])
        if new != text:
            path.write_text(new, encoding="utf-8")
        print(f"{fq}: dropped {n} stub(s)")
    return rc


if __name__ == "__main__":
    sys.exit(main())
