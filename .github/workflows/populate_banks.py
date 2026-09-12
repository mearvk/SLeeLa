#!/usr/bin/env python3
"""Workflow entry point for national banking data population.

The implementation remains in tools/banking/populate_banks.py. This file makes
that Python program visible directly alongside the GitHub Actions workflow.
"""

from pathlib import Path
import runpy

SCRIPT = Path(__file__).resolve().parents[2] / "tools" / "banking" / "populate_banks.py"

if not SCRIPT.is_file():
    raise FileNotFoundError(f"Population script not found: {SCRIPT}")

runpy.run_path(str(SCRIPT), run_name="__main__")
