#!/usr/bin/env python3
from pathlib import Path
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[1]
tool=ROOT/"tests/java_signature_comparison.py"
java=ROOT/"tests/java_equivalence/SignatureNormalization.java"
sleela=ROOT/"tests/java_equivalence/SignatureNormalization.sleela"

def main():
    with tempfile.TemporaryDirectory() as td:
        report=Path(td)/"signature.json"
        subprocess.run(["python3",str(tool),"--java",str(java),"--sleela",str(sleela),"--json",str(report)],check=True)
        text=report.read_text(encoding="utf-8")
        assert '"status": "PASS"' in text
    print("java-signature-comparison-suite: PASS")
if __name__=="__main__":
    main()
