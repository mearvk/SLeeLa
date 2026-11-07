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
    "UTF8-ROUNDTRIP=PASS",
    "UTF8-FIXTURE=Aé😀",
]
RESULT = {"suite": "SCS-TEST-003-sleela-vm", "cases": {}, "status": "FAIL"}


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
        output = completed.stdout.replace("\r\n", "\n").splitlines()
        results[name] = {
            "command": command,
            "exit_code": completed.returncode,
            "output": output,
        }
        if name == "valid":
            while output and not output[-1].strip():
                output.pop()
            actual = output[-len(EXPECTED_VALID):] if len(output) >= len(EXPECTED_VALID) else output
            if completed.returncode != 0 or actual != EXPECTED_VALID:
                raise AssertionError(
                    f"valid VM vectors failed: exit={completed.returncode}, "
                    f"expected={EXPECTED_VALID!r}, actual={actual!r}"
                )
            fixture = "Aé😀".encode("utf-8")
            if len(fixture) != 7 or len("Aé😀") != 3:
                raise AssertionError("host-side UTF-8 fixture invariant failed")
            print("PASS SLeeLa VM valid arrays and UTF-8 literal round-trip: 6 assertions")
        else:
            if completed.returncode == 0 or "array index out of range" not in completed.stdout:
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
        json.dumps(RESULT, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        RESULT["error"] = str(exc)
        (ROOT / "comparison-sleela-array-string-results.json").write_text(
            json.dumps(RESULT, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
        )
        print(f"FAIL SLeeLa VM array/string vectors: {exc}", file=sys.stderr)
        raise
