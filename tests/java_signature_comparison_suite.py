#!/usr/bin/env python3
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
tool = ROOT / "tests/java_signature_comparison.py"
java = ROOT / "tests/java_equivalence/SignatureNormalization.java"
sleela = ROOT / "tests/java_equivalence/SignatureNormalization.sleela"

def run(a, b):
    with tempfile.TemporaryDirectory() as td:
        report = Path(td) / "signature.json"
        return subprocess.run(
            ["python3", str(tool), "--java", str(a), "--sleela", str(b), "--json", str(report)],
            text=True, capture_output=True
        )

def main():
    result = run(java, sleela)
    assert result.returncode == 0, result.stdout + result.stderr

    with tempfile.TemporaryDirectory() as td:
        broken = Path(td) / "broken.sleela"
        broken.write_text(sleela.read_text(encoding="utf-8").replace(
            "public SignatureNormalization(T value)",
            "public SignatureNormalization(String value)"
        ), encoding="utf-8")
        mismatch = run(java, broken)
        assert mismatch.returncode == 1
        assert "MISMATCH" in mismatch.stdout

    print("java-signature-comparison-suite: PASS")

if __name__ == "__main__":
    main()
