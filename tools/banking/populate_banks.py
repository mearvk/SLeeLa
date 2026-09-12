#!/usr/bin/env python3
"""Build the SLeeLa national banking/economic table from sourced data.

The canonical 391-country registry is an input, not an inferred list. This
program enriches that registry remotely using bulk public international data:
World Bank country metadata and indicators plus REST Countries ISO/currency
metadata. It deliberately does not invent central banks, supervisors,
political classifications, or historical facts when a source is unavailable.
"""

import argparse
import json
import re
import time
import urllib.parse
import urllib.request
from datetime import date
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_REGISTRY = ROOT / "data" / "banking" / "registry.json"
DEFAULT_OUTPUT = ROOT / "data" / "banking" / "national_banking.json"
DEFAULT_TABLE = ROOT / "BANKS.md"

EXPECTED = 391
WB_BASE = "https://api.worldbank.org/v2"
REST_COUNTRIES = "https://restcountries.com/v3.1/all?fields=name,cca2,cca3,currencies,region,subregion,capital"

INDICATORS = {
    "gdp_usd": "NY.GDP.MKTP.CD",
    "gdp_per_capita_usd": "NY.GDP.PCAP.CD",
    "inflation_pct": "FP.CPI.TOTL.ZG",
    "unemployment_pct": "SL.UEM.TOTL.ZS",
    "trade_pct_gdp": "NE.TRD.GNFS.ZS",
}

REQUIRED_FIELDS = ("registry_id", "country_name")

# A small, explicit alias set handles common naming differences without making
# fuzzy guesses. Unmatched entries remain unassessed rather than being forced.
ALIASES = {
    "bolivia": "bolivia, plurinational state of",
    "brunei": "brunei darussalam",
    "cape verde": "cabo verde",
    "congo": "congo, republic of the",
    "czech republic": "czechia",
    "democratic republic of the congo": "congo, the democratic republic of the",
    "iran": "iran, islamic republic of",
    "laos": "lao people's democratic republic",
    "moldova": "moldova, republic of",
    "north korea": "korea, democratic people's republic of",
    "russia": "russian federation",
    "south korea": "korea, republic of",
    "syria": "syrian arab republic",
    "tanzania": "tanzania, united republic of",
    "venezuela": "venezuela, bolivarian republic of",
    "vietnam": "viet nam",
}


def get_json(url, retries=3):
    """Fetch JSON with bounded retries for transient network failures."""
    last_error = None
    for attempt in range(1, retries + 1):
        try:
            request = urllib.request.Request(
                url,
                headers={"User-Agent": "SLeeLa-BANKS/2.0 (+https://github.com/mearvk/SLeeLa)"},
            )
            with urllib.request.urlopen(request, timeout=45) as response:
                return json.load(response)
        except Exception as exc:
            last_error = exc
            if attempt < retries:
                time.sleep(attempt)
    raise RuntimeError(f"Remote JSON request failed after {retries} attempts: {last_error}")


def normalize_name(value):
    value = value.lower().strip()
    value = value.replace("&", " and ")
    value = re.sub(r"[^a-z0-9]+", " ", value)
    return re.sub(r"\s+", " ", value).strip()


def validate_registry(data):
    if isinstance(data, dict):
        expected = data.get("expected_count", EXPECTED)
        countries = data.get("countries", [])
    else:
        expected = EXPECTED
        countries = data

    if expected != EXPECTED:
        raise SystemExit(f"ERROR: registry expected_count is {expected}; SLeeLa requires exactly {EXPECTED}.")
    if not isinstance(countries, list) or len(countries) != EXPECTED:
        count = len(countries) if isinstance(countries, list) else 0
        raise SystemExit(f"ERROR: canonical registry contains {count} records; expected exactly {EXPECTED}.")

    ids = []
    errors = []
    for index, country in enumerate(countries, 1):
        if not isinstance(country, dict):
            errors.append(f"record {index}: expected an object")
            continue
        for field in REQUIRED_FIELDS:
            if not country.get(field):
                errors.append(f"record {index}: missing required field '{field}'")
        if country.get("registry_id"):
            ids.append(country["registry_id"])
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
        raise SystemExit(f"ERROR: registry validation failed:\n{preview}")
    return countries


def load_registry(path):
    try:
        return validate_registry(json.loads(path.read_text(encoding="utf-8")))
    except FileNotFoundError:
        raise SystemExit(f"ERROR: registry file not found: {path}")
    except json.JSONDecodeError as exc:
        raise SystemExit(f"ERROR: registry is not valid JSON: {exc}")


def fetch_world_bank_countries():
    url = f"{WB_BASE}/country?format=json&per_page=400"
    data = get_json(url)
    rows = data[1] if isinstance(data, list) and len(data) > 1 else []
    by_alpha2 = {}
    by_alpha3 = {}
    by_name = {}
    for row in rows:
        a2 = (row.get("iso2Code") or "").upper()
        a3 = (row.get("id") or "").upper()
        name = normalize_name(row.get("name", ""))
        if a2:
            by_alpha2[a2] = row
        if a3:
            by_alpha3[a3] = row
        if name:
            by_name[name] = row
    return {"alpha2": by_alpha2, "alpha3": by_alpha3, "name": by_name}


def fetch_rest_countries():
    data = get_json(REST_COUNTRIES)
    if not isinstance(data, list):
        return {"alpha2": {}, "alpha3": {}, "name": {}}
    by_alpha2 = {}
    by_alpha3 = {}
    by_name = {}
    for row in data:
        a2 = (row.get("cca2") or "").upper()
        a3 = (row.get("cca3") or "").upper()
        names = row.get("name") or {}
        candidates = [names.get("common"), names.get("official")]
        if a2:
            by_alpha2[a2] = row
        if a3:
            by_alpha3[a3] = row
        for candidate in candidates:
            if candidate:
                by_name[normalize_name(candidate)] = row
    return {"alpha2": by_alpha2, "alpha3": by_alpha3, "name": by_name}


def fetch_indicator(indicator):
    url = f"{WB_BASE}/country/all/indicator/{indicator}?format=json&per_page=30000"
    data = get_json(url)
    rows = data[1] if isinstance(data, list) and len(data) > 1 else []
    latest = {}
    for row in rows:
        code = (row.get("countryiso3code") or "").upper()
        value = row.get("value")
        if not code or value is None:
            continue
        year = int(row["date"]) if str(row.get("date", "")).isdigit() else None
        previous = latest.get(code)
        if previous is None or (year is not None and (previous[1] is None or year > previous[1])):
            latest[code] = (value, year)
    return latest


def build_source_indexes():
    print("Fetching World Bank country metadata...")
    wb = fetch_world_bank_countries()
    print("Fetching ISO/currency metadata...")
    rc = fetch_rest_countries()
    indicators = {}
    for field, indicator in INDICATORS.items():
        print(f"Fetching World Bank indicator {indicator}...")
        indicators[field] = fetch_indicator(indicator)
    return wb, rc, indicators


def match_country(country, wb, rc):
    name = normalize_name(country["country_name"])
    alias = ALIASES.get(name)

    # Prefer an explicit registry ISO code if a later registry version provides one.
    a2 = (country.get("iso_alpha2") or "").upper()
    if a2 and a2 in wb["alpha2"]:
        wb_row = wb["alpha2"][a2]
        rc_row = rc["alpha2"].get(a2)
        return wb_row, rc_row, "registry_iso2"

    for candidate in (name, alias):
        if not candidate:
            continue
        wb_row = wb["name"].get(normalize_name(candidate))
        rc_row = rc["name"].get(normalize_name(candidate))
        if wb_row or rc_row:
            return wb_row, rc_row, "exact_name"

    return None, None, "unmatched"


def currency_from_rest(row):
    if not row:
        return None
    currencies = row.get("currencies") or {}
    if not currencies:
        return None
    codes = sorted(currencies)
    code = codes[0]
    item = currencies[code] or {}
    name = item.get("name")
    symbol = item.get("symbol")
    result = {"code": code, "name": name}
    if symbol:
        result["symbol"] = symbol
    return result


def status(record):
    banking = record.get("banking") or {}
    if record.get("registry_id") and record.get("country_name") and record.get("iso_alpha3") and record.get("currency") and banking.get("central_bank") and banking.get("supervisor"):
        return "COMPLETE"
    if record.get("iso_alpha3") or record.get("currency") or any(x.get("value") is not None for x in record.get("economic_indicators", {}).values()):
        return "PARTIAL"
    return "MISSING"


def build(registry, wb, rc, indicators):
    records = []
    match_counts = {"registry_iso2": 0, "exact_name": 0, "unmatched": 0}

    for index, country in enumerate(registry, 1):
        wb_row, rc_row, method = match_country(country, wb, rc)
        match_counts[method] += 1

        iso2 = country.get("iso_alpha2") or (wb_row or {}).get("iso2Code") or (rc_row or {}).get("cca2")
        iso3 = country.get("iso_alpha3") or (wb_row or {}).get("id") or (rc_row or {}).get("cca3")
        iso2 = iso2.upper() if iso2 else None
        iso3 = iso3.upper() if iso3 else None

        economic = {}
        for field in INDICATORS:
            pair = indicators[field].get(iso3) if iso3 else None
            if pair:
                value, year = pair
                economic[field] = {"value": value, "year": year, "source": "World Bank API"}
            else:
                economic[field] = {
                    "value": None,
                    "year": None,
                    "source": "World Bank API",
                    "error": "No matched World Bank observation.",
                }

        currency = country.get("currency")
        if currency is None:
            currency = (country.get("banking") or {}).get("currency")
        if currency is None:
            currency = (country.get("economy") or {}).get("currency")
        if currency is None:
            currency = currency_from_rest(rc_row)

        registry_sources = country.get("sources", [])
        if isinstance(registry_sources, str):
            registry_sources = [registry_sources]
        elif not isinstance(registry_sources, list):
            registry_sources = []
        sources = list(dict.fromkeys(registry_sources + ["https://data.worldbank.org/", "https://restcountries.com/"]))

        banking = country.get("banking") or {}
        economy = dict(country.get("economy") or {})
        if wb_row:
            economy.setdefault("world_bank_region", (wb_row.get("region") or {}).get("value"))
            economy.setdefault("world_bank_income_level", (wb_row.get("incomeLevel") or {}).get("value"))
            economy.setdefault("world_bank_lending_type", (wb_row.get("lendingType") or {}).get("value"))
        if rc_row:
            economy.setdefault("region", rc_row.get("region"))
            economy.setdefault("subregion", rc_row.get("subregion"))
            capital = rc_row.get("capital") or []
            if capital:
                economy.setdefault("capital", capital[0])

        record = {
            "registry_id": country["registry_id"],
            "country_name": country["country_name"],
            "iso_alpha2": iso2,
            "iso_alpha3": iso3,
            "currency": currency,
            "state_formation": country.get("state_formation") or {},
            "banking": banking,
            "economic_influence": country.get("economic_influence") or {
                "global_level": "UNASSESSED",
                "regional_level": "UNASSESSED",
                "domestic_level": "UNASSESSED",
            },
            "economy": economy,
            "corporate_sector": country.get("corporate_sector") or {},
            "corporate_struggles": country.get("corporate_struggles") or [],
            "economic_crises": country.get("economic_crises") or [],
            "trade": country.get("trade") or {},
            "resources": country.get("resources") or {},
            "socialist_periods": country.get("socialist_periods") or [],
            "economic_indicators": economic,
            "data_match": {
                "method": method,
                "world_bank_name": (wb_row or {}).get("name"),
                "rest_countries_name": ((rc_row or {}).get("name") or {}).get("common"),
            },
            "verification": {
                "last_verified": str(date.today()),
                "source_status": "PRIMARY_OR_INTERNATIONAL",
                "sources": sources,
            },
        }
        record["status"] = status(record)
        records.append(record)
        print(f"[{index}/{EXPECTED}] {record['country_name']}: {record['status']} ({method})")

    print(json.dumps({"match_counts": match_counts}, indent=2))
    return records


def fmt_number(value):
    if value is None:
        return "N/A"
    if abs(value) >= 1_000_000_000:
        return f"${value / 1_000_000_000:.2f}B"
    if abs(value) >= 1_000_000:
        return f"${value / 1_000_000:.2f}M"
    if abs(value) >= 1_000:
        return f"${value:,.0f}"
    return f"{value:.2f}"


def latest_value(record, field):
    return (record.get("economic_indicators") or {}).get(field, {}).get("value")


def markdown_table(records):
    lines = [
        "## 391-Country National Economic Table",
        "",
        "This generated table is an evidence-backed data pull. `N/A` means the source did not provide a responsible value; it is not a zero. Banking authorities remain unpopulated unless independently sourced.",
        "",
        "| ID | Country/Jurisdiction | ISO | Currency | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |",
        "|---|---|---|---|---:|---:|---:|---:|---|---|---|",
    ]
    for record in records:
        economy = record.get("economy") or {}
        currency = record.get("currency")
        if isinstance(currency, dict):
            currency_display = currency.get("code") or "N/A"
        else:
            currency_display = str(currency) if currency else "N/A"
        banking = (record.get("banking") or {}).get("central_bank", "PENDING")
        income = economy.get("world_bank_income_level") or "N/A"
        lines.append(
            f"| {record['registry_id']} | {record['country_name']} | "
            f"{record.get('iso_alpha2') or 'N/A'} / {record.get('iso_alpha3') or 'N/A'} | "
            f"{currency_display} | {fmt_number(latest_value(record, 'gdp_usd'))} | "
            f"{fmt_number(latest_value(record, 'gdp_per_capita_usd'))} | "
            f"{fmt_number(latest_value(record, 'inflation_pct'))} | "
            f"{fmt_number(latest_value(record, 'trade_pct_gdp'))} | "
            f"{income} | {banking} | {record['status']} |"
        )
    return "\n".join(lines) + "\n"


def write_outputs(records, output, table):
    output.parent.mkdir(parents=True, exist_ok=True)
    coverage = {
        "total": len(records),
        "iso_alpha2": sum(bool(r.get("iso_alpha2")) for r in records),
        "iso_alpha3": sum(bool(r.get("iso_alpha3")) for r in records),
        "currency": sum(bool(r.get("currency")) for r in records),
        "gdp": sum(latest_value(r, "gdp_usd") is not None for r in records),
        "gdp_per_capita": sum(latest_value(r, "gdp_per_capita_usd") is not None for r in records),
        "inflation": sum(latest_value(r, "inflation_pct") is not None for r in records),
        "unemployment": sum(latest_value(r, "unemployment_pct") is not None for r in records),
        "trade": sum(latest_value(r, "trade_pct_gdp") is not None for r in records),
        "central_bank": sum(bool((r.get("banking") or {}).get("central_bank")) for r in records),
        "supervisor": sum(bool((r.get("banking") or {}).get("supervisor")) for r in records),
        "complete": sum(r.get("status") == "COMPLETE" for r in records),
        "partial": sum(r.get("status") == "PARTIAL" for r in records),
        "missing": sum(r.get("status") == "MISSING" for r in records),
    }
    payload = {
        "generated": str(date.today()),
        "expected": EXPECTED,
        "coverage": coverage,
        "sources": [
            "https://api.worldbank.org/",
            "https://data.worldbank.org/",
            "https://restcountries.com/",
        ],
        "notes": [
            "World Bank indicators use the newest non-null observation returned for each ISO alpha-3 code.",
            "REST Countries supplies ISO and currency metadata where matched.",
            "No central bank, supervisor, political, historical, corporate, or socialist-period value is invented when not present in the registry or an explicitly sourced enrichment.",
        ],
        "records": records,
    }
    output.write_text(json.dumps(payload, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    existing = table.read_text(encoding="utf-8") if table.exists() else "# SLeeLa — BANKS.md\n"
    marker = "## 391-Country National Economic Table"
    if marker in existing:
        existing = existing.split(marker)[0].rstrip() + "\n\n"
    table.write_text(existing + markdown_table(records), encoding="utf-8")
    print(json.dumps(coverage, indent=2))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--registry", default=str(DEFAULT_REGISTRY))
    parser.add_argument("--output", default=str(DEFAULT_OUTPUT))
    parser.add_argument("--table", default=str(DEFAULT_TABLE))
    args = parser.parse_args()

    registry = load_registry(Path(args.registry))
    wb, rc, indicators = build_source_indexes()
    records = build(registry, wb, rc, indicators)
    if len(records) != EXPECTED:
        raise SystemExit(f"ERROR: generated {len(records)} records; expected {EXPECTED}.")
    write_outputs(records, Path(args.output), Path(args.table))


if __name__ == "__main__":
    main()
