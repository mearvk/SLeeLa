# Java converter main-work continuation — 18 November 2026

## Repository baseline

- Repository: `mearvk/SLeeLa`
- Integration branch: `master`
- Inspected master HEAD: `a5a4feb825657b4eacba6ddef73b6f3d7b5d6c3a`
- CI/build/test execution: intentionally deferred by user. Nothing in this note is a test-pass claim.
- Production qualification: **HOLD** until clean builds, differential execution, JDK 28 evidence, and platform gates are completed.

## Work confirmed merged on master

- PR #371 — generic method target constraints, declared method bounds, common-reference inference, invocation conversion corrections, duplicate-type ambiguity handling.
- PR #373 — cross-file generic inheritance substitution fixtures, bounded inference/boxing coverage, unboxing and wrapper widening coverage, varargs most-specific selection, differential harness pipefail/summary fixes.
- PR #374 — package metadata and initial package/import/accessibility handling; same-package, explicit and wildcard imports, nested imports, visibility enforcement, and explicit refusal of unsupported module/annotation/switch-pattern constructs.
- Subsequent master commits cover generic constructor class substitution and qualified top-level/nested constructor allocation names. The latest inspected commit documents the qualified allocation support.

These changes are source/documentation changes only for this continuation; do not infer runtime correctness from merge status.

## Unmerged lower-bound inference work

PR #372, `Complete lower-bound generic inference and strengthen project resolution`, remains open and unmerged at the time of inspection.

Its head is `1ddbf6cdb622f20d84bc8316d396e477b6bc8940`, based on `ff9ba9b1b3cfa66ffa3c09fdd0833e3042197261`. Compared with current master `a5a4feb825657b4eacba6ddef73b6f3d7b5d6c3a`, its branch is **19 commits ahead and 463 behind**. Do not merge the stale branch wholesale. Its patch contains potentially useful, still-unmerged changes for:
- deferred constraints from direct `? super T` wildcard arguments;
- class type-parameter upper-bound retention and validation;
- explicit package import resolution against the project index;
- duplicate declarations within a source unit;
- positive and fail-closed differential fixtures.

Port/reconcile those changes selectively onto current master after reviewing the current resolver. Preserve newer master changes and do not claim the PR's workflow state as current verification.

## Remaining main-work sequence

1. **Canonical package identity:** migrate project symbols beyond a simple-name-only key so `a.Widget` and `b.Widget` can coexist. Update all type, inheritance, nested-type, import, constructor, and member lookup paths to use canonical identity. Keep ambiguous unresolved names fail-closed.
2. **Finish generic inference:** transplant and reconcile PR #372 lower-bound work; add full constraint reduction for nested wildcard formulas, capture conversion, target typing and supported intersection bounds. Do not approximate unsupported intersections.
3. **Generic constructors:** implement diamond inference and class/constructor type-variable constraints, including target-type constraints. Keep anonymous-class explicit type arguments refused until implemented equivalently.
4. **Invocation resolution:** verify JLS phase ordering, fixed arity before varargs, most-specific selection, bridge/override collapse, generic substitution and accessibility together. Extend method-reference and lambda target typing from the resolved functional-interface signature.
5. **Native converter parity:** inspect the SLeeLa-native converter separately. Its per-member fail-loud stub contract is not the C++ converter's whole-file fail-closed contract. Add project-wide overload-aware resolution without conflating stubs with implemented methods.
6. **Transactional/provenance safety:** preserve whole-run staging, publication, provenance validation, repeat-run determinism and stale-output cleanup while extending semantic resolution.
7. **Evidence and release:** after the user releases the CI hold, run builds, model/index checks, positive and refusal differential suites, representative program execution, JDK 28 campaign, sanitizer/regression and Linux/Windows 10+/macOS qualification. Record measured outcomes only.

## Known limitations to keep explicit

- The project class index is still simple-name keyed; duplicate simple names across packages are not independently addressable in every context.
- Full wildcard capture, nested lower-bound formulas, and intersection least-upper-bound representation remain incomplete.
- Generic diamond/constructor inference and some class-generic contexts remain refused.
- Static nested-type wildcard imports, module readability, and some protected/nested/private access edges remain incomplete.
- Functional-interface target collection and nested generated-class capture chains remain incomplete.
- SLeeLa-native project-wide overload-aware resolution remains separate work.

## Rules for this continuation

- Push completed, reviewable changes to `master` when ready.
- Do not start CI, builds, or test campaigns until explicitly requested.
- Do not report fixtures as passes merely because they were added.
- Keep the C++ whole-file refusal counts, SLeeLa-native stub counts, implemented-member counts, and behaviorally verified counts separate.


## Incremental implementation completed in source — qualified type fail-closed guard

A focused resolver correction was prepared on branch `work/java-qualified-type-fail-closed-2026-11-18`: exact package-qualified matches retain their existing accessibility/duplicate checks, while a nonmatching qualified name whose simple suffix exists in the project index now raises `[type-qualified-not-found]` rather than falling back to that unrelated simple-name declaration. A differential refusal fixture checks javac rejection, the converter diagnostic, and absence of generated SLeeLa files. This reduces accidental cross-package binding but **does not implement canonical package-qualified symbol storage or allow same-simple-name package declarations to coexist**. No build/test/CI was run per the user's hold.
