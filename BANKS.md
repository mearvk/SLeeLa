# SLeeLa — BANKS.md

## HTTP 3.0 National Banking Data Registry

### Purpose

This document defines the **National Banking Data Check** for the SLeeLa HTTP 3.0 specification.

The objective is to give the HTTP 3.0 implementation a deterministic, inspectable way to answer:

> **Do we have the required national banking data for every country in the project's 391-country universe?**

The registry is a **data-completeness contract**, not a claim that every jurisdiction has the same banking structure, a conventional central bank, or identical reporting requirements.

The number **391** is the project's declared country/jurisdiction universe for this data collection. It must be treated as a configured registry size rather than silently substituted with another country's-count convention.

---

## 1. HTTP 3.0 Check

HTTP 3.0 should treat national banking information as structured application data above the transport layer.

The check is:

```text
HTTP 3.0
   │
   ▼
Country / Jurisdiction Registry
   │
   ▼
National Banking Record
   │
   ├── Identity
   ├── Monetary Authority
   ├── Central / National Bank
   ├── Banking Supervisor
   ├── Payment-System Authority
   ├── Currency
   ├── Regulatory Sources
   └── Data Status / Provenance
   │
   ▼
Completeness Check
   │
   ├── COMPLETE
   ├── PARTIAL
   ├── MISSING
   └── NOT_APPLICABLE
```

The HTTP layer must not infer missing banking facts from a neighboring country. Missing information remains missing until a valid source is supplied.

---

## 2. Required Record

Each of the 391 registry entries should eventually resolve to a record with the following logical fields:

| Field | Required | Meaning |
|---|---|---|
| `registry_id` | Yes | Stable project identifier. |
| `country_name` | Yes | Canonical country/jurisdiction name. |
| `iso_alpha2` | Preferred | ISO-style two-letter identifier where applicable. |
| `iso_alpha3` | Preferred | ISO-style three-letter identifier where applicable. |
| `status` | Yes | `COMPLETE`, `PARTIAL`, `MISSING`, or `NOT_APPLICABLE`. |
| `central_bank_name` | Yes* | Central/national monetary authority, where one exists. |
| `monetary_authority_type` | Yes | Central bank, currency board, monetary authority, shared authority, or other documented arrangement. |
| `banking_supervisor` | Yes* | Principal banking supervisory authority, where applicable. |
| `deposit_insurance` | Preferred | Deposit-insurance institution or documented absence. |
| `payment_system_authority` | Preferred | Principal national payment-system operator/authority. |
| `currency` | Yes | Official currency or documented currency arrangement. |
| `banking_law_source` | Preferred | Primary legal/regulatory source. |
| `central_bank_source` | Preferred | Primary monetary-authority source. |
| `supervisor_source` | Preferred | Primary supervisory source. |
| `last_verified` | Yes | Date the record was last checked. |
| `source_status` | Yes | `PRIMARY`, `SECONDARY`, or `UNVERIFIED`. |
| `notes` | Optional | Exceptions, shared institutions, territories, or special arrangements. |

`*` A field may be explicitly marked `NOT_APPLICABLE` when the jurisdiction's documented institutional structure does not contain that category.

---

## 3. Completeness Rule

A national banking record is **COMPLETE** only when:

1. the jurisdiction has a stable registry identity;
2. its monetary/banking authority structure is identified;
3. the applicable central/national bank or monetary authority is identified;
4. the principal banking supervisor is identified or explicitly documented as not applicable;
5. the official currency arrangement is identified;
6. at least one authoritative source is recorded;
7. the verification date is recorded; and
8. no required field is silently unknown.

The distinction is intentional:

```text
UNKNOWN ≠ NONE
NONE ≠ NOT_APPLICABLE
NOT_APPLICABLE ≠ VERIFIED
```

A missing institution must never be represented by an invented institution name.

---

## 4. Status Semantics

### COMPLETE

All required fields are populated or explicitly documented as not applicable, with authoritative provenance.

### PARTIAL

The jurisdiction is identified, but one or more required banking fields remain unresolved or depend on secondary evidence.

### MISSING

The project has not yet established a usable national banking record.

### NOT_APPLICABLE

The jurisdiction is part of the 391-entry project universe but the requested banking category genuinely does not apply. The reason must be recorded in `notes`.

---

## 5. Program-Friendly Record Form

The canonical logical representation should be equivalent to:

```text
BANK_RECORD {
    registry_id
    country_name
    iso_alpha2
    iso_alpha3

    status

    central_bank_name
    monetary_authority_type
    banking_supervisor
    deposit_insurance
    payment_system_authority
    currency

    banking_law_source
    central_bank_source
    supervisor_source

    last_verified
    source_status
    notes
}
```

A future machine-readable companion may serialize this structure as JSON, CSV, SQLite, or another SLeeLa-native data object without changing the semantic contract.

---

## 6. The 391-Country Completeness Check

The HTTP 3.0 implementation should maintain two independent counts:

```text
EXPECTED = 391
RESOLVED = COMPLETE + PARTIAL + NOT_APPLICABLE
MISSING = EXPECTED - RESOLVED
COMPLETE_RATE = COMPLETE / EXPECTED
```

For operational purposes, the stronger check is:

```text
READY = (COMPLETE == 391)
```

However, the system should also report partial coverage rather than reducing the result to a Boolean.

Recommended result:

```text
BANKING_DATA_CHECK {
    expected: 391
    complete: N
    partial: N
    missing: N
    not_applicable: N
    ready: true | false
}
```

The implementation should reject impossible totals:

```text
complete + partial + missing + not_applicable = 391
```

---

## 7. HTTP 3.0 Behavior

The HTTP 3.0 specification should use the banking registry as **application metadata**, not as a modification of HTTP framing, stream semantics, or transport security.

A request or internal operation may ask:

```text
banking.status(country)
banking.record(country)
banking.require_complete(country)
banking.coverage()
```

Conceptually:

```text
HTTP request
    ↓
country identity
    ↓
BANK_RECORD lookup
    ↓
validation
    ↓
response / internal object
```

No national banking information should be exposed merely because an HTTP connection was established. Access, disclosure, caching, and retention policies remain application concerns.

---

## 8. Source Priority

The registry should prefer evidence in this order:

1. **National central bank / monetary authority**
2. **National banking regulator or supervisory authority**
3. **Official government legislation or regulatory publication**
4. **Official national payment-system operator**
5. **Recognized international institutional source**
6. **Secondary reference source**, only as a documented fallback

Secondary sources should never silently overwrite a primary source.

Each record should preserve enough provenance for a later verification pass.

---

## 9. Shared and Special Banking Arrangements

The data model must support jurisdictions that:

- share a central bank;
- use another state's currency;
- participate in a monetary union;
- use a currency board;
- have separate monetary and supervisory institutions;
- have more than one banking supervisor;
- have no conventional central bank;
- have a territory-specific banking authority;
- have a distinct payment-system operator.

Therefore `central_bank_name` should not be treated as a unique key.

The stable key is:

```text
registry_id + jurisdiction
```

Institution records may be reused by reference where multiple jurisdictions share the same authority.

---

## 10. Verification Discipline

The checker should distinguish **existence** from **verification**.

For example:

```text
central_bank_name = "Unknown"
```

is not equivalent to:

```text
central_bank_name = null
```

and neither is equivalent to:

```text
central_bank_name = "No separate central bank"
```

The latter requires a source and explanation.

Every update should therefore be traceable to:

```text
jurisdiction
→ field
→ source
→ verification date
→ record status
```

This permits HTTP 3.0 tooling to report exactly what remains incomplete.

---

## 11. Initial Registry Table

The project should maintain one row per member of the 391-country universe.

The initial skeleton is intentionally status-oriented rather than fabricated:

| Registry ID | Country / Jurisdiction | Banking Record | Verification |
|---|---|---|---|
| `001`–`391` | Project country registry | **TO BE POPULATED** | **PENDING** |

The canonical 391-country list should be imported from the project's existing country registry when available. It should **not** be reconstructed from memory inside this specification, because changing the country-universe definition would change the meaning of the completeness check.

Once imported, each row becomes an independently verifiable `BANK_RECORD`.

---

## 12. Suggested Directory Layout

The data layer can begin without forcing 391 large records into this document.

Recommended structure:

```text
BANKS.md
data/
└── banking/
    ├── registry.json
    ├── schema.json
    ├── sources.json
    └── countries/
        ├── <registry-id>.json
        ├── <registry-id>.json
        └── ...
```

For a smaller initial implementation, a single:

```text
data/banking/registry.json
```

is sufficient.

The Markdown document remains the human-readable contract; the data file becomes the machine-readable source.

---

## 13. Validation Algorithm

A first implementation can use the following deterministic sequence:

```text
load country registry
assert registry size == 391

for each jurisdiction:
    load BANK_RECORD

    verify stable identity

    verify monetary authority structure
    verify applicable banking supervisor
    verify currency arrangement
    verify authoritative source
    verify last_verified

    classify:
        COMPLETE
        PARTIAL
        MISSING
        NOT_APPLICABLE

count every classification

assert total == 391

report:
    expected
    complete
    partial
    missing
    not_applicable
    ready
```

The checker should fail loudly if:

- a jurisdiction appears twice;
- a registry identifier is duplicated;
- a required field is malformed;
- a status is outside the allowed set;
- the classification totals do not equal 391;
- a `COMPLETE` record lacks required provenance.

---

## 14. No Fabrication Rule

This registry is designed to drive a program, so false completeness is worse than visible incompleteness.

The following are prohibited:

- inventing a central bank;
- treating a commercial bank as a national monetary authority;
- assuming a supervisor from geography alone;
- copying a neighboring jurisdiction's record without documenting a shared authority;
- marking a record `COMPLETE` solely because a country name exists;
- treating a missing source as proof that an institution does not exist.

The correct fallback is:

```text
PARTIAL or MISSING
```

until the record can be verified.

---

## 15. Relationship to NETWORK.md

`NETWORK.md` defines the SLeeLa 1.1 TCP foundation. This document defines a higher-level national banking data contract.

The layers should remain separate:

```text
SLeeLa Network Layer
        ↓
HTTP 3.0 Protocol Layer
        ↓
Country / Jurisdiction Layer
        ↓
National Banking Data Layer
        ↓
BANK_RECORD
```

The banking registry therefore does not require new socket primitives.

It provides structured data that higher-level HTTP 3.0 services can consume.

---

## 16. HTTP 3.0 Readiness Gate

The intended readiness gate is:

```text
HTTP 3.0 NATIONAL BANKING CHECK

EXPECTED COUNTRIES: 391

COMPLETE:         N
PARTIAL:          N
MISSING:          N
NOT APPLICABLE:   N
--------------------
TOTAL:          391

READY: COMPLETE == 391
```

A production implementation may permit operation with partial data, but it must expose the incomplete state explicitly.

For compliance-sensitive or national-banking workflows, the caller should be able to require:

```text
banking.require_complete(country)
```

and receive a deterministic failure when the record is not complete.

---

## 17. Implementation Status

**BANKS.md — specification and completeness contract initialized.**

Current state:

- **391-country target:** defined.
- **National banking record schema:** defined.
- **Completeness states:** defined.
- **Provenance requirement:** defined.
- **HTTP 3.0 integration boundary:** defined.
- **Machine-readable companion:** recommended, not yet populated.
- **391 individual banking records:** **not yet verified by this document**.

The next data-generation pass should consume the project's canonical 391-country registry and populate one `BANK_RECORD` per jurisdiction. The resulting checker should report the exact complete/partial/missing/not-applicable counts rather than assuming completeness.

---

## 18. Design Principle

**The purpose of BANKS.md is not to declare that the world is complete. It is to make incompleteness measurable, attributable, and correctable.**

That principle gives HTTP 3.0 a clean starting contract:

```text
391 jurisdictions
→ 391 records
→ verified provenance
→ deterministic completeness check
→ explicit readiness state
```
