#!/usr/bin/env python3
"""Cross-check array bounds and strict UTF-8 behavior in C, C++, and Java."""
import json, os, pathlib, shlex, subprocess, sys, tempfile
ROOT=pathlib.Path(__file__).resolve().parents[2]
EXPECTED=["EMPTY_LENGTH=0","SINGLE_LENGTH=1","FIRST_VALUE=11","LAST_VALUE=33","NEGATIVE_INDEX=REJECTED","ONE_PAST_END=REJECTED","UTF8_ROUNDTRIP=PASS","UTF8_INVALID=REJECTED","UTF8_BYTES=7","UNICODE_CODEPOINTS=3"]
RESULT={"suite":"SCS-TEST-003-arrays-strings","expected":EXPECTED,"implementations":{}}
def run(cmd):
 r=subprocess.run(cmd,cwd=ROOT,check=True,text=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
 return r.stdout.replace("\r\n","\n").strip().splitlines()
def record(name,cmd):
 actual=run(cmd);RESULT["implementations"][name]={"command":cmd,"actual":actual,"status":"PASS" if actual==EXPECTED else "FAIL"}
 if actual!=EXPECTED: raise AssertionError(f"{name}: expected {EXPECTED!r}, got {actual!r}")
 print(f"PASS {name}: {len(actual)} array/string assertions")
def main():
 cc=shlex.split(os.environ.get("CC","cc"));cxx=shlex.split(os.environ.get("CXX","c++"));suffix=".exe" if os.name=="nt" else ""
 RESULT["platform"]=sys.platform;RESULT["python"]=sys.version.split()[0]
 with tempfile.TemporaryDirectory(prefix="sleela-array-string-") as t:
  b=pathlib.Path(t);ce=b/("arrays-c"+suffix);cpe=b/("arrays-cpp"+suffix);jo=b/"java";jo.mkdir()
  run(cc+["-std=c11","-Wall","-Wextra","-Werror",str(ROOT/"comparison/tests/native/array_string_vectors.c"),"-o",str(ce)]);record("C11",[str(ce)])
  run(cxx+["-std=c++17","-Wall","-Wextra","-Werror",str(ROOT/"comparison/tests/native/array_string_vectors.cpp"),"-o",str(cpe)]);record("C++17",[str(cpe)])
  run(["javac","-encoding","UTF-8","-Xlint:all","-d",str(jo),str(ROOT/"comparison/tests/java/ArrayStringVectors.java")]);record("Java 21+",["java","-cp",str(jo),"ArrayStringVectors"])
 RESULT["status"]="PASS";(ROOT/"comparison-array-string-results.json").write_text(json.dumps(RESULT,indent=2)+"\n",encoding="utf-8")
if __name__=="__main__":
 try: main()
 except Exception as e:
  RESULT["status"]="FAIL";RESULT["error"]=str(e);(ROOT/"comparison-array-string-results.json").write_text(json.dumps(RESULT,indent=2)+"\n",encoding="utf-8");print("FAIL array/string vectors:",e,file=sys.stderr);raise
