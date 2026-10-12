#!/usr/bin/env python3
"""Structural validation for Churn data files; this does not certify source accuracy."""
import csv
import json
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parent
errors = []
warnings = []
checked = 0
SUPPORTED_KINDS = {"EXACT", "MEASURED", "OBSERVED", "ASSUMED", "DERIVED"}


def check_json(path):
    with path.open(encoding="utf-8") as stream:
        value = json.load(stream)
    if not isinstance(value, (dict, list)):
        errors.append(f"{path}: JSON root must be object or array")
        return
    records = value if isinstance(value, list) else value.get("records")
    if isinstance(records, list) and all(isinstance(item, dict) for item in records):
        ids = [str(item["id"]).strip() for item in records
               if "id" in item and str(item["id"]).strip()]
        if len(ids) != len(set(ids)):
            errors.append(f"{path}: duplicate record id")
        for index, item in enumerate(records):
            if "evidence_kind" in item and item["evidence_kind"] not in SUPPORTED_KINDS:
                errors.append(f"{path}: record {index} has unknown evidence_kind")
            if "derived_from" in item and not isinstance(item["derived_from"], list):
                errors.append(f"{path}: record {index} derived_from must be an array")
            # Apply the shared contract when a dataset opts into it; legacy files
            # remain readable while migration proceeds deliberately.
            if value.get("record_contract") == "churn-1" if isinstance(value, dict) else False:
                required = ("id", "label", "dimension", "domain", "unit",
                            "evidence_kind", "source_uri")
                missing = [field for field in required if not item.get(field)]
                if missing:
                    errors.append(f"{path}: record {index} missing {', '.join(missing)}")
    if isinstance(value, dict) and value.get("record_contract") == "churn-1":
        if not value.get("source_uri"):
            errors.append(f"{path}: contract dataset requires source_uri")
        if not value.get("license"):
            errors.append(f"{path}: contract dataset requires license metadata")


for path in sorted(ROOT.rglob("*")):
    relative = path.relative_to(ROOT)
    if not path.is_file() or "build" in relative.parts or any(part.endswith("-out") for part in relative.parts):
        continue
    try:
        suffix = path.suffix.lower()
        if suffix == ".json":
            check_json(path)
            checked += 1
        elif suffix == ".xml":
            ET.parse(path)
            checked += 1
        elif suffix == ".csv":
            with path.open(encoding="utf-8-sig", newline="") as stream:
                rows = list(csv.reader(stream))
            if not rows or not any(cell.strip() for cell in rows[0]):
                errors.append(f"{relative}: missing header")
            elif len([c for c in rows[0] if c.strip()]) != len(set(c.strip() for c in rows[0] if c.strip())):
                errors.append(f"{relative}: duplicate column header")
            elif any(len(row) != len(rows[0]) for row in rows[1:]):
                errors.append(f"{relative}: inconsistent column count")
            elif any(not any(cell.strip() for cell in row) for row in rows[1:]):
                errors.append(f"{relative}: blank data row")
            else:
                id_columns = [i for i, name in enumerate(rows[0]) if name.strip().lower() == "id"]
                for column in id_columns:
                    ids = [row[column].strip() for row in rows[1:] if row[column].strip()]
                    if len(ids) != len(set(ids)):
                        errors.append(f"{relative}: duplicate id value")
                checked += 1
        elif suffix == ".txt":
            if not path.read_text(encoding="utf-8").strip():
                errors.append(f"{relative}: empty text file")
            else:
                checked += 1
    except (OSError, UnicodeError, json.JSONDecodeError, ET.ParseError, csv.Error) as exc:
        errors.append(f"{relative}: {type(exc).__name__}: {exc}")

if errors:
    print("Churn data validation: FAIL")
    print("\n".join("  - " + error for error in errors))
    sys.exit(1)
print(f"Churn data validation: PASS ({checked} JSON/XML/CSV/TXT file(s) checked)")
if warnings:
    print("\n".join("  - WARNING: " + warning for warning in warnings))
