# GRT evidence-model source

This subdirectory contains a small C ABI and C++ value wrapper for representing and structurally validating Global Ring of Trust (GRT) claims.

**Important:** this is a data model only. It does not verify signatures, establish identities, fetch evidence, contact revocation services, install trust roots, certify projects, authorize execution, or grant OS privileges. `GRT_CLAIM_WELL_FORMED` means only that required fields and basic time/digest syntax pass local checks. It is not a trust verdict.

The API is deliberately non-authorizing and deterministic. It uses no OS hooks, system calls, process launching, network access, or cryptographic code. A future interoperable format and signature profile must be selected through independent governance; do not extend this code with home-grown cryptography.

## Build and test

```sh
cmake -S os-level-support/trust -B build/grt
cmake --build build/grt
ctest --test-dir build/grt --output-on-failure
```
