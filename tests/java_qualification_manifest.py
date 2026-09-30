#!/usr/bin/env python3
"""Run the source-level Java/SLeeLa qualification layers as one system."""
import argparse,json,platform,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
VERSION=ROOT/"VERSION.md"
SUITES=[
 ("source-equivalence", ["python3","tests/java_source_equivalence.py","--java","tests/java_equivalence/EquivalenceBasic.java","--sleela","tests/java_equivalence/EquivalenceBasic.sleela"]),
 ("expression-equivalence", ["python3","tests/java_source_equivalence.py","--java","tests/java_equivalence/ExpressionSurface.java","--sleela","tests/java_equivalence/ExpressionSurface.sleela"]),
 ("signature-comparison", ["python3","tests/java_signature_comparison_suite.py"]),
 ("constraints", ["python3","tests/java_constraints_suite.py"]),
 ("flow", ["python3","tests/java_flow_suite.py"]),
 ("exceptions", ["python3","tests/java_exceptions_suite.py"]),
 ("api-dependencies", ["python3","tests/java_api_dependency_suite.py"]),
 ("platform-matrix", ["python3","tests/java_platform_qualification.py"]),
]
def run(name,cmd):
 p=subprocess.run(cmd,cwd=ROOT,text=True,capture_output=True)
 return {"name":name,"returncode":p.returncode,"status":"PASS" if p.returncode==0 else "FAIL","stdout":p.stdout[-4000:],"stderr":p.stderr[-4000:]}
def main():
 ap=argparse.ArgumentParser(); ap.add_argument("--json",type=Path); ap.add_argument("--stop-on-failure",action="store_true"); args=ap.parse_args()
 results=[]
 for name,cmd in SUITES:
  r=run(name,cmd); results.append(r); print("[%s] %s"%(r["status"],name))
  if args.stop_on_failure and r["status"]=="FAIL": break
 passed=sum(r["status"]=="PASS" for r in results); failed=sum(r["status"]=="FAIL" for r in results)
 version=next((x.split(":",1)[1].strip() for x in VERSION.read_text().splitlines() if x.startswith("**SLeeLa:**")),"")
 record={"schemaVersion":1,"tool":"sleela-java-qualification-manifest","scope":"Java/SLeeLa source/API congruence; no JVM/SLVM requirement","sleeLaVersion":version,"syntaxVersion":"1.6","javaSpecification":"Java SE 27","host":{"os":platform.platform(),"system":platform.system(),"architecture":platform.machine(),"python":platform.python_version()},"results":results,"summary":{"total":len(SUITES),"executed":len(results),"passed":passed,"failed":failed,"status":"PASS" if failed==0 and len(results)==len(SUITES) else "FAIL"}}
 if args.json: args.json.write_text(json.dumps(record,indent=2,sort_keys=True)+"\n",encoding="utf-8")
 print("qualification: %s (%d/%d passed)"%(record["summary"]["status"],passed,len(results)))
 return 0 if record["summary"]["status"]=="PASS" else 1
if __name__=="__main__": raise SystemExit(main())
