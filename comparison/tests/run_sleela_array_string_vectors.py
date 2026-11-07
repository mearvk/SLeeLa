#!/usr/bin/env python3
"""Run the SCS-TEST-003 array/string subset supported by the SLeeLa VM."""
from __future__ import annotations

import json
import os
import pathlib
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
BINARY = pathlib.Path(os.environ.get("SLEELA_BIN", ROOT / "impl/build/sleela"))
CASES = [
    ("valid", "comparison/tests/sleela/array_string_valid.sleela", 0),
    ("negative-index", "comparison/tests/sleela/array_negative_index.sleela", 1),
    ("one-past-end", "comparison/tests/sleela/array_past_end.sleela", 1),
]
EXPECTED_VALID = [
    "ARRAY-EMPTY=0",
    "ARRAY-SINGLE=1",
    "ARRAY-FIRST=11",
    "ARRAY-LAST=33",
    "STRING-LITERAL-EQUALITY=PASS",
    "STRING-FIXTURE=USEFFUL",
]
# The runtime emits documented security and operational telemetry on stdout
# before the program's own output. Strip only those known diagnostic records.
DIAGNOSTIC_PREFIXES = (
    "[memory-manager]",
    "[security]",
    "[defender]",
    "verification: PASS (",
)
RESULT = {"suite": "SCS-TEST-003-sleela-vm", "cases": {}, "status": "FAIL"}


def program_lines(output: str) -> list[str]:
    lines = output.replace("\r\n", "\n").splitlines()
    return [
        line for line in lines
        if not line.startswith(DIAGNOSTIC_PREFIXES)
    ]


def main() -> int:
    if not BINARY.is_file():
        raise FileNotFoundError(f"SLeeLa executable not found: {BINARY}")
    env = os.environ.copy()
    results = {}
    for name, relative, expected_code in CASES:
        command = [str(BINARY), "run", str(ROOT / relative)]
        completed = subprocess.run(
            command, cwd=ROOT, env=env, text=True,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        )
        raw_output = completed.stdout.replace("\r\n", "\n").splitlines()
        actual_output = program_lines(completed.stdout)
        results[name] = {
            "command": command,
            "exit_code": completed.returncode,
            "output": raw_output,
            "program_output": actual_output,
        }
        if name == "valid":
            while actual_output and not actual_output[-1].strip():
                actual_output.pop()
            if completed.returncode != expected_code or actual_output != EXPECTED_VALID:
                raise AssertionError(
                    f"valid VM vectors failed: exit={completed.returncode}, "
                    f"expected={EXPECTED_VALID!r}, actual={actual_output!r}"
                )
            print("PASS SLeeLa VM arrays and USEFFUL string-literal checks: 6 assertions")
        else:
            if completed.returncode != expected_code or "array index out of range" not in completed.stdout:
                raise AssertionError(
                    f"{name} was not rejected as expected; exit={completed.returncode}, "
                    f"output={completed.stdout!r}"
                )
            print(f"PASS SLeeLa VM {name} rejection")
    RESULT["cases"] = results
    RESULT["status"] = "PASS"
    RESULT["platform"] = sys.platform
    RESULT["python"] = sys.version.split()[0]
    (ROOT / "comparison-sleela-array-string-results.json").write_text(
        json.dumps(RESULT, indent=2, ensure_ascii=True) + "\n", encoding="utf-8"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        RESULT["error"] = str(exc)
        (ROOT / "comparison-sleela-array-string-results.json").write_text(
            json.dumps(RESULT, indent=2, ensure_ascii=True) + "\n", encoding="utf-8"
        )
        print(f"FAIL SLeeLa VM array/string vectors: {exc}", file=sys.stderr)
        raise
