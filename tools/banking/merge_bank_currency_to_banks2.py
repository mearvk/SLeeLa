#!/usr/bin/env python3
"""Create BANKS2.md by preserving BANKS.md and filling only missing Currency cells.

The existing BANKS.md table is treated as authoritative for all existing data.
Only Currency cells containing N/A or an empty value are populated. Currency
values are taken from the known-good SLeeLa currency table at commit
181127615bf06bb1bec79f778255183a3bb1ec13.

Max Rupplin - MEARVK LLC - 2026
"""

from __future__ import annotations

import re
from pathlib import Path
from urllib.request import Request, urlopen

INPUT = Path("BANKS.md")
OUTPUT = Path("BANKS2.md")
SOURCE_URL = (
    "https://raw.githubusercontent.com/mearvk/SLeeLa/"
    "181127615bf06bb1bec79f778255183a3bb1ec13/BANKS.md"
)
EXPECTED = 391


def fetch_source() -> str:
    request = Request(
        SOURCE_URL,
        headers={"User-Agent": "SLeeLa-BANKS2-Currency-Merger/1.0"},
    )
    with urlopen(request, timeout=30) as response:
        return response.read().decode("utf-8")


def table_rows(text: str) -> dict[int, tuple[int, list[str]]]:
    rows: dict[int, tuple[int, list[str]]] = {}
    in_table = False
    for index, line in enumerate(text.splitlines()):
        if line.startswith("## 391-Country National Economic Table"):
            in_table = True
            continue
        if not in_table or not line.startswith("|"):
            continue
        match = re.match(r"^\|\s*(\d{3})\s*\|", line)
        if not match:
            continue
        cells = [cell.strip() for cell in line.strip().strip("|").split("|")]
        rows[int(match.group(1))] = (index, cells)
    return rows


def currency_index(cells: list[str]) -> int:
    # Current BANKS schema: ID, Country, ISO, Currency, ...
    if len(cells) == 11:
        return 3
    # Historical expanded schema: ID, Country, Alpha-2, Alpha-3,
    # Numeric, Currency, ...
    if len(cells) >= 13:
        return 5
    raise SystemExit(f"Unsupported table row width: {len(cells)}")


def main() -> None:
    current = INPUT.read_text(encoding="utf-8")
    prior = fetch_source()
    current_lines = current.splitlines()
    prior_lines = prior.splitlines()
    current_rows = table_rows(current)
    prior_rows = table_rows(prior)

    if len(current_rows) != EXPECTED or len(prior_rows) != EXPECTED:
        raise SystemExit(
            f"Expected {EXPECTED} rows; current={len(current_rows)} prior={len(prior_rows)}"
        )

    added = 0
    preserved = 0
    for registry_id in range(1, EXPECTED + 1):
        current_index, current_cells = current_rows[registry_id]
        prior_index, prior_cells = prior_rows[registry_id]
        ci = currency_index(current_cells)
        pi = currency_index(prior_cells)
        existing_currency = current_cells[ci]
        source_currency = prior_cells[pi]

        if existing_currency not in ("", "N/A", "NA", "UNKNOWN"):
            preserved += 1
            continue
        if source_currency in ("", "N/A", "NA", "UNKNOWN"):
            continue

        current_cells[ci] = source_currency
        current_lines[current_index] = "| " + " | ".join(current_cells) + " |"
        added += 1

    OUTPUT.write_text("\n".join(current_lines) + "\n", encoding="utf-8")
    print(f"BANKS2.md created from BANKS.md: added={added}, preserved={preserved}")


if __name__ == "__main__":
    main()
