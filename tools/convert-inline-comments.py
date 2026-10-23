#!/usr/bin/env python3
"""Move trailing inline // comments onto their own preceding angle-bracket line.

The angle-bracket comment grammar has no inline form (the opener must be the
first non-whitespace on its line). To eliminate the remaining legacy inline
comments without losing them, each trailing '// comment' is lifted to a '< ... >'
line placed immediately BEFORE the code line, at the code line's indentation.

Safety: a '//' is only treated as a comment when it occurs OUTSIDE a double-
quoted string literal on that line. String scanning respects backslash escapes.
This protects URLs ("https://..."), '//' inside string data, etc. Lines whose
'//' is only ever inside a string are left untouched. Block '/* */' are handled
by the line-leading converter already; only '//' inline is addressed here.
"""
import os

ROOT = os.path.abspath(os.path.dirname(__file__) + "/..")
LIB = os.path.join(ROOT, "lib")


def find_inline_comment(line):
    """Return index of a real '//' comment start, or -1 if none/inside string."""
    in_str = False
    esc = False
    i = 0
    n = len(line)
    while i < n:
        c = line[i]
        if in_str:
            if esc:
                esc = False
            elif c == '\\':
                esc = True
            elif c == '"':
                in_str = False
            i += 1
            continue
        if c == '"':
            in_str = True
            i += 1
            continue
        if c == '/' and i + 1 < n and line[i+1] == '/':
            return i
        i += 1
    return -1


def convert(text):
    lines = text.split("\n")
    out = []
    changed = False
    for line in lines:
        idx = find_inline_comment(line)
        if idx < 0:
            out.append(line)
            continue
        code = line[:idx].rstrip()
        comment = line[idx+2:].strip()
        stripped = line.lstrip()
        # line-leading // should already be converted; if we somehow see one,
        # emit it as a single angle-bracket line (no code before it).
        if code.strip() == "":
            indent = line[:len(line) - len(stripped)]
            out.append(f'{indent}< {comment} >' if comment else f'{indent}<  >')
            changed = True
            continue
        # trailing inline comment: lift to a preceding < > line at code indent
        indent = line[:len(line) - len(line.lstrip())]
        if comment:
            out.append(f'{indent}< {comment} >')
        out.append(code)
        changed = True
    return "\n".join(out), changed


def main():
    files = []
    for dp, dn, fn in os.walk(LIB):
        for name in fn:
            if name.endswith(".sleela"):
                files.append(os.path.join(dp, name))
    files.sort()
    nchanged = 0
    for p in files:
        with open(p, "r", encoding="utf-8") as f:
            text = f.read()
        new, changed = convert(text)
        if changed and new != text:
            with open(p, "w", encoding="utf-8") as f:
                f.write(new)
            nchanged += 1
    print(f"files converted: {nchanged} / {len(files)}")


if __name__ == "__main__":
    main()
