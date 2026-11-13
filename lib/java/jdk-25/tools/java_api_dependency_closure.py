#!/usr/bin/env python3
"""Audit Java API counterpart dependency closure in lib/java."""
from __future__ import annotations
import argparse,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
REPO=ROOT.parent
JAVA_ROOT=ROOT
JAVA_REF=re.compile(r'\b(?:java|javax|jdk|sun)\.(?:[A-Za-z_$][\w$]*\.)+[A-Za-z_$][\w$]*')
IMPORT=re.compile(r'\bimport\s+((?:java|javax|jdk|sun)\.[A-Za-z0-9_$.]+)')
def candidate_path(q):
    p=q.split('.')
    return JAVA_ROOT.joinpath(*p[:-1],p[-1]+'.sleela') if len(p)>1 else None
def scan(root):
    refs={}
    for f in root.rglob('*.sleela'):
        if 'lib/java' in f.as_posix(): continue
        try: s=f.read_text(encoding='utf-8',errors='replace')
        except OSError: continue
        for q in set(JAVA_REF.findall(s))|set(IMPORT.findall(s)):
            refs.setdefault(q,[]).append(str(f.relative_to(root)))
    return refs
def main():
    ap=argparse.ArgumentParser(); ap.add_argument('--check',action='store_true'); ap.add_argument('--root',type=Path,default=REPO)
    a=ap.parse_args(); refs=scan(a.root)
    missing={q:v for q,v in refs.items() if candidate_path(q) is None or not candidate_path(q).exists()}
    print(f'Java API references discovered: {len(refs)}')
    print(f'Missing Java API counterparts: {len(missing)}')
    for q,v in sorted(missing.items()): print(f'MISSING {q} <- {", ".join(v[:5])}')
    return 1 if missing else 0
if __name__=='__main__': raise SystemExit(main())
