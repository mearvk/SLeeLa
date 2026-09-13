#!/usr/bin/env python3
"""
Normalize and populate BANKS.md country, currency-code columns.

Canonical national-table columns:
    ISO Alpha-2
    ISO Alpha-3
    ISO Numeric Code
    Currency Code

The current BANKS table historically stored both alpha codes in one cell such as
"US / USA". This script splits that value into separate columns, fills missing
ISO 3166-1 codes, and restores ISO 4217 currency codes from the same public
country-code reference dataset.

It intentionally does not change GDP, banking, inflation, trade, or year values.

Max Rupplin - MEARVK LLC - 2026
"""

from csv import DictReader
from io import StringIO
from pathlib import Path
from urllib.request import Request, urlopen

BANKS = Path("BANKS.md")
COUNTRY_CODES_URL = (
    "https://raw.githubusercontent.com/datasets/country-codes/main/data/country-codes.csv"
)

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


def normalize_name(value: str) -> str:
    return " ".join(value.strip().casefold().split())


def split_table_row(line: str):
    return [cell.strip() for cell in line.strip().strip("|").split("|")]


def make_table_row(cells):
    return "| " + " | ".join(cells) + " |"


def load_country_codes():
    request = Request(COUNTRY_CODES_URL, headers={"User-Agent": "SLeeLa-BANKS/1.0"})
    with urlopen(request, timeout=30) as response:
        text = response.read().decode("utf-8")

    records = {}
    for row in DictReader(StringIO(text)):
        name = row.get("name") or row.get("official_name_en") or ""
        alpha2 = (row.get("ISO3166-1-Alpha-2") or "").strip().upper()
        alpha3 = (row.get("ISO3166-1-Alpha-3") or "").strip().upper()
        numeric = (row.get("ISO3166-1-numeric") or "").strip()
        currency = (row.get("ISO4217-currency_alphabetic_code") or "").strip().upper()
        if not name or not alpha2 or not alpha3:
            continue
        records[normalize_name(name)] = (
            alpha2,
            alpha3,
            numeric.zfill(3),
            currency or "N/A",
        )
    return records


def lookup_codes(country_name, records):
    key = normalize_name(country_name)
    key = normalize_name(NAME_ALIASES.get(key, country_name))
    return records.get(key)


def parse_existing_iso(value):
    value = value.strip()
    if not value or value.upper() in {"N/A", "NA", "NONE"}:
        return "N/A", "N/A"

    parts = [part.strip().upper() for part in value.replace("|", "/").split("/")]
    parts = [part for part in parts if part]
    if all(part in {"N", "A", "NA", "N/A", "NONE"} for part in parts):
        return "N/A", "N/A"
    if len(parts) >= 2:
        return parts[0], parts[1]
    if len(parts) == 1:
        if len(parts[0]) == 2:
            return parts[0], "N/A"
        if len(parts[0]) == 3:
            return "N/A", parts[0]
    return "N/A", "N/A"


def main():
    if not BANKS.exists():
        raise SystemExit("BANKS.md was not found.")

    records = load_country_codes()
    lines = BANKS.read_text(encoding="utf-8").splitlines()
    changed = False
    found_national_table = False

    index = 0
    while index + 1 < len(lines):
        line = lines[index]
        if not line.lstrip().startswith("|"):
            index += 1
            continue

        old_headers = split_table_row(line)
        separator = lines[index + 1].strip()
        if not separator.startswith("|"):
            index += 1
            continue

        separator_cells = split_table_row(separator)
        if len(old_headers) != len(separator_cells) or not any("-" in cell for cell in separator_cells):
            index += 1
            continue

        country_index = next(
            (i for i, h in enumerate(old_headers) if h.strip().casefold() == "country/jurisdiction"),
            None,
        )
        if country_index is None:
            index += 1
            continue

        found_national_table = True

        renames = {
            "ISO": "ISO Alpha-2",
            "ISO2": "ISO Alpha-2",
            "ISO3": "ISO Alpha-3",
            "ISO Numeric": "ISO Numeric Code",
            "ISO Numeric Code": "ISO Numeric Code",
            "Currency": "Currency Code",
            "Currency Code": "Currency Code",
        }
        normalized_old = [renames.get(h.strip(), h.strip()) for h in old_headers]

        old_iso2_index = next(
            (i for i, h in enumerate(normalized_old) if h == "ISO Alpha-2"), None
        )
        old_iso3_index = next(
            (i for i, h in enumerate(normalized_old) if h == "ISO Alpha-3"), None
        )
        old_numeric_index = next(
            (i for i, h in enumerate(normalized_old) if h == "ISO Numeric Code"), None
        )
        old_currency_index = next(
            (i for i, h in enumerate(normalized_old) if h == "Currency Code"), None
        )

        if old_iso2_index is None:
            old_iso2_index = country_index + 1
            normalized_old.insert(old_iso2_index, "ISO Alpha-2")

        canonical_headers = list(normalized_old)
        if old_iso3_index is None:
            canonical_headers.insert(old_iso2_index + 1, "ISO Alpha-3")
        if old_numeric_index is None:
            alpha3_pos = canonical_headers.index("ISO Alpha-3")
            canonical_headers.insert(alpha3_pos + 1, "ISO Numeric Code")
        if old_currency_index is None:
            numeric_pos = canonical_headers.index("ISO Numeric Code")
            canonical_headers.insert(numeric_pos + 1, "Currency Code")

        lines[index] = make_table_row(canonical_headers)
        canonical_separator = ["---" for _ in canonical_headers]
        for right_aligned in ("GDP", "GDP/Capita", "Inflation", "Trade/GDP"):
            if right_aligned in canonical_headers:
                canonical_separator[canonical_headers.index(right_aligned)] = "---:"
        lines[index + 1] = make_table_row(canonical_separator)
        changed = True

        row_index = index + 2
        while row_index < len(lines) and lines[row_index].lstrip().startswith("|"):
            cells = split_table_row(lines[row_index])
            if len(cells) != len(old_headers):
                break

            country_name = cells[country_index]
            alpha2 = "N/A"
            alpha3 = "N/A"
            numeric = "N/A"
            currency = "N/A"

            if old_iso3_index is None:
                alpha2, alpha3 = parse_existing_iso(cells[old_iso2_index])
            else:
                alpha2 = cells[old_iso2_index].strip().upper() or "N/A"
                alpha3 = cells[old_iso3_index].strip().upper() or "N/A"
                if old_numeric_index is not None:
                    numeric = cells[old_numeric_index].strip() or "N/A"

            if old_currency_index is not None:
                currency = cells[old_currency_index].strip().upper() or "N/A"

            looked_up = lookup_codes(country_name, records)
            if looked_up:
                if alpha2 == "N/A":
                    alpha2 = looked_up[0]
                if alpha3 == "N/A":
                    alpha3 = looked_up[1]
                if numeric == "N/A":
                    numeric = looked_up[2]
                if currency == "N/A":
                    currency = looked_up[3]

            new_cells = []
            for i, cell in enumerate(cells):
                if i == old_iso2_index:
                    new_cells.append(alpha2)
                    if old_iso3_index is None:
                        new_cells.extend([alpha3, numeric])
                elif old_iso3_index is not None and i == old_iso3_index:
                    new_cells.append(alpha3)
                elif old_numeric_index is not None and i == old_numeric_index:
                    new_cells.append(numeric)
                elif old_currency_index is not None and i == old_currency_index:
                    new_cells.append(currency)
                else:
                    new_cells.append(cell)

            if old_currency_index is None:
                numeric_new_index = next(
                    i for i, h in enumerate(canonical_headers) if h == "ISO Numeric Code"
                )
                new_cells.insert(numeric_new_index + 1, currency)

            lines[row_index] = make_table_row(new_cells)
            changed = True
            row_index += 1

        index = row_index

    if not found_national_table:
        raise SystemExit("The 391-Country National Economic Table was not found.")

    if changed:
        BANKS.write_text("\n".join(lines) + "\n", encoding="utf-8")
        print("BANKS.md country, ISO, and currency-code columns normalized and populated.")
    else:
        print("BANKS.md country, ISO, and currency-code columns already normalized.")


if __name__ == "__main__":
    main()
