#!/usr/bin/env python3
"""Deterministic Java -> SLeeLa source-equivalence inventory.

This is a source/API qualification tool, not a JVM runner. It deliberately
separates Java compatibility vocabulary from native SLeeLa vocabulary.
"""

import argparse, json, re
from pathlib import Path

JAVA_KEYWORDS = set("""
abstract assert boolean break byte case catch char class const continue default do
double else enum extends final finally float for goto if implements import instanceof
int interface long native new package private protected public record return short
static strictfp super switch synchronized this throw throws transient try void
volatile while var yield sealed permits non-sealed when module open requires exports
opens to uses provides with transitive true false null
""".split())

STATEMENTS = {
    "if","else","while","do","for","switch","case","default","break","continue",
    "return","throw","try","catch","finally","synchronized","assert","yield"
}
EXPRESSION_MARKERS = {
    "new","instanceof","this","super","lambda","::","?","=","+","-","*","/","%",
    "==","!=","<",">","&&","||","!"
}
DECL_RE = re.compile(
    r"(?P<mods>(?:(?:public|protected|private|static|final|abstract|native|"
    r"synchronized|volatile|transient|strictfp|sealed|non-sealed|default)\s+)*)"
    r"(?P<kind>class|interface|enum|record)\s+(?P<name>[A-Za-z_$][\w$]*)"
)
METHOD_RE = re.compile(
    r"(?P<mods>(?:(?:public|protected|private|static|final|abstract|native|"
    r"synchronized|strictfp|default)\s+)*)"
    r"(?P<ret>[A-Za-z_$][\w$]*(?:\s*<[^;{}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)"
    r"\s+(?P<name>[A-Za-z_$][\w$]*)\s*\((?P<args>[^(){}]*)\)"
)
FIELD_RE = re.compile(
    r"(?P<mods>(?:(?:public|protected|private|static|final|volatile|transient)\s+)*)"
    r"(?P<type>[A-Za-z_$][\w$]*(?:\s*<[^;{}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)"
    r"\s+(?P<name>[A-Za-z_$][\w$]*)\s*(?:=[^;]*)?;"
)
ANNOTATION_RE = re.compile(r"@(?:[A-Za-z_$][\w$]*\.)*[A-Za-z_$][\w$]*")
API_REF_RE = re.compile(r"\b(?:java|javax|jdk|sun)\.[A-Za-z_$][\w$]*(?:\.[A-Za-z_$][\w$]*)*")

def strip_comments(s):
    s = re.sub(r"/\*.*?\*/", "", s, flags=re.S)
    return re.sub(r"//.*", "", s)

def normalize_type(s):
    return re.sub(r"\s+", "", s).replace("java.lang.", "")

def params(sig):
    if not sig.strip():
        return []
    out = []
    for p in sig.split(","):
        p = p.strip()
        p = re.sub(r"@[\w$.]+(?:\([^)]*\))?\s*", "", p)
        bits = p.split()
        if len(bits) >= 2:
            out.append(normalize_type(" ".join(bits[:-1])))
        else:
            out.append(normalize_type(p))
    return out

def inventory(text, label):
    src = strip_comments(text)
    words = re.findall(r"[A-Za-z_$][\w$-]*", src)
    compat = sorted({w for w in words if w in JAVA_KEYWORDS})
    annotations = sorted(set(ANNOTATION_RE.findall(src)))
    declarations = []
    for m in DECL_RE.finditer(src):
        declarations.append({
            "kind": m.group("kind"),
            "name": m.group("name"),
            "modifiers": m.group("mods").split()
        })
    methods = []
    for m in METHOD_RE.finditer(src):
        methods.append({
            "name": m.group("name"),
            "returnType": normalize_type(m.group("ret")),
            "parameterTypes": params(m.group("args")),
            "modifiers": m.group("mods").split()
        })
    fields = []
    for m in FIELD_RE.finditer(src):
        fields.append({
            "name": m.group("name"),
            "type": normalize_type(m.group("type")),
            "modifiers": m.group("mods").split()
        })
    statements = sorted({w for w in words if w in STATEMENTS})
    expr = sorted({x for x in EXPRESSION_MARKERS if x in src})
    api = sorted(set(API_REF_RE.findall(src)))
    return {
        "label": label,
        "lines": len(text.splitlines()),
        "compatibilityVocabulary": compat,
        "annotations": annotations,
        "declarations": declarations,
        "methods": methods,
        "fields": fields,
        "statementMarkers": statements,
        "expressionMarkers": expr,
        "javaApiReferences": api,
        "symbolDomains": {
            "native-sleela": 0,
            "java-compatibility": len(compat) + len(annotations),
            "java-api-counterpart": len(api)
        }
    }

def compare(java, sleela):
    jd = {(x["kind"], x["name"]) for x in java["declarations"]}
    sd = {(x["kind"], x["name"]) for x in sleela["declarations"]}
    missing = sorted(jd - sd)
    extra = sorted(sd - jd)
    jm = {(x["name"], x["returnType"], tuple(x["parameterTypes"])) for x in java["methods"]}
    sm = {(x["name"], x["returnType"], tuple(x["parameterTypes"])) for x in sleela["methods"]}
    return {
        "structural": {
            "status": "PASS" if not missing else "INCOMPLETE",
            "missingDeclarations": [list(x) for x in missing],
            "extraDeclarations": [list(x) for x in extra],
            "missingMethods": [list(x) for x in sorted(jm-sm)]
        },
        "semantic": {
            "status": "INCOMPLETE",
            "reason": "Full Java type/conversion, overload, override, definite-assignment, and exception analysis is not yet claimed by this inventory driver."
        },
        "functional": {
            "status": "INCOMPLETE",
            "reason": "Source/API inventory does not establish runtime behavior. Java API envelopes are not treated as behavioral implementations."
        }
    }

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--java", required=True, type=Path)
    ap.add_argument("--sleela", required=True, type=Path)
    ap.add_argument("--json", type=Path)
    args = ap.parse_args()
    j = inventory(args.java.read_text(encoding="utf-8"), "java")
    s = inventory(args.sleela.read_text(encoding="utf-8"), "sleela")
    result = {
        "tool": "sleela-java-source-equivalence",
        "scope": "source/API congruence; no JVM/SLVM requirement",
        "java": j,
        "sleela": s,
        "comparison": compare(j, s)
    }
    rendered = json.dumps(result, indent=2, sort_keys=True)
    if args.json:
        args.json.write_text(rendered + "\n", encoding="utf-8")
    print("SLeeLa Java Source Equivalence")
    print("  structural:", result["comparison"]["structural"]["status"])
    print("  semantic:", result["comparison"]["semantic"]["status"])
    print("  functional-source:", result["comparison"]["functional"]["status"])
    if result["comparison"]["structural"]["missingDeclarations"]:
        print("  missing declarations:", ", ".join(
            f"{k} {n}" for k,n in result["comparison"]["structural"]["missingDeclarations"]))
    if result["comparison"]["structural"]["missingMethods"]:
        print("  missing methods:", ", ".join(
            f"{n}({', '.join(p)})" for n,r,p in result["comparison"]["structural"]["missingMethods"]))
    if args.json:
        print("  report:", args.json)

if __name__ == "__main__":
    main()
