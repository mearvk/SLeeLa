#!/usr/bin/env python3
"""Extract type dependencies from Java API members in SLeeLa counterparts.

This is a conservative source-level audit. It records field, method return and
parameter, constructor parameter, generic-bound, inheritance, throws, and
annotation type references. It does not claim semantic or runtime equivalence.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOTS = ("java", "javax", "jdk", "sun", "com.sun", "org.w3c", "org.xml", "org.ietf", "org.omg")
IDENT = r"[A-Za-z_$][\w$]*"
MODIFIERS = "public|protected|private|static|final|abstract|native|synchronized|volatile|transient|strictfp|sealed|non-sealed|default"
MOD_RE = rf"(?:(?:{MODIFIERS})\s+)*"
DECL_RE = re.compile(rf"(?P<mods>{MOD_RE})(?P<kind>class|interface|enum|record)\s+(?P<name>{IDENT})(?P<generics>\s*<[^{{}};()]+>)?(?P<tail>[^{{;]*)(?=\{{)")
METHOD_RE = re.compile(rf"(?P<mods>{MOD_RE})(?P<generics><[^{{}}()]+>\s*)?(?P<ret>[A-Za-z_$][\w$]*(?:\s*<[^;{{}}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)\s+(?P<name>{IDENT})\s*\((?P<args>[^(){{}}]*)\)(?:\s+throws\s+(?P<throws>[^{{;]+))?")
CTOR_RE = re.compile(rf"(?P<mods>(?:(?:public|protected|private)\s+)*)(?P<generics><[^{{}}()]+>\s*)?(?P<name>{IDENT})\s*\((?P<args>[^(){{}}]*)\)(?:\s+throws\s+(?P<throws>[^{{;]+))?")
FIELD_RE = re.compile(rf"(?P<mods>(?:(?:public|protected|private|static|final|volatile|transient)\s+)*)(?P<type>[A-Za-z_$][\w$]*(?:\s*<[^;{{}}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)\s+(?P<name>{IDENT})\s*(?:=[^;]*)?;")
IMPORT_RE = re.compile(rf"(?m)^\s*import\s+(?P<static>static\s+)?(?P<name>{IDENT}(?:\.{IDENT})+)(?P<wild>\.\*)?\s*;")
PACKAGE_RE = re.compile(rf"(?m)^\s*package\s+({IDENT}(?:\.{IDENT})+)\s*;")
ANNOT_RE = re.compile(r"@([A-Za-z_$][\w$.]*)")
TYPE_TOKEN_RE = re.compile(rf"[A-Za-z_$][\w$]*(?:\.[A-Za-z_$][\w$]*)*")
COMMENT_RE = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*.*?\*/', re.S)
PRIMITIVES = {"void","boolean","byte","short","int","long","float","double","char","var","extends","super"}

def strip_comments(source: str) -> str:
    def sub(m):
        token=m.group(0)
        return token if token.startswith(('"','\'')) else re.sub(r"[^\n]"," ",token)
    return COMMENT_RE.sub(sub,source)

def split_top(source: str) -> list[str]:
    out=[]; start=0; depth=0
    for i,ch in enumerate(source):
        if ch in "<([": depth+=1
        elif ch in ">)]": depth=max(0,depth-1)
        elif ch=="," and depth==0:
            out.append(source[start:i].strip()); start=i+1
    tail=source[start:].strip()
    if tail: out.append(tail)
    return out

def type_names(expr: str) -> list[str]:
    expr=re.sub(r"@[\w$.]+(?:\([^)]*\))?"," ",expr)
    names=[]
    for token in TYPE_TOKEN_RE.findall(expr):
        if token not in PRIMITIVES and token not in {"final","volatile","transient"}:
            names.append(token)
    return names

def annotations(expr: str) -> list[str]:
    return sorted(set(ANNOT_RE.findall(expr)))

def type_params(expr: str) -> list[str]:
    if not expr: return []
    inner=expr.strip()[1:-1]
    result=[]
    for part in split_top(inner):
        # Drop the declared type-variable name, retaining all bounds.
        m=re.match(rf"{IDENT}\s+extends\s+(.+)",part)
        if m: result.extend(type_names(m.group(1)))
    return result

def parameter_types(expr: str) -> list[str]:
    out=[]
    for part in split_top(expr):
        part=re.sub(r"@(?:[\w$.]+)(?:\([^)]*\))?\s*","",part)
        part=re.sub(r"\b(final|volatile|transient)\b\s+","",part).strip().replace("...","[]")
        bits=part.split()
        if len(bits)>=2: out.extend(type_names(" ".join(bits[:-1])))
        else: out.extend(type_names(part))
    return out

def inheritance_types(tail: str) -> list[tuple[str,str]]:
    found=[]
    for key in ("extends","implements","permits"):
        m=re.search(rf"\b{key}\s+(.+?)(?=\b(?:extends|implements|permits)\b|$)",tail)
        if m:
            for part in split_top(m.group(1)):
                found.extend((x,key) for x in type_names(part))
    return found

def owner_at(source: str, offset: int, declarations: list[dict]) -> str:
    # Select the innermost preceding type whose opening brace is still open.
    candidates=[]
    for d in declarations:
        if d["offset"]>offset: continue
        open_at=source.find("{",d["end"])
        if open_at<0 or open_at>offset: continue
        depth=0; in_string=False; quote=""
        for ch in source[open_at:offset]:
            if in_string:
                if ch==quote: in_string=False
            elif ch in ('"',"'"): in_string=True; quote=ch
            elif ch=="{": depth+=1
            elif ch=="}": depth-=1
        if depth>0: candidates.append(d)
    return candidates[-1]["name"] if candidates else "<file>"

def resolve(name: str, package: str, imports: list[dict], index: dict[str,str]) -> list[str]:
    name=name.strip()
    if name in PRIMITIVES: return []
    if name in index: return [name]
    if "." in name:
        candidates=[q for q in index if name==q or name.startswith(q+".")]
        if candidates: return [max(candidates,key=len)]
        # It may be a qualified nested type whose enclosing class is indexed.
        return []
    simple=name.rsplit(".",1)[-1]
    explicit=[]
    for item in imports:
        if item["wild"]: continue
        imported=item["name"]
        if imported.rsplit(".",1)[-1]==simple:
            candidates=[q for q in index if imported==q or imported.startswith(q+".")]
            if candidates: explicit.append(max(candidates,key=len))
    if explicit: return sorted(set(explicit))
    local=package+"."+simple if package else simple
    if local in index: return [local]
    jl="java.lang."+simple
    if jl in index: return [jl]
    wildcard=[]
    for item in imports:
        if item["wild"] and not item["static"]:
            candidate=item["name"]+"."+simple
            if candidate in index: wildcard.append(candidate)
    return sorted(set(wildcard)) if len(set(wildcard))==1 else []

def main() -> int:
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root",type=Path,default=Path(__file__).resolve().parents[3])
    ap.add_argument("--json",type=Path)
    ap.add_argument("--check",action="store_true")
    args=ap.parse_args()
    repo=args.root.resolve(); api=repo/"lib/java"
    if not api.is_dir():
        print(f"ERROR: Java API root not found: {api}",file=sys.stderr); return 2
    files=sorted(api.rglob("*.sleela"))
    index={}
    for path in files:
        parts=list(path.relative_to(api).with_suffix("").parts)
        if parts and parts[0]=="early_access":
            if len(parts)<7 or parts[1:4]!=["jdk28","docs","api"]: continue
            parts=parts[5:]
        if not parts or parts[-1] in {"module-info","package-info"}: continue
        q=".".join(parts)
        if any(q==r or q.startswith(r+".") for r in ROOTS): index.setdefault(q,path.relative_to(repo).as_posix())
    edges=[]; members=[]; unresolved=[]
    for q,rel in sorted(index.items()):
        raw=(repo/rel).read_text(encoding="utf-8",errors="replace")
        src=strip_comments(raw)
        pm=PACKAGE_RE.search(src); package=pm.group(1) if pm else q.rpartition(".")[0]
        imports=[{"name":m.group("name"),"wild":bool(m.group("wild")),"static":bool(m.group("static"))} for m in IMPORT_RE.finditer(src)]
        decls=[]
        for m in DECL_RE.finditer(src):
            decls.append({"name":m.group("name"),"offset":m.start(),"end":m.end(),"generics":m.group("generics") or "","tail":m.group("tail")})
        records=[]
        for m in METHOD_RE.finditer(src):
            owner=owner_at(src,m.start(),decls)
            # Ignore method-like expressions and declarations outside a type.
            if owner=="<file>": continue
            records.append(("method",owner,m.group("name"),m.start(),[
                (m.group("ret"),"return"), *((x,"parameter") for x in parameter_types(m.group("args"))),
                *((x,"throws") for x in type_names(m.group("throws") or "")),
                *((x,"generic-bound") for x in type_params(m.group("generics") or ""))]))
        type_names_decl={d["name"] for d in decls}
        for m in CTOR_RE.finditer(src):
            if m.group("name") not in type_names_decl: continue
            owner=owner_at(src,m.start(),decls)
            records.append(("constructor",owner,m.group("name"),m.start(),[
                *((x,"parameter") for x in parameter_types(m.group("args"))),
                *((x,"throws") for x in type_names(m.group("throws") or "")),
                *((x,"generic-bound") for x in type_params(m.group("generics") or ""))]))
        for m in FIELD_RE.finditer(src):
            owner=owner_at(src,m.start(),decls)
            if owner!="<file>": records.append(("field",owner,m.group("name"),m.start(),[(m.group("type"),"field-type")]))
        for d in decls:
            owner=owner_at(src,d["offset"],decls)
            if owner=="<file>": owner=d["name"]
            refs=[(x,"generic-bound") for x in type_params(d["generics"])]
            refs.extend(inheritance_types(d["tail"]))
            for ann in annotations(src[max(0,d["offset"]-500):d["offset"]]):
                refs.append((ann,"annotation"))
            records.append(("type",owner,d["name"],d["offset"],refs))
        for kind,owner,name,offset,refs in records:
            mid=f"{q}#{owner}.{name}@{offset}"
            resolved=[]
            # Include declaration-site annotations for methods, constructors,
            # and fields as well as type declarations.
            prefix=src[max(0,offset-400):offset]
            member_refs=list(refs)+[(ann,"annotation") for ann in annotations(prefix)]
            for expr,role in member_refs:
                for token in type_names(expr):
                    targets=resolve(token,package,imports,index)
                    for target in targets:
                        if target!=q:
                            resolved.append((target,role,token))
                    imported_missing=[it["name"] for it in imports if not it["wild"] and it["name"].rsplit(".",1)[-1]==token and any(it["name"].startswith(r+".") for r in ROOTS) and it["name"] not in index]
                    qualified_missing="." in token and any(token.startswith(r+".") for r in ROOTS) and not any(token==x or token.startswith(x+".") for x in index)
                    if not targets and (imported_missing or qualified_missing):
                        unresolved.append({"source":rel,"owner":owner,"member":name,"reference":imported_missing[0] if imported_missing else token,"role":role,"line":src.count("\n",0,offset)+1})
            for target,role,token in sorted(set(resolved)):
                edges.append({"from":q,"to":target,"owner":owner,"member":name,"member_kind":kind,"role":role,"reference":token,"line":src.count("\n",0,offset)+1})
            members.append({"id":mid,"owner":owner,"name":name,"kind":kind,"dependency_count":len(set(resolved))})
    edges.sort(key=lambda x:(x["from"],x["owner"],x["member"],x["to"],x["role"],x["line"]))
    unresolved.sort(key=lambda x:(x["source"],x["line"],x["reference"]))
    graph={"schema":"sleela-java-api-member-dependencies-v1","scope":"static member-level source type dependencies; not behavioral equivalence","summary":{"source_files":len(files),"indexed_types":len(index),"members":len(members),"dependency_edges":len(edges),"unresolved_references":len(unresolved)},"members":members,"edges":edges,"unresolved":unresolved}
    encoded=json.dumps(graph,indent=2,sort_keys=True)+"\n"
    if args.json:
        args.json.parent.mkdir(parents=True,exist_ok=True); args.json.write_text(encoded,encoding="utf-8")
    print("Java API member dependency audit")
    for k,v in graph["summary"].items(): print(f"{k}: {v}")
    print("member-dependencies: "+("FAIL" if args.check and unresolved else "REVIEW REQUIRED" if unresolved else "PASS"))
    return 1 if args.check and unresolved else 0

if __name__=="__main__":
    raise SystemExit(main())
