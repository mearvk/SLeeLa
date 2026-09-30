#!/usr/bin/env python3
"""Deterministic Java checked-exception qualification suite."""
from pathlib import Path
import subprocess
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/"tests"/".java-exceptions"
cmd=["c++","-std=c++17","-O2","-Wall","-Wextra","-pedantic","-I"+str(ROOT/"impl"/"frontend"),
     str(ROOT/"tests"/"java_exceptions.cpp"),str(ROOT/"impl"/"frontend"/"java_exceptions.cpp"),"-o",str(OUT)]
try:
    subprocess.run(cmd,check=True)
    subprocess.run([str(OUT)],check=True)
finally:
    if OUT.exists(): OUT.unlink()
print("java-exceptions-suite: PASS")
