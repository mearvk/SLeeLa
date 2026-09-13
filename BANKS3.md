# SLeeLa — BANKS3.md

## HTTP 3.0 National Banking, Economic & Historical Registry — Consolidated Table

`BANKS3.md` is the designated single-table consolidation target for the SLeeLa banking registry.

The consolidated record is defined as the union of:

- `BANKS.md` — national banking, economic, historical, GDP, inflation, trade, income, banking and status fields.
- `BANKS2.md` — the 391-entry currency assignment pass using ISO 4217 codes where applicable.

### Consolidation Schema

| ID | Country/Jurisdiction | ISO | Currency | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |
|---:|---|---|---|---:|---:|---:|---:|---|---|---|
| 001–391 | 391-country/jurisdiction registry | From BANKS.md | From BANKS2.md | From BANKS.md | From BANKS.md | From BANKS.md | From BANKS.md | From BANKS.md | From BANKS.md | From BANKS.md |

### Source / Precedence

Currency values are taken from `BANKS2.md`; all other national economic and banking fields remain sourced from `BANKS.md`. Historical entities retain `N/A` where no current ISO 4217 currency assignment is applicable.

### Generation

The repository's banking merge tooling is responsible for materializing the complete row-level 391-entry table from the two source registries. `BANKS3.md` is the intended single-document destination and should be regenerated whenever either source table changes.
