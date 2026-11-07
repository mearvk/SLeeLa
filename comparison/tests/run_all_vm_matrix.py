#!/usr/bin/env python3
"""Run the Basic Native VM and all eleven runnable Premium SLVMs."""
from __future__ import annotations
import json, pathlib, subprocess, sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
CLI = ROOT / "impl" / "build" / ("sleela.exe" if sys.platform == "win32" else "sleela")
SOURCE = ROOT / "comparison" / "tests" / "sleela" / "common_vectors.sleela"
EXPECTED = ["42", "15", "hello"]
RESULT = {"suite": "SCS-VM-MATRIX-001", "status": "PASS",
          "expected_output": EXPECTED, "standard_vms": [], "runnable_vms": []}

def run(label, command):
    p = subprocess.run(command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, check=True, timeout=180)
    lines = [x.strip() for x in p.stdout.replace("\r\n", "\n").splitlines() if x.strip()]
    actual = lines[-len(EXPECTED):]
    if actual != EXPECTED:
        raise AssertionError(f"{label}: expected {EXPECTED!r}, got {actual!r}\n{p.stdout}")
    RESULT["runnable_vms"].append({"target": label, "status": "PASS", "output": actual})
    print(f"PASS {label}: {' | '.join(actual)}", flush=True)

def main():
    if not CLI.is_file() or not SOURCE.is_file():
        raise FileNotFoundError("Native CLI or shared workload is missing")
    for n in range(1, 11):
        folder = ROOT / "virtual-machine" / "vm-version" / f"{n:03d}"
        manifest = folder / "MODIUM.json"
        source = folder / f"SLVmVersion{n:03d}.sleela"
        if not manifest.is_file() or not source.is_file():
            raise FileNotFoundError(f"Standard VM {n:03d} package is incomplete")
        RESULT["standard_vms"].append({"generation": n, "identity": f"idea-vm-{n:03d}",
            "status": "REGISTERED_NOT_BOOTABLE"})
    print("PASS Standard VM inventory: 10/10 registered; execution gated", flush=True)
    run("Basic Native VM", [str(CLI), "run", str(SOURCE)])
    for n in range(1, 12):
        run(f"Premium SLVM/{n}", [str(CLI), "vm", "run", "premium", str(n), str(SOURCE)])
    RESULT["counts"] = {"standard_registered_not_bootable": 10, "runnable_executed": 12,
                        "premium_executed": 11, "basic_native_executed": 1}
    (ROOT / "comparison-vm-matrix-results.json").write_text(
        json.dumps(RESULT, indent=2) + "\n", encoding="utf-8")
    print("PASS VM matrix: 12/12 runnable targets; 10 Standard entries inventoried")

if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        RESULT["status"] = "FAIL"
        RESULT["error"] = str(exc)
        (ROOT / "comparison-vm-matrix-results.json").write_text(
            json.dumps(RESULT, indent=2) + "\n", encoding="utf-8")
        print(f"FAIL VM matrix: {exc}", file=sys.stderr)
        raise
