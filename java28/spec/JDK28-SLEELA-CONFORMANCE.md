# JDK 28 / SLeeLa Conformance

## Objective

When a SLeeLa declaration binds to a Java SE/JDK 28 type, execution should use the actual Java implementation rather than a SLeeLa approximation.

**SLeeLa syntax and control + actual Java/JDK behavior.**

## Conformance levels

1. **Type resolution** — the Java binary name resolves to the intended class.
2. **Signature resolution** — the requested constructor or method resolves with its Java parameter types.
3. **Behavioral execution** — the actual Java constructor or method executes and supplies its result or exception.
4. **Differential testing** — representative bridge calls are compared with equivalent direct Java calls.
5. **API coverage** — every supported binding has a Java binary name and an execution path through the bridge.

## Guarantee boundary

This architecture gives the strongest practical source-integration guarantee: SLeeLa does not duplicate JDK internals. Supported operations are delegated to the actual Java runtime.

Exact compatibility must still be verified against the intended JDK 28 runtime and conformance suite because the JDK 28 specification is currently draft/ad-hoc and may change.

## SLeeLa control

The binding declares what Java functionality may be requested; it does not bypass SLeeLa compiler, loader, memory, or security rules.

Unsupported, inaccessible, or non-public operations must fail rather than silently substitute another implementation.
