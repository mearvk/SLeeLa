#!/usr/bin/env python3
"""Generate the reflected `ret` rows of conversion.model (section 10b).

The rows give the result type of a JDK member, keyed by the simple name of the
receiver's static type and the member name, exactly as the hand-written rows of
section 10. They are taken from the public API reflected from a real JDK
(lib/java/jdk-25/api/jdk-signatures.jsonl and jdk-api.jsonl, OpenJDK 25.0.2), so
nothing is invented. A row is written only when it is unambiguous:

  * the member is public or protected, not a constructor, and returns a value;
  * the result maps to a model type: a primitive, String, or a top-level class
    or interface (by simple name). Type variables, arrays and nested types are
    left out, so such a result stays unknown (and the converters fail closed);
  * every overload of the member, inherited or declared, on every JDK type with
    that simple name (java.util.List and java.awt.List are both "List") agrees
    on the result; overloads whose result is always the type of the first
    argument (Math.abs) are written as @arg0, the existing notation;
  * a hand-written row of section 10 for the same owner and member wins: the
    generator never writes a second row for it.

Inherited members are included (ArrayList.isEmpty comes from AbstractCollection),
with the most derived declaration of each signature winning (covariant returns).

usage: python3 impl/subjects/conversion/generate_ret_rows.py [--check]
  rewrites the block between the BEGIN / END markers of conversion.model;
  --check exits 1 when the block is out of date, without writing.
"""
from __future__ import annotations

import collections
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
MODEL = HERE / "conversion.model"
API = ROOT / "lib/java/jdk-25/api/jdk-api.jsonl"
SIGS = ROOT / "lib/java/jdk-25/api/jdk-signatures.jsonl"
BEGIN = "# ---- BEGIN 10b generated ret rows (generate_ret_rows.py; do not edit by hand) ----"
END = "# ---- END 10b generated ret rows ----"

# Packages whose types are owners of generated rows: the procedural core of
# java.base that converted programs call. Supertypes outside the list still
# contribute inherited members.
SCOPE = (
    "java.lang", "java.util", "java.util.concurrent", "java.util.concurrent.atomic",
    "java.util.regex", "java.util.function", "java.util.stream", "java.math",
    "java.time", "java.time.format", "java.time.temporal", "java.io", "java.nio",
    "java.nio.charset", "java.nio.file", "java.text", "java.net",
)
PRIMITIVES = {"boolean", "byte", "short", "char", "int", "long", "float", "double"}


def package_of(t: dict) -> str:
    b = t["binary"]
    return b.split("$", 1)[0].rsplit(".", 1)[0]


def simple_of(t: dict) -> str:
    return t["name"][len(package_of(t)) + 1:]


def map_type(java: str, known_top: set[str]) -> str | None:
    """A Java type as a model type name, or None when it has no model type."""
    t = java.strip()
    if t in PRIMITIVES:
        return t
    if t.endswith("]") or t.endswith("..."):
        return None
    base = re.sub(r"<.*", "", t).strip()
    if base == "java.lang.String":
        return "String"
    if "." not in base:
        return None  # a type variable
    if base not in known_top:
        return None  # a nested type, or not a public JDK type
    return base.rsplit(".", 1)[1]


def main() -> int:
    check = "--check" in sys.argv[1:]
    api = {}
    for line in API.read_text(encoding="utf-8").splitlines():
        if line.strip():
            t = json.loads(line)
            api[t["name"]] = t
    sigs = {}
    for line in SIGS.read_text(encoding="utf-8").splitlines():
        if line.strip():
            t = json.loads(line)
            sigs[t["name"]] = t
    top_level = {n for n, t in api.items() if "$" not in t["binary"]}

    def declared(name: str):
        s = sigs.get(name)
        if not s:
            return []
        out = []
        for m in s["members"]:
            if m["name"] == "<init>" or not m["ret"] or m["ret"] == "void":
                continue
            if "private" in m["mods"].split():
                continue
            out.append(m)
        return out

    cache: dict[str, dict] = {}

    def resolved(name: str, depth: int = 0) -> dict:
        """(method, param types) -> return type, most derived first."""
        if name in cache:
            return cache[name]
        table: dict = {}
        if depth < 40 and name in api:
            t = api[name]
            for m in declared(name):
                key = (m["name"], tuple(re.sub(r"<.*", "", p[0]) for p in m["params"]))
                table.setdefault(key, m["ret"])
            supers = []
            if t.get("super"):
                supers.append(t["super"])
            supers.extend(t.get("interfaces", []))
            for s in supers:
                for key, ret in resolved(re.sub(r"<.*", "", s), depth + 1).items():
                    table.setdefault(key, ret)
        cache[name] = table
        return table

    hand = set()
    text = MODEL.read_text(encoding="utf-8")
    head, sep, rest = text.partition(BEGIN)
    if sep:
        _, sep2, tail = rest.partition(END)
        if not sep2:
            print("conversion.model: BEGIN marker without END marker")
            return 1
    else:
        tail = None
    for line in head.splitlines() + ([] if tail is None else tail.splitlines()):
        f = line.split()
        if len(f) >= 4 and f[0] == "ret":
            hand.add((f[1], f[2]))

    # (simple owner, method) -> set of results over every JDK type of that simple name
    results: dict = collections.defaultdict(set)
    in_scope_owner: set[str] = set()
    for name, t in api.items():
        if "$" in t["binary"]:
            continue
        simple = simple_of(t)
        if package_of(t) in SCOPE:
            in_scope_owner.add(simple)
        groups: dict = collections.defaultdict(list)
        for (mname, params), ret in resolved(name).items():
            groups[mname].append((params, ret))
        for mname, overloads in groups.items():
            mapped = [map_type(r, top_level) for _, r in overloads]
            if all(m is not None for m in mapped) and len(set(mapped)) == 1:
                results[(simple, mname)].add(mapped[0])
                continue
            # @arg0: every overload returns the type of its first parameter
            arg0 = True
            for (params, r), m in zip(overloads, mapped):
                if not params or m is None or map_type(params[0], top_level) != m:
                    arg0 = False
                    break
            results[(simple, mname)].add("@arg0" if arg0 else None)

    rows = []
    for (owner, mname), found in sorted(results.items()):
        if owner not in in_scope_owner or (owner, mname) in hand:
            continue
        if len(found) != 1:
            continue
        (only,) = tuple(found)
        if only is None:
            continue
        rows.append(f"ret {owner} {mname} {only}")

    block = "\n".join([
        BEGIN,
        "# Result types reflected from the public API of OpenJDK 25.0.2",
        "# (lib/java/jdk-25/api/jdk-signatures.jsonl, jdk-api.jsonl): typing only, as",
        "# section 10. Owners: the packages listed in generate_ret_rows.py SCOPE.",
        "# A member is listed only when every overload, declared or inherited, on",
        "# every JDK type of that simple name, agrees; hand rows above win.",
        f"# {len(rows)} rows.",
        *rows,
        END,
    ])
    if tail is None:
        anchor = "# ---- 11. Statement-level call rewrites"
        if anchor not in head:
            print("conversion.model: section 11 anchor not found")
            return 1
        new = head.replace(anchor, block + "\n\n" + anchor, 1)
    else:
        new = head + block + tail
    if check:
        if new != text:
            print("conversion.model: section 10b is out of date; run generate_ret_rows.py")
            return 1
        print(f"conversion.model: section 10b up to date ({len(rows)} rows)")
        return 0
    MODEL.write_text(new, encoding="utf-8")
    print(f"conversion.model: wrote section 10b, {len(rows)} ret rows ({len(hand)} hand rows kept)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
