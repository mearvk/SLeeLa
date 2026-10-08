# CPU Register Specification

Each processor profile should distinguish:

1. architectural registers visible to software;
2. control/system registers;
3. status/condition registers;
4. implementation/debug registers when publicly documented;
5. hidden/internal state, which is modeled only when documentation permits.

Register records should contain name, width, reset value when known, access class, purpose, side effects, encoding and privilege level.

The register model must not manufacture undocumented internal registers.