#!/usr/bin/env python3
"""
Normalize BANKS.md column names without changing the underlying data.

Canonical names:
    ISO                 -> ISO Alpha-2
    ISO2                -> ISO Alpha-2
    ISO3                -> ISO Alpha-3
    ISO Numeric         -> ISO Numeric Code
    Start Year          -> Country Start Year
    End Year            -> Country End Year
    Currency            -> Currency Code
    Currency Name       -> Currency Name

This script intentionally changes column names only. It does not populate,
calculate, or otherwise modify data values.

Max Rupplin - MEARVK LLC - 2026
"""

from pathlib import Path

BANKS = Path("BANKS.md")

COLUMN_RENAMES = {
    "ISO": "ISO Alpha-2",
    "ISO2": "ISO Alpha-2",
    "ISO3": "ISO Alpha-3",
    "ISO Numeric": "ISO Numeric Code",
    "ISO Numeric Code": "ISO Numeric Code",
    "Start Year": "Country Start Year",
    "Country Start": "Country Start Year",
    "Country Start Date": "Country Start Year",
    "End Year": "Country End Year",
    "Country End": "Country End Year",
    "Country End Date": "Country End Year",
    "Currency": "Currency Code",
    "Currency Code": "Currency Code",
    "Currency Name": "Currency Name",
}


def normalize_header(header: str) -> str:
    return COLUMN_RENAMES.get(header.strip(), header.strip())


def split_table_row(line: str):
    return [cell.strip() for cell in line.strip().strip("|").split("|")]


def make_table_row(cells):
    return "| " + " | ".join(cells) + " |"


def main():
    if not BANKS.exists():
        raise SystemExit("BANKS.md was not found.")

    lines = BANKS.read_text(encoding="utf-8").splitlines()
    changed = False

    for index, line in enumerate(lines):
        if not line.lstrip().startswith("|") or index + 1 >= len(lines):
            continue

        cells = split_table_row(line)
        separator = lines[index + 1].strip()
        if not separator.startswith("|"):
            continue

        separator_cells = split_table_row(separator)
        if len(cells) != len(separator_cells):
            continue

        # Identify a Markdown table header by the following separator row.
        if not any("-" in cell for cell in separator_cells):
            continue

        normalized = [normalize_header(cell) for cell in cells]
        if normalized != cells:
            lines[index] = make_table_row(normalized)
            changed = True

    if changed:
        BANKS.write_text("\n".join(lines) + "\n", encoding="utf-8")
        print("BANKS.md column names normalized.")
    else:
        print("BANKS.md column names already normalized.")


if __name__ == "__main__":
    main()
