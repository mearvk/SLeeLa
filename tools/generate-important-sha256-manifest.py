#!/usr/bin/env python3
"""Generate the SLeeLa important-file SHA-256 integrity manifest."""
from __future__ import annotations
import argparse, hashlib, json
from pathlib import Path

SOURCE_SUFFIXES={".c",".h",".cc",".hh",".cpp",".hpp",".py",".sh",".ps1",".sleela",".sst",".model"}
DOC_SUFFIXES={".md",".html",".xml",".xsd",".json",".txt",".yml",".yaml",".cfg",".conf"}
ROOT_FILES={
"README.md","ARCHITECTURE.md","BUILD.md","CLASS.md","COMPILER.md","CONNECTOR.md",
"DEFINITIONS.md","GLOSSARY.md","MEMORY_MANAGER.md","NATIVE_API.md","NETWORK.md",
"SERVER.EDITION.md","SLEELA.syntax","VERSION.md","REVISIONS.md","BODI.md",
"BODI_GARDULUS_II.md","FILE.SYSTEM.md","FILEIO.md","IO.md","CHANGES-MEMORY-NATIVE.md",
"TIMINGS.md","SYNCHRO.md","MUNCTION.md",
}
EXCLUDE_NAMES={"important-sha256-manifest.json","sha256-manifest.json","sha256-manifest.regenerated.json"}

def digest(path:Path)->str:
    h=hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda:f.read(1024*1024),b""): h.update(chunk)
    return h.hexdigest()

def include(path:Path,root:Path)->bool:
    rel=path.relative_to(root).as_posix()
    if any(part.startswith(".git") for part in path.relative_to(root).parts): return False
    if path.name in EXCLUDE_NAMES: return False
    if rel in ROOT_FILES: return True
    top=path.relative_to(root).parts[0]
    if top=="international-criminal-court": return True
    if top==".github": return path.suffix in {".yml",".yaml"}
    if top in {"api","server-edition","security"}:
        return path.suffix in SOURCE_SUFFIXES|DOC_SUFFIXES or path.suffix in {".docx",".pem",".pub"}
    return path.suffix in SOURCE_SUFFIXES

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--root",type=Path,default=Path("."))
    ap.add_argument("--output",type=Path,default=Path("security/important-sha256-manifest.json"))
    args=ap.parse_args()
    root=args.root.resolve()
    files=[]
    for f in sorted(root.rglob("*")):
        if f.is_file() and include(f,root):
            files.append({"path":f.relative_to(root).as_posix(),"sha256":digest(f)})
    if not files: raise SystemExit("no important files found")
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(json.dumps({"algorithm":"sha256","scope":"important-files","files":files},indent=2)+"\n",encoding="utf-8")
    print(f"generated {len(files)} SHA-256 entries")

if __name__=="__main__": main()
