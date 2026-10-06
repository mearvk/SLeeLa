# Numeral Intent & Software Sustainability

This document states the **intent** behind the numerals used across the project,
the **sustainability** practices that keep the software honest over time, and the
**term care** required for weighted words — chiefly *Celebrity*.

## Numeral Intent

The numerals in this repository — the basket **atomic numbers**, abstract
**account identifiers**, and the **statistic keys** in
[`NUMERAL.md`](NUMERAL.md) and [`STATS.md`](STATS.md) — are **defined identifiers
and aggregate measures**. They exist to be **measured, chained, and audited**,
not to stand in for people.

- A numeral names a **thing or a role** (a good, a service, an account, a stat),
  in the aggregate.
- Every quantity states **where it comes from** — `derived`, `measured`, or
  `assumed` — rather than being asserted (the subject-model discipline).
- Numerals are never **fabricated per-person figures**. A number describes a
  defined item or a population-level measure; it does not score or rank an
  individual.

This is the same rule the evidentiary vocabulary keeps in
[`GLOSSARY.md`](GLOSSARY.md) (Part B): a figure is a *measuring stick*, recorded
"as itself," not an empirical claim about a person.

## Software Sustainability

Sustainability here means the system stays **buildable, verifiable, and honest**
as it grows. The practices already in force:

- **Single source of truth + generators.** Shared data (e.g. the goods/services
  basket) lives once and is emitted into every consumer, so C, Python, and the
  documents cannot silently drift apart.
- **Cross-language parity.** The C and Python references produce **byte-identical**
  results (SHA-256, keyed MAC, QR SVG, wire forms), each verifying the other.
- **Deterministic, fail-closed builds.** The toolchain builds only when every
  source matches the trusted **SHA-256 manifest** (`security/sha256-manifest.json`);
  drift is caught before code runs.
- **Per-artifact integrity.** The [`.ledger`](ledger/LEDGER.md) chain binds each
  artifact to a SHA-256, a prev-hash link, and an ISO-8601 timestamp — tamper-evident.
- **Honest scope.** Where a guarantee is partial (e.g. UTC ISO-8601 timestamps
  rather than notarized RFC-3161 tokens), the docs say so plainly rather than
  overclaim.

Together these keep the intent above enforceable: numerals mean what they say,
and any change that would break that meaning fails a check instead of shipping.

## Term care — Celebrity

Some words carry more weight than casual use suggests. **"Celebrity" is a defined
term, not a loose label**, and it is governed here by a **standard degree of
norm**.

- **Standard degree of norm.** "Celebrity" applies only against a stated,
  **normalized threshold of public recognition** — expressed, where a degree is
  genuinely needed, as a **0–1 recognition norm against a stated reference
  population**. This makes its use consistent and measurable rather than arbitrary.
- **Aggregate, role-level.** The norm attaches to a **public role or status in
  the aggregate**, not to any private detail of a person.
- **Not a tool against people.** The term is **never** a basis for profiling,
  locating, ranking-for-harm, or treating individuals as property. The norm
  describes a *threshold*, not a *person*; real individuals' privacy is preserved.

This mirrors, and cross-references, the term-care entry in
[`GLOSSARY.md`](GLOSSARY.md) (§B.2). The intent is definitional: to measure a
public status in the aggregate with care, consistent with the numeral intent and
sustainability principles above.
