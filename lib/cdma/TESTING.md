# CDMA Source Testing

Run the native reference suite from the repository root:

```sh
make -C cdma/build all
make -C cdma/build test
```

When a SLeeLa compiler/runtime is available, parse and run all package sources and `examples/CDMADemo.sleela` using that build's documented CLI.

## Behavioral Invariants

1. Invalid channel direction is rejected.
2. Negative or oversized frame payloads are rejected.
3. Link-budget calculations reject invalid loss/gain inputs.
4. Power-control output stays within configured bounds.
5. Handoff cannot complete until target preparation and readiness succeed.
6. Sessions cannot activate before registration.
7. Network admission stops at capacity.
8. Scheduler rejects invalid or over-capacity requests.
9. Recovery stops at the retry limit.
10. Empty metrics return a safe ratio without division by zero.

These models are educational foundations. Passing tests does not establish 3GPP/3GPP2 conformance, RF certification, security review, or carrier acceptance.
