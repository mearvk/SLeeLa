#!/usr/bin/env python3
"""Validate SLeeLa VM vector output while allowing documented runtime diagnostics."""
from __future__ import annotations

import pathlib
import sys

EXPECTED = ["42", "15", "hello"]


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: check_sleela_output.py OUTPUT_FILE", file=sys.stderr)
        return 2

    output_path = pathlib.Path(sys.argv[1])
    lines = output_path.read_text(encoding="utf-8").replace("\r\n", "\n").splitlines()
    while lines and not lines[-1].strip():
        lines.pop()

    actual = lines[-len(EXPECTED):] if len(lines) >= len(EXPECTED) else lines
    if actual != EXPECTED:
        print(f"FAIL SLeeLa VM vectors: expected trailing lines {EXPECTED!r}", file=sys.stderr)
        print(f"Actual output lines: {lines!r}", file=sys.stderr)
        return 1

    diagnostics = lines[:-len(EXPECTED)]
    if diagnostics:
        print(f"INFO SLeeLa runtime emitted {len(diagnostics)} diagnostic line(s) before program output")
    print("PASS SLeeLa VM: " + " | ".join(actual))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
