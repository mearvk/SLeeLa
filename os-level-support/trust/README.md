# GRT claim-model source

This subdirectory provides a small C ABI and C++ value wrapper for representing and structurally validating Global Ring of Trust (GRT) claims.

**Data model only.** It does not verify signatures, establish identities, fetch evidence, contact revocation services, install trust roots, certify projects, authorize execution, or grant OS privileges. `GRT_CLAIM_WELL_FORMED` means only that required fields and basic time/digest syntax pass local checks; it is not a trust verdict.

The API is deterministic and has no OS hooks, process launching, network access, or cryptographic code. Interoperable formats and signature profiles must be selected through independent governance; do not add home-grown cryptography.

Standalone build:

```sh
cmake -S os-level-support/trust -B build/grt
cmake --build build/grt
ctest --test-dir build/grt --output-on-failure
```
