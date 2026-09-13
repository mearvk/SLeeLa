#!/usr/bin/env python3
"""Repair and normalize BANKS.md country-year columns.

This pass is deliberately defensive: it reconstructs ISO 3166 identifiers
from the public country-code dataset and restores the intended table layout:
ID | Country | Start Year | End Year | ISO-2 | ISO-3 | Numeric | Currency | ...

It also reuses the curated historical year map from the existing population
script. Existing economic values after the currency column are preserved.
"""
from __future__ import annotations

import csv
import importlib.util
import re
from io import StringIO
from pathlib import Path
from urllib.request import Request, urlopen

BANKS = Path("BANKS.md")
CODES_URL = "https://raw.githubusercontent.com/datasets/country-codes/main/data/country-codes.csv"
INDEPENDENCE_URL = "https://raw.githubusercontent.com/samayo/country-json/master/src/country-by-independence-date.json"

NAME_ALIASES = {
    "côte d'ivoire": "Cote d'Ivoire",
    "cote d'ivoire": "Cote d'Ivoire",
    "democratic republic of the congo": "Democratic Republic of the Congo",
    "iran": "Iran (Islamic Republic of)",
    "laos": "Lao People's Democratic Republic",
    "moldova": "Moldova, Republic of",
    "north korea": "Korea (the Democratic People's Republic of)",
    "south korea": "Korea, Republic of",
    "syria": "Syrian Arab Republic",
    "tanzania": "Tanzania, United Republic of",
    "venezuela": "Venezuela (Bolivarian Republic of)",
    "bolivia": "Bolivia (Plurinational State of)",
    "brunei": "Brunei Darussalam",
    "vatican city": "Holy See (Vatican City State)",
}


def load_module():
    path = Path("tools/populate-bank-country-years.py")
    spec = importlib.util.spec_from_file_location("bank_years", path)
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(module)
    return module


def norm(value: str) -> str:
    return " ".join(value.strip().casefold().split())


def fetch(url: str):
    req = Request(url, headers={"User-Agent": "SLeeLa-BANKS-country-years-repair/1.0"})
    with urlopen(req, timeout=30) as response:
        return response.read().decode("utf-8")


def load_codes():
    records = {}
    for row in csv.DictReader(StringIO(fetch(CODES_URL))):
        name = row.get("name") or row.get("official_name_en") or ""
        a2 = (row.get("ISO3166-1-Alpha-2") or "").strip().upper()
        a3 = (row.get("ISO3166-1-Alpha-3") or "").strip().upper()
        numeric = (row.get("ISO3166-1-numeric") or "").strip().zfill(3)
        currency = (row.get("ISO4217-currency_alphabetic_code") or "").strip().upper() or "N/A"
        if name and a2 and a3:
            records[norm(name)] = (a2, a3, numeric, currency)
    return records


def lookup(name, records):
    key = norm(name)
    key = norm(NAME_ALIASES.get(key, name))
    return records.get(key)


def load_independence():
    data = {}
    for item in __import__("json").loads(fetch(INDEPENDENCE_URL)):
        data[norm(item["country"])] = item["independence"]
    return data


def year_value(value: str) -> bool:
    value = value.strip().upper()
    return bool(re.fullmatch(r"-?\d{1,4}|OPEN|UNKNOWN|N/A", value))


def main():
    helper = load_module()
    historical = helper.HISTORICAL
    aliases = {norm(k): norm(v) for k, v in helper.ALIASES.items()}
    codes = load_codes()
    independence = load_independence()

    lines = BANKS.read_text(encoding="utf-8").splitlines()
    out = []
    repaired = 0

    for line in lines:
        if not re.match(r"^\|\s*\d+\s*\|", line):
            if line.startswith("| ID | Country/Jurisdiction |"):
                out.append("| ID | Country/Jurisdiction | Country Start Year | Country End Year | ISO Alpha-2 | ISO Alpha-3 | ISO Numeric Code | Currency Code | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |")
            elif line.startswith("| --- | --- |") and "Country Start Year" not in line:
                out.append("| --- | --- | ---: | ---: | --- | --- | --- | --- | ---: | ---: | ---: | ---: | --- | --- | --- |")
            else:
                out.append(line)
            continue

        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if len(cells) < 7:
            out.append(line)
            continue

        row_id = int(cells[0])
        name = cells[1]
        existing_years = year_value(cells[2]) and year_value(cells[3]) if len(cells) >= 4 else False

        if existing_years:
            start, end = cells[2], cells[3]
            tail = cells[4:]
        else:
            # Original layout: ID, Country, ISO2, ISO3, Numeric, Currency, ...
            start = end = "UNKNOWN"
            tail = cells[2:]
            if row_id <= 249:
                key = norm(NAME_ALIASES.get(norm(name), name))
                start = str(independence.get(key, "UNKNOWN"))
                end = "OPEN" if start != "UNKNOWN" else "UNKNOWN"
            elif name in historical:
                start, end = map(str, historical[name])

        code = lookup(name, codes)
        if code:
            a2, a3, numeric, currency = code
        else:
            # Preserve useful values from either the damaged or original row.
            if existing_years:
                numeric = tail[2] if len(tail) > 2 else "N/A"
                currency = tail[3] if len(tail) > 3 else "N/A"
            else:
                numeric = tail[2] if len(tail) > 2 else "N/A"
                currency = tail[3] if len(tail) > 3 else "N/A"
            a2 = tail[0] if len(tail) > 0 and not year_value(tail[0]) else "N/A"
            a3 = tail[1] if len(tail) > 1 and not year_value(tail[1]) else "N/A"

        # Tail after ISO/currency depends on the source layout.
        if existing_years:
            economic = tail[4:]
        else:
            economic = tail[4:]

        rebuilt = [str(row_id), name, start, end, a2, a3, numeric, currency] + economic
        out.append("| " + " | ".join(rebuilt) + " |")
        repaired += 1

    BANKS.write_text("\n".join(out) + "\n", encoding="utf-8")
    print(f"Rebuilt {repaired} BANKS rows with start/end years and ISO/currency columns.")


if __name__ == "__main__":
    main()
