#!/usr/bin/env python3
"""Convert remaining legacy comments to the angle-bracket style in lib/ files.

Safety: only converts comments whose opener is the first non-whitespace on its
line (exactly what the angle-bracket grammar requires). This avoids touching
trailing inline comments, '//' inside strings, URLs like http://, and division.

  - A line-leading '// text'         -> '< text >'
  - A line-leading '/* ... */' block -> '< ... >' (one line) or '<< ... >>' (multi)

Trailing inline comments (code then '//') are left as-is: the angle-bracket
form has no inline variant (the opener must start the line), and moving them
would be invasive/risky. Legacy inline comments remain valid per the spec.
"""
import os, re

ROOT = os.path.abspath(os.path.dirname(__file__) + "/..")
LIB = os.path.join(ROOT, "lib")


def convert(text):
    lines = text.split("\n")
    out = []
    i = 0
    n = len(lines)
    changed = False
    while i < n:
        line = lines[i]
        stripped = line.lstrip()
        indent = line[:len(line) - len(stripped)]

        # line-leading /* ... */ block
        if stripped.startswith("/*"):
            block = [line]
            # single-line /* ... */ ?
            if "*/" in stripped[2:]:
                inner = stripped
                inner = re.sub(r'^/\*+', '', inner)
                inner = re.sub(r'\*+/\s*$', '', inner)
                inner = inner.strip()
                out.append(f'{indent}< {inner} >' if inner else f'{indent}<  >')
                changed = True
                i += 1
                continue
            # multi-line block
            j = i
            while j < n and "*/" not in lines[j]:
                j += 1
            if j < n:
                block = lines[i:j+1]
                inner = []
                for b in block:
                    s = b
                    s = re.sub(r'^\s*/\*+', '', s)
                    s = re.sub(r'\*+/\s*$', '', s)
                    s = re.sub(r'^\s*\*\s?', '', s)
                    inner.append(s.rstrip())
                while inner and inner[0].strip() == "":
                    inner.pop(0)
                while inner and inner[-1].strip() == "":
                    inner.pop()
                if len(inner) <= 1:
                    out.append(f'{indent}< {inner[0].strip() if inner else ""} >')
                else:
                    out.append(f'{indent}<< {inner[0].strip()}')
                    for mid in inner[1:-1]:
                        out.append(f'{indent}   {mid.strip()}')
                    out.append(f'{indent}   {inner[-1].strip()} >>')
                changed = True
                i = j + 1
                continue
            # unterminated: leave as-is
            out.append(line)
            i += 1
            continue

        # line-leading // comment
        if stripped.startswith("//"):
            body = stripped[2:].strip()
            out.append(f'{indent}< {body} >' if body else f'{indent}<  >')
            changed = True
            i += 1
            continue

        out.append(line)
        i += 1
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
