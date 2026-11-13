#!/usr/bin/env python3
"""Audit Java-style member visibility and constructor inheritance contracts.

The report distinguishes declared members from effective inherited members.
Constructors are deliberately never inherited; accessible superclass
constructors are reported as candidates for an explicit super-constructor call.
"""
from __future__ import annotations
import argparse
import json
import re
import sys
from pathlib import Path

ROOTS=("java","javax","jdk","sun","com.sun","org.w3c","org.xml","org.ietf","org.omg")
TYPE_RE=re.compile(r"(?m)^\s*(?P<mods>(?:(?:public|protected|private|abstract|final|static|sealed|non-sealed)\s+)*)?(?P<kind>class|interface|enum|record)\s+(?P<name>[A-Za-z_$][\w$]*)(?P<tail>[^\n{]*)")
MEMBER_RE=re.compile(r"(?m)^\s*(?P<mods>(?:(?:public|protected|private|static|final|abstract|native|synchronized|default|volatile|transient|strictfp)\s+)*)(?P<sig>[^\n;{}]+?)\s*(?:;|\{)")
CTOR_RE=re.compile(r"(?m)^\s*(?P<mods>(?:(?:public|protected|private)\s+)*)(?P<name>[A-Za-z_$][\w$]*)\s*\((?P<args>[^(){}]*)\)\s*(?:throws\s+[^{};]+)?\s*\{")
IDENT=re.compile(r"[A-Za-z_$][\w$]*")
COMMENT_RE=re.compile(r'"(?:\\.|[^"\\])*"|//[^\n]*|/\*.*?\*/',re.S)

def strip_comments(s):
    return COMMENT_RE.sub(lambda m: m.group(0) if m.group(0).startswith('"') else re.sub(r"[^\n]"," ",m.group(0)),s)

def visibility(mods):
    words=set(mods.split())
    return "private" if "private" in words else "protected" if "protected" in words else "public" if "public" in words else "package"

def accessible(vis, from_package, owner_package, subclass=False):
    if vis=="public": return True
    if vis=="private": return False
    if vis=="package": return from_package==owner_package
    return subclass or from_package==owner_package

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root",type=Path,default=Path(__file__).resolve().parents[3])
    ap.add_argument("--json",type=Path)
    ap.add_argument("--check",action="store_true")
    a=ap.parse_args(); repo=a.root.resolve(); api=repo/"lib/java"
    if not api.is_dir():
        print(f"ERROR: Java API root not found: {api}",file=sys.stderr); return 2
    types={}
    for p in sorted(api.rglob("*.sleela")):
        parts=list(p.relative_to(api).with_suffix("").parts)
        if parts and parts[0]=="early_access":
            if len(parts)<7 or parts[1:4]!=["jdk28","docs","api"]: continue
            parts=parts[5:]
        if not parts or parts[-1] in {"module-info","package-info"}: continue
        q=".".join(parts)
        if not any(q==r or q.startswith(r+".") for r in ROOTS): continue
        src=strip_comments(p.read_text(encoding="utf-8",errors="replace"))
        tm=TYPE_RE.search(src)
        if not tm: continue
        package=q.rpartition(".")[0]
        kind=tm.group("kind"); name=tm.group("name")
        tail=tm.group("tail")
        sm=re.search(r"\bextends\s+([A-Za-z_$][\w$.]*)",tail)
        im=re.search(r"\bimplements\s+(.+)$",tail)
        parents=[]
        if sm: parents.append(sm.group(1))
        if im: parents.extend(x.strip().split("<",1)[0] for x in im.group(1).split(","))
        import_map={}
        for imp in re.finditer(r"(?m)^\s*import\s+(?!static\s+)([A-Za-z_$][\w$.]*);",src):
            import_map[imp.group(1).rsplit(".",1)[-1]]=imp.group(1)
        parents=[x if "." in x else import_map.get(x,package+"."+x) for x in parents]
        ctors=[]
        for cm in CTOR_RE.finditer(src):
            if cm.group("name")==name:
                ctors.append({"visibility":visibility(cm.group("mods")),"parameters":cm.group("args").strip(),"declared":True})
        if kind=="class" and not ctors:
            ctors=[{"visibility":visibility(tm.group("mods") or ""),"parameters":"","declared":False,"implicit_default":True}]
        members=[]
        for mm in MEMBER_RE.finditer(src):
            sig=mm.group("sig").strip()
            if not sig or sig.startswith(("package ","import ","return ","throw ","new ")): continue
            # Avoid treating nested type declarations and constructors as fields/methods.
            if re.match(r"(?:class|interface|enum|record)\b",sig) or re.match(rf"{re.escape(name)}\s*\(",sig): continue
            if "(" in sig:
                mname=re.search(r"([A-Za-z_$][\w$]*)\s*\([^()]*\)\s*(?::|$)",sig)
                if not mname: mname=re.search(r"([A-Za-z_$][\w$]*)\s*\(",sig)
                if not mname: continue
                member_name=mname.group(1); mkind="method"
            else:
                bits=sig.split()
                if len(bits)<2: continue
                member_name=re.sub(r"[^A-Za-z0-9_$].*$","",bits[-1]); mkind="field"
            if not member_name: continue
            members.append({"name":member_name,"kind":mkind,"visibility":visibility(mm.group("mods")),"signature":sig})
        types[q]={"path":p.relative_to(repo).as_posix(),"package":package,"kind":kind,"visibility":visibility(tm.group("mods") or ""),"parents":parents,"constructors":ctors,"members":members}
    inherited=[]; ctor_rules=[]; unresolved=[]
    for q,t in sorted(types.items()):
        seen=set(); frontier=[(p,1) for p in t["parents"]]
        while frontier:
            parent,depth=frontier.pop(0)
            if parent in seen: continue
            seen.add(parent)
            pt=types.get(parent)
            if not pt:
                if any(parent.startswith(r+".") for r in ROOTS):
                    unresolved.append({"type":q,"reference":parent,"role":"supertype"})
                continue
            for m in pt["members"]:
                vis=m["visibility"]
                if vis=="private": continue
                if vis=="package" and t["package"]!=pt["package"]: continue
                # A public/protected member is inherited; package members only within package.
                inherited.append({"type":q,"declared_in":parent,"member":m["name"],"kind":m["kind"],"visibility":vis,"depth":depth,"signature":m["signature"]})
            # Java constructors are not inherited. Report accessible superclass
            # constructors as explicit super-call options, not subclass members.
            for ctor in pt["constructors"]:
                if accessible(ctor["visibility"],t["package"],pt["package"],subclass=True):
                    ctor_rules.append({"subtype":q,"supertype":parent,"constructor_parameters":ctor["parameters"],"visibility":ctor["visibility"],"inherited":False,"requires_explicit_super_invocation":True})
            frontier.extend((p,depth+1) for p in pt["parents"])
    inherited.sort(key=lambda x:(x["type"],x["depth"],x["declared_in"],x["name"]))
    ctor_rules.sort(key=lambda x:(x["subtype"],x["supertype"],x["constructor_parameters"]))
    result={"schema":"sleela-java-visibility-constructor-audit-v1","scope":"source-level Java-style visibility and constructor inheritance; conservative syntax recognizer","summary":{"indexed_types":len(types),"declared_members":sum(len(t["members"]) for t in types.values()),"effective_inherited_members":len(inherited),"accessible_super_constructors":len(ctor_rules),"unresolved_supertypes":len(unresolved)},"types":types,"inherited_members":inherited,"super_constructor_options":ctor_rules,"unresolved":unresolved,"rules":{"private_members":"not inherited","package_members":"inherited only within the same package","protected_members":"inherited by subclasses","public_members":"inherited","constructors":"never inherited; accessible superclass constructors may be invoked explicitly"}}
    encoded=json.dumps(result,indent=2,sort_keys=True)+"\n"
    if a.json:
        a.json.parent.mkdir(parents=True,exist_ok=True); a.json.write_text(encoded,encoding="utf-8")
    print("Java visibility and constructor audit")
    for k,v in result["summary"].items(): print(f"{k}: {v}")
    for x in unresolved: print(f"UNRESOLVED {x['type']} -> {x['reference']}")
    print("visibility-constructor-audit: "+("FAIL" if a.check and unresolved else "REVIEW REQUIRED" if unresolved else "PASS"))
    return 1 if a.check and unresolved else 0
if __name__=="__main__":
    raise SystemExit(main())
