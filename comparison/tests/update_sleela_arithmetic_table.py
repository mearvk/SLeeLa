#!/usr/bin/env python3
"""Insert validated SLeeLa VM medians into the matching rows of ARITHMETIC.SPEED.001.md."""
from __future__ import annotations
import json, os, pathlib, sys
ROOT = pathlib.Path(__file__).resolve().parents[2]
RESULTS = ROOT / "comparison-sleela-vm-arithmetic-results.json"
REPORT = ROOT / "comparison" / "ARITHMETIC.SPEED.001.md"
TARGETS = ["Basic Native"] + [f"Premium SLVM {n}" for n in range(1, 12)]
OPS = {"ADD": "+", "SUB": "-", "MUL": "*", "DIV": "/"}
TYPE_MAP = {"INT": "i64", "DOUBLE": "f64"}

def main() -> int:
    data = json.loads(RESULTS.read_text(encoding="utf-8"))
    if data.get("status") != "PASS": raise SystemExit("VM arithmetic results are not PASS")
    targets = data.get("targets", {})
    if set(targets) != set(TARGETS): raise SystemExit(f"Expected exactly {TARGETS}; got {sorted(targets)}")
    lines = REPORT.read_text(encoding="utf-8").splitlines()
    updated = set()
    for index, line in enumerate(lines):
        if not line.startswith("|") or line.startswith("|---"): continue
        cells = [cell.strip() for cell in line.strip().strip("|").split("|")]
        if len(cells) != 17: continue
        row_type, operator = cells[0], cells[1].strip(chr(96))
        source_type = next((kind for kind, mapped in TYPE_MAP.items() if mapped == row_type), None)
        source_op = next((kind for kind, symbol in OPS.items() if symbol == operator), None)
        if source_type is None or source_op is None: continue
        key = f"{source_type}_{source_op}"
        for column, target in enumerate(TARGETS, start=5):
            record = targets[target].get(key)
            if not isinstance(record, dict) or record.get("status") != "PASS": raise SystemExit(f"Missing PASS result: {target} {key}")
            median, samples = record.get("median_ns_per_operation"), record.get("samples_ns")
            if not isinstance(median, (int, float)) or median <= 0 or not isinstance(samples, list) or len(samples) != 5: raise SystemExit(f"Invalid median/sample set: {target} {key}")
            cells[column] = f"{median:.3f}"
        lines[index] = "| " + " | ".join(cells) + " |"
        updated.add((row_type, operator))
    expected = {(kind, symbol) for kind in ("i64", "f64") for symbol in OPS.values()}
    if updated != expected: raise SystemExit(f"Expected to update {sorted(expected)}, updated {sorted(updated)}")
    marker = "### SLeeLa VM table completion — hosted CI"
    if marker not in lines:
        insert_at = next((i for i, line in enumerate(lines) if line.startswith("## 4. Coverage")), None)
        if insert_at is None: raise SystemExit("Could not find section 4 insertion point")
        run_id = os.environ.get("GITHUB_RUN_ID", "unknown")
        note = ["", marker, "", f"**Workflow:** [SLeeLa VM arithmetic table CI — run {run_id}](https://github.com/mearvk/SLeeLa/actions/runs/{run_id})", "",
                "- Basic Native and Premium SLVM/1–11 values are populated for i64 and f64 only.",
                "- SLeeLa int maps to i64: SLEELA.syntax section 21.1 specifies int, long, and int64 as 64-bit signed integers. double maps to f64.",
                "- Other fixed-width rows remain NOT MEASURED; no narrow signed/unsigned or float32 equivalence is inferred.",
                "- Values are the median of five hosted-CI samples, in ns/op. Raw samples and checksums are retained in the workflow artifact.", ""]
        lines[insert_at:insert_at] = note
    REPORT.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("PASS updated i64/f64 rows for Basic Native and Premium SLVM 1–11")
    return 0
if __name__ == "__main__": raise SystemExit(main())