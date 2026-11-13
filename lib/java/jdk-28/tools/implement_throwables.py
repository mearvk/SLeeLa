#!/usr/bin/env python3
"""Give every JDK 28 Throwable subclass working standard constructors.

usage: python3 lib/java/jdk-28/tools/implement_throwables.py [--check]

java.lang.Throwable carries the state (tools/impl/java.lang.Throwable.sleela-frag).
Every class below it in lib/java/jdk-28 declares its constructors as stubs:

    @throwable("java.lang.UnsupportedOperationException")
    public IllegalArgumentException(String s) { super("constructor"); }

For the four standard shapes the stub becomes the real chaining constructor
Java writes:

    ()                       -> super();
    (String message)         -> super(message);
    (String message, cause)  -> super(message, cause);
    (cause)                  -> super(cause);
    (String, cause, boolean, boolean)  -> super(message, cause, a, b);

A shape is rewritten only when the immediate superclass declares the same
shape (or is java.lang.Throwable, which declares all five), so the generated
`super(...)` always resolves. A `cause` parameter is typed java.lang.Throwable
(the stub erased it to Any) so that (String) and (Throwable) stay distinct
overloads. All other constructor shapes keep their stub. Stubs that only
override a java.lang.Throwable method with `return null` (getMessage, toString,
getCause, ...) are dropped so the working inherited method is reached.
Idempotent.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import drop_stubs

ROOT = Path(__file__).resolve().parents[1]          # lib/java/jdk-28
THROWABLE = "java.lang.Throwable"
CAUSE_NAMES = {"cause", "t", "e", "th", "throwable", "ex", "exception", "rootCause", "reason"}

CTOR_BLOCK = re.compile(r"(^  < ---- BEGIN java constructors[^\n]*\n)(.*?)(^  < ---- END java constructors ---- >\n)", re.S | re.M)
# optional annotation lines, then one declaration line
CTOR = re.compile(r"((?:^  @[^\n]*\n)*)^  ((?:public |protected )?)(\w+)\(([^)]*)\) \{ ?(?:super\(\"constructor\"\); ?)?\}\n", re.M)


def parse_params(raw: str):
    out = []
    for p in [x.strip() for x in raw.split(",") if x.strip()]:
        t, _, n = p.rpartition(" ")
        out.append((t.strip(), n.strip()))
    return out


def shape_of(params):
    """Standard shape name for a parameter list, or None."""
    types = [{"java.lang.String": "String", "java.lang.Throwable": "Any", "Throwable": "Any"}.get(t, t) for t, _ in params]
    names = [n for _, n in params]
    if types == []:
        return "()"
    if types == ["String"]:
        return "(S)"
    if types == ["String", "Any"] and names[1] in CAUSE_NAMES:
        return "(S,T)"
    if types == ["Any"] and names[0] in CAUSE_NAMES:
        return "(T)"
    if types == ["String", "Any", "boolean", "boolean"] and names[1] in CAUSE_NAMES:
        return "(S,T,B,B)"
    return None


def load():
    info = {}
    for p in ROOT.rglob("*.sleela"):
        if "tools" in p.parts:
            continue
        s = p.read_text(encoding="utf-8")
        m = re.search(r'^  String javaType = "([^"]+)";', s, re.M)
        if not m:
            continue
        sup = re.search(r'^  String javaSuper = "([^"]*)";', s, re.M)
        info[m.group(1)] = (p, sup.group(1) if sup else "", s)
    return info


def descendants(info):
    memo = {}

    def is_t(t, seen=()):
        if t == THROWABLE:
            return True
        if t in memo:
            return memo[t]
        if t not in info or t in seen:
            return False
        r = is_t(info[t][1], seen + (t,))
        memo[t] = r
        return r

    return [t for t in info if t != THROWABLE and is_t(t)]


def declared_shapes(text: str, cls: str):
    shapes = set()
    m = CTOR_BLOCK.search(text)
    if m:
        for c in re.finditer(r"^  (?:public |protected )?" + re.escape(cls) + r"\(([^)]*)\)", m.group(2), re.M):
            s = shape_of(parse_params(c.group(1)))
            if s:
                shapes.add(s)
    return shapes


# Throwable's own behavior. A subclass stub with one of these signatures only
# returns the default value (null), which would hide the working inherited method.
INHERITED = [("getMessage", []), ("getLocalizedMessage", []), ("toString", []), ("getCause", []),
             ("initCause", ["Any"]), ("fillInStackTrace", []), ("getStackTrace", []),
             ("setStackTrace", ["Any[]"]), ("getSuppressed", []), ("addSuppressed", ["Any"]),
             ("printStackTrace", [])]


def transform(text: str, fq: str, parent_shapes) -> str:
    cls = fq.split(".")[-1]
    text, _ = drop_stubs.drop_members(text, INHERITED)
    m = CTOR_BLOCK.search(text)
    if not m:
        return text

    def fix(c: re.Match) -> str:
        annos, mods, name, raw = c.group(1), c.group(2), c.group(3), c.group(4)
        if name != cls:
            return c.group(0)
        params = parse_params(raw)
        shape = shape_of(params)
        if shape is None or shape not in parent_shapes:
            return c.group(0)
        names = [n for _, n in params]
        typed = []
        for (t, n) in params:
            if t == "String":
                t = "java.lang.String"
            elif t == "Any":
                t = "java.lang.Throwable"
            typed.append(f"{t} {n}")
        keep = "".join(a for a in annos.splitlines(keepends=True) if "@throwable" not in a)
        return f"{keep}  {mods}{cls}({', '.join(typed)}) {{ super({', '.join(names)}); }}\n"

    body = CTOR.sub(fix, m.group(2))
    return text[:m.start(2)] + body + text[m.end(2):]


def main() -> int:
    check = "--check" in sys.argv
    info = load()
    kids = descendants(info)
    # parent shapes: Throwable declares all five; others as currently declared
    all5 = {"()", "(S)", "(S,T)", "(T)", "(S,T,B,B)"}
    # order by depth so a parent is rewritten (and its shapes known) before its children
    def depth(t):
        d = 0
        while t != THROWABLE and t in info:
            t = info[t][1]
            d += 1
        return d

    changed = 0
    shapes = {THROWABLE: all5}
    for fq in sorted(kids, key=depth):
        path, sup, text = info[fq]
        parent = shapes.get(sup, set())
        # a superclass that is a nested type (Outer.Inner.sleela) is spelled with
        # its simple name in the `extends` clause, which the loader cannot yet
        # resolve; leave such constructors as stubs
        if sup in info and "." in info[sup][0].stem:
            parent = set()
        new = transform(text, fq, parent)
        shapes[fq] = declared_shapes(text, fq.split(".")[-1])   # shapes this class declares (stub or real)
        if new != text:
            changed += 1
            if not check:
                path.write_text(new, encoding="utf-8")
    print(f"throwables: {len(kids)} subclasses, {changed} {'would change' if check else 'updated'}")
    return 1 if (check and changed) else 0


if __name__ == "__main__":
    sys.exit(main())
