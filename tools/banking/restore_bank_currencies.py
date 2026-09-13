#!/usr/bin/env python3
"""Restore BANKS.md Currency Code values from the known-good prior commit.

This is deliberately a restoration pass, not a fresh currency lookup.  The
currency column from commit 181127615bf06bb1bec79f778255183a3bb1ec13 is the
source of truth.  All other BANKS.md fields are preserved byte-for-byte except
for the Currency Code cell in the generated 391-row table.

Max Rupplin - MEARVK LLC - 2026
"""

from __future__ import annotations

import re
from pathlib import Path
from urllib.request import Request, urlopen

BANKS = Path("BANKS.md")
SOURCE_URL = (
    "https://raw.githubusercontent.com/mearvk/SLeeLa/"
    "181127615bf06bb1bec79f778255183a3bb1ec13/BANKS.md"
)
EXPECTED = 391


def fetch_source() -> str:
    request = Request(SOURCE_URL, headers={"User-Agent": "SLeeLa-BANKS-Currency-Restorer/1.0"})
    with urlopen(request, timeout=30) as response:
        return response.read().decode("utf-8")


def table_rows(text: str) -> dict[int, tuple[int, str]]:
    rows: dict[int, tuple[int, str]] = {}
    lines = text.splitlines()
    in_table = False
    for index, line in enumerate(lines):
        if line.startswith("## 391-Country National Economic Table"):
            in_table = True
            continue
        if not in_table or not line.startswith("|"):
            continue
        match = re.match(r"^\|\s*(\d{3})\s*\|", line)
        if not match:
            continue
        cells = [cell.strip() for cell in line.strip().strip("|").split("|")]
        if len(cells) < 6:
            raise SystemExit(f"Malformed BANKS row {match.group(1)}")
        registry_id = int(match.group(1))
        # Prior schema: ID, Country, ISO Alpha-2, ISO Alpha-3, ISO Numeric, Currency.
        rows[registry_id] = (index, cells[5])
    return rows


def restore() -> None:
    current = BANKS.read_text(encoding="utf-8")
    prior = fetch_source()
    current_lines = current.splitlines()
    prior_rows = table_rows(prior)
    current_rows = table_rows(current)

    if len(prior_rows) != EXPECTED or len(current_rows) != EXPECTED:
        raise SystemExit(
            f"Expected {EXPECTED} rows; prior={len(prior_rows)} current={len(current_rows)}"
        )

    changed = 0
    for registry_id, (current_index, _) in current_rows.items():
        prior_index, currency = prior_rows[registry_id]
        prior_line = prior.splitlines()[prior_index]
        prior_cells = [cell.strip() for cell in prior_line.strip().strip("|").split("|")]
        current_cells = [cell.strip() for cell in current_lines[current_index].strip().strip("|").split("|")]
        if len(current_cells) < 4 or len(prior_cells) < 6:
            raise SystemExit(f"Malformed row {registry_id:03d}")
        # Current schema may be ISO/Currency or the expanded ISO/Currency form.
        currency_index = 3 if len(current_cells) == 11 else 5
        if currency_index >= len(current_cells):
            raise SystemExit(f"Currency column not found for row {registry_id:03d}")
        if current_cells[currency_index] != currency:
            current_cells[currency_index] = currency
            current_lines[current_index] = "| " + " | ".join(current_cells) + " |"
            changed += 1

    BANKS.write_text("\n".join(current_lines) + "\n", encoding="utf-8")
    print(f"Restored {changed} Currency Code values from {SOURCE_URL}")


if __name__ == "__main__":
    restore()
