#!/usr/bin/env python3
"""Build the SLeeLa national banking/economic table from sourced data.

The canonical 391-country registry is an input, not an inferred list.  This
prevents a different country-count convention from silently changing the
project's universe.

The registry must therefore be populated separately before this program can
run.  This program validates the registry, preserves registry provenance,
and enriches each ISO-coded jurisdiction with World Bank indicators.
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

WORLD_BANK = (
    "https://api.worldbank.org/v2/country/{code}/indicator/{indicator}"
    "?format=json&per_page=20"
)

INDICATORS = {
    "gdp_usd": "NY.GDP.MKTP.CD",
    "gdp_per_capita_usd": "NY.GDP.PCAP.CD",
    "inflation_pct": "FP.CPI.TOTL.ZG",
    "unemployment_pct": "SL.UEM.TOTL.ZS",
    "trade_pct_gdp": "NE.TRD.GNFS.ZS",
}

REQUIRED_FIELDS = ("registry_id", "country_name")
REQUIRED_BANKING_FIELDS = ("central_bank", "supervisor")


def get_json(url, retries=3):
    """Fetch JSON with bounded retries for transient HTTP/network failures."""
    last_error = None
    for attempt in range(1, retries + 1):
        try:
            request = urllib.request.Request(
                url,
                headers={"User-Agent": "SLeeLa-BANKS/1.1"},
            )
            with urllib.request.urlopen(request, timeout=30) as response:
                return json.load(response)
        except Exception as exc:  # urllib exposes several network exception types.
            last_error = exc
            if attempt < retries:
                time.sleep(attempt)
    raise RuntimeError(f"World Bank request failed after {retries} attempts: {last_error}")


def world_bank_value(code, indicator):
    """Return the newest non-null World Bank value and its observation year."""
    try:
        data = get_json(
            WORLD_BANK.format(
                code=urllib.parse.quote(code),
                indicator=indicator,
            )
        )
        rows = data[1] if isinstance(data, list) and len(data) > 1 else []
        for row in rows:
            if row.get("value") is not None:
                return row["value"], row.get("date"), None
        return None, None, "No non-null World Bank observation returned."
    except Exception as exc:
        return None, None, str(exc)


def status(record):
    """Classify record completeness without treating UNKNOWN as complete."""
    banking = record.get("banking") or {}
    required = [
        record.get("registry_id"),
        record.get("country_name"),
        record.get("currency"),
        banking.get("central_bank"),
        banking.get("supervisor"),
    ]
    if all(x not in (None, "", "PENDING", "UNKNOWN") for x in required):
        return "COMPLETE"
    if any(x not in (None, "", "PENDING") for x in required):
        return "PARTIAL"
    return "MISSING"


def validate_registry(data):
    """Validate the canonical registry and provide actionable errors."""
    if isinstance(data, dict):
        expected = data.get("expected_count", 391)
        countries = data.get("countries", [])
    else:
        expected = 391
        countries = data

    if not isinstance(countries, list):
        raise SystemExit("ERROR: registry 'countries' must be a JSON array.")

    if expected != 391:
        raise SystemExit(
            f"ERROR: registry expected_count is {expected}; SLeeLa requires exactly 391."
        )

    if len(countries) != 391:
        raise SystemExit(
            "ERROR: canonical registry contains "
            f"{len(countries)} records; expected exactly 391. "
            "Populate data/banking/registry.json before running the population job."
        )

    ids = []
    errors = []
    for index, country in enumerate(countries, 1):
        if not isinstance(country, dict):
            errors.append(f"record {index}: expected an object")
            continue
        for field in REQUIRED_FIELDS:
            if not country.get(field):
                errors.append(f"record {index}: missing required field '{field}'")
        registry_id = country.get("registry_id")
        if registry_id:
            ids.append(registry_id)
        banking = country.get("banking", {})
        if banking is not None and not isinstance(banking, dict):
            errors.append(f"record {index}: 'banking' must be an object")
        for field in ("state_formation", "economic_influence", "economy", "corporate_sector"):
            value = country.get(field, {})
            if value is not None and not isinstance(value, dict):
                errors.append(f"record {index}: '{field}' must be an object")
        for field in ("corporate_struggles", "economic_crises", "socialist_periods"):
            value = country.get(field, [])
            if value is not None and not isinstance(value, list):
                errors.append(f"record {index}: '{field}' must be an array")

    if len(ids) != len(set(ids)):
        errors.append("duplicate registry_id detected")

    if errors:
        preview = "\n".join(f" - {error}" for error in errors[:20])
        suffix = "\n - ... additional validation errors omitted" if len(errors) > 20 else ""
        raise SystemExit(f"ERROR: registry validation failed:\n{preview}{suffix}")

    return countries


def load_registry(path):
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        raise SystemExit(f"ERROR: registry file not found: {path}")
    except json.JSONDecodeError as exc:
        raise SystemExit(f"ERROR: registry is not valid JSON: {exc}")
    return validate_registry(data)


def iso_code(country):
    """Prefer ISO alpha-3 because it is the stable World Bank country key."""
    return country.get("iso_alpha3") or country.get("iso_alpha2")


def build(registry):
    records = []
    for index, country in enumerate(registry, 1):
        code = iso_code(country)
        economic = {}

        if code:
            for field, indicator in INDICATORS.items():
                value, year, error = world_bank_value(code, indicator)
                entry = {
                    "value": value,
                    "year": year,
                    "source": "World Bank API",
                }
                if error:
                    entry["error"] = error
                economic[field] = entry
                time.sleep(0.05)
        else:
            for field in INDICATORS:
                economic[field] = {
                    "value": None,
                    "year": None,
                    "source": "World Bank API",
                    "error": "No ISO alpha-3 or alpha-2 code in registry.",
                }

        banking = country.get("banking") or {}
        currency = country.get("currency")
        if currency is None:
            currency = banking.get("currency")
        if currency is None:
            economy = country.get("economy") or {}
            currency = economy.get("currency")

        registry_sources = country.get("sources", [])
        if isinstance(registry_sources, str):
            registry_sources = [registry_sources]
        elif not isinstance(registry_sources, list):
            registry_sources = []

        sources = list(dict.fromkeys(registry_sources + ["https://data.worldbank.org/"]))

        record = {
            "registry_id": country["registry_id"],
            "country_name": country["country_name"],
            "iso_alpha2": country.get("iso_alpha2"),
            "iso_alpha3": country.get("iso_alpha3"),
            "currency": currency,
            "state_formation": country.get("state_formation") or {},
            "banking": banking,
            "economic_influence": country.get("economic_influence") or {
                "global_level": "UNASSESSED",
                "regional_level": "UNASSESSED",
                "domestic_level": "UNASSESSED",
            },
            "economy": country.get("economy") or {},
            "corporate_sector": country.get("corporate_sector") or {},
            "corporate_struggles": country.get("corporate_struggles") or [],
            "economic_crises": country.get("economic_crises") or [],
            "trade": country.get("trade") or {},
            "resources": country.get("resources") or {},
            "socialist_periods": country.get("socialist_periods") or [],
            "economic_indicators": economic,
            "verification": {
                "last_verified": str(date.today()),
                "source_status": "PRIMARY_OR_INTERNATIONAL",
                "sources": sources,
            },
        }
        record["status"] = status(record)
        records.append(record)
        print(f"[{index}/391] {record['country_name']}: {record['status']}")
    return records


def markdown_table(records):
    lines = [
        "## 391-Country National Economic Table",
        "",
        "| ID | Country/Jurisdiction | Founded / State Formation | Economic Influence | Economy | Banking | Corporate Sector | Corporate Struggles | Socialist Years | Status |",
        "|---|---|---|---|---|---|---|---|---|---|",
    ]
    for record in records:
        state_formation = record.get("state_formation") or {}
        influence_data = record.get("economic_influence") or {}
        economy_data = record.get("economy") or {}
        banking_data = record.get("banking") or {}
        corporate_data = record.get("corporate_sector") or {}
        founding = state_formation.get("founding_date", "PENDING")
        influence = influence_data.get("global_level", "UNASSESSED")
        economy = economy_data.get("classification", "PENDING")
        banking = banking_data.get("central_bank", "PENDING")
        corporate = corporate_data.get("ownership", "PENDING")
        struggles = len(record.get("corporate_struggles") or [])
        periods = record.get("socialist_periods") or []
        socialist = "; ".join(
            f"{period.get('start_year', '?')}-{period.get('end_year', '?')}"
            for period in periods
        ) or "NONE / UNASSESSED"
        lines.append(
            f"| {record['registry_id']} | {record['country_name']} | {founding} | "
            f"{influence} | {economy} | {banking} | {corporate} | "
            f"{struggles} event(s) | {socialist} | {record['status']} |"
        )
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--registry", default=str(DEFAULT_REGISTRY))
    parser.add_argument("--output", default=str(DEFAULT_OUTPUT))
    parser.add_argument("--table", default=str(DEFAULT_TABLE))
    args = parser.parse_args()

    registry = load_registry(Path(args.registry))
    records = build(registry)

    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(
        json.dumps(
            {
                "generated": str(date.today()),
                "expected": 391,
                "records": records,
            },
            indent=2,
            ensure_ascii=False,
        )
        + "\n",
        encoding="utf-8",
    )

    table = Path(args.table)
    existing = (
        table.read_text(encoding="utf-8")
        if table.exists()
        else "# SLeeLa — BANKS.md\n"
    )
    marker = "## 391-Country National Economic Table"
    if marker in existing:
        existing = existing.split(marker)[0].rstrip() + "\n\n"
    table.write_text(existing + markdown_table(records), encoding="utf-8")

    counts = {
        state: sum(record["status"] == state for record in records)
        for state in ("COMPLETE", "PARTIAL", "MISSING")
    }
    print(
        json.dumps(
            {
                "expected": 391,
                **counts,
                "total": len(records),
                "ready": counts["COMPLETE"] == 391,
            },
            indent=2,
        )
    )


if __name__ == "__main__":
    main()
