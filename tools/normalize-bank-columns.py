#!/usr/bin/env python3
"""
Normalize and populate BANKS.md country-code columns.

Canonical national-table columns:
    ISO Alpha-2
    ISO Alpha-3
    ISO Numeric Code

The current BANKS table historically stored both alpha codes in one cell such as
"US / USA". This script splits that value into separate columns and fills missing
ISO 3166-1 codes from the public country-code reference dataset.

It intentionally does not change economic, banking, currency, or year values.

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

# Common names used in BANKS.md that differ from ISO/UN display names.
NAME_ALIASES = {
    "cabo verde": "Cabo Verde",
    "congo": "Congo",
    "côte d'ivoire": "Cote d'Ivoire",
    "cote d'ivoire": "Cote d'Ivoire",
    "democratic republic of the congo": "Democratic Republic of the Congo",
    "gambia": "Gambia",
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
        if not name or not alpha2 or not alpha3:
            continue
        records[normalize_name(name)] = (alpha2, alpha3, numeric.zfill(3))
    return records


def lookup_codes(country_name, records):
    key = normalize_name(country_name)
    alias = NAME_ALIASES.get(key)
    if alias:
        key = normalize_name(alias)
    return records.get(key)


def parse_existing_iso(value):
    value = value.strip()
    if not value or value.upper() in {"N/A", "NA", "NONE"}:
        return "N/A", "N/A"

    parts = [part.strip().upper() for part in value.replace("|", "/").split("/")]
    parts = [part for part in parts if part]
    if len(parts) >= 2:
        return parts[0], parts[1]
    if len(parts) == 1:
        code = parts[0]
        if len(code) == 2:
            return code, "N/A"
        if len(code) == 3:
            return "N/A", code
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

        headers = split_table_row(line)
        separator = lines[index + 1].strip()
        if not separator.startswith("|"):
            index += 1
            continue

        separator_cells = split_table_row(separator)
        if len(headers) != len(separator_cells) or not any("-" in cell for cell in separator_cells):
            index += 1
            continue

        normalized = [cell.strip() for cell in headers]
        is_national = any(h.casefold() == "country/jurisdiction" for h in normalized)
        if not is_national:
            index += 1
            continue

        found_national_table = True

        # Normalize legacy names first.
        renames = {
            "ISO": "ISO Alpha-2",
            "ISO2": "ISO Alpha-2",
            "ISO3": "ISO Alpha-3",
            "ISO Numeric": "ISO Numeric Code",
            "ISO Numeric Code": "ISO Numeric Code",
            "Currency": "Currency Code",
            "Currency Code": "Currency Code",
        }
        normalized = [renames.get(h, h) for h in normalized]

        iso2_index = next((i for i, h in enumerate(normalized) if h == "ISO Alpha-2"), None)
        iso3_index = next((i for i, h in enumerate(normalized) if h == "ISO Alpha-3"), None)
        numeric_index = next((i for i, h in enumerate(normalized) if h == "ISO Numeric Code"), None)
        country_index = next(i for i, h in enumerate(normalized) if h == "Country/Jurisdiction")

        if iso2_index is None:
            iso2_index = country_index + 1
            normalized.insert(iso2_index, "ISO Alpha-2")
            iso3_index = None if iso3_index is None else iso3_index + 1
            numeric_index = None if numeric_index is None else numeric_index + 1

        # If the old combined ISO column exists, use it as the source and split it.
        if iso3_index is None:
            iso3_index = iso2_index + 1
            normalized.insert(iso3_index, "ISO Alpha-3")
            if numeric_index is not None and numeric_index >= iso3_index:
                numeric_index += 1

        if numeric_index is None:
            numeric_index = iso3_index + 1
            normalized.insert(numeric_index, "ISO Numeric Code")

        # Update the header and separator to match the expanded table.
        lines[index] = make_table_row(normalized)
        separator_cells = ["---" for _ in normalized]
        separator_cells[normalized.index("GDP")] = "---:"
        separator_cells[normalized.index("GDP/Capita")] = "---:"
        separator_cells[normalized.index("Inflation")] = "---:"
        separator_cells[normalized.index("Trade/GDP")] = "---:"
        lines[index + 1] = make_table_row(separator_cells)
        changed = True

        row_index = index + 2
        while row_index < len(lines) and lines[row_index].lstrip().startswith("|"):
            cells = split_table_row(lines[row_index])
            if len(cells) != len(headers):
                # Already-expanded row or a malformed row; stop at the table boundary.
                if len(cells) == len(normalized):
                    row_index += 1
                    continue
                break

            country_name = cells[country_index]
            old_iso = cells[iso2_index] if iso2_index < len(cells) else "N/A"
            alpha2, alpha3 = parse_existing_iso(old_iso)

            # If the source cell was already separate, preserve it.
            if iso3_index < len(cells) and cells[iso3_index].strip().upper() not in {"", "N/A"}:
                alpha3 = cells[iso3_index].strip().upper()

            looked_up = lookup_codes(country_name, records)
            if looked_up:
                if alpha2 == "N/A":
                    alpha2 = looked_up[0]
                if alpha3 == "N/A":
                    alpha3 = looked_up[1]
                numeric = looked_up[2]
            else:
                numeric = "N/A"

            # Rebuild from the original row, removing the legacy combined ISO cell
            # and inserting the three canonical code fields.
            new_cells = []
            for original_index, cell in enumerate(cells):
                if original_index == iso2_index:
                    new_cells.extend([alpha2, alpha3, numeric])
                elif original_index == iso3_index or original_index == numeric_index:
                    continue
                else:
                    new_cells.append(cell)

            lines[row_index] = make_table_row(new_cells)
            changed = True
            row_index += 1

        index = row_index

    if not found_national_table:
        raise SystemExit("The 391-Country National Economic Table was not found.")

    if changed:
        BANKS.write_text("\n".join(lines) + "\n", encoding="utf-8")
        print("BANKS.md country-code columns normalized and populated.")
    else:
        print("BANKS.md country-code columns already normalized.")


if __name__ == "__main__":
    main()
