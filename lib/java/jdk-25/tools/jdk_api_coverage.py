#!/usr/bin/env python3
"""Measure how much of the JDK public API /lib/java implements in SLeeLa.

Inputs:
  lib/java/jdk-25/api/jdk-api.jsonl   -- the public API of every exported package of
                                  every java.* / jdk.* module, one JSON object
                                  per type, written by tools/ApiDump.java from
                                  a real JDK (reflection, not documentation).
  lib/java/{java,javax,jdk}/** -- the SLeeLa sources.

A type's file is lib/java/<package path>/<Simple or Outer.Inner>.sleela.
Only members inside the generated/hand-written implementation blocks
(`BEGIN java implementation` .. `END java implementation`) count, so the
envelope helpers (typeName, invoke, ...) never inflate coverage.

Counting rules (deliberately conservative where it matters):
  * a Java constructor is covered when the type declares a SLeeLa
    constructor with the same number of parameters (a class admits one
    constructor until overload resolution lands, so usually one Java
    constructor per type counts);
  * an abstract method is covered when its exact Java signature is declared
    (signatures block, body throws AbstractMethodError, as on the JVM);
  * a method entry is covered when the implementation declares a method with
    the Java name or the Java name plus an overload suffix (absInt, addAt,
    toStringRadix); each Java overload counts once, but no more overloads of a
    name are counted than the implementation has methods for that name
    (or 1, for the base name, when the type covers every overload of a group
    with a single erased Any signature);
  * a field is covered when the implementation (or the type body) declares a
    field of that name;
  * a type is "present" when its file exists and "implemented" when it has an
    implementation block.

usage: python3 lib/java/jdk-25/tools/jdk_api_coverage.py [--tier jdk-25|jdk-28] [--markdown lib/java/COVERAGE.md]
"""
from __future__ import annotations

import argparse
import collections
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
API = ROOT / "api" / "jdk-api.jsonl"
BLOCK = re.compile(r"< ---- BEGIN java implementation.*?---- >\n(.*?)  < ---- END java implementation ---- >", re.S)
SIG = re.compile(r"< ---- BEGIN java signatures.*?---- >\n(.*?)  < ---- END java signatures ---- >", re.S)
DECL = re.compile(r"< ---- BEGIN java declarations.*?---- >\n(.*?)  < ---- END java declarations ---- >", re.S)
METHOD = re.compile(r"^  (?:@\w+\s+)?(?:(?:public|protected|private|static|final|abstract|default|synchronized|native|strictfp) )*(?:<[^()]*> )?[A-Za-z_][\w.]*(?:<[^()]*>)?(?:\[\])* +(\w+)\s*\(", re.M)
ABSTRACT = re.compile(r"^  (?:public |protected )?abstract [^\n]*? (\w+)\s*\([^\n]*AbstractMethodError", re.M)
CTOR = re.compile(r"^  (?:(?:public|protected|private) )?([A-Z]\w*)\s*\(([^)]*)\)\s*(?:throws [^{]*)?\{", re.M)


def arities(text):
    """Parameter counts of the constructors declared in text."""
    out = set()
    for _, params in CTOR.findall(text):
        depth, n = 0, (1 if params.strip() else 0)
        for ch in params:
            if ch == "<":
                depth += 1
            elif ch == ">":
                depth -= 1
            elif ch == "," and depth == 0:
                n += 1
        out.add(n)
    return out
FIELD = re.compile(r"^  (?:static )?(?:final )?[A-Za-z_][\w\[\]<>]* +(\w+)\s*(?:=|;)", re.M)


# Which SLeeLa tree is measured. "jdk-25" is the package-path layout next to the
# API dump (lib/java/jdk-25/<package>/<Type>.sleela). "jdk-28" is the one-file-
# per-type JDK 28 layout (lib/java/jdk-28/<module>/<package>/<Type>.sleela). The
# reflected API (jdk-25/api/jdk-api.jsonl) is the yardstick for both.
TIER = "jdk-25"
TIER_ROOT = ROOT


def type_path(name: str, binary: str, module: str = "") -> Path:
    pkg = binary.rsplit(".", 1)[0] if "$" not in binary else binary.split("$", 1)[0].rsplit(".", 1)[0]
    simple = name[len(pkg) + 1:]
    if TIER == "jdk-28":
        return TIER_ROOT.joinpath(module, *pkg.split("."), simple + ".sleela")
    return ROOT.joinpath(*pkg.split("."), simple + ".sleela")


def analyse(path: Path):
    if not path.is_file():
        return None
    src = path.read_text(encoding="utf-8", errors="replace")
    m = BLOCK.search(src)
    d = DECL.search(src)
    impl = m.group(1) if m else ""
    decl = d.group(1) if d else ""
    sg = SIG.search(src)
    sig_text = sg.group(1) if sg else ""
    abstract_names = collections.Counter(ABSTRACT.findall(sig_text))
    sig_count = len(METHOD.findall(sig_text)) - sum(abstract_names.values())
    return {"implemented": bool(m) or bool(d) or bool(abstract_names), "behavior": bool(m), "signatures": sig_count,
            "methods": collections.Counter(METHOD.findall(impl + decl)) + abstract_names,
            "impl_methods": collections.Counter(METHOD.findall(impl)),
            "ctors": arities(impl + decl),
            "impl_ctors": arities(impl),
            "fields": set(FIELD.findall(impl + decl)) | (set(FIELD.findall(src)) if m else set()),
            "impl_fields": set(FIELD.findall(impl))}


def covered_methods(java_methods, impl_methods):
    """Number of Java method entries covered by the implementation."""
    groups = collections.Counter(name for name, *_ in java_methods if name != "<init>")
    total = 0
    for name, overloads in groups.items():
        have = sum(c for m, c in impl_methods.items()
                   if m == name or (m.startswith(name) and len(m) > len(name) and m[len(name)].isupper()))
        total += min(overloads, have)
    return total


def write_files_report(path, per_type, tot):
    """Sorted list of the SLeeLa files that hold working Java members."""
    total = sum(r[3] for r in per_type)
    lines = ["# /lib/java files holding working Java members", "",
             "Generated by `lib/java/jdk-25/tools/jdk_api_coverage.py --files`. A file is listed when it has "
             "hand-implemented behavior; the count follows the rules in that tool (a constructor counts "
             "by arity, a method by name and overload).",
             "", f"**{len(per_type)} files, {total} working members** (of {tot['members']} public JDK members).", "",
             "## Largest first", "",
             "| # | Java type | File | Working | Members | % |", "|---:|---|---|---:|---:|---:|"]
    for i, (pkg, name, rel, b, n) in enumerate(sorted(per_type, key=lambda r: (-r[3], r[1])), 1):
        share = f"{100.0 * b / n:.0f}%" if n else "-"
        lines.append(f"| {i} | {name} | `{rel}` | {b} | {n} | {share} |")
    lines += ["", "## By package, then type", ""]
    cur = None
    for pkg, name, rel, b, n in sorted(per_type, key=lambda r: (r[0], r[1])):
        if pkg != cur:
            cur = pkg
            subtotal = sum(r[3] for r in per_type if r[0] == pkg)
            lines += ["", f"### {pkg} ({subtotal})", "", "| Java type | File | Working | Members |", "|---|---|---:|---:|"]
        lines.append(f"| {name} | `{rel}` | {b} | {n} |")
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--tier", choices=("jdk-25", "jdk-28"), default="jdk-25", help="which SLeeLa tree to measure (default jdk-25)")
    ap.add_argument("--markdown", help="write a Markdown report to this path")
    ap.add_argument("--json", help="write per-package numbers as JSON")
    ap.add_argument("--files", help="write a Markdown list of the files holding working Java members, sorted")
    args = ap.parse_args()
    global TIER, TIER_ROOT
    TIER = args.tier
    TIER_ROOT = ROOT.parent / "jdk-28"
    types = [json.loads(line) for line in API.read_text(encoding="utf-8").splitlines() if line.strip()]
    pkg_rows = collections.defaultdict(lambda: collections.Counter())
    tot = collections.Counter()
    per_type = []
    for t in types:
        pkg = t["binary"].split("$", 1)[0].rsplit(".", 1)[0]
        info = analyse(type_path(t["name"], t["binary"], t["module"]))
        row = pkg_rows[(t["module"], pkg)]
        ctors = [m for m in t["methods"] if m[0] == "<init>"]
        meths = [m for m in t["methods"] if m[0] != "<init>"]
        n_members = len(ctors) + len(meths) + len(t["fields"]) + len(t.get("constants", []))
        row["types"] += 1
        row["members"] += n_members
        if info is not None:
            row["present"] += 1
            row["signatures"] += info["signatures"]
        if info is not None and info["implemented"]:
            row["implemented"] += 1
            if info["behavior"]:
                row["behavior_types"] += 1

            def count(meth, ctor, fld):
                # a Java constructor counts when SLeeLa declares one of the same arity
                c = sum(1 for k in ctors if len(k[2]) in ctor)
                c += covered_methods(meths, meth)
                c += sum(1 for f in t["fields"] if f[0] in fld)
                c += sum(1 for k in t.get("constants", []) if k in fld)
                return min(c, n_members)
            row["covered"] += count(info["methods"], info["ctors"], info["fields"])
            b = count(info["impl_methods"], info["impl_ctors"], info["impl_fields"])
            row["behavior"] += b
            if b:
                rel = type_path(t["name"], t["binary"], t["module"]).relative_to(ROOT.parent).as_posix()
                per_type.append((pkg, t["name"], rel, b, n_members))
        for k in ("types", "members"):
            pass
    for row in pkg_rows.values():
        tot.update(row)

    def pct(a, b):
        return f"{(100.0 * a / b):.1f}%" if b else "-"

    summary = (f"types {tot['types']}, present {tot['present']}, with SLeeLa members {tot['implemented']} "
               f"({pct(tot['implemented'], tot['types'])}), hand-implemented {tot['behavior_types']}; "
               f"members {tot['members']}, covered {tot['covered']} ({pct(tot['covered'], tot['members'])}), "
               f"of which hand-implemented behavior {tot['behavior']} ({pct(tot['behavior'], tot['members'])}); "
               f"exact-signature stubs awaiting logic {tot['signatures']} (not counted)")
    print(summary)
    by_module = collections.defaultdict(collections.Counter)
    for (mod, pkg), row in pkg_rows.items():
        by_module[mod].update(row)
    if args.files:
        write_files_report(Path(args.files), per_type, tot)
    if args.json:
        Path(args.json).write_text(json.dumps({f"{m}/{p}": dict(r) for (m, p), r in sorted(pkg_rows.items())}, indent=1))
    if args.markdown:
        lines = [f"# JDK API coverage of /lib/java ({TIER} tier)",
                 "",
                 "Generated by `lib/java/jdk-25/tools/jdk_api_coverage.py` from `lib/java/jdk-25/api/jdk-api.jsonl` "
                 "(the reflected public API of a real JDK; see that tool for the counting rules). "
                 "Members are constructors, methods, fields and enum constants.",
                 "",
                 f"**{summary}**",
                 "",
                 "## By module",
                 "",
                 "Covered = declared from the API (constants, fields, abstract methods, complete enums and "
                 "records) plus hand-implemented behavior; the Behavior column counts only the latter.",
                 "",
                 "| Module | Types | Members | Covered | Coverage | Behavior | Behavior % |",
                 "|---|---:|---:|---:|---:|---:|---:|"]
        for mod, r in sorted(by_module.items(), key=lambda kv: -kv[1]["members"]):
            lines.append(f"| {mod} | {r['types']} | {r['members']} | {r['covered']} | {pct(r['covered'], r['members'])} | {r['behavior']} | {pct(r['behavior'], r['members'])} |")
        lines += ["", "## By package (packages with hand-implemented behavior, then the largest remaining)", "",
                  "| Package | Types | Members | Covered | Coverage | Behavior | Behavior % |",
                  "|---|---:|---:|---:|---:|---:|---:|"]
        rows = sorted(pkg_rows.items(), key=lambda kv: (-kv[1]["behavior"], -kv[1]["members"]))
        shown = [kv for kv in rows if kv[1]["behavior"] > 0] + [kv for kv in rows if kv[1]["behavior"] == 0][:40]
        for (mod, pkg), r in shown:
            lines.append(f"| {pkg} | {r['types']} | {r['members']} | {r['covered']} | {pct(r['covered'], r['members'])} | {r['behavior']} | {pct(r['behavior'], r['members'])} |")
        Path(args.markdown).write_text("\n".join(lines) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
