# into-docs — safety copies

This directory holds **pre-edit snapshots** of the `BANK.INFORMATION.PANAMANIAN.*`
registry documents, captured before the row/column completion pass that filled
the deterministic identity columns (ISO 3166-1, ISO 4217 Currency, and the
banking/monetary authority) for the real jurisdictions that had previously
dropped out of the data pull.

| File | What it is |
|---|---|
| `BANK.INFORMATION.PANAMANIAN.001.md` | Original economic table, as of the backup date. |
| `BANK.INFORMATION.PANAMANIAN.002.md` | Original currency-authoritative sibling. |
| `BANK.INFORMATION.PANAMANIAN.003.md` | Original combined (001 ∪ 002) HTTP-3.0 registry. |

The live, updated versions remain at the repository root. These copies are kept
for safety / reciprocity and are not deleted.

**Note on economic figures:** the update only filled stable, independently
verifiable identity facts. No GDP, per-capita, inflation, trade, or income-group
figures were fabricated — those remain `N/A` for the back-filled rows pending a
single consistent World Bank WDI / IMF WEO vintage.
