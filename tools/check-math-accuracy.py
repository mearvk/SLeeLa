#!/usr/bin/env python3
"""Check that SLeeLa's native `math` output is *indeed accurate*.

Unlike the in-tree ``impl/tests/subjects/math_values.sleela`` harness -- which
compares SLeeLa against reference constants that are themselves hard-coded in
the ``.sleela`` source -- this checker computes every reference value
*independently in Python* (the ``math`` module and plain arithmetic) and
compares it against what the SLeeLa runtime actually prints.

How it works:
  1. It generates a small ``.sleela`` program that evaluates a battery of
     ``math.*`` operations and prints each as ``name=<value>``.
  2. It runs that program through the built ``sleela`` binary.
  3. It parses the ``name=<value>`` lines and, for each case, compares the
     runtime's value to the Python-computed reference.

Because SLeeLa's ``print`` renders doubles at roughly six significant figures,
the comparison uses a *relative* tolerance (default 1e-5) with an absolute
floor for values near zero. Pass ``--tolerance`` to tighten or loosen it.

Exit status:
  0  every case is accurate within tolerance
  1  one or more cases are inaccurate (details printed)
  2  setup/usage error (binary not found, program failed to run, etc.)

Usage:
  python3 tools/check-math-accuracy.py                    # auto-locate the binary
  python3 tools/check-math-accuracy.py --sleela impl/build/sleela
  python3 tools/check-math-accuracy.py --tolerance 1e-4 --verbose
"""
from __future__ import annotations

import argparse
import math
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path


# ---------------------------------------------------------------------------
# The battery of cases. Each case is:
#   (name, sleela_expression, python_reference_callable)
# `name` is the token printed by the generated program and used to match the
# output line; the two sides are deliberately written independently so the
# reference is a genuine cross-check, not a copy of a literal.
# ---------------------------------------------------------------------------
def build_cases() -> list[tuple[str, str, float]]:
    cases = [
        ("sqrt2",        "math.sqrt(2.0)",              math.sqrt(2.0)),
        ("sqrt_144",     "math.sqrt(144.0)",            math.sqrt(144.0)),
        ("pow_2_10",     "math.pow(2.0, 10.0)",         math.pow(2.0, 10.0)),
        ("pow_frac",     "math.pow(9.0, 0.5)",          math.pow(9.0, 0.5)),
        ("exp_0",        "math.exp(0.0)",               math.exp(0.0)),
        ("exp_1",        "math.exp(1.0)",               math.exp(1.0)),
        ("exp_3",        "math.exp(3.0)",               math.exp(3.0)),
        ("log_e",        "math.log(2.718281828459045)", math.log(math.e)),
        ("log_1000",     "math.log(1000.0)",            math.log(1000.0)),
        ("log10_1000",   "math.log10(1000.0)",          math.log10(1000.0)),
        ("log10_82100",  "math.log10(82100.0)",         math.log10(82100.0)),
        ("sin_0",        "math.sin(0.0)",               math.sin(0.0)),
        ("sin_1",        "math.sin(1.0)",               math.sin(1.0)),
        ("sin_pi_half",  "math.sin(1.5707963267948966)", math.sin(math.pi / 2)),
        ("cos_0",        "math.cos(0.0)",               math.cos(0.0)),
        ("cos_1",        "math.cos(1.0)",               math.cos(1.0)),
        ("hypot_3_4",    "math.hypot(3.0, 4.0)",        math.hypot(3.0, 4.0)),
        ("hypot_5_12",   "math.hypot(5.0, 12.0)",       math.hypot(5.0, 12.0)),
        ("fmod_7p5_2",   "math.fmod(7.5, 2.0)",         math.fmod(7.5, 2.0)),
        ("fmod_10_3",    "math.fmod(10.0, 3.0)",        math.fmod(10.0, 3.0)),
        ("abs_neg",      "math.abs(-7.25)",             abs(-7.25)),
        ("pi",           "math.pi()",                   math.pi),
        ("floor_pos",    "math.floor(3.7)",             math.floor(3.7)),
        ("floor_neg",    "math.floor(-3.2)",            math.floor(-3.2)),
        ("ceil_pos",     "math.ceil(3.2)",              math.ceil(3.2)),
        ("ceil_neg",     "math.ceil(-3.7)",             math.ceil(-3.7)),
        ("trunc_pos",    "math.trunc(3.7)",             math.trunc(3.7)),
        ("trunc_neg",    "math.trunc(-3.7)",            math.trunc(-3.7)),
        ("min",          "math.min(3.0, 8.0)",          min(3.0, 8.0)),
        ("max",          "math.max(3.0, 8.0)",          max(3.0, 8.0)),
        ("clamp_hi",     "math.clamp(15.0, 0.0, 10.0)", 10.0),
        ("clamp_lo",     "math.clamp(-5.0, 0.0, 10.0)", 0.0),
    ]
    return cases


def generate_program(cases: list[tuple[str, str, float]]) -> str:
    lines = ["#sleela 1.0", "import math;", "class MathAccuracy {", "    void main() {"]
    for name, expr, _ref in cases:
        lines.append(f'        print("{name}=" + {expr});')
    lines.append("    }")
    lines.append("}")
    return "\n".join(lines) + "\n"


def locate_sleela(root: Path, override: str | None) -> Path | None:
    if override:
        p = Path(override)
        return p if p.is_file() and os.access(p, os.X_OK) else None
    for cand in (root / "impl" / "build" / "sleela",
                 root / "impl" / "build" / "sleela.exe",
                 root / "bin" / "sleela"):
        if cand.is_file() and os.access(cand, os.X_OK):
            return cand
    return None


def run_program(sleela: Path, program_path: Path, root: Path) -> tuple[int, str, str]:
    env = dict(os.environ)
    env.setdefault("SLEELA_SHEET", str(root / "SHEET.sheet"))
    env.setdefault("SLEELA_SHA256_MANIFEST", str(root / "security" / "sha256-manifest.json"))
    proc = subprocess.run(
        [str(sleela), "run", str(program_path)],
        capture_output=True, text=True, env=env, cwd=str(root),
    )
    return proc.returncode, proc.stdout, proc.stderr


VALUE_LINE = re.compile(r"^([A-Za-z0-9_]+)=(-?[0-9]+(?:\.[0-9]+)?(?:[eE][-+]?[0-9]+)?)\s*$")


def parse_values(stdout: str) -> dict[str, float]:
    out: dict[str, float] = {}
    for line in stdout.splitlines():
        m = VALUE_LINE.match(line.strip())
        if m:
            try:
                out[m.group(1)] = float(m.group(2))
            except ValueError:
                pass
    return out


def is_accurate(got: float, ref: float, rel_tol: float, abs_floor: float) -> tuple[bool, float]:
    diff = abs(got - ref)
    # Relative tolerance with an absolute floor so values near zero don't
    # divide by ~0. The floor also absorbs SLeeLa's ~6-sig-fig print rounding
    # for small magnitudes.
    allowed = max(abs_floor, rel_tol * abs(ref))
    return diff <= allowed, diff


def main() -> int:
    parser = argparse.ArgumentParser(description="Check that SLeeLa's math output is accurate.")
    parser.add_argument("--sleela", help="Path to the built sleela binary (default: auto-locate).")
    parser.add_argument("--root", type=Path, default=None,
                        help="Repository root (default: inferred from this script's location).")
    parser.add_argument("--tolerance", type=float, default=1e-5,
                        help="Relative tolerance for the comparison (default: 1e-5).")
    parser.add_argument("--abs-floor", type=float, default=1e-6,
                        help="Absolute tolerance floor for values near zero (default: 1e-6).")
    parser.add_argument("--verbose", action="store_true", help="Print every case, not just failures.")
    args = parser.parse_args()

    root = (args.root or Path(__file__).resolve().parent.parent).resolve()

    sleela = locate_sleela(root, args.sleela)
    if not sleela:
        print("check-math-accuracy: sleela binary not found or not executable.", file=sys.stderr)
        print("  Build it first:  make -C impl", file=sys.stderr)
        print("  Or pass --sleela /path/to/sleela", file=sys.stderr)
        return 2

    cases = build_cases()
    program = generate_program(cases)

    with tempfile.TemporaryDirectory() as td:
        prog_path = Path(td) / "math_accuracy_probe.sleela"
        prog_path.write_text(program, encoding="utf-8")
        rc, stdout, stderr = run_program(sleela, prog_path, root)

    if rc != 0:
        print(f"check-math-accuracy: the SLeeLa program failed to run (exit {rc}).", file=sys.stderr)
        if stderr.strip():
            print("  stderr:", file=sys.stderr)
            for line in stderr.strip().splitlines():
                print("    " + line, file=sys.stderr)
        return 2

    values = parse_values(stdout)

    print(f"=== SLeeLa math accuracy (binary: {sleela}) ===")
    print(f"    reference: Python math module | relative tolerance: {args.tolerance:g} "
          f"(abs floor {args.abs_floor:g})")

    failures = 0
    missing = 0
    for name, expr, ref in cases:
        if name not in values:
            print(f"  MISSING {name:14s}  (no '{name}=' line in output; expr={expr})")
            missing += 1
            continue
        got = values[name]
        ok, diff = is_accurate(got, ref, args.tolerance, args.abs_floor)
        if not ok:
            print(f"  INACCURATE {name:14s} got={got!r} reference={ref!r} |diff|={diff:g}")
            failures += 1
        elif args.verbose:
            print(f"  ok  {name:14s} got={got!r} reference={ref:.6g} |diff|={diff:g}")

    total = len(cases)
    checked = total - missing
    print(f"--- {checked}/{total} cases evaluated; "
          f"{checked - failures} accurate, {failures} inaccurate, {missing} missing ---")

    if failures or missing:
        print("MATH ACCURACY: FAIL")
        return 1
    print("MATH ACCURACY: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
