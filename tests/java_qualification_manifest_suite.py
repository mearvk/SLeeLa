#!/usr/bin/env python3
import json,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
tool=ROOT/"tests/java_qualification_manifest.py"
def main():
 with tempfile.TemporaryDirectory() as td:
  out=Path(td)/"qualification.json"; p=subprocess.run(["python3",str(tool),"--json",str(out)],cwd=ROOT,text=True,capture_output=True); data=json.loads(out.read_text()); assert data["schemaVersion"]==1; assert data["summary"]["total"]==8; assert len(data["results"])==8; assert p.returncode in (0,1)
 print("java-qualification-manifest-suite: PASS")
if __name__=="__main__": main()
