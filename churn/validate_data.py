#!/usr/bin/env python3
"""Parseability/completeness checks for the Churn data files; not source verification."""
import csv
import json
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parent
errors = []
checked = 0

for path in sorted(ROOT.rglob("*")):
    if not path.is_file() or "build" in path.relative_to(ROOT).parts:
        continue
    try:
        if path.suffix.lower() == ".json":
            with path.open(encoding="utf-8") as stream:
                value = json.load(stream)
            if not isinstance(value, (dict, list)):
                errors.append(f"{path.relative_to(ROOT)}: JSON root must be object or array")
            else:
                checked += 1
        elif path.suffix.lower() == ".xml":
            ET.parse(path)
            checked += 1
        elif path.suffix.lower() == ".csv":
            with path.open(encoding="utf-8-sig", newline="") as stream:
                rows = list(csv.reader(stream))
            if not rows or not any(cell.strip() for cell in rows[0]):
                errors.append(f"{path.relative_to(ROOT)}: missing header")
            elif any(len(row) != len(rows[0]) for row in rows[1:]):
                errors.append(f"{path.relative_to(ROOT)}: inconsistent column count")
            else:
                checked += 1
        elif path.suffix.lower() == ".txt":
            if not path.read_text(encoding="utf-8").strip():
                errors.append(f"{path.relative_to(ROOT)}: empty text file")
            else:
                checked += 1
    except (OSError, UnicodeError, json.JSONDecodeError, ET.ParseError, csv.Error) as exc:
        errors.append(f"{path.relative_to(ROOT)}: {type(exc).__name__}: {exc}")

if errors:
    print("Churn data validation: FAIL")
    print("\n".join("  - " + error for error in errors))
    sys.exit(1)
print(f"Churn data validation: PASS ({checked} JSON/XML/CSV/TXT file(s) checked)")
