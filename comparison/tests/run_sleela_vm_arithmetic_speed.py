#!/usr/bin/env python3
"""Steady-state int/double arithmetic timings inside Basic Native and Premium SLVM 1-11."""
from __future__ import annotations
import json, os, pathlib, platform, re, statistics, subprocess, sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
CLI = pathlib.Path(os.environ.get("SLEELA_BIN", ROOT / "impl" / "build" / ("sleela.exe" if os.name == "nt" else "sleela")))
SOURCE = ROOT / "comparison" / "tests" / "sleela" / "arithmetic_speed.sleela"
OPS = ("INT_ADD", "INT_SUB", "INT_MUL", "INT_DIV",
       "DOUBLE_ADD", "DOUBLE_SUB", "DOUBLE_MUL", "DOUBLE_DIV")
RESULT = {
    "suite": "SCS-PERF-SLEELA-VM-001", "status": "RUNNING",
    "host": {"os": platform.platform(), "machine": platform.machine(),
             "processor": platform.processor(), "logical_cores": os.cpu_count()},
    "measurement": {"iterations_per_case": 250000, "warmup_iterations": 50000,
                    "samples": 5, "clock": "timeMonotonicNanos",
                    "startup_and_admission_in_timed_region": False},
    "targets": {}
}
def invoke(label, command):
    p = subprocess.run(command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, timeout=240)
    if p.returncode:
        raise RuntimeError(f"{label} exited {p.returncode}:\n{p.stdout}")
    found = {}
    for line in p.stdout.replace("\r\n", "\n").splitlines():
        m = re.fullmatch(r"(INT|DOUBLE)_(ADD|SUB|MUL|DIV)_NS=(\d+)", line.strip())
        if m:
            found[m.group(1) + "_" + m.group(2)] = int(m.group(3))
    missing = sorted(set(OPS) - set(found))
    if missing:
        raise AssertionError(f"{label}: missing timing records {missing}; output:\n{p.stdout}")
    for op, ns in found.items():
        if ns <= 0:
            raise AssertionError(f"{label}: non-positive timing for {op}: {ns}")
    return found, p.stdout
def main():
    if not CLI.is_file() or not SOURCE.is_file():
        raise FileNotFoundError(f"Missing SLeeLa CLI or arithmetic workload: {CLI}, {SOURCE}")
    targets = [("Basic Native", [str(CLI), "run", str(SOURCE)])]
    targets += [(f"Premium SLVM {n}", [str(CLI), "vm", "run", "premium", str(n), str(SOURCE)])
                for n in range(1, 12)]
    for label, command in targets:
        samples = {op: [] for op in OPS}
        for _ in range(5):
            measured, _ = invoke(label, command)
            for op in OPS:
                samples[op].append(measured[op])
        RESULT["targets"][label] = {
            op: {"samples_ns": values, "median_ns": statistics.median(values),
                 "median_ns_per_operation": statistics.median(values) / 250000,
                 "status": "PASS"}
            for op, values in samples.items()
        }
        print(f"PASS {label}: 8 arithmetic cases x 5 samples", flush=True)
    RESULT["status"] = "PASS"
    RESULT["counts"] = {"targets": 12, "premium_vms": 11, "types": 2,
                        "operators": 4, "measurements": 12 * 2 * 4 * 5}
    out = ROOT / "comparison-sleela-vm-arithmetic-results.json"
    out.write_text(json.dumps(RESULT, indent=2) + "\n", encoding="utf-8")
    print("PASS SLeeLa VM arithmetic:", out.name)
    return 0
if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        RESULT["status"] = "FAIL"
        RESULT["error"] = str(exc)
        (ROOT / "comparison-sleela-vm-arithmetic-results.json").write_text(
            json.dumps(RESULT, indent=2) + "\n", encoding="utf-8")
        print(f"FAIL SLeeLa VM arithmetic: {exc}", file=sys.stderr)
        raise
