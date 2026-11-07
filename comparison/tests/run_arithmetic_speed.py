#!/usr/bin/env python3
"""Arithmetic throughput sweep: C11, C++17, Java 21+."""
from __future__ import annotations
import csv,json,os,pathlib,platform,shlex,subprocess,tempfile,statistics
ROOT=pathlib.Path(__file__).resolve().parents[2]
SRC=ROOT/"comparison/tests/native/arithmetic_speed.c"; JAVA=ROOT/"comparison/tests/java/ArithmeticSpeed.java"
RESULT={"suite":"SCS-PERF-ARITH-001","status":"RUNNING","host":{"os":platform.platform(),"machine":platform.machine(),"processor":platform.processor(),"logical_cores":os.cpu_count()},"targets":{},"notes":["Hosted-runner timings are informational, not release-grade rankings.","C/C++ use exact-width integer types; Java unsigned narrow types are masked emulations.","i128 is tested in C/C++ only when __int128 is supported; Java has no primitive i128.","Boolean is excluded from +,-,*,/ because arithmetic on booleans is not defined."]}
def run(cmd): return subprocess.run(cmd,cwd=ROOT,check=True,text=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE).stdout
def parse(s):
 rows=list(csv.DictReader(s.splitlines()))
 for r in rows:r["operations"]=int(r["operations"]);r["ns_per_operation"]=float(r["ns_per_operation"]);r["checksum"]=int(r["checksum"])
 return rows
def aggregate(samples):
 groups={}
 for sample in samples:
  for r in sample:groups.setdefault((r["type"],r["operator"]),[]).append(r)
 out=[]
 for _,rs in sorted(groups.items()):
  checks={r["checksum"] for r in rs}
  out.append({**rs[0],"ns_per_operation":statistics.median(r["ns_per_operation"] for r in rs),"samples_ns_per_operation":[r["ns_per_operation"] for r in rs],"status":"PASS" if len(checks)==1 else "UNSTABLE_CHECKSUM"})
 return out
def main():
 cc=shlex.split(os.environ.get("CC","cc"));cxx=shlex.split(os.environ.get("CXX","c++"))
 with tempfile.TemporaryDirectory(prefix="sleela-arith-") as td:
  p=pathlib.Path(td);ce=p/("c.exe" if os.name=="nt" else "c");cpe=p/("cpp.exe" if os.name=="nt" else "cpp")
  run(cc+["-O2","-std=c11","-Wall","-Wextra","-Werror",str(SRC),"-o",str(ce)])
  run(cxx+["-O2","-std=c++17","-Wall","-Wextra","-Werror","-x","c++",str(SRC),"-o",str(cpe)])
  for name,exe in (("C11",ce),("C++17",cpe)):
   run([str(exe)]);RESULT["targets"][name]=aggregate([parse(run([str(exe)])) for _ in range(5)])
  jo=p/"java";jo.mkdir();run(["javac","-encoding","UTF-8","-d",str(jo),str(JAVA)])
  for _ in range(3):run(["java","-cp",str(jo),"ArithmeticSpeed"])
  RESULT["targets"]["Java 21+"]=aggregate([parse(run(["java","-cp",str(jo),"ArithmeticSpeed"])) for _ in range(5)])
  RESULT["warmup_sweeps"]={"native":1,"java":3};RESULT["measured_sweeps"]=5
  RESULT["status"]="PASS" if all(r["status"]=="PASS" for t in RESULT["targets"].values() for r in t) else "FAIL"
  (ROOT/"comparison-arithmetic-speed-results.json").write_text(json.dumps(RESULT,indent=2)+"\n",encoding="utf-8")
  print(RESULT["status"],"arithmetic speed sweep; targets:",", ".join(RESULT["targets"]),"cases:",sum(map(len,RESULT["targets"].values())))
  return 0 if RESULT["status"]=="PASS" else 1
if __name__=="__main__":
 try:raise SystemExit(main())
 except Exception as e:
  RESULT["status"]="FAIL";RESULT["error"]=str(e);(ROOT/"comparison-arithmetic-speed-results.json").write_text(json.dumps(RESULT,indent=2)+"\n",encoding="utf-8");raise
