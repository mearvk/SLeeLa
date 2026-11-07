#!/usr/bin/env python3
"""Cross-check fixed-width integer and UTF-8/code-point vectors in C, C++, and Java."""
from __future__ import annotations

import json
import os
import pathlib
import shlex
import subprocess
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[2]
EXPECTED = [
    "INT32_MIN=-2147483648",
    "INT32_MAX=2147483647",
    "U32_WRAP=0",
    "SIGNED_DIV=-2",
    "SIGNED_REM=-1",
    "UTF8_BYTES=7",
    "UNICODE_CODEPOINTS=3",
]
RESULTS = {
    "suite": "SCS-TEST-002-numeric-unicode",
    "expected": EXPECTED,
    "implementations": {},
}


def run(command: list[str]) -> list[str]:
    result = subprocess.run(
        command, cwd=ROOT, check=True, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    )
    return result.stdout.replace("\r\n", "\n").strip().splitlines()


def version(command: list[str]) -> str:
    try:
        result = subprocess.run(
            command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT, check=False,
        )
        return result.stdout.splitlines()[0] if result.stdout else "unknown"
    except OSError as exc:
        return f"unavailable: {exc}"


def record(name: str, command: list[str]) -> None:
    actual = run(command)
    RESULTS["implementations"][name] = {
        "command": command,
        "actual": actual,
        "status": "PASS" if actual == EXPECTED else "FAIL",
    }
    if actual != EXPECTED:
        raise AssertionError(f"{name}: expected {EXPECTED!r}, got {actual!r}")
    print(f"PASS {name}: {len(actual)} edge-vector assertions")


def main() -> int:
    cc = shlex.split(os.environ.get("CC", "cc"))
    cxx = shlex.split(os.environ.get("CXX", "c++"))
    suffix = ".exe" if os.name == "nt" else ""
    RESULTS["toolchains"] = {
        "c": version(cc + ["--version"]),
        "cpp": version(cxx + ["--version"]),
        "java": version(["java", "-version"]),
        "javac": version(["javac", "-version"]),
    }
    RESULTS["python"] = sys.version.split()[0]
    RESULTS["platform"] = sys.platform

    with tempfile.TemporaryDirectory(prefix="sleela-edge-vectors-") as tmp:
        build = pathlib.Path(tmp)
        c_exe = build / f"edge-c{suffix}"
        cpp_exe = build / f"edge-cpp{suffix}"
        java_out = build / "java"
        run(cc + ["-std=c11", "-Wall", "-Wextra", "-Werror",
                  str(ROOT / "comparison/tests/native/edge_vectors.c"),
                  "-o", str(c_exe)])
        record("C11", [str(c_exe)])
        run(cxx + ["-std=c++17", "-Wall", "-Wextra", "-Werror",
                   str(ROOT / "comparison/tests/native/edge_vectors.cpp"),
                   "-o", str(cpp_exe)])
        record("C++17", [str(cpp_exe)])
        java_out.mkdir()
        run(["javac", "-encoding", "UTF-8", "-d", str(java_out),
             str(ROOT / "comparison/tests/java/EdgeVectors.java")])
        record("Java 21+", ["java", "-cp", str(java_out), "EdgeVectors"])

    RESULTS["status"] = "PASS"
    (ROOT / "comparison-edge-results.json").write_text(
        json.dumps(RESULTS, indent=2) + "\n", encoding="utf-8"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        RESULTS["status"] = "FAIL"
        RESULTS["error"] = str(exc)
        (ROOT / "comparison-edge-results.json").write_text(
            json.dumps(RESULTS, indent=2) + "\n", encoding="utf-8"
        )
        print(f"FAIL numeric/Unicode edge vectors: {exc}", file=sys.stderr)
        raise
