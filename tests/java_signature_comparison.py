#!/usr/bin/env python3
"""Normalized Java/SLeeLa declaration and signature comparison.

This is a source-level comparison tool. It compares declaration kinds,
names, modifiers, generic parameter shape, parameter types, return types,
throws types, fields, and constructors after deterministic normalization.
It does not compare JVM bytecode descriptors or runtime behavior.
"""
import argparse
import json
import re
from pathlib import Path

DECL_RE = re.compile(
    r"(?P<mods>(?:(?:public|protected|private|static|final|abstract|native|"
    r"synchronized|volatile|transient|strictfp|sealed|non-sealed|default)s+)*)"
    r"(?P<kind>class|interface|enum|record)s+"
    r"(?P<name>[A-Za-z_$][\w$]*)"
    r"(?P<generics>\s*<[^{};()]+>)?"
)
METHOD_RE = re.compile(
    r"(?P<mods>(?:(?:public|protected|private|static|final|abstract|native|"
    r"synchronized|strictfp|default)s+)*)"
    r"(?P<generics><[^{}()]+>\s*)?"
    r"(?P<ret>[A-Za-z_$][\w$]*(?:\s*<[^;{}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)"
    r"\s+(?P<name>[A-Za-z_$][\w$]*)\s*\((?P<args>[^(){}]*)\)"
    r"(?:\s+throws\s+(?P<throws>[^\{;]+))?"
)
FIELD_RE = re.compile(
    r"(?P<mods>(?:(?:public|protected|private|static|final|volatile|transient)\s+)*)"
    r"(?P<type>[A-Za-z_$][\w$]*(?:\s*<[^;{}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)"
    r"\s+(?P<name>[A-Za-z_$][\w$]*)\s*(?:=[^;]*)?;"
)

def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    return re.sub(r"//.*", "", text)

def normalize_type(text):
    text = re.sub(r"@(?:[\w$.]+)(?:\([^)]*\))?\s*", "", text.strip())
    text = re.sub(r"\s+", "", text)
    return text.replace("java.lang.", "")

def split_top(text):
    out, start, depth = [], 0, 0
    for i, ch in enumerate(text):
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1
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
    out = []
    for item in split_top(inner):
        item = re.sub(r"\s+", " ", item.strip())
        item = re.sub(r"\b(final|varargs)\b\s+", "", item)
        out.append(item)
    return out

def params(text):
    out = []
    for item in split_top(text):
        item = re.sub(r"@(?:[\w$.]+)(?:\([^)]*\))?\s*", "", item).strip()
        bits = item.split()
        if len(bits) >= 2:
            out.append(normalize_type(" ".join(bits[:-1])))
        elif item:
            out.append(normalize_type(item))
    return out

def throws(text):
    if not text:
        return []
    return sorted(normalize_type(x) for x in split_top(text))

def normalize(text):
    src = strip_comments(text)
    classes = []
    for m in DECL_RE.finditer(src):
        classes.append({
            "kind": m.group("kind"),
            "name": m.group("name"),
            "modifiers": sorted(m.group("mods").split()),
            "typeParameters": generic_shape(m.group("generics"))
        })
    methods = []
    for m in METHOD_RE.finditer(src):
        methods.append({
            "name": m.group("name"),
            "modifiers": sorted(m.group("mods").split()),
            "typeParameters": generic_shape(m.group("generics")),
            "returnType": normalize_type(m.group("ret")),
            "parameterTypes": params(m.group("args")),
            "throws": throws(m.group("throws"))
        })
    fields = []
    for m in FIELD_RE.finditer(src):
        fields.append({
            "name": m.group("name"),
            "modifiers": sorted(m.group("mods").split()),
            "type": normalize_type(m.group("type"))
        })
    return {
        "declarations": sorted(classes, key=lambda x:(x["kind"],x["name"])),
        "methods": sorted(methods, key=lambda x:(x["name"],x["returnType"],x["parameterTypes"])),
        "fields": sorted(fields, key=lambda x:(x["name"],x["type"]))
    }

def compare(java, sleela):
    def delta(key):
        j = {json.dumps(x, sort_keys=True) for x in java[key]}
        s = {json.dumps(x, sort_keys=True) for x in sleela[key]}
        return [json.loads(x) for x in sorted(j-s)], [json.loads(x) for x in sorted(s-j)]
    result = {}
    missing_any = False
    for key in ("declarations","methods","fields"):
        missing, extra = delta(key)
        result[key] = {"missing":missing,"extra":extra}
        missing_any |= bool(missing or extra)
    result["status"] = "PASS" if not missing_any else "MISMATCH"
    return result

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--java", required=True, type=Path)
    ap.add_argument("--sleela", required=True, type=Path)
    ap.add_argument("--json", type=Path)
    args = ap.parse_args()
    java = normalize(args.java.read_text(encoding="utf-8"))
    sleela = normalize(args.sleela.read_text(encoding="utf-8"))
    result = {"tool":"sleela-java-normalized-signature-comparison",
              "scope":"source/API declaration congruence; no JVM/SLVM requirement",
              "java":java,"sleela":sleela,"comparison":compare(java,sleela)}
    rendered=json.dumps(result,indent=2,sort_keys=True)
    if args.json:
        args.json.write_text(rendered+"\n",encoding="utf-8")
    print("SLeeLa Java Normalized Signature Comparison")
    print("  status:", result["comparison"]["status"])
    if args.json: print("  report:", args.json)
    return 0 if result["comparison"]["status"]=="PASS" else 1

if __name__ == "__main__":
    raise SystemExit(main())
