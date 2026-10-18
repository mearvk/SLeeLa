#!/usr/bin/env python3
"""Generate BANKS3.md by joining the canonical 391-row BANKS table with BANKS2 currency data."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
BANKS = ROOT / "BANK.INFORMATION.PANAMANIAN.001.md"
BANKS2 = ROOT / "BANK.INFORMATION.PANAMANIAN.002.md"
OUT = ROOT / "BANKS3.md"

ROW_RE = re.compile(r"^\|\s*(\d{3})\s*\|\s*([^|]+?)\s*\|\s*(.*?)\s*\|$")

def read_currency():
    text = BANKS2.read_text(encoding="utf-8")
    currency = {}
    for line in text.splitlines():
        m = ROW_RE.match(line)
        if m:
            currency[m.group(1)] = m.group(3).strip()
    return currency

def read_banks_rows():
    text = BANKS.read_text(encoding="utf-8")
    marker = "## 391-Country National Economic Table"
    start = text.find(marker)
    if start < 0:
        raise RuntimeError("BANK.INFORMATION.PANAMANIAN.001.md: 391-Country National Economic Table not found")
    section = text[start:]
    rows = {}
    for line in section.splitlines():
        if not line.startswith("|") or line.startswith("|---"):
            continue
        parts = [p.strip() for p in line.strip().strip("|").split("|")]
        if len(parts) != 11 or not parts[0].isdigit():
            continue
        rid = parts[0].zfill(3)
        rows[rid] = parts
    if len(rows) != 391:
        raise RuntimeError(f"BANK.INFORMATION.PANAMANIAN.001.md: expected 391 country rows, found {len(rows)}")
    return rows

def main():
    currency = read_currency()
    rows = read_banks_rows()
    missing = [rid for rid in rows if rid not in currency]
    if missing:
        raise RuntimeError(f"BANK.INFORMATION.PANAMANIAN.002.md: missing currency rows: {', '.join(missing)}")

    out = [
        "# SLeeLa — BANKS3.md",
        "",
        "## HTTP 3.0 National Banking, Economic & Historical Registry — Combined 391-Country Table",
        "",
        "This is the materialized union of `BANK.INFORMATION.PANAMANIAN.001.md` and `BANK.INFORMATION.PANAMANIAN.002.md`, joined by the canonical three-digit registry ID and country/jurisdiction. Economic, banking and status fields come from `BANK.INFORMATION.PANAMANIAN.001.md`; Currency comes from `BANK.INFORMATION.PANAMANIAN.002.md`.",
        "",
        "| ID | Country/Jurisdiction | ISO | Currency | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |",
        "|---:|---|---|---|---:|---:|---:|---:|---|---|---|",
    ]
    for rid in sorted(rows, key=lambda x: int(x)):
        p = rows[rid]
        p[3] = currency[rid]
        out.append("| " + " | ".join(p) + " |")
    out += [
        "",
        "### Join Contract",
        "",
        "- Expected rows: **391**.",
        "- Join key: canonical three-digit `ID`.",
        "- Country/Jurisdiction and all non-currency economic fields: `BANK.INFORMATION.PANAMANIAN.001.md`.",
        "- Currency: `BANK.INFORMATION.PANAMANIAN.002.md`.",
        "- Historical entities retain `N/A` where no current ISO 4217 assignment applies.",
    ]
    OUT.write_text("\n".join(out) + "\n", encoding="utf-8")
    print(f"Generated BANKS3.md with {len(rows)} rows")

if __name__ == "__main__":
    main()
