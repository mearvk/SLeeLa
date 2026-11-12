#!/usr/bin/env python3
"""Connect the working Java support to the JDK 28 tier.

`import java.jdk-28;` selects lib/java/jdk-28/<module>/<package>/<Type>.sleela,
one reference file per JDK 28 type. The hand-written logic, the API-determined
declarations (constants, enums, records), the exact signatures and the
constructors were generated into lib/java/jdk-25/java/..., so a program that
imports the JDK 28 tier saw only stubs. This tool carries those blocks across:

  for each jdk-25 unit with any of the blocks
      BEGIN java implementation / declarations / signatures / constructors
  it finds the JDK 28 unit of the same Java type (jdk-28/INDEX.tsv) and
    * replaces the JDK 28 constructors and signatures blocks with the four
      jdk-25 blocks (implementation, declarations, signatures, constructors);
    * takes the Java type parameters and superclass from the jdk-25 class line;
    * adds the jdk-25 library imports, retargeted from `java.jdk-25.<pkg>` to
      `java.jdk-28` (the JDK 28 tier);
    * raises metaVersion to 1.1.0.

WORK IN PROGRESS, DO NOT RUN WITH --write YET. Known flaw: it strips the JDK 28
unit's own declarations / constructors / signatures blocks and replaces them with
the jdk-25 ones, so a JDK 28 member that jdk-25 lacks would be lost, and it only
recognizes the "(JDK 28, SLeeLa logic)" marker, so it overwrote StrictMath's
"(JDK 28, mapped to SLeeLa Math)" block. The planned rewrite keeps the JDK 28 unit
as the base, inserts only the jdk-25 implementation block, and drops just the
JDK 28 stubs (same name and arity) and declarations the implementation provides.
Without --write the tool only reports what it would change.

A JDK 28 unit that already holds its own implementation block (Integer, Long,
Math, StrictMath) is never touched. Idempotent: re-running replaces the same
blocks. usage: jdk28_connect.py [--write] [--only java.util.UUID ...]
"""
import argparse, re, sys
from pathlib import Path

HERE = Path(__file__).resolve()
JDK25 = HERE.parents[1]                 # lib/java/jdk-25
LIBJAVA = JDK25.parent                  # lib/java
JDK28 = LIBJAVA / "jdk-28"
KINDS = ("implementation", "declarations", "signatures", "constructors")
MARK = "  < ---- BEGIN java {k}"
ENDM = "  < ---- END java {k}"

def block(src, kind):
    b = src.find(MARK.format(k=kind))
    if b < 0:
        return None
    e = src.find(ENDM.format(k=kind), b)
    e = src.index("\n", e) + 1
    return src[b:e]

def strip(src, kind):
    while True:
        b = src.find(MARK.format(k=kind))
        if b < 0:
            return src
        e = src.index("\n", src.find(ENDM.format(k=kind), b)) + 1
        src = src[:b].rstrip("\n") + "\n\n" + src[e:].lstrip("\n")


# The JDK 28 tier has Math, StrictMath, Integer and Long as real static classes
# (Math.abs(x), Integer.toUnsignedLong(x)). The jdk-25 code reaches them the old
# way (`Math M = new Math(); M.addExactLong(a, b)`), which the compiler refuses
# for a static method. Rewrite those uses while copying.
CONVERTED = ("Math", "StrictMath", "Integer", "Long")
RENAME = {
    "addExactInt": "Math.addExact", "addExactLong": "Math.addExact",
    "subtractExactInt": "Math.subtractExact", "subtractExactLong": "Math.subtractExact",
    "multiplyExactInt": "Math.multiplyExact", "multiplyExactLong": "Math.multiplyExact",
    "wrapBits": "SLJavaBits.wrap", "digitChar": "SLJavaHex.digitChar",
}

def adapt(body):
    decl = re.compile(r"\b(Math|StrictMath|Integer|Long) (\w+) = new \1\(([^;()]*)\);[ \t]*\n?")
    vars_ = {}
    def take(m):
        vars_[m.group(2)] = (m.group(1), m.group(3).strip())
        return ""
    body = decl.sub(take, body)
    for v, (cls, arg) in vars_.items():
        if arg not in ("", "0"):
            body = re.sub(r"\b%s\.hashCode\(\)" % v, f"{cls}.hashCode({arg})", body)
        def call(m, cls=cls):
            name = m.group(1)
            if name == "toUnsignedStringRadix":
                return "Integer.toHexString("      # only ever used with radix 16
            return RENAME.get(name, f"{cls}.{name}") + "("
        body = re.sub(r"\b%s\.(\w+)\(" % v, call, body)
    body = re.sub(r"Integer\.toHexString\((\w+), 16\)", r"Integer.toHexString(\1)", body)
    body = re.sub(r"new (?:Math|StrictMath)\(\)\.(\w+)\(", lambda m: RENAME.get(m.group(1), "Math." + m.group(1)) + "(", body)
    return body

def index28():
    m = {}
    for line in (JDK28 / "INDEX.tsv").read_text().splitlines():
        if line.startswith("#") or not line.strip():
            continue
        f = line.split("\t")
        m[f[0]] = LIBJAVA.parent.parent / f[3]
    return m

def type_of(path):
    rel = path.relative_to(JDK25 / "java").with_suffix("")
    return "java." + ".".join(rel.parts)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--write", action="store_true", help="actually write files (see the known flaw above)")
    ap.add_argument("--only", nargs="*")
    a = ap.parse_args()
    if not a.write:
        a.dry_run = True
    idx = index28()
    done = skipped_native = no_target = untouched = 0
    for p in sorted((JDK25 / "java").rglob("*.sleela")):
        s25 = p.read_text()
        blocks = {k: block(s25, k) for k in KINDS}
        if not any(blocks.values()):
            continue
        t = type_of(p)
        if a.only and t not in a.only:
            continue
        target = idx.get(t)
        if target is None or not target.exists():
            no_target += 1
            continue
        s28 = target.read_text()
        own = block(s28, "implementation")
        if own and "JDK 28, SLeeLa logic" in own:
            skipped_native += 1
            continue
        out = s28
        for k in KINDS:
            out = strip(out, k)
        m25 = re.search(r"^class (\w+)(<[^{}]*>)?(?: extends ([\w.]+))? \{$", s25, re.M)
        m28 = re.search(r"^class (\w+)(<[^{}]*>)?(?: extends ([\w.]+))? \{$", out, re.M)
        if not m25 or not m28 or m25.group(1) != m28.group(1):
            no_target += 1
            continue
        tp = m25.group(2) or ""          # Java type parameters, so E / T / V in the signatures resolve
        sup = m25.group(3) or m28.group(3)
        ext = f" extends {sup}" if sup else ""
        out = out[:m28.start()] + f"class {m28.group(1)}{tp}{ext} {{" + out[m28.end():]
        for imp in re.findall(r"^import java\.jdk-25[\w.\-]*;$", s25, re.M):
            line = "import java.jdk-28;"
            if line not in out:
                out = out.replace("material;\n", f"material;\n{line}\n", 1)
        body = adapt("".join((blocks[k] + "\n") for k in KINDS if blocks[k]))
        tail = out.rstrip()
        assert tail.endswith("}")
        out = tail[:-1].rstrip("\n") + "\n\n" + body + "}\n"
        out = out.replace('String metaVersion = "1.0.0";', 'String metaVersion = "1.1.0";', 1)
        if out != s28:
            done += 1
            if not a.dry_run:
                target.write_text(out)
        else:
            untouched += 1
    print(f"connected {done}, already current {untouched}, native JDK 28 units skipped {skipped_native}, no JDK 28 target {no_target}")

main()
