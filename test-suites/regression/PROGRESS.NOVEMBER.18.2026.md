# Regression and converter validation: session progress (18 November 2026)

**Repository:** `mearvk/SLeeLa`  
**Master baseline:** `c00fd2f3ed760a2a840cecd02a02daec032b6453`  
**Session status:** documentation updated; execution gates remain pending  
**Production decision:** HOLD

## Session summary

This session followed the Java signature-index and JDK 28 campaign merge. It reviewed the current regression records, converter status, Java specification refusal list, and production release qualification report, then updated the regression documentation so historical counts are not mistaken for current validation.

### Completed repository changes

- PR #375 was already merged to master before this session. It contains the signature-index, generic metadata, inherited implementation, constructor compatibility and JDK 28 campaign scaffolding.
- A production release qualification report was prepared separately in PR #376. At the time of this update PR #376 remained open and was not assumed to be part of master.
- Updated root `REGRESSION.md` with the November 18 Java converter/JDK 28 session, evidence status, refusal categories and next regression sequence.
- Updated this regression suite README to link the session status and continuation note.
- Updated `docs/reference/JAVA.CONVERSION.STATUS.md` with current master baseline, merged work, current unrun gates, historical measurement caveats and release hold.
- This progress record and the session continuation note capture what was done and how to resume.

## Evidence ledger

| Area | Latest evidence | November 18 status |
|---|---|---|
| Language-wide regression | 211 tests, 0 failures, 206 s (2026-11-17) | Historical; not re-run |
| Fast regression | 198 tests, 0 failures, 113 s (2026-11-17) | Historical; not re-run |
| ASan/UBSan native suites | 17 tests, 0 failures (2026-11-17) | Historical; not re-run |
| Mutation testing | 85 injected defects detected (2026-11-17) | Historical; not re-run |
| JDK 25 Java differential | 32/32 checks, 22 positive, 9 refusals, stale-output check (2026-11-13) | Historical; not current model validation |
| JDK 25 selected whole-tree conversion | 420/884; 426/884 with `--single-thread` (2026-11-13) | Historical; not compile or execution coverage |
| Current master runtime build | No current log | NOT RUN |
| Current C++ converter build/self-check | No current log | NOT RUN |
| Current Java positive/refusal differential suite | No current log | NOT RUN |
| SLeeLa-native per-member stub and repeat-run tests | No current log | NOT RUN |
| JDK 28 campaign artifacts | No completed artifact inspected | PENDING |
| JDK 28 functional equivalence | Stubs/envelopes remain nonfunctional contracts | PARTIAL |
| Hosted status for PR #375 merge commit | No workflow runs/statuses exposed at inspection | UNVERIFIED |

No historical result is promoted to a November 18 pass.

## Open refusal and behavior categories

The Java specification still lists generic target inference, wildcard capture, intersection LUB, generic constructors, unknown functional-interface targets, nested capture chains, local classes, qualified inner allocation, switch type/record patterns, sealed exhaustiveness, static initializer blocks, unsupported inherited Object methods, boxed null and wrapper arrays, multithreaded synchronization/volatile semantics, and native methods without a verified ABI binding.

The C++ converter refuses a whole file if a required construct cannot be lowered. The SLeeLa-native converter preserves supported siblings and emits fail-loud per-member stubs for unsupported members. Keep refusal counts, stub counts and implemented behavior as separate metrics.

## Next steps

1. Check out exact `master` and record `git rev-parse HEAD`; verify it equals the intended release candidate.
2. Regenerate the checkout-local SHA-256 manifest and important-file manifest; run integrity verification.
3. Build the SLeeLa runtime and C++ converter; retain full stdout/stderr and exit status.
4. Run converter self-check and generated signature/result-type row checks.
5. Run every positive differential fixture and every refusal fixture. Save the exact refusal diagnostic and prove no stale output is published.
6. Run the SLeeLa-native per-member stub-policy test and transactional repeat-run/provenance tests.
7. Execute representative converted Java programs and compare Java reference output with SLeeLa output byte-for-byte.
8. Run the JDK 28 reflection, signature-index, cross-version API, conformance and functional-equivalence campaign. Archive all artifacts.
9. Run the general regression suite, sanitizers and native portability matrix. Report Linux, Windows 10+ and macOS separately.
10. Update counts only from raw current-run artifacts. Do not mark production ready while any required gate is pending or any placeholder is counted as behavior.

## Session boundary

Repository documentation and pull-request records were updated using the GitHub repository interface. This session did not have a local repository checkout or shell execution path. No compiler build, regression command, converter run or representative program execution was performed in this session.

## Post-merge correction

The production qualification report PR [#376](https://github.com/mearvk/SLeeLa/pull/376) was subsequently merged at `d2b766c7280b1fb8d8b14ceb95bb84bffd1665c5`. The regression/session-record PR [#377](https://github.com/mearvk/SLeeLa/pull/377) was then merged at `9835bb53591cb27e590a4cf08be959682059f862`. Earlier statements in this progress record that #376 remained open describe the state at the time they were written and are superseded by this addendum.

No validation gate is promoted to PASS by those documentation merges. Build, converter, differential, transactional/provenance, representative-execution, JDK 28, sanitizer and platform evidence remain pending/unverified.


---

## Focused `sleelactl` Java → SLeeLa converter review (18 November 2026)

**Reviewed repository:** `mearvk/SLeeLa`  
**Master HEAD observed:** `360e830b5e5ad09009acafb3e185d4e93d865a28`  
**Primary implementation:** `sleelactl/convert/` (C lexer/model loader and C++17 translator)  
**Shared semantic model:** `impl/subjects/conversion/conversion.model`  
**Normative Java tier:** `lib/java/JAVA.SPEC.md`  
**Review scope:** remaining converter implementation and proof obligations, separate from the ongoing full CI campaign.

### Assessment

The C++ `sleelactl/convert` path already has substantial infrastructure: whole-project source collection, two-pass signature/body translation, model-backed primitive and conversion rules, partial generic and overload resolution, imports/accessibility checks, lambda/method-reference lowering, nested/inner type lowering, whole-directory staged publication, ownership tracking and provenance output.

The README describes directory-level transactional publication and provenance generation as implemented. This review does **not** treat those descriptions as verified behavior: the corresponding current-master clean build, self-check, expanded differential fixtures, fault-injection and repeat-run tests still need completed evidence. Likewise, the historical 32/32 differential result is not proof for the newer generic, overload, import and transaction changes. The README currently identifies 44 positive differential fixtures and 22 refusal fixtures; it explicitly says the newer fixtures and resolver changes have not yet been run.

The initial production service should remain the **C++ `sleelactl` converter**. Completing the separate SLeeLa-native converter is not a prerequisite for establishing this service's correctness. Keep its per-member fail-loud stub contract separate from the C++ converter's whole-file fail-closed contract.

### Remaining implementation work — ordered

1. **Finish Java generic constraint resolution.**
   - Target-type inference from assignment, field-initializer and return contexts.
   - Wildcard capture, including lower-bound constraints and capture conversion.
   - Interface-intersection least-upper-bound inference and intersection bounds.
   - Generic constructor inference and complete class-level bound validation.
   - Remaining unresolved class-generic receiver contexts.
   - Keep unresolved constraints explicitly refused; never guess a type.

2. **Close project-wide type identity and lookup gaps.**
   - Replace or complete the simple-name-keyed declaration store with package-qualified type identity so same-named declarations in different packages can coexist.
   - Complete nested-type resolution in import, inheritance, member and expression contexts.
   - Finish static nested-type wildcard imports and the remaining protected-field receiver/accessibility rules.
   - Preserve deterministic ambiguity diagnostics when more than one legal candidate remains.
   - JPMS module readability is outside the current source resolver and should remain explicitly unsupported until modelled.

3. **Complete overload and invocation semantics.**
   - Prove the full JLS applicability phases: strict invocation, loose invocation (boxing/unboxing), then variable arity.
   - Complete most-specific selection, generic method inference, inherited candidates, bridge/erasure collisions and ambiguity handling.
   - Add differential fixtures for null/reference overloads, primitive widening versus boxing, generic bounds, varargs, inherited overloads and method references.
   - README lists the newer overload/generic/import fixtures as not yet verified; run them before treating the claimed partial support as production evidence.

4. **Finish nested executable scopes and capture lowering.**
   - Nested lambda/anonymous-class capture chains (capture through an enclosing generated class).
   - Local classes with lexical scope and capture environments.
   - Qualified inner allocation, especially `outer.new Inner(...)`, with correct enclosing-instance evaluation.
   - Resolve ambiguous nested simple names using full lexical/package identity rather than refusing all collisions where Java resolution is unambiguous.

5. **Implement remaining Java syntax and initialization rules.**
   - Switch type patterns and record patterns, including per-arm binding scope.
   - Sealed-type exhaustiveness and permitted-subclass handling.
   - Static initializer blocks through a guarded, ordered class-initialization state machine that runs on Java active use.
   - Keep unsupported preview/newer syntax fail-closed until its semantics and fixtures exist.

6. **Close semantic parity gaps that can currently produce different behavior.**
   - Java `Object` identity methods (`equals`, `hashCode`, `getClass`) for generated objects, including runtime class names; keep `wait`, `notify` and `clone` refused unless fully supported.
   - Record `equals`/`hashCode` for floating-point and reference components.
   - Nullable boxed values and wrapper arrays without collapsing Java null/reference identity into primitive storage.
   - Runtime-aware text conversion for objects held in erased generics.
   - Java binary nested-class names in exception text (`Outer$Inner` versus generated `Outer__Inner`).
   - Runtime `String.format` support for non-literal formats and remaining conversions/indexes.
   - Null dereference and stack-overflow behavior; currently documented as fatal rather than Java exceptions.
   - Real multi-threaded `synchronized`/`volatile` semantics, or retain and clearly enforce the single-thread-only contract.
   - Native declarations only when an exact owner/signature/ABI binding is modelled and verified.

7. **Prove the transaction and provenance contract.**
   - Test whole-run staging and atomic publication, including failures during conversion, manifest/report write, old-output rename and final publish; prove the previous generation is restored or left intact.
   - Verify ownership-manifest cleanup removes only converter-owned stale files and preserves unowned files.
   - Verify symlinks and special files are rejected without following them; test path traversal, duplicate output paths and collisions.
   - Verify provenance hashes cover exact immutable inputs, model bytes, converter sources and emitted outputs, with deterministic output and no timestamps/absolute paths.
   - Run repeat conversions and changed-input/refusal runs; prove idempotence, unchanged-file mtime preservation and absence of stale generated files.
   - Add fault-injection tests for every publication boundary. Treat implementation descriptions as unverified until these tests pass on the exact candidate.

### Required proof after the current CI campaign

- Build the runtime and `sleelactl/convert` cleanly from one exact `master` SHA; retain full logs and exit codes.
- Run `javaconv --self-check` and model/signature/result-type index checks.
- Run the complete current positive fixture set (README: 44 fixtures) and refusal set (README: 22 fixtures), including the newer generic, overload, import and method-reference cases. Record accepted, refused, compiled and behaviorally verified counts separately.
- Run the transaction/provenance/repeat-run and fault-injection tests described above.
- Run representative multi-file projects through conversion, SLeeLa checking and execution; compare stdout, stderr, exit status and relevant exception behavior against the reference JDK.
- Record every remaining refusal with source location and construct category. Do not turn a refusal into generated output merely to increase conversion counts.
- Refresh the selected JDK whole-tree measurement only after the focused differential suite passes; conversion acceptance is not equivalent to compilation or execution.
- Update this progress record and `docs/reference/JAVA.CONVERSION.STATUS.md` from raw artifacts. The production decision remains HOLD until the required gates have actual passing evidence.

### Explicitly not a blocker for the initial C++ converter service

- Full completion of `lib/java/converter` (the SLeeLa-native converter) is a separate workstream. Its per-member stubs must continue to be counted separately from implemented and behaviorally verified members.
- Full JDK 28 library behavioral coverage is a broader qualification milestone, not a reason to weaken the C++ converter's fail-closed behavior.
- The current full CI campaign is understood to be running per the user's update; this review records converter work items and does not duplicate or reinterpret those running CI results.

### Session boundary

This was a repository-document and implementation-scope review through the GitHub repository interface. No local shell, compiler invocation or converter test was run as part of this focused review. The list above is a prioritized implementation/proof backlog, not a claim that these items were newly fixed or tested.
