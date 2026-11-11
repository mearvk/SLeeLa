#!/usr/bin/env python3
"""Implement probed JDK accessor contracts in /lib/java.

Input: lib/java/api/jdk-accessors.jsonl, written by tools/ApiAccessorProbe.java.
The probe exercises the public JDK API only (it never reads implementation
source) and records, per class, which behavior holds on independent runs:

  param      getter g() returns constructor argument i
  constant   getter g() returns the same value on every fresh instance
             (the initial state of that property)
  setter     after s(v), the paired getter returns v

For each probed class without hand-written behavior this tool writes an
implementation block: one state field per observed property, the probed
constructor with its exact Java signature (storing the mapped arguments
and the initial-state values), the getters and setters with their exact
Java signatures. Properties are fields rather than hard-coded returns, so
later logic that changes the state stays consistent.

Re-run jdk_api_signatures.py afterwards (it drops the stubs of implemented
names). Idempotent. usage: python3 lib/java/tools/jdk_api_accessors.py [--check]
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "implementation"))
import jdk_api_declare as dec  # noqa: E402
import jdk_api_signatures as sg  # noqa: E402

ROOT = dec.ROOT
ACC = ROOT / "api" / "jdk-accessors.jsonl"
BEGIN = "  < ---- BEGIN java implementation ---- >"
END = "  < ---- END java implementation ---- >"
MAX_FIELDS = 64


def raw(t: str) -> str:
    t = re.sub(r"<.*>", "", t).replace("...", "[]")
    return t.strip()


def prop_of(getter: str) -> str:
    for p in ("get", "is", "has"):
        if getter.startswith(p) and len(getter) > len(p) and getter[len(p)].isupper():
            return getter[len(p):]
    return getter[:1].upper() + getter[1:]


def head(m: dict) -> str:
    mods = [x for x in m["mods"].split() if x not in ("abstract", "native", "default")]
    parts = mods + ([m["tparams"]] if m["tparams"] else []) + [m["ret"], m["name"]]
    return " ".join(parts)


def pname(n: str) -> str:
    return n + "_" if n in sg.RESERVED_PARAMS else n


def literal(sl_t: str, v) -> str | None:
    if v is None:
        return None
    if sl_t == "String":
        s = str(v)
        return '"' + s.replace("\\", "\\\\").replace('"', '\\"') + '"'
    if sl_t == "boolean":
        return "true" if v is True or v == "true" else "false"
    if sl_t in ("int", "long"):
        iv = int(v)
        return "-9223372036854775807 - 1" if iv == -9223372036854775808 else str(iv)
    if sl_t == "double":
        s = str(v)
        if "E" not in s and "e" not in s and "." not in s:
            s += ".0"
        return s
    return None


def build(entry, sigs, cls, existing_fields):
    t = sigs.get(entry["name"])
    if t is None:
        return None
    members = t["members"]
    by_name = {}
    for m in members:
        by_name.setdefault(m["name"], []).append(m)
    ctor = None
    for m in by_name.get("<init>", []):
        if [raw(p[0]) for p in m["params"]] == [raw(x) for x in entry["ctor"]] and "public" in m["mods"]:
            ctor = m
            break
    if ctor is None or not sg.renderable(ctor):
        return None
    fields, inits, methods = [], [], []
    held = {}
    for kind, a, b in [(o[0], o[1], o[2]) for o in entry["obs"]]:
        if kind == "setter":
            continue
        g = next((m for m in by_name.get(a, []) if not m["params"] and not m["static"] and "public" in m["mods"]), None)
        if g is None or not sg.renderable(g) or not dec.usable(a, cls):
            continue
        sl_t = dec.sl_type(raw(g["ret"]))
        field = "held" + prop_of(a)
        if field in existing_fields or field in held.values():
            continue
        if kind == "param":
            p = ctor["params"][b]
            if dec.sl_type(raw(p[0])) != sl_t:
                continue
            init = pname(p[1])
        else:
            init = literal(sl_t, b)
            if init is None:
                continue
        held[a] = field
        fields.append(f"  {sl_t} {field};")
        inits.append(f"this.{field} = {init};")
        methods.append(f"  {head(g)}() {{ return this.{field}; }}")
    for kind, a, b in [(o[0], o[1], o[2]) for o in entry["obs"]]:
        if kind != "setter" or b not in held:
            continue
        s = next((m for m in by_name.get(a, []) if len(m["params"]) == 1 and not m["static"] and "public" in m["mods"]), None)
        if s is None or not sg.renderable(s) or not dec.usable(a, cls):
            continue
        v = pname(s["params"][0][1])
        methods.append(f"  {head(s)}({s['params'][0][0]} {v}) {{ this.{held[b]} = {v}; }}")
    if not methods:
        return None
    params = ", ".join(f"{p[0]} {pname(p[1])}" for p in ctor["params"])
    throws = (" throws " + ", ".join(ctor["throws"])) if ctor["throws"] else ""
    body = [
        f"  < ---- {entry['name']} accessor contract (generated by tools/jdk_api_accessors.py) ---- >",
        "  << State and accessors whose behavior was observed on the JDK by",
        "     tools/ApiAccessorProbe.java: getters returning a constructor argument,",
        "     the initial state of a property, and setter / getter round trips.",
        "     Methods beyond these keep their exact-signature stubs. >>",
    ] + fields + [f"  public {cls}({params}){throws} {{ {' '.join(inits)} }}"] + methods
    return "\n".join(body) + "\n", len(fields)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args()
    sigs = {json.loads(l)["name"]: json.loads(l) for l in sg.SIGS.read_text(encoding="utf-8").splitlines() if l.strip()}
    api = {json.loads(l)["name"]: json.loads(l) for l in dec.API.read_text(encoding="utf-8").splitlines() if l.strip()}
    changed = written = 0
    for line in ACC.read_text(encoding="utf-8").splitlines():
        if not line.strip():
            continue
        entry = json.loads(line)
        t = api.get(entry["name"])
        if t is None or entry["name"] in sg.SKIP_TYPES:
            continue
        path = dec.type_file(t["name"], t["binary"])
        if not path.is_file():
            continue
        src = path.read_text(encoding="utf-8", errors="replace")
        impl = re.search(re.escape(BEGIN) + r"\n(.*?)" + re.escape(END), src, re.S)
        if impl and "accessor contract (generated by tools/jdk_api_accessors.py)" not in impl.group(1):
            continue  # hand-written behavior wins
        mo = re.search(r"^class (\w+)(<[^{}]*>)?(?: extends \w+)? \{$", src, re.M)
        if not mo:
            continue
        cls = mo.group(1)
        base = re.sub(re.escape(BEGIN) + r".*?" + re.escape(END) + r"\n", "", src, flags=re.S)
        existing = set(dec.FIELD_DECL.findall(base))
        inst = len(re.findall(r"^  (?!static)[A-Za-z_][\w\[\]<>.,? ]* +\w+\s*(?:=[^;\n]*)?;", base, re.M))
        res = build(entry, sigs, cls, existing)
        if res is None:
            continue
        body, nfields = res
        if inst + nfields > MAX_FIELDS:
            continue
        block = BEGIN + "\n" + body + END + "\n"
        marks = [m for m in ("  < ---- BEGIN java declarations", "  < ---- BEGIN java signatures") if m in base]
        if marks:
            i = min(base.index(m) for m in marks)
            new = base[:i].rstrip("\n") + "\n\n" + block + "\n" + base[i:]
        else:
            b = base.rstrip()
            new = b[:-1].rstrip("\n") + "\n\n" + block + "}\n"
        new = re.sub(r"^#sleela \d+\.\d+$", "#sleela 3.0", new, count=1, flags=re.M)
        written += 1
        if new != src:
            changed += 1
            if not args.check:
                path.write_text(new, encoding="utf-8")
    print(f"accessors: {written} classes implemented from probed contracts; files {'to update' if args.check else 'updated'}: {changed}")
    return 1 if args.check and changed else 0


if __name__ == "__main__":
    sys.exit(main())
