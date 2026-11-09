#!/usr/bin/env python3
"""Portable SLJIP test runner: compile Java endpoint, exercise it, test native codec."""
from __future__ import annotations

import os
import shlex
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
JAVA_DIR = ROOT / "connector/java/com/mearvk/sleela/connector"
JAVA_SOURCES = [
    JAVA_DIR / "SleelaClassLoader.java",
    JAVA_DIR / "SleelaJdkBridge.java",
    JAVA_DIR / "SleelaJdkProtocolServer.java",
    JAVA_DIR / "SleelaJdkProtocolFixture.java",
]
C_SOURCES = [
    ROOT / "runtime/java_interop_protocol.c",
    ROOT / "test-suites/test-java-protocol-codec.c",
]


def run(command: list[str]) -> None:
    print("+", " ".join(command), flush=True)
    subprocess.run(command, cwd=ROOT, check=True)


def main() -> int:
    javac = os.environ.get("JAVAC", "javac")
    java = os.environ.get("JAVA", "java")
    cc = shlex.split(os.environ.get("CC", "cc"))
    for tool in (javac, java, cc[0] if cc else ""):
        if not tool or shutil.which(tool) is None:
            print(f"ERROR: required tool not found: {tool}", file=sys.stderr)
            return 2

    with tempfile.TemporaryDirectory(prefix="sleela-sljip-") as temp:
        classes = Path(temp) / "classes"
        classes.mkdir()
        run([javac, "--release", "17", "-Xlint:all", "-d", str(classes),
             *map(str, JAVA_SOURCES)])
        run([java, "-version"])
        run([sys.executable, str(ROOT / "test-suites/test-java-protocol.py"), str(classes)])

        executable = Path(temp) / ("sljip-codec.exe" if os.name == "nt" else "sljip-codec")
        compiler_name = Path(cc[0]).name.lower()
        if os.name == "nt" and compiler_name in {"cl", "cl.exe"}:
            run(cc + ["/nologo", "/std:c11", "/W4", "/WX",
                      "/I" + str(ROOT / "runtime"),
                      "/Fo" + str(Path(temp)) + os.sep,
                      str(C_SOURCES[0]), str(C_SOURCES[1]),
                      "/Fe:" + str(executable)])
        else:
            run(cc + ["-std=c11", "-Wall", "-Wextra", "-Werror", "-pedantic",
                      "-I" + str(ROOT / "runtime"),
                      *map(str, C_SOURCES), "-o", str(executable)])
        run([str(executable)])
    print("SLJIP v1 protocol tests: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
