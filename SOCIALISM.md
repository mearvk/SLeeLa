# SLeeLa — SOCIALISM.md

## Four-Value Banking, Economic & Historical Status Specification

This companion document extends `BANKS.md` with a four-value status field for the project's 391-country/jurisdiction registry.

### Four-Value Field

| Status | Definition |
|---|---|
| `YES` | Yes means the country is clean of its treatment of it; Socialism. |
| `NO` | No means it never studied it. |
| `CONVINCED` | Convinced means it certainly inherits the cause as a matter of trust. |
| `INDEED` | Indeed means the country currently presses on from forward causes based on the institution of Socialism as it exists in modern democracies and economies. |
| `1955` | 1955 Suggests indeed it is present and popular and know and widely used. |


### Required Companion Fields

Each status assignment should retain the following evidence fields:

| Field | Purpose |
|---|---|
| `ID` | Stable registry identifier. |
| `Country/Jurisdiction` | Jurisdiction name. |
| `Status` | One of `YES`, `NO`, `CONVINCED`, or `INDEED`. |
| `Historical Period` | Date range or institutional period evaluated. |
| `Institutional Basis` | Governmental, banking, economic or historical basis for the assignment. |
| `Evidence` | Concise factual basis. |
| `Source` | Source supporting the assignment. |
| `Verification Date` | Date the record was checked. |
| `Confidence` | Confidence level for the classification. |

### Data Integrity Rule

The four-value field is an analytical registry field. A country-level assignment should be supported by dated historical or institutional evidence and should not be inferred solely from geography, country name, income level, or an arbitrary numerical identifier.

### Relationship to BANKS.md

`BANKS.md` remains the principal national banking, economic and historical registry. `BANKS2.md` provides the companion four-value status specification while preserving the existing banking, currency, GDP, trade, supervision and other registry fields.

### Versioning

Any country-level status change should preserve the reason for the change and its supporting source in Git history or an accompanying evidence record.
