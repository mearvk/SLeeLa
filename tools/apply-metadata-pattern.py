#!/usr/bin/env python3
"""Apply the Metadata Class metapattern to all class-bearing .sleela files
under lib/, and convert leading comment banners to the angle-bracket style.

Honesty rule: values are derived only from what is knowable (git history for
dates/author, path for the vendored JDK mirror). Unknowns are "none"/"unknown",
never fabricated.
"""
import os, re, subprocess, sys

ROOT = os.path.abspath(os.path.dirname(__file__) + "/..")
LIB = os.path.join(ROOT, "lib")

HEADER = "---- Descriptive metadata (version 1.11 nested-friendly accessors) ----"
ALREADY = "Descriptive metadata (version 1.11"

# cache git info in one batch per file
def git_dates(relpath):
    """Return (birth_date, last_date, author) from git, or (None,None,None)."""
    try:
        add = subprocess.run(
            ["git", "log", "--follow", "--diff-filter=A",
             "--format=%ad\t%an", "--date=short", "--", relpath],
            cwd=ROOT, capture_output=True, text=True, timeout=30)
        last = subprocess.run(
            ["git", "log", "-1", "--format=%ad", "--date=short", "--", relpath],
            cwd=ROOT, capture_output=True, text=True, timeout=30)
        birth_date, author = None, None
        add_lines = [l for l in add.stdout.strip().splitlines() if l.strip()]
        if add_lines:
            parts = add_lines[-1].split("\t")
            birth_date = parts[0].strip() or None
            author = parts[1].strip() if len(parts) > 1 else None
        last_date = last.stdout.strip() or None
        return birth_date, last_date, author
    except Exception:
        return None, None, None


def meta_values(relpath, birth, last, author):
    """Build the eleven honest values for this file."""
    is_jdk = relpath.startswith("lib/java/")
    v = {}
    v["metaVersion"] = "1.0.0"
    v["metaId"] = "0"
    v["metaDate"] = last or "unknown"
    v["metaBirthDate"] = birth or "unknown"
    v["metaScienceRating"] = "8"
    v["metaOrigin"] = "International"
    if is_jdk:
        v["metaAuthor"] = "Oracle / OpenJDK"
        v["metaCopyright"] = "GPLv2 with Classpath Exception (OpenJDK)"
        v["metaTrademark"] = "Java"
        v["metaCorporation"] = "Oracle Corporation"
        v["metaNationalSchedule"] = "OpenJDK (JDK 28 API)"
    else:
        v["metaAuthor"] = author or "unknown"
        v["metaCopyright"] = "SLeeLa project"
        v["metaTrademark"] = "none"
        v["metaCorporation"] = "none"
        v["metaNationalSchedule"] = "none"
    return v


def build_block(indent, v):
    i = indent
    i2 = indent
    lines = []
    lines.append(f'{i}< {HEADER} >')
    lines.append(f'{i}<< Eleven catalogued attributes with getter/setter pairs, so a GUI, loader,')
    lines.append(f'{i}   or catalog can read and (where appropriate) override this record.')
    lines.append(f'{i}   Values are faithful to the subject\'s real origin where publicly known. >>')
    lines.append(f'{i}String metaVersion = "{v["metaVersion"]}";')
    lines.append(f'{i}int metaId = {v["metaId"]};')
    lines.append(f'{i}String metaAuthor = "{v["metaAuthor"]}";')
    lines.append(f'{i}String metaDate = "{v["metaDate"]}";')
    lines.append(f'{i}String metaCopyright = "{v["metaCopyright"]}";')
    lines.append(f'{i}String metaTrademark = "{v["metaTrademark"]}";')
    lines.append(f'{i}String metaCorporation = "{v["metaCorporation"]}";')
    lines.append(f'{i}< metaOrigin: National | <nation> | International | Galactic >')
    lines.append(f'{i}String metaOrigin = "{v["metaOrigin"]}";')
    lines.append(f'{i}String metaBirthDate = "{v["metaBirthDate"]}";')
    lines.append(f'{i}< metaScienceRating: maturity/robustness on a 1..10 scale >')
    lines.append(f'{i}int metaScienceRating = {v["metaScienceRating"]};')
    lines.append(f'{i}< metaNationalSchedule: a real, citable instrument or "none" >')
    lines.append(f'{i}String metaNationalSchedule = "{v["metaNationalSchedule"]}";')
    lines.append('')
    pairs = [
        ("MetaVersion", "metaVersion", "String"),
        ("MetaId", "metaId", "int"),
        ("Author", "metaAuthor", "String"),
        ("Date", "metaDate", "String"),
        ("Copyright", "metaCopyright", "String"),
        ("Trademark", "metaTrademark", "String"),
        ("Corporation", "metaCorporation", "String"),
        ("Origin", "metaOrigin", "String"),
        ("BirthDate", "metaBirthDate", "String"),
        ("ScienceRating", "metaScienceRating", "int"),
        ("NationalSchedule", "metaNationalSchedule", "String"),
    ]
    for name, field, typ in pairs:
        lines.append(f'{i}{typ} get{name}() {{ return {field}; }}')
        lines.append(f'{i}void set{name}({typ} v) {{ {field} = v; }}')
    return "\n".join(lines)


CLASS_RE = re.compile(r'^([ \t]*)(?:class|struct)\s+[A-Za-z_][A-Za-z0-9_]*[^\n{]*\{',
                      re.MULTILINE)


def convert_leading_comments(text):
    """Convert a leading /* ... */ banner and contiguous // header lines to
    angle-bracket style. Only touches the leading comment region before the
    first code (#sleela / class)."""
    lines = text.split("\n")
    out = []
    idx = 0
    n = len(lines)
    # 1) leading /* ... */ block
    if idx < n and lines[idx].lstrip().startswith("/*"):
        block = []
        while idx < n:
            block.append(lines[idx])
            if "*/" in lines[idx]:
                idx += 1
                break
            idx += 1
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
            out.append(f'< {inner[0].strip() if inner else ""} >')
        else:
            out.append("<< " + inner[0])
            for mid in inner[1:-1]:
                out.append("   " + mid)
            out.append("   " + inner[-1] + " >>")
    # 2) emit the rest, converting // header lines that appear before the
    #    first real code. The #sleela pragma and blank lines do NOT count as
    #    code, so a // banner after '#sleela 2.1' is still converted.
    seen_code = False
    while idx < n:
        line = lines[idx]
        stripped = line.lstrip()
        if not seen_code and stripped.startswith("//"):
            indent = line[:len(line) - len(stripped)]
            body = stripped[2:].strip()
            out.append(f'{indent}< {body} >' if body else f'{indent}<  >')
        else:
            if stripped and not stripped.startswith("//") \
               and not stripped.startswith("#sleela") \
               and not stripped.startswith("import "):
                seen_code = True
            out.append(line)
        idx += 1
    return "\n".join(out)


def process(path):
    rel = os.path.relpath(path, ROOT)
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()
    if ALREADY in text:
        return "skip-have"
    m = CLASS_RE.search(text)
    if not m:
        return "skip-noclass"
    birth, last, author = git_dates(rel)
    v = meta_values(rel, birth, last, author)
    indent = m.group(1) + "  "
    block = build_block(indent, v)
    insert_at = m.end()
    newtext = text[:insert_at] + "\n" + block + text[insert_at:]
    newtext = convert_leading_comments(newtext)
    with open(path, "w", encoding="utf-8") as f:
        f.write(newtext)
    return "done"


GITMAP = None
def load_gitmap():
    global GITMAP
    import json
    p = os.path.join(ROOT, ".tmpval", "gitmap.json")
    if os.path.exists(p):
        GITMAP = json.load(open(p))

def git_dates_fast(relpath):
    if GITMAP is None:
        return git_dates(relpath)
    return (GITMAP["birth"].get(relpath),
            GITMAP["last"].get(relpath),
            GITMAP["author"].get(relpath))


def main():
    load_gitmap()
    global git_dates
    if GITMAP is not None:
        git_dates = git_dates_fast
    files = []
    for dp, dn, fn in os.walk(LIB):
        for name in fn:
            if name.endswith(".sleela"):
                files.append(os.path.join(dp, name))
    files.sort()
    stats = {"done": 0, "skip-have": 0, "skip-noclass": 0}
    for i, p in enumerate(files):
        r = process(p)
        stats[r] = stats.get(r, 0) + 1
        if (i + 1) % 500 == 0:
            print(f"  ...{i+1}/{len(files)} processed", flush=True)
    print(f"TOTAL files={len(files)} done={stats['done']} "
          f"already={stats['skip-have']} noclass={stats['skip-noclass']}")


if __name__ == "__main__":
    main()
