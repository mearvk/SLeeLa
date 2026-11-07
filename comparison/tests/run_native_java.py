#!/usr/bin/env python3
"""Compile and compare the shared SLeeLa/Java/C/C++ baseline vectors."""
from __future__ import annotations
import json, os, pathlib, shlex, subprocess, sys, tempfile

ROOT = pathlib.Path(__file__).resolve().parents[2]
EXPECTED = ["42", "15", "hello"]
RESULTS = {"suite": "SCS-TEST-001-baseline", "expected": EXPECTED, "implementations": {}}

def run(command):
    result = subprocess.run(command, cwd=ROOT, check=True, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return result.stdout.replace("\r\n", "\n").strip().splitlines()

def record(name, command):
    actual = run(command)
    RESULTS["implementations"][name] = {
        "command": command, "actual": actual,
        "status": "PASS" if actual == EXPECTED else "FAIL"
    }
    if actual != EXPECTED:
        raise AssertionError(f"{name}: expected {EXPECTED!r}, got {actual!r}")
    print(f"PASS {name}: {' | '.join(actual)}")

def main():
    cc = shlex.split(os.environ.get("CC", "cc"))
    cxx = shlex.split(os.environ.get("CXX", "c++"))
    suffix = ".exe" if os.name == "nt" else ""
    with tempfile.TemporaryDirectory(prefix="sleela-comparison-") as tmp:
        build = pathlib.Path(tmp)
        c_exe, cpp_exe, java_out = build / f"vectors-c{suffix}", build / f"vectors-cpp{suffix}", build / "java"
        run(cc + ["-std=c11", "-Wall", "-Wextra", "-Werror",
                  str(ROOT / "comparison/tests/native/common_vectors.c"), "-o", str(c_exe)])
        record("C11", [str(c_exe)])
        run(cxx + ["-std=c++17", "-Wall", "-Wextra", "-Werror",
                   str(ROOT / "comparison/tests/native/common_vectors.cpp"), "-o", str(cpp_exe)])
        record("C++17", [str(cpp_exe)])
        java_out.mkdir()
        run(["javac", "-encoding", "UTF-8", "-d", str(java_out),
             str(ROOT / "comparison/tests/java/CommonVectors.java")])
        record("Java", ["java", "-cp", str(java_out), "CommonVectors"])
    RESULTS["status"] = "PASS"
    RESULTS["python"] = sys.version.split()[0]
    RESULTS["platform"] = sys.platform
    (ROOT / "comparison-results.json").write_text(json.dumps(RESULTS, indent=2) + "\n", encoding="utf-8")
    return 0

if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        RESULTS["status"] = "FAIL"
        RESULTS["error"] = str(exc)
        (ROOT / "comparison-results.json").write_text(json.dumps(RESULTS, indent=2) + "\n", encoding="utf-8")
        print(f"FAIL comparison vectors: {exc}", file=sys.stderr)
        raise
