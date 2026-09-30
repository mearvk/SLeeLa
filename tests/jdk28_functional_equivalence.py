#!/usr/bin/env python3
"""JDK 28 SLeeLa/Java conformance and functional-equivalence audit.

This test deliberately distinguishes:
  1. API identity: a .sleela file names a real documented JDK 28 type.
  2. Envelope contract: the generated SLeeLa declaration exposes the required
     SLeeLa/Java bridge operations.
  3. Functional equivalence: the SLeeLa declaration actually represents the
     Java type's documented constructors, methods, fields and inheritance.

The current generated envelopes are contracts, not implementations. Therefore
the functional-equivalence gate is expected to fail until the compiler/runtime
can execute the Java API surface rather than merely identify it.
"""
from __future__ import annotations
import argparse, html, re, sys, urllib.request
from pathlib import Path
from urllib.parse import urljoin, urlparse

INDEX_URL = "https://download.java.net/java/early_access/jdk28/docs/api/allclasses-index.html"
ROOT = Path(__file__).resolve().parents[1] / "lib" / "java"
REQUIRED = {
    "typeName": r'String\s+typeName\s*\(',
    "available": r'boolean\s+available\s*\(',
    "construct": r'String\s+construct\s*\(',
    "invoke": r'String\s+invoke\s*\(\s*String\s+operation\s*\)',
    "invokeStatic": r'String\s+invokeStatic\s*\(\s*String\s+operation\s*\)',
}

def fetch(url: str) -> str:
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-JDK28-Conformance/1.0"})
    with urllib.request.urlopen(req, timeout=60) as r:
        return r.read().decode("utf-8", errors="replace")

def documented_types(index: str) -> set[str]:
    out=set()
    for raw in re.findall(r'href=["\']([^"\']+\.html(?:#[^"\']*)?)["\']', index):
        href=html.unescape(raw.split("#",1)[0])
        p=urlparse(urljoin(INDEX_URL, href)).path.strip("/").split("/")
        if len(p) < 3 or not p[-1].endswith(".html"):
            continue
        name=p[-1][:-5]
        if name in {"module-info","package-info","package-summary","module-summary","package-use","deprecated-list","index","overview-summary","class-use","serialized-form"}:
            continue
        if any(x in {"doc-files","index-files"} for x in p):
            continue
        out.add(".".join(p[1:-1]+[name]))
    return out

def source_types() -> list[tuple[Path,str,str,str]]:
    result=[]
    for f in ROOT.rglob("*.sleela"):
        s=f.read_text(encoding="utf-8", errors="replace")
        m=re.search(r'String\s+javaType\s*=\s*"([^"]+)"\s*;', s)
        c=re.search(r'\bclass\s+([A-Za-z_$][\w$]*)\s*\{', s)
        if not m or not c:
            result.append((f,"","",s))
        else:
            result.append((f,m.group(1),c.group(1),s))
    return sorted(result)

def main() -> int:
    ap=argparse.ArgumentParser()
    ap.add_argument("--functional", action="store_true",
                    help="require actual Java API behavior/signature representation")
    ap.add_argument("--offline-index", type=Path)
    args=ap.parse_args()

    index=(args.offline_index.read_text(encoding="utf-8") if args.offline_index else fetch(INDEX_URL))
    docs=documented_types(index)
    files=source_types()

    malformed=[]; unknown=[]; missing_contract=[]; behavioral=[]
    for f,qt,cn,s in files:
        if not qt or not cn:
            malformed.append(str(f)); continue
        if qt not in docs:
            unknown.append((str(f),qt))
        for name,pat in REQUIRED.items():
            if not re.search(pat,s):
                missing_contract.append((str(f),name))
        # A true implementation must declare the Java API surface. The current
        # envelopes contain only the bridge methods above, so flag them rather
        # than treating the generic invoke() operation as Java-method parity.
        bridge_only = all(re.search(pat,s) for pat in REQUIRED.values())
        declared_api = re.findall(r'\b(?:public|protected|private)?\s*(?:static\s+)?[A-Za-z_$][\w$<>,\[\].?]*\s+[A-Za-z_$][\w$]*\s*\([^;{}]*\)\s*\{', s)
        if bridge_only and len(declared_api) <= len(REQUIRED):
            behavioral.append((str(f),qt))

    print(f"JDK 28 documented types: {len(docs)}")
    print(f"SLeeLa .sleela files inspected: {len(files)}")
    print(f"Malformed SLeeLa envelopes: {len(malformed)}")
    print(f"Types not found in JDK 28 index: {len(unknown)}")
    print(f"Missing envelope contract methods: {len(missing_contract)}")
    print(f"Not behaviorally represented by Java API declarations: {len(behavioral)}")

    if malformed or unknown or missing_contract:
        for x in malformed[:20]: print("MALFORMED",x)
        for x in unknown[:20]: print("UNKNOWN",*x)
        for x in missing_contract[:20]: print("MISSING_CONTRACT",*x)
        return 1

    if args.functional and behavioral:
        print()
        print("FUNCTIONAL-EQUIVALENCE GATE: FAIL")
        print("The inspected SLeeLa files identify Java types and provide bridge")
        print("operations, but do not declare the Java cousins' actual API members.")
        print("They are source contracts, not functional implementations.")
        for x in behavioral[:25]: print("BEHAVIORAL_GAP",*x)
        if len(behavioral) > 25:
            print(f"... {len(behavioral)-25} additional behavioral gaps")
        return 2

    print("API-IDENTITY/ENVELOPE-CONTRACT GATE: PASS")
    if not args.functional:
        print("Functional equivalence was not requested.")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
