# Production Compiler Integration Gate

Max Rupplin - MEARVK LLC - 2026

## Current status

The root `/compiler` project is an experimental C++20 subset compiler. It is not the production SLeeLa frontend and must not replace `sleelvac`, the established `/impl/frontend` pipeline, or SLVM dispatch. The SLeeLa classes in `/lib/compiler` define a source-level workbench and native-backend contract; they do not, by themselves, implement a native bridge to production IR.

## Preconditions

- [ ] Identify and document the production frontend's stable typed/verified IR representation, ownership, lifetime, diagnostics, and version metadata.
- [ ] Define a narrow adapter API that consumes only verified IR; reject missing or mismatched syntax-version metadata.
- [ ] Keep experimental compiler dispatch opt-in and disabled by default.
- [ ] Do not run generated artifacts automatically as a side effect of compilation.
- [ ] Preserve caller-controlled output paths and fail closed on invalid source, invalid IR, missing toolchains, and output errors.

## Required parity suite

Run the same fixture corpus through the established frontend and any candidate adapter. Compare:

1. Acceptance and rejection, including source locations and diagnostics.
2. Signed integer boundaries, overflow, division by zero, and evaluation order.
3. Local-variable binding and invalid IR rejection.
4. Emitted bytecode/native source and runtime outcomes where formats are comparable.
5. Existing `/impl`, SLVM, `/lib`, and operating-system build/test suites.

## Rollout

1. Implement the adapter as a separate target; do not alter default compiler selection.
2. Add regression fixtures for every behavior difference found during parity review.
3. Run Linux, Windows, and macOS CI plus the repository's existing verification manifest.
4. Enable an explicit experimental flag only after all required checks pass.
5. Retain immediate rollback to the established frontend until a release has completed regression testing.

## Exit criteria

Production integration is complete only when the adapter is implemented, its typed-IR contract is documented, parity tests pass, and a reviewed change intentionally enables the new path. Documentation-only interfaces and the experimental subset compiler do not satisfy these criteria.
