# Syntax 1.9 GC-Hint API

## Forms

The syntax 1.9 regression fixtures document these assignment forms:

```sleela
target = gc 80;
target = mem 25;
target = gc;
```

The forms are contextual: `gc` and `mem` remain ordinary identifiers outside the whole right-hand side of a recognized hint assignment. The bare `gc` form is documented by the fixtures as a default-aggressiveness request.

## Contract

- The optional numeric aggressiveness is in the inclusive range **0–100**.
- Values outside that range must produce a semantic error, not be silently clamped.
- The hint requires syntax **1.9**; use under syntax 1.8 must be rejected.
- The assignment target must satisfy the compiler's in-scope target rules.
- A hint does not replace ordinary object-lifetime rules and must not free a still-reachable object.
- The request is not a promise that a particular byte count will be reclaimed.

## Implementation boundaries

This is the language-level hint contract. It is not the same interface as `gc_collect_young` / `gc_collect_full` in the native collector, nor the native memory manager's tracked allocation API. The runtime decides how to honor a valid hint safely.

## Regression evidence

See [test matrix](TEST-MATRIX.md) and the canonical [syntax 1.9 fixture README](../../../tests/sleela-syntax-1.9/README.md).
