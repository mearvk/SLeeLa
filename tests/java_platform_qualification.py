#!/usr/bin/env python3
"""Platform/reproducibility qualification record for Java/SLeeLa source work.

This tool verifies the platform matrix itself and emits a deterministic record
of the host environment. It does not claim that an unexecuted remote platform
has passed. Use --platform to validate a declared target record; use --json to
write the observed host record.

Java source/API qualification remains independent of JVM availability.
"""
import argparse
import json
import platform
import shutil
import subprocess
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MATRIX = Path(__file__).with_name("java_platforms.json")

def load():
    data = json.loads(MATRIX.read_text(encoding="utf-8"))
    assert data["schemaVersion"] == 1
    assert data["syntaxVersion"] == "1.6"
    assert data["javaSpecification"] == "Java SE 27"
    assert set(data["platforms"]) == {"linux", "windows-10-plus", "macos"}
    return data

def host_key():
    system = platform.system().lower()
    if system == "windows":
        return "windows-10-plus"
    if system == "darwin":
        return "macos"
    if system == "linux":
        return "linux"
    return system

def tool_state():
    return {
        "python": shutil.which("python3") or shutil.which("python") or "",
        "cxx": shutil.which("c++") or shutil.which("g++") or shutil.which("clang++") or "",
        "make": shutil.which("make") or "",
    }

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--platform", choices=["linux", "windows-10-plus", "macos"])
    ap.add_argument("--json", type=Path)
    args = ap.parse_args()

    matrix = load()
    selected = args.platform or host_key()
    if selected not in matrix["platforms"]:
        print("java-platform-qualification: UNSUPPORTED HOST")
        print("  host:", host_key())
        return 2

    record = {
        "schemaVersion": matrix["schemaVersion"],
        "status": "OBSERVED",
        "scope": matrix["scope"],
        "targetPlatform": selected,
        "hostPlatform": host_key(),
        "os": platform.platform(),
        "architecture": platform.machine(),
        "pythonVersion": platform.python_version(),
        "compiler": tool_state()["cxx"],
        "make": tool_state()["make"],
        "requiredTools": tool_state(),
        "declaredTarget": matrix["platforms"][selected],
        "executedHere": selected == host_key(),
        "timestampUtc": datetime.now(timezone.utc).isoformat(),
    }

    # The record is evidence of environment observation, not a remote pass.
    # A target is READY when its declared matrix is internally complete and the
    # host actually matches it; otherwise it remains NOT_EXECUTED.
    record["qualificationState"] = "READY" if record["executedHere"] else "NOT_EXECUTED"

    if args.json:
        args.json.write_text(json.dumps(record, indent=2, sort_keys=True) + "\n", encoding="utf-8")

    print("SLeeLa Java Platform Qualification")
    print("  target:", selected)
    print("  host:", record["hostPlatform"])
    print("  architecture:", record["architecture"])
    print("  state:", record["qualificationState"])
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
