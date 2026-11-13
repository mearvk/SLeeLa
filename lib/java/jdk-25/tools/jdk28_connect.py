#!/usr/bin/env python3
"""Connect the working Java support to the JDK 28 tier.

`import java.jdk-28;` selects lib/java/jdk-28/<module>/<package>/<Type>.sleela,
one reference file per JDK 28 type. The hand-written logic was generated into
lib/java/jdk-25/..., so a program that imports the JDK 28 tier saw only stubs
(for example `new ArrayList().add("x")` left `size()` at 0). This tool carries
the logic across, keeping the JDK 28 unit as the base:

  for each jdk-25 unit that has a `BEGIN java implementation` block
  it finds the JDK 28 unit of the same Java type (jdk-28/INDEX.tsv) and
    * inserts ONLY the implementation block, marked
      `BEGIN java implementation (ported from the jdk-25 tier ...)`;
    * drops from the JDK 28 constructors / signatures blocks exactly the stubs
      (@throwable entries) the block now defines (same member name and same
      arity), so a JDK 28 member that jdk-25 lacks keeps its loud stub;
    * keeps a JDK 28 member that is already real logic (for example the
      chaining Throwable constructors of tools/implement_throwables.py) and
      drops the ported member of the same name and arity instead;
    * for a Throwable subclass, drops every ported constructor: those belong
      to the JDK 28 Throwable family (lib/java/jdk-28/tools/implement_throwables.py);
    * drops from the JDK 28 declarations block a member or field the block
      redefines (complete enums: constructor, name(), ordinal(), constants);
    * takes the Java type parameters from the jdk-25 class line, so E / K / V
      written in the logic resolve; the JDK 28 superclass is kept, and the
      jdk-25 one is used only when the JDK 28 unit records none;
    * renames a metadata accessor that a Java member shadows
      (java.util.Date.getDate) to getMeta<X> / setMeta<X>, as jdk-25 does;
    * adds `import java.jdk-28;`, so the unit resolves Integer, Math, ... in
      the JDK 28 tier even when it is checked on its own;
    * rewrites the old `Math M = new Math(); M.f()` instance form to the real
      statics (Math, StrictMath, Integer, Long are real static classes at 28);
    * reports a class that would exceed the 64-field layout limit and skips it.

A JDK 28 unit that already holds its own implementation block (Integer, Long,
Math, StrictMath) is never touched. A hand-written additions block
(lib/java/jdk-28/tools/put_additions.py) does not count as one: run this tool
first, then put_additions.py. A unit already connected is skipped, so the
tool is idempotent. Without --write it only reports.
usage: jdk28_connect.py [--write] [--only java.util.UUID ...] [--verbose]
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
    rel = path.relative_to(JDK25).with_suffix("")
    return ".".join(rel.parts)

PORT_BEGIN = "  < ---- BEGIN java implementation (ported from the jdk-25 tier by lib/java/jdk-25/tools/jdk28_connect.py) ---- >"
PORT_END = "  < ---- END java implementation ---- >"
MODS = r"(?:(?:public|protected|private|static|final|synchronized|abstract|native|default)\s+)*"
HEAD = re.compile(r"^  (?:@\w+(?:\([^)]*\))?\s+)*" + MODS + r"(?:[\w.\[\]<>?,]+\s+)?(\w+)\s*\(([^)]*)\)\s*(?:throws [\w., ]+)?\{", re.M)
FIELD = re.compile(r"^  (?:(?:public|protected|private|static|final|volatile|transient)\s+)*[\w.\[\]<>?,]+\s+(\w+)\s*(?:=[^;]*)?;", re.M)

def arity(params):
    params = params.strip()
    if not params:
        return 0
    depth = n = 0
    for ch in params:
        if ch in "<([": depth += 1
        elif ch in ">)]": depth -= 1
        elif ch == "," and depth == 0: n += 1
    return n + 1

def impl_members(body):
    """(name, arity) of every method / constructor head, and the field names, in an implementation block."""
    kw = {"if", "while", "for", "switch", "catch", "return", "else", "new", "throw", "synchronized"}
    heads = {(m.group(1), arity(m.group(2))) for m in HEAD.finditer(body) if m.group(1) not in kw}
    fields = {m.group(1) for m in FIELD.finditer(body)}
    return heads, fields

def prune(block_text, heads, fields, stubs_only=False):
    """Drop stub members whose (name, arity) is defined (with their annotation
    lines and, for a multi-line member, every line up to its closing brace), and
    fields whose name is redeclared."""
    out, pending, dropped = [], [], 0
    in_c = False
    skip_depth = 0
    for line in block_text.split("\n"):
        if skip_depth > 0:
            skip_depth += line.count("{") - line.count("}")
            continue
        st = line.strip()
        if in_c:
            out.append(line)
            if ">>" in line: in_c = False
            continue
        if st.startswith("<<"):
            out.extend(pending); pending = []
            out.append(line)
            if ">>" not in line: in_c = True
            continue
        top = line.startswith("  ") and not line.startswith("   ")
        if top and st.startswith("@"):
            pending.append(line)
            continue
        m = HEAD.match(line) if top else None
        is_stub = any(p.strip().startswith("@throwable") for p in pending)
        if m and (m.group(1), arity(m.group(2))) in heads and (is_stub or not stubs_only):
            pending = []
            dropped += 1
            skip_depth = line.count("{") - line.count("}")
            continue
        f = FIELD.match(line) if top and not m else None
        if f and f.group(1) in fields:
            pending = []
            dropped += 1
            continue
        out.extend(pending)
        pending = []
        out.append(line)
    out.extend(pending)
    return "\n".join(out), dropped

def real_members(text):
    """(name, arity) of the members in the JDK 28 constructors / signatures blocks that are real logic, not @throwable stubs."""
    out = set()
    for kind in ("constructors", "signatures"):
        b = block(text, kind)
        if not b:
            continue
        pending_stub = False
        for line in b.split("\n"):
            st = line.strip()
            if st.startswith("@"):
                pending_stub = pending_stub or st.startswith("@throwable")
                continue
            m = HEAD.match(line) if line.startswith("  ") and not line.startswith("   ") else None
            if m and not pending_stub:
                out.add((m.group(1), arity(m.group(2))))
            pending_stub = False
    return out

def field_count(text):
    """Instance fields only: static members take no layout slot."""
    return sum(1 for m in FIELD.finditer(text) if " static " not in " " + m.group(0).split("=")[0] + " ")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true", help="write the JDK 28 files (default: report only)")
    ap.add_argument("--only", nargs="*")
    ap.add_argument("--verbose", action="store_true")
    a = ap.parse_args()
    idx = index28()
    st = dict(connected=0, current=0, native=0, no_target=0, too_wide=0, mismatch=0, dropped=0)
    notes = []
    units = sorted(list((JDK25 / "java").rglob("*.sleela")) + list((JDK25 / "javax").rglob("*.sleela")))
    for p in units:
        s25 = p.read_text()
        impl = block(s25, "implementation")
        if not impl:
            continue
        t = type_of(p)
        if a.only and t not in a.only:
            continue
        target = idx.get(t)
        if target is None or not target.exists():
            st["no_target"] += 1
            continue
        s28 = target.read_text()
        heads28 = re.findall(r"^  < ---- BEGIN java implementation(.*)$", s28, re.M)
        if any("ported from the jdk-25 tier" in h for h in heads28):
            st["current"] += 1
            continue
        if any("additions" not in h for h in heads28):
            st["native"] += 1          # its own JDK 28 logic (Integer, Long, Math, StrictMath)
            continue
        m25 = re.search(r"^class (\w+)(<[^{}]*>)?(?: extends ([\w.]+))? \{$", s25, re.M)
        m28 = re.search(r"^class (\w+)(<[^{}]*>)?(?: extends ([\w.]+))? \{$", s28, re.M)
        if not m25 or not m28 or m25.group(1) != m28.group(1):
            st["mismatch"] += 1
            notes.append(f"{t}: class line does not match")
            continue
        body = adapt(impl)
        # strip jdk-25 marker lines, put ours
        lines = body.rstrip("\n").split("\n")
        assert lines[0].strip().startswith("< ---- BEGIN java implementation")
        inner = "\n".join(lines[1:-1])
        if "this.stackTrace = new StackTraceElement" in inner:
            # A Throwable subclass: its constructors belong to the JDK 28 Throwable
            # family (lib/java/jdk-28/tools/implement_throwables.py), whose state
            # differs from the jdk-25 one. Drop every ported constructor.
            ctors = {(m.group(1), arity(m.group(2))) for m in HEAD.finditer(inner) if m.group(1) == m28.group(1)}
            inner, _c = prune(inner, ctors, set())
        real28 = real_members(s28)
        if real28:
            inner, _kept = prune(inner, real28, set())   # a real JDK 28 member wins over the ported one
        heads, fields = impl_members(inner)
        out = s28
        dropped = 0
        for kind in ("constructors", "signatures", "declarations"):
            b = block(out, kind)
            if not b:
                continue
            nb, d = prune(b, heads, fields if kind == "declarations" else set(), stubs_only=(kind != "declarations"))
            dropped += d
            out = out.replace(b, nb, 1)
        # A Java member that shares a metadata accessor name (java.util.Date
        # getDate/setDate) wins; the metadata accessor becomes getMeta<X>/setMeta<X>.
        for mm in re.finditer(r"^  (\w+) (get|set)(\w+)\((\w+ v)?\) \{ (?:return )?meta\w+(?: = v)?; \}$", out, re.M):
            acc = mm.group(2) + mm.group(3)
            if (acc, 1 if mm.group(4) else 0) in heads and not mm.group(3).startswith("Meta"):
                ren = mm.group(0).replace(" " + acc + "(", " " + mm.group(2) + "Meta" + mm.group(3) + "(", 1)
                out = out.replace(mm.group(0), ren, 1)
        tp = m25.group(2) or ""
        sup = m28.group(3) or m25.group(3)   # JDK 28 superclass; jdk-25 only when JDK 28 records none
        ext = f" extends {sup}" if sup else ""
        m28 = re.search(r"^class (\w+)(<[^{}]*>)?(?: extends ([\w.]+))? \{$", out, re.M)
        out = out[:m28.start()] + f"class {m28.group(1)}{tp}{ext} {{" + out[m28.end():]
        if "import java.jdk-28;" not in out:   # the unit lives in the JDK 28 tier; resolve its names there
            out = out.replace("material;\n", "material;\nimport java.jdk-28;\n", 1)
        anchor = re.search(r"^  String javaSuper = \".*\";\n\n", out, re.M)
        if not anchor:
            st["mismatch"] += 1
            notes.append(f"{t}: no javaSuper anchor")
            continue
        ported = PORT_BEGIN + "\n" + inner + "\n" + PORT_END + "\n"
        out = out[:anchor.end()] + ported + "\n" + out[anchor.end():]
        out = out.replace('String metaVersion = "1.0.0";', 'String metaVersion = "1.1.0";', 1)
        if field_count(out) > MAX_FIELDS:
            st["too_wide"] += 1
            notes.append(f"{t}: {field_count(out)} instance fields > {MAX_FIELDS}; skipped")
            continue
        st["connected"] += 1
        st["dropped"] += dropped
        if a.verbose:
            notes.append(f"{t}: dropped {dropped} stub(s)")
        if a.write:
            target.write_text(out)
    for n in notes:
        print(n)
    print(("wrote" if a.write else "would connect") + f" {st['connected']} units (dropped {st['dropped']} stubs); "
          f"already connected {st['current']}, native JDK 28 units skipped {st['native']}, "
          f"no JDK 28 target {st['no_target']}, class-line mismatch {st['mismatch']}, over 64 fields {st['too_wide']}")

MAX_FIELDS = 64
main()
