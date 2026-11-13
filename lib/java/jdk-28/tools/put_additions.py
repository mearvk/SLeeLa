#!/usr/bin/env python3
"""Put a hand-written JDK 28 additions block into a /lib/java/jdk-28 file.

usage: python3 lib/java/jdk-28/tools/put_additions.py <java.type.Name> [<fragment-file>]

The fragment (default: lib/java/jdk-28/tools/impl/<java.type.Name>.additions-frag)
holds SLeeLa members written from the JDK 28 API specification of that type,
with Java names and arities. It is wrapped in

    < ---- BEGIN java implementation (JDK 28 additions, SLeeLa logic) ---- >
    ...
    < ---- END java implementation ---- >

and placed after the unit's other implementation block (the logic ported from
the jdk-25 tier by lib/java/jdk-25/tools/jdk28_connect.py), replacing an earlier
additions block. Every stub in the constructors / signatures / declarations
blocks that the fragment now defines (same member name, same arity) is dropped,
with its @throwable line, so a member is never declared twice. A member of the
ported block with the same name and arity is dropped too: the additions win.
Stubs the fragment does not define keep their place. The unit gets
`import java.jdk-28;` so the names the additions use resolve in the JDK 28 tier.

Order: run jdk28_connect.py --write first, then this tool. Re-running it with
the same fragment gives the same file.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve()
JDK28 = HERE.parents[1]
REPO = JDK28.parents[2]
sys.path.insert(0, str(JDK28.parent / "jdk-25" / "tools"))

BEGIN = "  < ---- BEGIN java implementation (JDK 28 additions, SLeeLa logic) ---- >"
END = "  < ---- END java implementation ---- >"
ADD_RE = re.compile(r"^  < ---- BEGIN java implementation \(JDK 28 additions, SLeeLa logic\) ---- >\n.*?^  < ---- END java implementation ---- >\n\n?", re.S | re.M)


def load_connect():
    """The member parser and stub pruner of jdk28_connect.py, without running it."""
    src = (JDK28.parent / "jdk-25" / "tools" / "jdk28_connect.py").read_text(encoding="utf-8")
    src = src[: src.rindex("\nmain()")]
    ns: dict = {"__name__": "jdk28_connect_lib", "__file__": str(JDK28.parent / "jdk-25" / "tools" / "jdk28_connect.py")}
    exec(compile(src, "jdk28_connect.py", "exec"), ns)
    return ns


def main() -> int:
    if len(sys.argv) not in (2, 3):
        print(__doc__)
        return 2
    fq = sys.argv[1]
    frag_path = Path(sys.argv[2]) if len(sys.argv) == 3 else JDK28 / "tools" / "impl" / f"{fq}.additions-frag"
    frag = frag_path.read_text(encoding="utf-8").rstrip("\n")
    row = next((l.split("\t") for l in (JDK28 / "INDEX.tsv").read_text(encoding="utf-8").splitlines()
                if l.split("\t")[0] == fq), None)
    if row is None:
        print(f"unknown Java type {fq}")
        return 1
    path = REPO / row[3]
    text = ADD_RE.sub("", path.read_text(encoding="utf-8"))
    c = load_connect()
    heads, fields = c["impl_members"](frag)
    dropped = 0
    for kind in ("constructors", "signatures", "declarations"):
        b = c["block"](text, kind)
        if not b:
            continue
        nb, d = c["prune"](b, heads, fields if kind == "declarations" else set())
        dropped += d
        text = text.replace(b, nb, 1)
    # The additions win over a member of the same name and arity in the logic
    # ported from jdk-25 (for example a constructor that dropped an argument).
    ported = re.search(r"^  < ---- BEGIN java implementation \(ported from.*?^  < ---- END java implementation ---- >\n", text, re.S | re.M)
    if ported:
        nb, d = c["prune"](ported.group(0), heads, fields)
        dropped += d
        text = text[: ported.start()] + nb + text[ported.end():]
    block = BEGIN + "\n" + frag + "\n" + END + "\n\n"
    ported = re.search(r"^  < ---- BEGIN java implementation.*?^  < ---- END java implementation ---- >\n\n?", text, re.S | re.M)
    if ported:
        text = text[: ported.end()] + block + text[ported.end():]
    else:
        anchor = re.search(r'^  String javaSuper = ".*";\n\n', text, re.M)
        text = text[: anchor.end()] + block + text[anchor.end():]
    if "import java.jdk-28;" not in text:   # resolve the names the additions use in the JDK 28 tier
        text = text.replace("material;\n", "material;\nimport java.jdk-28;\n", 1)
    path.write_text(text, encoding="utf-8")
    print(f"{fq}: additions block written to {row[3]}; dropped {dropped} stub(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
