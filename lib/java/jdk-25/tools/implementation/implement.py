#!/usr/bin/env python3
"""Insert implementation bodies into the lib/java JDK 28 envelopes.

A body is the SLeeLa member text of one Java type. It may begin with
directive lines `@extends Parent` (the Java superclass) and `@import pkg`
(a library import the unit needs, e.g. java.java.util when a simple name
such as Vector also exists elsewhere in /lib). The body is placed between
BEGIN/END markers before the closing brace of the envelope, replacing any
previous body; the metadata block and envelope helpers are preserved, the
unit is raised to #sleela 3.0 and metaVersion to 1.1.0.

usage: implement.py <java.pkg.Type.body> ...
The generators in this directory call apply_dir() directly.
"""
import re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2] / "java"
BEGIN = "  < ---- BEGIN java implementation ---- >"
END = "  < ---- END java implementation ---- >"

def apply_file(bf):
    bp = Path(bf)
    fq = bp.name[:-len(".body")]
    parts = fq.split(".")
    assert parts[0] == "java"
    target = ROOT.joinpath(*parts[1:-1], parts[-1] + ".sleela")
    name = parts[-1]
    body = bp.read_text()
    ext = None
    imports = []
    lines = body.split("\n")
    while lines and lines[0].startswith("@"):
        d = lines.pop(0)
        if d.startswith("@extends "):
            ext = d.split()[1]
        elif d.startswith("@import "):
            imports.append(d.split()[1])
    body = "\n".join(lines).strip("\n") + "\n"
    src = target.read_text()
    mo = re.search(r"^class (\w+)(<[^{}]*>)?(?: extends \w+)? \{$", src, re.M)
    assert mo and mo.group(1) == name, target
    tp = mo.group(2) or ""
    header = f"class {name}{tp} extends {ext} {{" if ext else f"class {name}{tp} {{"
    src = src[:mo.start()] + header + src[mo.end():]
    src = re.sub(re.escape(BEGIN) + r".*?" + re.escape(END) + r"\n", "", src, flags=re.S)
    block = BEGIN + "\n" + body + END + "\n"
    marks = [m for m in ("  < ---- BEGIN java declarations", "  < ---- BEGIN java signatures") if m in src]
    if marks:
        # keep the hand-written implementation ahead of the generated blocks
        i = min(src.index(m) for m in marks)
        src = src[:i].rstrip("\n") + "\n\n" + block + src[i:]
    else:
        b = src.rstrip()
        assert b.endswith("}")
        src = b[:-1].rstrip("\n") + "\n\n" + block + "}\n"
    src = re.sub(r"^#sleela \d+\.\d+$", "#sleela 3.0", src, count=1, flags=re.M)
    src = src.replace("SLeeLa Java SE/JDK 28 source envelope.", "SLeeLa Java SE/JDK 28 source implementation.", 1)
    src = src.replace('String metaVersion = "1.0.0";', 'String metaVersion = "1.1.0";', 1)
    for imp in imports:
        line = f"import {imp};"
        if line not in src:
            src = src.replace("material;\n", f"material;\n{line}\n", 1)
    target.write_text(src)
    print("applied", target.relative_to(ROOT.parent))


def apply_dir(directory):
    for bf in sorted(Path(directory).glob("*.body")):
        apply_file(bf)


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        apply_file(arg)
