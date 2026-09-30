#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]
tool=ROOT/"lib/java/tools/java_api_dependency_closure.py"
with tempfile.TemporaryDirectory() as td:
    root=Path(td); (root/"lib/java/java/lang").mkdir(parents=True)
    (root/"lib/java/java/lang/String.sleela").write_text("#sleela 1.3\nclass String {}\n")
    (root/"Example.sleela").write_text("#sleela 1.3\njava.lang.String value;\n")
    p=subprocess.run(["python3",str(tool),"--check","--root",str(root)],capture_output=True,text=True)
    if p.returncode!=0: raise SystemExit(p.stdout+p.stderr)
print("java-api-dependency-suite: PASS")
