#!/usr/bin/env python3
"""Populate BANKS4.md's final column with a NEUTRAL, SOURCED indicator.

This replaces an earlier four-value "Socialism Status" heuristic
({YES, NO, CONVINCED, INDEED}) that assigned an opaque, editorializing label to
every one of the 391 rows from a hardcoded, uncited country list. That approach
contradicted the registry's own evidence discipline (see BANKS.md: "No ...
political, historical ... value is invented when not present in the registry or
an explicitly sourced enrichment").

The replacement records a single, observable, citable fact:

    CONSTITUTIONAL SOCIALISM REFERENCE
    ----------------------------------
    Does the jurisdiction's own CURRENT constitution/basic law explicitly
    reference socialism (or a socialist state/economy) in its text?

Values (the only permitted values):
  * CONSTITUTIONAL  - the current constitution explicitly references socialism.
  * NONE            - the current constitution contains no such reference.
  * UNASSESSED      - not yet verified against a cited constitutional source.

Nothing is invented: only jurisdictions with an explicit, cited constitutional
basis recorded in CONSTITUTIONAL_REFERENCE below receive CONSTITUTIONAL or NONE.
Every other row is left UNASSESSED rather than guessed. This is a descriptive
constitutional-text indicator, NOT a judgment about a country's economy, policy,
or political character.

The column is titled "Constitutional Socialism Reference" in BANKS4.md.
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
PATH = ROOT / "BANKS4.md"

VALID = {"CONSTITUTIONAL", "NONE", "UNASSESSED"}

# Jurisdictions whose value is backed by an explicit, citable constitutional
# source. Each entry is (value, source). Countries absent here stay UNASSESSED.
# Sources are the jurisdiction's own constitution / basic law; the neutral,
# widely-used aggregator "Constitute Project" (constituteproject.org) hosts the
# official texts referenced.
CONSTITUTIONAL_REFERENCE = {
    # Constitutions that explicitly describe the state/economy as socialist.
    "China": ("CONSTITUTIONAL", "Constitution of the PRC, Art. 1 (\"socialist state\") — Constitute Project"),
    "Cuba": ("CONSTITUTIONAL", "Constitution of Cuba (2019), Art. 1 (\"socialist state\") — Constitute Project"),
    "Lao People's Democratic Republic": ("CONSTITUTIONAL", "Constitution of Laos, Preamble/Art. 2 — Constitute Project"),
    "Vietnam": ("CONSTITUTIONAL", "Constitution of Vietnam (2013), Art. 2 (\"socialist state\") — Constitute Project"),
    "North Korea": ("CONSTITUTIONAL", "DPRK Socialist Constitution, Art. 1 — Constitute Project"),
    "India": ("CONSTITUTIONAL", "Constitution of India, Preamble (\"SOCIALIST\", 42nd Amendment 1976) — Constitute Project"),
    "Bangladesh": ("CONSTITUTIONAL", "Constitution of Bangladesh, Art. 8 (socialism as a state principle) — Constitute Project"),
    "Sri Lanka": ("CONSTITUTIONAL", "Constitution of Sri Lanka (1978), Preamble (\"Democratic Socialist Republic\") — Constitute Project"),
    "Nepal": ("CONSTITUTIONAL", "Constitution of Nepal (2015), Preamble (\"socialism-oriented\") — Constitute Project"),
    "Portugal": ("CONSTITUTIONAL", "Constitution of Portugal (1976), Preamble references socialism — Constitute Project"),
    "Guyana": ("CONSTITUTIONAL", "Constitution of Guyana, Preamble (\"transition ... to socialism\") — Constitute Project"),
    "Tanzania": ("CONSTITUTIONAL", "Constitution of Tanzania, Art. 3/9 (socialism, Ujamaa) — Constitute Project"),
    # Explicitly NOT constitutionally socialist (representative, cited).
    "United States": ("NONE", "U.S. Constitution — no socialism reference (full text, National Archives)"),
    "United Kingdom": ("NONE", "Uncodified constitution — no codified socialism reference"),
    "Norway": ("NONE", "Constitution of Norway — no socialism reference — Constitute Project"),
    "Sweden": ("NONE", "Instrument of Government (Sweden) — no socialism reference — Constitute Project"),
    "Denmark": ("NONE", "Constitutional Act of Denmark — no socialism reference — Constitute Project"),
    "Finland": ("NONE", "Constitution of Finland — no socialism reference — Constitute Project"),
    "Iceland": ("NONE", "Constitution of Iceland — no socialism reference — Constitute Project"),
    "Germany": ("NONE", "Basic Law for the Federal Republic of Germany — no socialism reference — Constitute Project"),
    "France": ("NONE", "Constitution of France (1958) — no socialism reference — Constitute Project"),
    "Japan": ("NONE", "Constitution of Japan — no socialism reference — Constitute Project"),
}


def value_for(country: str) -> str:
    entry = CONSTITUTIONAL_REFERENCE.get(country)
    return entry[0] if entry else "UNASSESSED"


def main() -> None:
    if not PATH.exists():
        raise SystemExit("BANKS4.md not found; generate it first (tools/banking/update_banks4.py).")
    text = PATH.read_text(encoding="utf-8")
    lines = []
    rows = 0
    counts = {"CONSTITUTIONAL": 0, "NONE": 0, "UNASSESSED": 0}
    for line in text.splitlines():
        if re.match(r"^\| \d{3} \|", line):
            cells = line.split("|")
            ident = int(cells[1].strip())
            country = cells[2].strip()
            if not 1 <= ident <= 391:
                raise ValueError(f"Unexpected ID: {ident}")
            v = value_for(country)
            counts[v] += 1
            # Final table cell is the indicator; preserve every preceding cell.
            cells[-2] = f" {v} "
            line = "|".join(cells)
            rows += 1
        lines.append(line)
    if rows != 391:
        raise ValueError(f"Expected 391 rows, found {rows}")
    PATH.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"BANKS4 Constitutional Socialism Reference: updated {rows} rows "
          f"(CONSTITUTIONAL={counts['CONSTITUTIONAL']}, NONE={counts['NONE']}, "
          f"UNASSESSED={counts['UNASSESSED']})")


if __name__ == "__main__":
    main()
