#!/usr/bin/env python3
"""Deterministic normalized Java/SLeeLa declaration comparison.

This is a source/API congruence tool. It deliberately does not compare JVM
descriptors, bytecode, or runtime behavior.

The normal form records:
- owned/nested type declarations and inheritance clauses;
- class/interface/enum/record declaration modifiers and type parameters;
- fields;
- constructors (distinct from methods);
- methods, including generic parameters, varargs, receiver-like metadata,
  parameter modifiers, return type, throws types, and annotations.

The parser is intentionally lightweight: it is a qualification fixture tool,
not the production Java parser.
"""
import argparse
import json
import re
from pathlib import Path

IDENT = r"[A-Za-z_$][\w$]*"
MODIFIERS = (
    "public|protected|private|static|final|abstract|native|synchronized|"
    "volatile|transient|strictfp|sealed|non-sealed|default"
)
DECL_RE = re.compile(
    rf"(?P<mods>(?:(?:{MODIFIERS})\s+)*)"
    rf"(?P<kind>class|interface|enum|record)\s+"
    rf"(?P<name>{IDENT})"
    rf"(?P<generics>\s*<[^{{}};()]+>)?"
    rf"(?P<tail>[^{{;]*)(?=\{{)"
)
METHOD_RE = re.compile(
    rf"(?P<mods>(?:(?:{MODIFIERS})\s+)*)"
    rf"(?P<generics><[^{{}}()]+>\s*)?"
    rf"(?P<ret>[A-Za-z_$][\w$]*(?:\s*<[^;{{}}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)"
    rf"\s+(?P<name>{IDENT})\s*\((?P<args>[^(){{}}]*)\)"
    rf"(?:\s+throws\s+(?P<throws>[^{{;]+))?"
)
CTOR_RE = re.compile(
    rf"(?P<mods>(?:(?:public|protected|private)\s+)*)"
    rf"(?P<generics><[^{{}}()]+>\s*)?"
    rf"(?P<name>{IDENT})\s*\((?P<args>[^(){{}}]*)\)"
    rf"(?:\s+throws\s+(?P<throws>[^{{;]+))?"
)
FIELD_RE = re.compile(
    rf"(?P<mods>(?:(?:public|protected|private|static|final|volatile|transient)\s+)*)"
    rf"(?P<type>[A-Za-z_$][\w$]*(?:\s*<[^;{{}}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)"
    rf"\s+(?P<name>{IDENT})\s*(?:=[^;]*)?;"
)

def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    return re.sub(r"//.*", "", text)

def normalize_type(text):
    text = re.sub(r"@(?:[\w$.]+)(?:\([^)]*\))?\s*", "", text.strip())
    text = re.sub(r"\b(final|volatile|transient)\b\s+", "", text)
    text = re.sub(r"\s+", "", text)
    return text.replace("java.lang.", "")

def split_top(text):
    out, start, depth = [], 0, 0
    pairs = {"<": ">", "(": ")", "[": "]"}
    closing = set(pairs.values())
    for i, ch in enumerate(text):
        if ch in pairs:
            depth += 1
        elif ch in closing:
            depth = max(0, depth - 1)
        elif ch == "," and depth == 0:
            out.append(text[start:i].strip())
            start = i + 1
    tail = text[start:].strip()
    if tail:
        out.append(tail)
    return out

def generic_shape(text):
    if not text:
        return []
    inner = text.strip()[1:-1]
    return [re.sub(r"\s+", " ", item.strip()) for item in split_top(inner) if item.strip()]

def annotations(text):
    return sorted(set(re.findall(r"@([A-Za-z_$][\w$.]*)", text)))

def params(text):
    out = []
    for item in split_top(text):
        item = item.strip()
        if not item:
            continue
        ann = annotations(item)
        item = re.sub(r"@(?:[\w$.]+)(?:\([^)]*\))?\s*", "", item)
        item = re.sub(r"\b(final|volatile|transient)\b\s+", "", item).strip()
        varargs = "..." in item
        item = item.replace("...", "[]")
        bits = item.split()
        if len(bits) >= 2:
            typ, name = " ".join(bits[:-1]), bits[-1]
        else:
            typ, name = item, ""
        out.append({
            "name": name,
            "type": normalize_type(typ),
            "varargs": varargs,
            "annotations": ann,
        })
    return out

def throws(text):
    if not text:
        return []
    return sorted(normalize_type(x) for x in split_top(text))

def modifiers(text):
    return sorted(text.split())

def inheritance(tail):
    tail = re.sub(r"\s+", " ", tail.strip())
    result = {"extends": [], "implements": [], "permits": []}
    for key in result:
        m = re.search(rf"\b{key}\s+(.+?)(?=\b(?:extends|implements|permits)\b|$)", tail)
        if m:
            result[key] = sorted(normalize_type(x) for x in split_top(m.group(1)))
    return result

def normalize(text):
    src = strip_comments(text)
    declarations = []
    # Stack ownership is intentionally approximate but deterministic for the
    # qualification corpus: declarations are ordered by source occurrence.
    decl_matches = list(DECL_RE.finditer(src))
    owners = []
    for m in decl_matches:
        name = m.group("name")
        owner = "::".join(owners) if owners else ""
        path = f"{owner}::{name}" if owner else name
        declarations.append({
            "kind": m.group("kind"),
            "name": name,
            "path": path,
            "modifiers": modifiers(m.group("mods")),
            "typeParameters": generic_shape(m.group("generics")),
            **inheritance(m.group("tail")),
            "annotations": annotations(src[max(0, m.start()-300):m.start()]),
        })
        # Nested declaration ownership is inferred only when a prior type's
        # opening brace encloses this declaration.
        opens = src.find("{", m.end())
        closes_before = src.rfind("}", 0, m.start())
        if opens >= 0 and opens > closes_before:
            owners.append(name)

    type_names = {d["name"] for d in declarations}
    constructors = []
    for m in CTOR_RE.finditer(src):
        if m.group("name") in type_names:
            constructors.append({
                "owner": m.group("name"),
                "name": m.group("name"),
                "modifiers": modifiers(m.group("mods")),
                "typeParameters": generic_shape(m.group("generics")),
                "parameters": params(m.group("args")),
                "throws": throws(m.group("throws")),
                "annotations": annotations(src[max(0, m.start()-300):m.start()]),
            })

    constructor_spans = {(m.start(), m.end()) for m in CTOR_RE.finditer(src) if m.group("name") in type_names}
    methods = []
    for m in METHOD_RE.finditer(src):
        if (m.start(), m.end()) in constructor_spans:
            continue
        methods.append({
                "name": m.group("name"),
                "modifiers": modifiers(m.group("mods")),
                "typeParameters": generic_shape(m.group("generics")),
                "returnType": normalize_type(m.group("ret")),
                "parameters": params(m.group("args")),
                "throws": throws(m.group("throws")),
                "annotations": annotations(src[max(0, m.start()-300):m.start()]),
            })

    fields = []
    for m in FIELD_RE.finditer(src):
        fields.append({
            "name": m.group("name"),
            "modifiers": modifiers(m.group("mods")),
            "type": normalize_type(m.group("type")),
            "annotations": annotations(src[max(0, m.start()-300):m.start()]),
        })

    return {
        "normalizationVersion": 2,
        "declarations": sorted(declarations, key=lambda x: x["path"]),
        "constructors": sorted(constructors, key=lambda x: (x["owner"], x["parameters"])),
        "methods": sorted(methods, key=lambda x: (x["name"], x["returnType"], x["parameters"])),
        "fields": sorted(fields, key=lambda x: (x["name"], x["type"])),
    }

def compare(java, sleela):
    def delta(key):
        j = {json.dumps(x, sort_keys=True) for x in java[key]}
        s = {json.dumps(x, sort_keys=True) for x in sleela[key]}
        return [json.loads(x) for x in sorted(j-s)], [json.loads(x) for x in sorted(s-j)]
    result = {}
    mismatch = False
    for key in ("declarations", "constructors", "methods", "fields"):
        missing, extra = delta(key)
        result[key] = {"missing": missing, "extra": extra}
        mismatch |= bool(missing or extra)
    result["status"] = "PASS" if not mismatch else "MISMATCH"
    return result

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--java", required=True, type=Path)
    ap.add_argument("--sleela", required=True, type=Path)
    ap.add_argument("--json", type=Path)
    args = ap.parse_args()
    java = normalize(args.java.read_text(encoding="utf-8"))
    sleela = normalize(args.sleela.read_text(encoding="utf-8"))
    result = {
        "tool": "sleela-java-normalized-signature-comparison",
        "normalizationVersion": 2,
        "scope": "source/API declaration congruence; no JVM/SLVM requirement",
        "java": java,
        "sleela": sleela,
        "comparison": compare(java, sleela),
    }
    rendered = json.dumps(result, indent=2, sort_keys=True)
    if args.json:
        args.json.write_text(rendered + "\n", encoding="utf-8")
    print("SLeeLa Java Normalized Signature Comparison")
    print("  status:", result["comparison"]["status"])
    if args.json:
        print("  report:", args.json)
    return 0 if result["comparison"]["status"] == "PASS" else 1

if __name__ == "__main__":
    raise SystemExit(main())
