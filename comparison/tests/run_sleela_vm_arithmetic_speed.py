#!/usr/bin/env python3
"""Steady-state int/double arithmetic timings inside Basic Native and Premium SLVM 1-11."""
from __future__ import annotations
import json, os, pathlib, platform, re, statistics, subprocess, sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
CLI = pathlib.Path(os.environ.get("SLEELA_BIN", ROOT / "impl" / "build" / ("sleela.exe" if os.name == "nt" else "sleela")))
SOURCE = ROOT / "comparison" / "tests" / "sleela" / "arithmetic_speed.sleela"
OPS = ("INT_ADD", "INT_SUB", "INT_MUL", "INT_DIV",
       "DOUBLE_ADD", "DOUBLE_SUB", "DOUBLE_MUL", "DOUBLE_DIV")
CHECK_SCALE = 1_000_000  # Six decimal places, emitted as an exact integer.
EXPECTED = {"INT_ADD": 8888888, "INT_SUB": 7530865, "INT_MUL": 83810205,
            "INT_DIV": 10802469, "DOUBLE_ADD": 1_999_999_100,
            "DOUBLE_SUB": 7_530_865_100, "DOUBLE_MUL": 9_740_678_400,
            "DOUBLE_DIV": 10_802_469_000}
TIMING_RE = re.compile(r"(INT|DOUBLE)_(ADD|SUB|MUL|DIV)_NS=(\d+)")
CHECK_RE = re.compile(
    r"(INT|DOUBLE)_(ADD|SUB|MUL|DIV)_CHECK="
    r"([+-]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
)
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
    timings, checks = {}, {}
    for line in p.stdout.replace("\r\n", "\n").splitlines():
        line = line.strip()
        tm = TIMING_RE.fullmatch(line)
        if tm:
            key = tm.group(1) + "_" + tm.group(2)
            if key in timings:
                raise AssertionError(f"{label}: duplicate timing record {key}")
            timings[key] = int(tm.group(3))
            continue
        cm = CHECK_RE.fullmatch(line)
        if cm:
            key = cm.group(1) + "_" + cm.group(2)
            if key in checks:
                raise AssertionError(f"{label}: duplicate correctness record {key}")
            checks[key] = float(cm.group(3))
    missing_timings = sorted(set(OPS) - set(timings))
    missing_checks = sorted(set(OPS) - set(checks))
    if missing_timings:
        raise AssertionError(f"{label}: missing timing records {missing_timings}; output:\n{p.stdout}")
    if missing_checks:
        raise AssertionError(f"{label}: missing correctness records {missing_checks}; output:\n{p.stdout}")
    for op in OPS:
        ns = timings[op]
        if ns <= 0:
            raise AssertionError(f"{label}: non-positive timing for {op}: {ns}")
        expected = EXPECTED[op]
        # Floating values are scaled and rounded inside the SLeeLa workload,
        # then printed as integers. This avoids the runtime double-to-text %g
        # formatting limit while validating the result to 1e-6 precision.
        actual = int(checks[op])
        if actual != expected:
            raise AssertionError(
                f"{label}: {op} scaled checksum {actual} != {expected} "
                f"(scale {CHECK_SCALE}, 1e-6 resolution)"
            )
    return timings, checks, p.stdout


def main():
    if not CLI.is_file() or not SOURCE.is_file():
        raise FileNotFoundError(f"Missing SLeeLa CLI or arithmetic workload: {CLI}, {SOURCE}")
    targets = [("Basic Native", [str(CLI), "run", str(SOURCE)])]
    targets += [(f"Premium SLVM {n}", [str(CLI), "vm", "run", "premium", str(n), str(SOURCE)])
                for n in range(1, 12)]
    for label, command in targets:
        samples = {op: [] for op in OPS}
        checks_by_op = {op: [] for op in OPS}
        for _ in range(5):
            measured, checks, _ = invoke(label, command)
            for op in OPS:
                samples[op].append(measured[op])
                checks_by_op[op].append(checks[op])
        RESULT["targets"][label] = {
            op: {"samples_ns": values, "median_ns": statistics.median(values),
                 "median_ns_per_operation": statistics.median(values) / 250000,
                 "checksum": checks_by_op[op][0],
                 "checksum_samples": checks_by_op[op],
                 "status": "PASS"}
            for op, values in samples.items()
        }
        print(f"PASS {label}: 8 timing and checksum cases x 5 samples", flush=True)
    RESULT["status"] = "PASS"
    RESULT["counts"] = {"targets": 12, "premium_vms": 11, "types": 2,
                        "operators": 4, "measurements": 12 * 2 * 4 * 5,
                        "validated_checksums": 12 * 2 * 4 * 5}
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
