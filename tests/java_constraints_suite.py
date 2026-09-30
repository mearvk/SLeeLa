#!/usr/bin/env python3
"""Deterministic negative/constraint corpus qualification.

The corpus is source-level. It does not require a JVM or bytecode executor.
The suite validates the fixture/diagnostic contract and exercises the existing
semantic qualification suites, plus a deliberate missing-API counterpart case.
"""
from pathlib import Path
import json
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
CORPUS = ROOT / "tests" / "java_constraints"
MANIFEST = CORPUS / "manifest.json"

def run(cmd):
    subprocess.run(cmd, cwd=ROOT, check=True)

def main():
    data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    assert data["schema"] == "sleela-java-constraint-corpus-1"
    assert data["fixtures"]

    for filename, diagnostic in data["fixtures"]:
        path = CORPUS / filename
        assert path.is_file(), f"missing fixture: {filename}"
        text = path.read_text(encoding="utf-8")
        assert f"EXPECT: {diagnostic}" in text, f"{filename}: missing EXPECT marker"

    # Existing semantic engines are the executable qualification targets for
    # the constraint families they currently model.
    run(["python3", "tests/java_flow_suite.py"])
    run(["python3", "tests/java_exceptions_suite.py"])
    run(["make", "-C", "tests", "java-overload"])

    tool = ROOT / "lib" / "java" / "tools" / "java_api_dependency_closure.py"
    with tempfile.TemporaryDirectory() as td:
        root = Path(td)
        (root / "lib/java/java/lang").mkdir(parents=True)
        (root / "lib/java/java/lang/String.sleela").write_text(
            "#sleela 1.3\nclass String {}\n", encoding="utf-8"
        )
        source = (root / "Example.sleela")
        source.write_text(
            "#sleela 1.3\njava.example.missing.Type value;\n",
            encoding="utf-8",
        )
        result = subprocess.run(
            ["python3", str(tool), "--check", "--root", str(root)],
            capture_output=True, text=True,
        )
        if result.returncode == 0:
            raise AssertionError("missing Java API counterpart was not diagnosed")
        if "java.example.missing.Type" not in result.stdout:
            raise AssertionError("missing counterpart name absent from diagnostic")

    print(f"java-constraints-suite: PASS ({len(data['fixtures'])} fixtures)")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
