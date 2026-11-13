#!/usr/bin/env python3
"""Add the Sleela.home lineage to the lib/java/jdk-28 stub classes.

For every class stub whose JDK 28 superclass also has a jdk-28 file, this
adds `extends <Parent>` to the class line and a root-directed
`super("constructor");` as the first statement of each stub constructor
(SLEELA.syntax section 42.5). Idempotent. Run from the repository root:

    python3 tools/jdk28_add_lineage.py [--check]
"""
import re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
J28 = ROOT / "lib" / "java" / "jdk-28"
check_only = "--check" in sys.argv

rows = [l.rstrip("\n").split("\t") for l in (J28 / "INDEX.tsv").read_text().splitlines()[1:]]
known = {r[0]: r for r in rows}            # qualified Java name -> row
SUPER_RE = re.compile(r"Java kind: class; extends ([\w.$]+)")
changed = skipped = 0

for qname, module, kind, rel, *_ in rows:
    if kind != "class":
        continue
    path = ROOT / rel
    if not path.is_file():
        continue
    simple = qname.rsplit(".", 1)[-1]
    if "." in simple or "$" in simple:
        continue
    text = path.read_text()
    m = SUPER_RE.search(text)
    if not m:
        continue
    parent_q = m.group(1)
    if parent_q == "java.lang.Object" or parent_q not in known:
        continue
    parent = parent_q.rsplit(".", 1)[-1]
    if "." in parent:
        continue
    new = text
    cls_re = re.compile(r"^class %s \{$" % re.escape(simple), re.M)
    if cls_re.search(new):
        new = cls_re.sub("class %s extends %s {" % (simple, parent), new, count=1)
    elif not re.search(r"^class %s extends %s \{$" % (re.escape(simple), re.escape(parent)), new, re.M):
        skipped += 1
        continue
    # constructors in the stub block: `public Name(...) { }`
    ctor_re = re.compile(r"^(\s*public %s\([^)]*\)) \{ \}$" % re.escape(simple), re.M)
    new = ctor_re.sub(r'\1 { super("constructor"); }', new)
    if new != text:
        changed += 1
        if not check_only:
            path.write_text(new)

print("jdk-28 lineage: %d file(s) %s, %d skipped" % (changed, "need changes" if check_only else "updated", skipped))
