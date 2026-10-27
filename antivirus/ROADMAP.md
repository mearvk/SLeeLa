# Sleelavirin™ Roadmap

## Phase 1 — Structure

- package convention;
- manifest and provenance;
- normalized verdict model;
- digest model;
- heuristic/signature documentation;
- platform adapter boundaries.

## Phase 2 — Engine adapters

- Windows AMSI adapter;
- Linux provider adapter;
- macOS provider adapter;
- scanner capability discovery;
- bounded scan APIs.

## Phase 3 — Detection framework

- rule loader;
- signature registry;
- heuristic evaluator;
- evidence aggregation;
- deterministic policy evaluation.

## Phase 4 — Verification

- signed rule sets;
- reproducible scanner configuration;
- package-build integration;
- CI security gates;
- audit/report generation.

## Phase 5 — SLeeLa-native security services

- portable scan API;
- daemon/service integration;
- event and quarantine interfaces;
- IDE/build/package-manager integration.

## Native engine status (C / C++)

The portable native engine (`antivirus/src/`, built via `make`) now provides,
license-clean and dependency-free:

- **whole-file SHA-256 hash matching** (exact) — `sleelavirin_native.c`;
- **content byte-pattern matching** via a from-scratch **Aho–Corasick** automaton
  — `sleelavirin_ac.c` (addresses the content-signature gap noted in
  `CLAMAV-REFERENCE-ANALYSIS.md`);
- a **content-signature format + loader** (`.slv`) — `sleelavirin_sig.c`;
- **fuzzy similarity** (feature fingerprint + Hamming distance) and the **Trap-2**
  heuristic scorer — `sleelavirin_scan.c`, `sleelavirin_heuristic.c`;
- a **stable C bridge ABI** (request → validate → normalized verdict) —
  `sleelavirin_bridge.c` (Phase 5 "portable scan API");
- a **C++ wrapper library** (RAII, `std::optional`) — `sleelavirin_cpp.cpp`;
- the **`sleelavirin-scan` CLI** supporting both hash and content scans.

See `NATIVE-ENGINE.md` for build, usage, and design details. External-provider
adapters (Phase 2) and signed rule sets (Phase 4) remain the next steps; the
signing/definition plan is documented in `DEFINITIONS-SOURCING.md`.
