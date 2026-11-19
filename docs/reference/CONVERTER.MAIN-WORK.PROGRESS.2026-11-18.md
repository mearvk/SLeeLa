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


## Reassessment against current master — 2026-11-19

A fresh source inspection after merge `229d987c01e3fa39eef65d88e2d597c28930cfed` changes the disposition of the old PR #372 checklist. Several of its advertised changes are already present in current `sleelactl/convert/java_to_sleela.cpp` and must not be blindly ported:

- Method-invocation inference already records deferred lower-bound constraints for `? super T`, recursively detects method variables inside nested bound types, resolves deferred variables after argument/target inference, and checks the resulting lower-bound compatibility.
- The resolver already validates retained method type-parameter upper bounds after inference.
- `ClassInfo::typeParamBounds` is present in the public symbol model, and class/record declarations retain class bounds. Concrete type-argument validation is also present in constructor allocation and type parsing paths.
- Duplicate declarations are marked ambiguous whenever a prior declaration has populated `sourceName`, including declarations in the same source unit.
- Explicit imports are matched against indexed declarations by complete package name before returning the internal simple-name key; conflicting simple-name imports fail closed.

Therefore PR #372 remains open/stale and its branch must not be merged. Its headline work is substantially represented in current master; remaining value is limited to reviewing its exact fixtures/edge cases against current code. This is a source inspection only, not behavioral verification. The existing older paragraph above describing PR #372 as wholly unmerged lower-bound implementation is historical and superseded by this reassessment.

## Canonical package identity — implementation design constraint

Current `SymbolTable::classes` remains `map<string, ClassInfo>` keyed by internal/source simple or hoisted names. `ClassInfo::packageName` is metadata, not the primary key. On a collision, the existing entry is marked ambiguous and its metadata/member data may be overwritten as subsequent source units are collected. This is why simply adding a fully-qualified lookup alias is insufficient: the second declaration's independent `ClassInfo` would already have been lost.

The safe migration must retain independent declaration records keyed by canonical identity (package + enclosing-type path + declared type name), while maintaining a separate, explicitly ambiguity-aware simple-name index for legacy lookup. Every semantic edge must be migrated together: superclass/interface references, nested types, imports, field/method owner lookup, constructors, static imports, subtype traversal, generic substitution, and emitted internal names. Internal emitted names must be deterministic and collision-free without changing Java source identity. Until this is done, duplicate simple names across packages remain a documented refusal; do not claim they coexist.

## Current next sequence

1. Introduce canonical declaration storage without removing the legacy simple-name index; add deterministic canonical identity and collision-preserving collection.
2. Migrate exact qualified/import lookup and superclass/interface graph edges to canonical identities.
3. Migrate member/constructor lookup, nested-type ownership, static imports and accessibility to the same identity.
4. Retain fail-closed behavior for ambiguous unqualified names and add positive same-simple-name cross-package fixtures plus negative ambiguity fixtures.
5. Re-audit current lower-bound and class-bound logic against PR #372 fixtures; port only genuinely missing cases.
6. Continue generic diamond/target typing, method references/lambda resolution, then SLeeLa-native resolver parity and transaction/provenance review.

No build, test, or CI was run for this reassessment.
