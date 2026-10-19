# SLeeLa CDMA Source Library

The `/lib/cdma/` package is the language-facing CDMA domain model complementing the native C/C++ reference implementation in `/cdma/`. It provides SLeeLa classes for technology profiles, channels, frames, spreading-code metadata, link budgets, power control, handoff, sessions, capacity, scheduling, recovery, metrics, and orchestration.

## Coverage

- IS-95 / cdmaOne, CDMA2000 1xRTT, EV-DO, W-CDMA / UMTS profile metadata.
- Explicit input validation, diagnostics, bounded state transitions, and capacity checks.
- Educational simulation models and source-level example.
- Native implementation counterpart: [`/cdma/`](../../cdma/).

This is a source-level domain model, not a radio driver, complete PHY/MAC stack, conformance certificate, or claim of over-the-air interoperability. Hardware transmission requires authorized spectrum and standards-compliant integration. Numeric calculations are simplified estimates.

All source files declare `#sleela 2.1`, consistent with current language-facing compiler examples. Parse and run them using the compiler command syntax supported by your current build; do not infer source compilation success from native C/C++ tests.

## Inventory

`CDMAConstants`, `CDMAValidation`, `CDMASpreadingCode`, `CDMAChannel`, `CDMAFrame`, `CDMALinkBudget`, `CDMAPowerControl`, `CDMAHandoff`, `CDMASession`, `CDMANetwork`, `IS95Profile`, `CDMA2000Profile`, `EVDOProfile`, `WCDMAProfile`, `CDMAMultiUserScheduler`, `CDMARecovery`, `CDMAMetrics`, and `CDMAEngine` are the core objects. See [TESTING.md](TESTING.md) and [examples/CDMADemo.sleela](examples/CDMADemo.sleela).

## Native Tests

```sh
make -C cdma/build all
make -C cdma/build test
```

These commands test the native counterpart, not every SLeeLa file.

**SLeeLa — MEARVK LLC — 2026**
