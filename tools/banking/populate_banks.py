#!/usr/bin/env python3
"""Build the SLeeLa national banking/economic table from sourced data.

The canonical 391-country registry is an input, not an inferred list.
This prevents a different country-count convention from silently changing
the project's universe.
"""

import argparse
import json
import time
import urllib.parse
import urllib.request
from datetime import date
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_REGISTRY = ROOT / "data" / "banking" / "registry.json"
DEFAULT_OUTPUT = ROOT / "data" / "banking" / "national_banking.json"
DEFAULT_TABLE = ROOT / "BANKS.md"

WORLD_BANK = "https://api.worldbank.org/v2/country/{code}/indicator/{indicator}?format=json&per_page=5"

INDICATORS = {
    "gdp_usd": "NY.GDP.MKTP.CD",
    "gdp_per_capita_usd": "NY.GDP.PCAP.CD",
    "inflation_pct": "FP.CPI.TOTL.ZG",
    "unemployment_pct": "SL.UEM.TOTL.ZS",
    "trade_pct_gdp": "NE.TRD.GNFS.ZS",
}

def get_json(url):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-BANKS/1.0"})
    with urllib.request.urlopen(req, timeout=30) as response:
        return json.load(response)

def world_bank_value(code, indicator):
    try:
        data = get_json(WORLD_BANK.format(code=urllib.parse.quote(code), indicator=indicator))
        rows = data[1] if len(data) > 1 else []
        for row in rows:
            if row.get("value") is not None:
                return row["value"], row.get("date")
    except Exception:
        return None, None
    return None, None

def status(record):
    required = [
        record.get("registry_id"),
        record.get("country_name"),
        record.get("currency"),
        record.get("banking", {}).get("central_bank"),
        record.get("banking", {}).get("supervisor"),
    ]
    if all(x not in (None, "", "PENDING", "UNKNOWN") for x in required):
        return "COMPLETE"
    if any(x not in (None, "", "PENDING") for x in required):
        return "PARTIAL"
    return "MISSING"

def load_registry(path):
    data = json.loads(path.read_text(encoding="utf-8"))
    if isinstance(data, dict):
        data = data.get("countries", [])
    if len(data) != 391:
        raise SystemExit(f"ERROR: canonical registry contains {len(data)} records; expected exactly 391.")
    ids = [x["registry_id"] for x in data]
    if len(ids) != len(set(ids)):
        raise SystemExit("ERROR: duplicate registry_id detected.")
    return data

def build(registry):
    records = []
    for i, country in enumerate(registry, 1):
        code = country.get("iso_alpha2") or country.get("iso_alpha3")
        economic = {}
        if code:
            for field, indicator in INDICATORS.items():
                value, year = world_bank_value(code, indicator)
                economic[field] = {"value": value, "year": year, "source": "World Bank API"}
                time.sleep(0.05)

        record = {
            "registry_id": country["registry_id"],
            "country_name": country["country_name"],
            "iso_alpha2": country.get("iso_alpha2"),
            "iso_alpha3": country.get("iso_alpha3"),
            "state_formation": country.get("state_formation", {}),
            "banking": country.get("banking", {}),
            "economic_influence": country.get("economic_influence", {"global_level": "UNASSESSED", "regional_level": "UNASSESSED", "domestic_level": "UNASSESSED"}),
            "economy": country.get("economy", {}),
            "corporate_sector": country.get("corporate_sector", {}),
            "corporate_struggles": country.get("corporate_struggles", []),
            "economic_crises": country.get("economic_crises", []),
            "trade": country.get("trade", {}),
            "resources": country.get("resources", {}),
            "socialist_periods": country.get("socialist_periods", []),
            "economic_indicators": economic,
            "verification": {
                "last_verified": str(date.today()),
                "source_status": "PRIMARY_OR_INTERNATIONAL",
                "sources": ["https://data.worldbank.org/"],
            },
        }
        record["status"] = status(record)
        records.append(record)
        print(f"[{i}/391] {record['country_name']}: {record['status']}")
    return records

def markdown_table(records):
    lines = [
        "## 391-Country National Economic Table",
        "",
        "| ID | Country/Jurisdiction | Founded / State Formation | Economic Influence | Economy | Banking | Corporate Sector | Corporate Struggles | Socialist Years | Status |",
        "|---|---|---|---|---|---|---|---|---|---|",
    ]
    for r in records:
        founding = r.get("state_formation", {}).get("founding_date", "PENDING")
        influence = r.get("economic_influence", {}).get("global_level", "UNASSESSED")
        economy = r.get("economy", {}).get("classification", "PENDING")
        banking = r.get("banking", {}).get("central_bank", "PENDING")
        corporate = r.get("corporate_sector", {}).get("ownership", "PENDING")
        struggles = len(r.get("corporate_struggles", []))
        periods = r.get("socialist_periods", [])
        socialist = "; ".join(f"{p.get('start_year','?')}-{p.get('end_year','?')}" for p in periods) or "NONE / UNASSESSED"
        lines.append(f"| {r['registry_id']} | {r['country_name']} | {founding} | {influence} | {economy} | {banking} | {corporate} | {struggles} event(s) | {socialist} | {r['status']} |")
    return "\n".join(lines) + "\n"

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--registry", default=str(DEFAULT_REGISTRY))
    ap.add_argument("--output", default=str(DEFAULT_OUTPUT))
    ap.add_argument("--table", default=str(DEFAULT_TABLE))
    args = ap.parse_args()

    registry = load_registry(Path(args.registry))
    records = build(registry)

    out = Path(args.output)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({"generated": str(date.today()), "expected": 391, "records": records}, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    table = Path(args.table)
    existing = table.read_text(encoding="utf-8") if table.exists() else "# SLeeLa — BANKS.md\n"
    marker = "## 391-Country National Economic Table"
    if marker in existing:
        existing = existing.split(marker)[0].rstrip() + "\n\n"
    table.write_text(existing + markdown_table(records), encoding="utf-8")

    counts = {s: sum(r["status"] == s for r in records) for s in ("COMPLETE", "PARTIAL", "MISSING")}
    print(json.dumps({"expected": 391, **counts, "total": len(records), "ready": counts["COMPLETE"] == 391}, indent=2))

if __name__ == "__main__":
    main()
