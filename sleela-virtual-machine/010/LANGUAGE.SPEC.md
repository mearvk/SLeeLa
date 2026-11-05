# SLVM/10 — Authoritative SLeeLa Language Specification

This VM must interpret language-level type and source metadata according to the repository-wide [SLeeLa syntax specification](../../SLEELA.syntax). The current normative language edition is **Syntax 3.0**. The shared compiler frontend accepts declared source syntax **1.0 through 3.0** as defined by [`impl/frontend/version.h`](../../impl/frontend/version.h). Each source file's `#sleela X.Y` declaration and every historical feature-specific minimum gate remain in force. The SLVM generation number is independent of the language syntax edition; this shared document does not assert that every generation implements every language feature. If a feature or artifact operation is unsupported by a selected VM generation, it must be rejected explicitly rather than silently misinterpreted.

## Unsigned integer source signatures

- Canonical family: `U<n>`, where `1 <= n <= 1048576`.
- Examples: `U1`, `U8`, `U16`, `U32`, `U64`, and `U1048576`.
- Each type represents exactly `n` unsigned value bits, with range `0..(2^n - 1)`.
- Preserve the declared width and unsignedness across parsing, compiler lowering, bytecode/module metadata, VM loading, execution, serialization, and diagnostics.
- Never silently substitute a signed integer or a host 64-bit integer for a wider type. If a VM version does not yet implement an operation or ABI path for a width, it must reject that path explicitly rather than misexecute it.

The canonical source-signature contract is [`markdown/SOURCE.md`](../../markdown/SOURCE.md); the C/C++ arbitrary-width implementation and focused tests currently live in [`impl/frontend/unsigned_integer.h`](../../impl/frontend/unsigned_integer.h) and [`impl/tests/unsigned_integer_test.cpp`](../../impl/tests/unsigned_integer_test.cpp). These references are shared across SLVM/1 through SLVM/11 so that VM-specific implementations do not fork the language definition.

## Conformance requirement

Treat `SLEELA.syntax` at the selected repository revision as normative. Before claiming full support, each VM implementation must run conformance tests for version acceptance, width validation, arithmetic overflow/underflow, division by zero, bitwise operations, shifts, serialization, and boundary widths including `U1` and `U1048576`. Documentation visibility alone is not proof of end-to-end runtime support.
