#!/usr/bin/env python3
"""Run the SLeeLa-native Java String subset smoke program."""
from __future__ import annotations

import os
import pathlib
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
BINARY = pathlib.Path(os.environ.get("SLEELA_BIN", ROOT / "impl/build/sleela"))
SOURCE = ROOT / "tests/java_string_native.sleela"
EXPECTED = [
    "CONCAT=Hello World",
    "EQUALS=true",
    "EMPTY=true",
    "REPEAT=ababab",
    "JOIN=red, green, blue",
    "JOIN-EMPTY=",
    "NATIVE=SLeeLa-native/java.lang.String-subset",
    "UTF16=false",
]
DIAGNOSTIC_PREFIXES = (
    "[memory-manager]", "[security]", "[defender]", "verification: PASS (",
)


def main() -> int:
    if not BINARY.is_file():
        raise FileNotFoundError(f"SLeeLa executable not found: {BINARY}")
    result = subprocess.run(
        [str(BINARY), "run", str(SOURCE)],
        cwd=ROOT,
        env=os.environ.copy(),
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    output = [
        line for line in result.stdout.replace("\r\n", "\n").splitlines()
        if not line.startswith(DIAGNOSTIC_PREFIXES)
    ]
    while output and not output[-1].strip():
        output.pop()
    if result.returncode != 0 or output != EXPECTED:
        print("FAIL: SLeeLa-native Java String smoke test", file=sys.stderr)
        print(f"exit={result.returncode}", file=sys.stderr)
        print(f"expected={EXPECTED!r}", file=sys.stderr)
        print(f"actual={output!r}", file=sys.stderr)
        print(result.stdout, file=sys.stderr)
        return 1
    print(f"PASS: SLeeLa-native Java String subset ({len(EXPECTED)} output assertions)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
