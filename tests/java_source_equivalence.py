#!/usr/bin/env python3
import argparse,json,re
from pathlib import Path
JAVA_KEYWORDS=set("""abstract assert boolean break byte case catch char class const continue default do double else enum extends final finally float for goto if implements import int interface long native new package private protected public record return short static strictfp super switch synchronized this throw throws transient try void volatile while var yield sealed permits non-sealed module open requires exports opens to uses provides with transitive true false null""".split())
STATEMENTS={"if","else","while","do","for","switch","case","default","break","continue","return","throw","try","catch","finally","synchronized","assert","yield"}
EXPRESSION_MARKERS={"new","instanceof","this","super","::","?","=","+","-","*","/","%","==","!=","<",">","&&","||","!"}
DECL_RE=re.compile(r"(?P<mods>(?:(?:public|protected|private|static|final|abstract|native|synchronized|volatile|transient|strictfp|sealed|non-sealed|default)\s+)*)"+r"(?P<kind>class|interface|enum|record)\s+(?P<name>[A-Za-z_$][\w$]*)")
METHOD_RE=re.compile(r"(?P<mods>(?:(?:public|protected|private|static|final|abstract|native|synchronized|strictfp|default)\s+)*)"+r"(?P<ret>[A-Za-z_$][\w$]*(?:\s*<[^;{}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)\s+(?P<name>[A-Za-z_$][\w$]*)\s*\((?P<args>[^(){}]*)\)")
FIELD_RE=re.compile(r"(?P<mods>(?:(?:public|protected|private|static|final|volatile|transient)\s+)*)"+r"(?P<type>[A-Za-z_$][\w$]*(?:\s*<[^;{}()]+>)?(?:\[\])*(?:\.[A-Za-z_$][\w$]*)*)\s+(?P<name>[A-Za-z_$][\w$]*)\s*(?:=[^;]*)?;")
ANNOTATION_RE=re.compile(r"@(?:[A-Za-z_$][\w$]*\.)*[A-Za-z_$][\w$]*")
API_REF_RE=re.compile(r"\b(?:java|javax|jdk|sun)\.[A-Za-z_$][\w$]*(?:\.[A-Za-z_$][\w$]*)*")
def clean(s): return re.sub(r"//.*","",re.sub(r"/\*.*?\*/","",s,flags=re.S))
def nt(s): return re.sub(r"\s+","",s).replace("java.lang.","")
def params(s):
 out=[]
 for p in filter(None,[x.strip() for x in s.split(",")]):
  p=re.sub(r"@[\w$.]+(?:\([^)]*\))?\s*","",p); b=p.split(); out.append(nt(" ".join(b[:-1] if len(b)>1 else b)))
 return out
def inv(text,label):
 src=clean(text); words=re.findall(r"[A-Za-z_$][\w$-]*",src)
 return {"label":label,"lines":len(text.splitlines()),"compatibilityVocabulary":sorted({w for w in words if w in JAVA_KEYWORDS}),"annotations":sorted(set(ANNOTATION_RE.findall(src))),"declarations":[{"kind":m.group("kind"),"name":m.group("name"),"modifiers":m.group("mods").split()} for m in DECL_RE.finditer(src)],"methods":[{"name":m.group("name"),"returnType":nt(m.group("ret")),"parameterTypes":params(m.group("args")),"modifiers":m.group("mods").split()} for m in METHOD_RE.finditer(src)],"fields":[{"name":m.group("name"),"type":nt(m.group("type")),"modifiers":m.group("mods").split()} for m in FIELD_RE.finditer(src)],"statementMarkers":sorted({w for w in words if w in STATEMENTS}),"expressionMarkers":sorted({x for x in EXPRESSION_MARKERS if x in src}),"javaApiReferences":sorted(set(API_REF_RE.findall(src)))}
def compare(j,s):
 jd={(x["kind"],x["name"]) for x in j["declarations"]}; sd={(x["kind"],x["name"]) for x in s["declarations"]}
 jm={(x["name"],x["returnType"],tuple(x["parameterTypes"])) for x in j["methods"]}; sm={(x["name"],x["returnType"],tuple(x["parameterTypes"])) for x in s["methods"]}
 return {"structural":{"status":"PASS" if not jd-sd else "INCOMPLETE","missingDeclarations":[list(x) for x in sorted(jd-sd)],"extraDeclarations":[list(x) for x in sorted(sd-jd)],"missingMethods":[list(x) for x in sorted(jm-sm)]},"semantic":{"status":"INCOMPLETE","reason":"Full Java type/conversion, overload, override, definite-assignment, and exception analysis is not yet claimed."},"functional":{"status":"INCOMPLETE","reason":"Source/API inventory does not establish runtime behavior; API envelopes are not treated as implementations."}}
def main():
 ap=argparse.ArgumentParser(); ap.add_argument("--java",required=True,type=Path); ap.add_argument("--sleela",required=True,type=Path); ap.add_argument("--json",type=Path); a=ap.parse_args()
 r={"tool":"sleela-java-source-equivalence","scope":"source/API congruence; no JVM/SLVM requirement","java":inv(a.java.read_text(),"java"),"sleela":inv(a.sleela.read_text(),"sleela")}; r["comparison"]=compare(r["java"],r["sleela"]); out=json.dumps(r,indent=2,sort_keys=True)
 if a.json:a.json.write_text(out+"\n")
 print("SLeeLa Java Source Equivalence"); print("  structural:",r["comparison"]["structural"]["status"]); print("  semantic:",r["comparison"]["semantic"]["status"]); print("  functional-source:",r["comparison"]["functional"]["status"])
 if a.json:print("  report:",a.json)
if __name__=="__main__":main()
