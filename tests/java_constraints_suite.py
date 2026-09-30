#!/usr/bin/env python3
"""Deterministic negative/constraint corpus qualification."""
from pathlib import Path
import json
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
CORPUS = ROOT / "tests" / "java_constraints"
MANIFEST = CORPUS / "manifest.json"

def run(cmd):
    subprocess.run(cmd, cwd=ROOT, check=True)

def main():
    data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    assert data["schema"] == "sleela-java-constraint-corpus-1"
    for filename, diagnostic in data["fixtures"]:
        path = CORPUS / filename
        assert path.is_file(), f"missing fixture: {filename}"
        assert f"EXPECT: {diagnostic}" in path.read_text(encoding="utf-8")

    run(["python3", "tests/java_flow_suite.py"])
    run(["python3", "tests/java_exceptions_suite.py"])

    overload_out = ROOT / "tests" / ".java-constraints-overload"
    try:
        run([
            "c++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
            "-I" + str(ROOT / "impl" / "frontend"),
            str(ROOT / "tests" / "java_overload_override.cpp"),
            str(ROOT / "impl" / "frontend" / "java_equivalence.cpp"),
            "-o", str(overload_out),
        ])
        run([str(overload_out)])
    finally:
        if overload_out.exists():
            overload_out.unlink()

    tool = ROOT / "lib/java/tools/java_api_dependency_closure.py"
    with tempfile.TemporaryDirectory() as td:
        root = Path(td)
        (root / "lib/java/java/lang").mkdir(parents=True)
        (root / "lib/java/java/lang/String.sleela").write_text(
            "#sleela 1.3\nclass String {}\n", encoding="utf-8"
        )
        (root / "Example.sleela").write_text(
            "#sleela 1.3\njava.example.missing.Type value;\n", encoding="utf-8"
        )
        result = subprocess.run(
            ["python3", str(tool), "--check", "--root", str(root)],
            capture_output=True, text=True,
        )
        assert result.returncode != 0
        assert "java.example.missing.Type" in result.stdout

    print(f"java-constraints-suite: PASS ({len(data['fixtures'])} fixtures)")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
