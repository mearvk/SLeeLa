#!/usr/bin/env python3
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
source = (ROOT / "BANKS3.md").read_text(encoding="utf-8")
lines = source.splitlines()
header_i = next(i for i, line in enumerate(lines) if line.startswith("| ID | Country/Jurisdiction |"))
rows = []
for line in lines[header_i + 2:]:
    if not line.startswith("|"):
        if rows:
            break
        continue
    cells = [c.strip() for c in line.strip().strip("|").split("|")]
    if len(cells) == 11 and cells[0].isdigit():
        rows.append(cells)
if len(rows) != 391:
    raise SystemExit(f"Expected 391 rows, found {len(rows)}")
if [int(r[0]) for r in rows] != list(range(1, 392)):
    raise SystemExit("BANKS3 IDs are not 001-391")
if any(not r[9] for r in rows):
    raise SystemExit("Every row must have a Banking value")

out = [
    "# SLeeLa — BANKS4.md", "",
    "## HTTP 3.0 National Banking, Economic & Historical Registry — Banking-Enriched 391-Country Table", "",
    "This is the 391-row BANKS3 registry with the Banking field explicitly retained for every canonical country/jurisdiction.", "",
    "| ID | Country/Jurisdiction | ISO | Currency | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Constitutional Socialism Reference |",
    "|---:|---|---|---|---:|---:|---:|---:|---|---|---|",
]
for r in rows:
    out.append("| " + " | ".join(r) + " |")
out += ["", "### Validation", "", "- Expected rows: **391**.", "- Canonical join key: three-digit `ID`.", "- Banking field: column 10 from `BANKS3.md`."]
(ROOT / "BANKS4.md").write_text("\n".join(out) + "\n", encoding="utf-8")
print("BANKS4.md generated with 391 rows and Banking column")
