# SLeeLa — SOCIALISM.md

## Four-Value Banking, Economic & Historical Status Specification

This companion document extends `BANKS.md` with a four-value status field for the project's 391-country/jurisdiction registry.

### Four-Value Field

| Status | Definition |
|---|---|
| `YES` | The applicable condition is supported by the evidence recorded for the jurisdiction and period under review. |
| `NO` | The applicable condition is not established by the evidence recorded for the jurisdiction and period under review. |
| `CONVINCED` | The record documents substantial historical or institutional continuity relevant to the condition. |
| `INDEED` | The record documents a current or continuing institutional condition relevant to the condition being measured. |

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
