#!/usr/bin/env python3
"""Audit Java API counterpart dependency closure in lib/java.

This is source/API qualification only. It discovers Java-qualified references
in SLeeLa source and verifies that a corresponding lib/java envelope exists.
It never invents or overwrites API files.
"""
from __future__ import annotations
import argparse,re
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
REPO=ROOT.parent
JAVA_ROOT=ROOT

JAVA_REF=re.compile(r'\b(?:java|javax|jdk|sun)\.(?:[A-Za-z_$][\w$]*\.)+[A-Za-z_$][\w$]*')
IMPORT=re.compile(r'\bimport\s+((?:java|javax|jdk|sun)\.[A-Za-z0-9_$.]+)')

def candidate_path(q):
    parts=q.split(".")
    if len(parts)<2: return None
    return JAVA_ROOT.joinpath(*parts[:-1],parts[-1]+".sleela")

def scan(root):
    refs={}
    for p in root.rglob("*.sleela"):
        if "lib/java" in p.as_posix():
            continue
        try: text=p.read_text(encoding="utf-8",errors="replace")
        except OSError: continue
        found=set(JAVA_REF.findall(text))
        found.update(IMPORT.findall(text))
        for q in found: refs.setdefault(q,[]).append(str(p.relative_to(REPO)))
    return refs

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--check",action="store_true")
    ap.add_argument("--root",type=Path,default=REPO)
    args=ap.parse_args()
    refs=scan(args.root)
    missing={q:files for q,files in refs.items() if candidate_path(q) is None or not candidate_path(q).exists()}
    print(f"Java API references discovered: {len(refs)}")
    print(f"Missing Java API counterparts: {len(missing)}")
    for q,files in sorted(missing.items()):
        print(f"MISSING {q} <- {', '.join(files[:5])}")
    return 1 if missing else 0

if __name__=="__main__":
    raise SystemExit(main())
