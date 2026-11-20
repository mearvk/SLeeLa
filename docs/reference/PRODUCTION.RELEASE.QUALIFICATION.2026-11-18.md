# SLeeLa Production Release Qualification Report

## Verification refresh — 2026-11-20 (master `7126f6c6c`), measured

**Decision remains: HOLD.** Unlike the refreshes below, this one records commands that were actually run, in a full
checkout of `master` on Linux x86-64 (Amazon Linux 2023, AMD EPYC 9R14, GCC 11.5.0, Corretto 25.0.5.1). JDK 28 is not
installed in that environment, so the JDK 28 gates could not be run.

| Gate | Result |
|---|---|
| 1. Master baseline | Confirmed: `7126f6c6c`. |
| 2. SHA-256 integrity and provenance | **PASS.** `cd impl && make check-manifest` OK against the committed manifest; `tools/verify-before-execution.py` PASS (18,673 items) on a stock checkout. Converter provenance (`provenance` check) PASS. |
| 3. Runtime and converter builds | **PASS.** `make -C impl build/sleela build/sleelvac build/nordshrift` and `make -C sleelactl/convert`, no warnings. Rebuilds are byte-identical to `working-binaries/` (`SLEELA-BIN-1.0122`, `NORDSHRIFT-BIN-1.0121`, `JAVACONV-BIN-1.0121`). |
| 4. Conversion model and JDK 28 signatures | Model **PASS**: `javaconv --self-check` (model `java-to-sleela` 1.6.0, 59 refusal rows), `generate_ret_rows.py --check` up to date (8,922 rows). JDK 28 signatures **NOT RUN** (no JDK 28). |
| 5. Differential and refusal suites | **PASS: 116 of 116** (`tests/javaconv_differential_test.sh` with `impl/build/sleela`): 55 positive programs byte-identical to Corretto 25.0.5.1, 38 refusal fixtures fail closed with no output, 9 multi-file projects byte-identical to the JDK, 9 project-level refusals, and the stale-output, provenance, repeat-run, transaction and model-registry checks. CI `java-convert.yml` succeeded on master `7fbcdd0ae`, `69716e1bd`, `4fea26aba` and `3679be471`. |
| 6. Native per-member policy and transactions | **PASS.** `lib/java/converter/tests/per-member-stub-policy.sh` (supported sibling kept, unsupported member fails loudly, report counts stubs) and `repeat-run-transaction.sh` (provenance, identical bytes/paths/mtimes, failed run keeps the prior generation). |
| 7. Representative Java execution | **PASS for the fixtures in gate 5.** Whole-tree: `java.base` subset (884 files) 262 converted; all of `java.base` (3,344 files) 862 converted. Link-time check (`tests/javaconv_check_tree.sh`): subset 22 errors in 11 of 267 emitted files (measured on the 259-file generation); all of `java.base` 148 errors in 53 of 885 files on an intermediate generation; **not re-run on the final generation**. |
| 8. JDK 28 and platform qualification | **NOT RUN.** No JDK 28; Linux x86-64 only. Windows and macOS not executed. |

### Closure criteria (section 7) at `7126f6c6c`

| Criterion | State |
|---|---|
| Clean runtime build, compile output archived | Partly: built (gate 3); the log is not archived in the repository |
| C++ converter builds against the model; strict self-check passes | **Met** (model 1.6.0) |
| SLeeLa-native converter builds and consumes the shared model consistently | Partly: it runs and passes its policy and transaction tests (gate 6); consistency with the C++ converter is not measured |
| All positive fixtures byte-identical | **Met** (55 programs, 9 multi-file projects) |
| All refusal fixtures fail closed | **Met** (38 fixtures, up from the 22 this report expected) |
| Per-member stub policy, transactional repeat-run and provenance | **Met** (gates 5 and 6) |
| JDK 28 reflection/index campaign | Not run (no JDK 28) |
| Signature collisions enumerated | Not run (part of the JDK 28 campaign) |
| Functional-equivalence gaps implemented or excluded; stubs never counted as implemented | Partly: the C++ converter refuses calls into `/lib` signature stubs and reads of placeholder fields (`[library-stub]`), so stubs are not converted as if implemented. The accepted-but-different cases of section 5.3 were not re-checked |
| JDK 25/28 API differences reviewed | Not run (no JDK 28) |
| Linux, Windows 10+, macOS on real hosts | Partly: Linux x86-64 only |
| README, status, converter README and this report agree on dated counts and commit | **Met at this refresh**: root `README.md`, `JAVA.CONVERSION.STATUS.md`, `lib/java/JAVA.SPEC.md` §7, `sleelactl/convert/README.md` and this report state the counts above |
| Reproducible artifacts, matching manifests, rollback recorded | Partly: byte-identical rebuilds and a matching manifest; no rollback instructions recorded |

Summary: 5 of 13 criteria met, 5 partly met, 3 not runnable without JDK 28. The decision stays **HOLD**. The next
steps are a JDK 28 environment for the campaign gates, the section 5.3 runtime cases, Windows and macOS hosts, and a
link-time check of the final generation.

Speed (`sleelactl/convert/PERFORMANCE.md`): `javaconv` converts the 884-file subset in 60 s; converted programs run
correctly but pay SLeeLa start-up of about 3.5 s per run (1.9 s of it the SHA-256 gate); steady-state compute speed
is not yet measured.

## Verification refresh — 2026-11-18 (master 3478f4856d04)

**Decision remains: HOLD.** This refresh records repository and workflow metadata inspected after the earlier report baseline; it does not claim that queued jobs passed.

| Gate | Latest observed result |
|---|---|
| 1. Master baseline | Confirmed: `3478f4856d044b615ff2e999ca5730a80eedd394`. |
| 2. SHA-256 integrity and provenance | UNVERIFIED: no regenerated current-checkout manifest or provenance artifact inspected. |
| 3. Runtime and converter builds | UNVERIFIED: no completed current-master build logs inspected. |
| 4. Conversion model and JDK 28 signatures | PENDING: no completed self-check or JDK 28 campaign artifact inspected. |
| 5. Differential and refusal suites | PENDING: Java converter and regression workflow jobs remain queued. Historical 32/32 is not current evidence. |
| 6. Native per-member policy and transactions | UNVERIFIED: no completed policy, repeat-run, transactional-publication, or provenance test results inspected. |
| 7. Representative Java execution | UNVERIFIED: no current-baseline Java-reference/SLeeLa output comparisons inspected. |
| 8. JDK 28 and platform qualification | PENDING: JDK 28, Linux, Windows 10+/MinGW-w64, and macOS jobs are queued/pending, not passed. |

### Workflow and branch evidence

PR [#378](https://github.com/mearvk/SLeeLa/pull/378) remains open and unmerged. Its head is `6f7fc49723eed52dfbbe9827c047944c1787e43c`; its recorded base is `622d28ce4912d2002689032f3a5a7f7a8c365aa3`. Comparing that branch with current master reports **4 commits ahead / 9 behind**, with only four workflow files changed: `.github/workflows/java-convert.yml`, `.github/workflows/jdk28-validation.yml`, `.github/workflows/regression-set.yml`, and `.github/workflows/root-build-install.yml`. Do not merge it without reconciling the divergence.

Observed runs, all on PR head `6f7fc497...` rather than current master:
- [Native C/C++ portability](https://github.com/mearvk/SLeeLa/actions/runs/44405716080): Linux, Windows 10+/MinGW-w64, macOS jobs queued.
- [Java converter](https://github.com/mearvk/SLeeLa/actions/runs/44405716173): differential job queued.
- [Regression set](https://github.com/mearvk/SLeeLa/actions/runs/44405715950): sanitizer and Linux/macOS/Windows jobs queued.
- [JDK 28 validation](https://github.com/mearvk/SLeeLa/actions/runs/44405715986): API/converter job pending.

No workflow run was found for the inspected current master SHA at the time of this refresh. The branch tip and run states are point-in-time observations; rerun or recheck before release. No build, test, or representative program execution is claimed by this documentation update.


**Assessment date:** 2026-11-18  
**Repository:** `mearvk/SLeeLa`  
**Source baseline inspected:** `3478f4856d044b615ff2e999ca5730a80eedd394` (master tip at verification time; subsequent commits in this update are documentation-only).
**Decision:** **HOLD — not production-qualified**  
**Scope:** Java-to-SLeeLa conversion, Java API signature generation, JDK 28 validation, compiler acceptance, representative execution, refusal accounting, and release evidence.

## 1. Executive decision

The current source tree contains substantial Java conversion and API-index work, but the available evidence does not justify a production release.

The shared conversion model is revision 1.5.0. The November 18 merge added deterministic erased-signature grouping, generic metadata preservation, inherited interface/superclass implementation checks, constructor compatibility checks, JDK 28 reflection inputs, and a cross-version API comparison campaign. The merge itself explicitly records that validation remains pending and that generated API stubs are not functional Java implementations.

**No new compile or representative-program execution is claimed in this report.** The GitHub repository interface available during this assessment can inspect and edit repository content and inspect some workflow metadata, but it does not provide a local shell or a workflow-dispatch operation. No checkout of the repository was mounted in the execution environment. Consequently, running `make`, `javac`, `sleela run`, or the JDK 28 campaign here was not possible. The relevant current-master commands and acceptance criteria are recorded below so the run can be performed without substituting historical results.

The report distinguishes:
- **Historical PASS:** a result documented from a prior dated run.
- **PENDING:** source or test exists, but no current-baseline result is available.
- **PARTIAL:** the declaration/contract exists, but behavior is not implemented or demonstrated.
- **REFUSED:** the converter deliberately rejects a construct.
- **BLOCKED:** a required run or release prerequisite has not been completed.

## 2. Baseline and change inventory

The baseline is the merged master commit `c00fd2f3ed760a2a840cecd02a02daec032b6453`.

The merged JDK 28 work includes:
- deterministic Java member signature indexing by SLeeLa-erased signatures;
- retained Java generic type-parameter metadata using `@generic`;
- collision grouping/reporting instead of silently treating erased overloads as distinct;
- array/varargs erasure and primitive/String distinction;
- superclass and interface inheritance traversal when determining implemented methods;
- constructor compatibility checks based on parameter kinds rather than arity alone;
- JDK 28 reflection generation with visible reflection errors rather than silently swallowed exceptions;
- JDK 25-to-JDK 28 signature comparison and validation-campaign scaffolding;
- preservation of the documented per-member stub policy in the SLeeLa-native converter.

These are implementation changes, not proof that the current code builds or that all generated Java API members execute correctly.

## 3. Current compile and execution record

### 3.1 Commands required on a clean checkout

Run from the repository root on Linux with the documented native dependencies and JDK 28 installed:

```sh
python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json
make -C impl build/sleela
impl/build/sleela version

make -C sleelactl/convert
sleelactl/convert/build/javaconv --self-check
python3 impl/subjects/conversion/generate_ret_rows.py --check

bash tests/javaconv_differential_test.sh
bash lib/java/converter/tests/per-member-stub-policy.sh
bash lib/java/converter/tests/repeat-run-transaction.sh

make -C tests java-qualification
make -C tests jdk28-conformance
make -C tests jdk28-functional
make -C tests jdk28-signatures

bash lib/java/jdk-28/tools/run_validation_campaign.sh
```

The JDK 28 workflow in the repository is intended to build the runtime from the tested commit, regenerate the checkout-local integrity manifest, run the campaign, and upload reports even when the functional-equivalence subgate is partial or failing.

### 3.2 Execution evidence available at assessment time

| Check | Result | Evidence / qualification |
|---|---|---|
| Current master runtime compile | **BLOCKED / NOT RUN** | No current build log or local checkout was available in this assessment. |
| Current master C++ converter compile | **BLOCKED / NOT RUN** | Model 1.5.0 and recent translator changes have not been validated by an observed build here. |
| Converter model self-check | **PENDING** | Required command: `javaconv --self-check`. |
| Generated result-type row integrity | **PENDING** | Required command: `generate_ret_rows.py --check`. |
| Positive Java-to-SLeeLa differential suite | **PENDING** | Current README documents 44 positive fixtures; the converter README says later generic/boxing/overload/import fixtures have not yet been run. |
| Fail-closed refusal suite | **PENDING** | Current converter README documents 22 refusal fixtures; current-baseline results are not available. |
| Per-member stub policy | **PENDING** | Regression script exists; it must demonstrate a supported sibling is retained, an unsupported member throws, and the stub count is recorded. |
| Repeat-run / transaction / provenance | **PENDING** | Repeat-run script and provenance checks exist; no current execution log was available. |
| JDK 28 API signature campaign | **PENDING** | The merged campaign is present; no completed campaign artifact was available. |
| JDK 28 functional equivalence | **PARTIAL / EXPECTED TO FAIL** | Generated envelopes are API contracts, not Java implementations. The functional gate must not be reported as passing until real member behavior is represented and tested. |
| Windows 10+ / macOS host execution | **NOT EXECUTED** | Linux workflow evidence cannot be presented as native Windows or macOS qualification. |

### 3.3 Historical results — not current release evidence

The detailed status page records a 2026-11-13 run:
- 22 positive Java programs byte-identical to OpenJDK 25.0.2;
- 9 refusal fixtures correctly rejected;
- 1 stale-output check;
- 32/32 total checks;
- 420 of 884 selected JDK 25.0.2 `java.base` source files converted (426 with `--single-thread`).

Those results predate the November 18 model 1.5.0 consolidation and the latest generic-resolution/signature changes. They must remain labelled historical. They do not establish current compile success, tree-wide SLeeLa type-check success, JDK 28 behavioral parity, or full JDK conversion.

The root README currently says 31/31 while the detailed status page says 32/32. This discrepancy must be reconciled against the actual test output before release.

## 4. Representative programs to compile and execute

The following are the minimum representative positive cases. For each, compile/run the Java source on the selected reference JDK, convert it, run the emitted SLeeLa program, and compare stdout and stderr byte-for-byte under the suite's documented filtering rules.

| Area | Representative fixture(s) | What must be demonstrated |
|---|---|---|
| Primitive and numeric semantics | `Basics`, `Operands`, `Floats`, `Bounds` | Java overflow, narrowing, division/remainder, floating-point rounding and array bounds match. |
| Strings and text | `Strings`, `Formats`, `ObjectText` | String conversion, formatting, char-as-text and object text match. |
| Control flow | `Control`, `Flow`, `Switches`, `Resources` | switch/fallthrough, switch expressions, labels, try/finally and resource closing match. |
| Object model | `Init`, `Nested`, `Records`, `Enums` | initialization order, nested/inner types, record behavior and enum lowering match. |
| Functional constructs | `Functional`, `Anon`, `Refs` | lambda, anonymous-class and method-reference behavior match. |
| Generic resolution | `GenericIdentity`, `GenericCommonReference`, `BoundedGeneric`, `NestedGenericInference`, `ClassGenericReceiver`, `InheritedGenericReceiver`, `WildcardGeneric` | type substitution, bounds, receiver substitution, inherited generic members and wildcard behavior match. |
| Boxing and overloads | `BoxingOverload`, `BoxingUnboxingWidening`, `OverloadWidening` | strict/loose invocation phases, boxing/unboxing, primitive widening and most-specific selection match. |
| Project-wide resolution | project-index, project-generic, package-import, protected-access cases in `tests/javaconv_differential_test.sh` | cross-file declarations, package identity, wildcard/nested imports and accessibility match. |
| Transactional conversion | stale-output, repeat-run, provenance fixtures | failed conversions publish no stale file; unchanged outputs are stable; provenance matches actual inputs and outputs. |

**Required result format:** retain the raw build log, Java reference output, converted SLeeLa output, unified diff, exit code, fixture identifier, JDK version, SLeeLa commit, converter/model hashes, and host/toolchain identity. A test counts as passed only when the real commands complete and the required comparisons succeed.

## 5. Remaining refusals and unsupported behavior

### 5.1 Whole-file C++ converter (`sleelactl/convert`)

The converter's fail-closed rule is mandatory: a source file with any unsupported member writes no converted output. The README identifies 22 refusal fixtures in the current corpus. The exact per-run inventory must be taken from the 22 fixture files and the emitted diagnostics; the historical nine-refusal run must not be mistaken for a current 22-case run.

Known refusal categories in the current Java specification include:

| Refusal / diagnostic family | Remaining limitation |
|---|---|
| `untyped` / unresolved generic invocation | target-type inference, interface-intersection LUB constraints, wildcard capture, generic constructors, and unresolved class-generic contexts remain incomplete. |
| Unknown functional target | target SAM signature cannot be resolved for an uncollected or unseeded JDK interface. |
| Nested capture chain | lambda or anonymous class nested inside another generated class. |
| `local-type` / qualified inner allocation | local classes and `outer.new Inner()` lack lexical/enclosing-instance lowering. |
| Switch type/record patterns | `case Type t ->`, record patterns, and sealed exhaustiveness are not fully lowered. |
| `init-block` | static initializer blocks require guarded, lazy, ordered class-initialization state. |
| `object-method` | inherited `Object` operations such as equals/hashCode/getClass/clone/wait/notify are unavailable where no supported declaration/ancestor supplies them. |
| `boxed` / `boxed-null` | wrapper nullability and wrapper arrays cannot be represented by primitive scalar storage. |
| `synchronized` / `volatile` | concurrency semantics are not preserved; the supported simplification is explicitly single-threaded only. |
| `native` | no declared and verified Java-to-SLeeLa native ABI binding. |
| Ambiguous nested names | one simple nested type name resolves to multiple owners. |
| Syntax/model limits | unsupported Java syntax, reserved-member rewrites, and model capacity limits remain fail-closed. |

Specific negative fixtures named in the specification include `QualifiedNested`, `LocalClass`, `LambdaUse`, `NestedLambda`, `StaticInitializer`, `ObjectMethod`, `BareObjectMethod`, `BoxedDecl`, `BoxedNull`, `BoxedNullParameter`, and `BoxedNullField`. The complete 22-case list and actual diagnostics must be copied from the current `sleelactl/convert/tests/reject/` directory after execution.

The documented whole-tree JDK 25 historical first-refusal counts were: nested-ambiguous 131, nested 67, untyped 57, volatile 32, object-method 31, syntax 28, init-block 25, model 18, synchronized 16, boxed 13, native 12, and other 34. These sum to 464 rejected files out of 884 in that 2026-11-13 measurement. This is not a JDK 28 refusal census.

### 5.2 SLeeLa-native converter (`lib/java/converter`)

This is a different conversion contract. It retains supported members and emits a fail-loud `UnsupportedOperationException` stub for an unsupported member, preserving the original Java body as a comment and recording the stub count. Its output must never be described as fully behaviorally equivalent merely because the type file was emitted.

The per-member regression must prove all of the following:
1. a supported sibling method remains in the emitted type;
2. the unsupported method is represented by a fail-loud stub;
3. the report records a nonzero stub count;
4. the stub throws at runtime rather than returning a default or silently succeeding;
5. repeat runs preserve transactional publication and provenance.

### 5.3 Runtime semantic gaps

The historical status page explicitly identifies cases where conversion may be accepted but still differ from Java: erased generic object text, nested user exception naming, char values passed to APIs other than the specifically modelled StringBuilder/StringBuffer overloads, uncaught exception formatting, floating remainder by zero, null dereference, and stack overflow. These require either implementation plus differential tests or explicit fail-closed treatment. They must not be hidden in the refusal count because accepted-but-different behavior is a separate and more serious equivalence gap.

## 6. JDK 28 release gates

The JDK 28 campaign must produce and retain:
- reflected JDK 28 type/member inventories;
- signature-index and erased-overload collision reports;
- generic metadata and constructor compatibility results;
- JDK 25-to-28 API delta report;
- compiler acceptance and source-conformance results;
- positive and negative differential fixture output;
- converter provenance and repeat-run results;
- whole-run staging/publication evidence;
- a functional-equivalence report that separately counts implemented behavior, partial API contracts, fail-loud stubs, and refused members.

A successful API identity or envelope-contract check is not a functional-equivalence pass. The `--functional` audit is expected to fail while envelopes contain only generic bridge methods. Preserve that failure as an honest release diagnostic.

## 7. Release blockers and closure criteria

Production release remains blocked until all of these are true:

- [ ] Clean current-master runtime build succeeds and full compile output is archived.
- [ ] C++ converter builds against model 1.5.0; strict self-check passes.
- [ ] SLeeLa-native converter builds and consumes the shared model consistently.
- [ ] All positive fixtures compile and execute against the reference JDK and SLeeLa VM with byte-identical output.
- [ ] All 22 current refusal fixtures fail closed with the expected construct and no emitted output.
- [ ] Per-member stub-policy and transactional repeat-run/provenance tests pass.
- [ ] JDK 28 reflection/index campaign completes with zero silently omitted reflection members.
- [ ] Signature collisions are enumerated and either intentionally resolved or documented as unrepresentable.
- [ ] Functional-equivalence gaps are implemented or explicitly excluded from the release claim; stubs are never counted as implemented.
- [ ] JDK 25/JDK 28 API differences are reviewed and classified.
- [ ] Linux, Windows 10+, and macOS qualification results are recorded on actual hosts; unobserved platforms remain NOT EXECUTED.
- [ ] README, detailed status, converter README, and release report agree on the same dated counts and commit.
- [ ] Release artifacts are reproducible, integrity manifests match the final checkout, and rollback instructions are recorded.

## 8. Release conclusion

**Release status: HOLD.** The repository has meaningful converter and signature-index progress, historical positive differential evidence, a fail-closed refusal model, and a JDK 28 campaign. It does not yet have current-master compile logs, current positive/negative execution results, completed JDK 28 campaign artifacts, or complete Java API functional implementations.

Do not tag or advertise this baseline as a production Java-conversion release. The next valid release candidate is the first commit for which the commands in section 3 have completed and their raw outputs and machine-readable reports have been archived. Update the measured counts only from those outputs.


## Verification refresh — current master `0efee0beeee62f883b369c54dc55cf9f620ae308` (2026-11-18, latest inspection)

This section supersedes the earlier branch/run metadata above where it differs.

| Gate | Current finding | Qualification |
|---|---|---|
| 1. Master baseline | **CONFIRMED** — master points to `0efee0beeee62f883b369c54dc55cf9f620ae308` (`docs: clarify Java verification baseline`). | Baseline identity verified through GitHub. |
| 2. SHA-256 manifest impact | **PASS — change-impact scope only.** Comparing `3478f4856d044b615ff2e999ca5730a80eedd394` to current master shows five commits and only three changed files: `comparison/ARITHMETIC.SPEED.001.md`, this report, and `docs/reference/JAVA.CONVERSION.STATUS.md`. None are in the generator's protected source directories or explicit native-input list. | No manifest regeneration is indicated by this delta. The byte-for-byte `tools/verify-before-execution.py` run was not executed; do not describe the runtime verifier itself as passed. |
| 3. Runtime/compiler/converter builds | **NOT VERIFIED** — no completed current-master build logs were found. | No compile success claimed. |
| 4. Conversion model and JDK 28 signatures | **PENDING** — no completed current-master self-check or JDK 28 campaign artifact inspected. | Model 1.5.0 still needs execution against the current tree. |
| 5. Differential and refusal suites | **PENDING** — PR Java-converter and regression jobs remain queued; no completed current-master results. | Historical 32/32 is not current evidence. |
| 6. Native per-member policy, transactional publication, provenance, repeat-run | **UNVERIFIED** — no completed current-baseline logs/artifacts inspected. | Test scripts/contracts are not proof of execution. |
| 7. Representative Java execution | **UNVERIFIED** — no current-baseline JDK-reference versus SLeeLa output comparison inspected. | No runtime equivalence claim. |
| 8. JDK 28 and platform qualification | **PENDING** — current-master native portability run is queued; its Linux, Windows 10+/MinGW-w64, and macOS jobs are queued. JDK 28, Java differential, regression, and root-build qualification have not completed on current master. | No platform or JDK 28 qualification claim. |

### Current CI and branch state

- Current master SHA: `0efee0beeee62f883b369c54dc55cf9f620ae308`.
- Combined commit status currently exposes **no status entries** for this SHA.
- Latest native portability workflow on current master: [run 44417706501](https://github.com/mearvk/SLeeLa/actions/runs/44417706501); all three platform jobs are queued.
- Other recent master push runs inspected (including regression and portability runs on earlier commits) are also queued; none supplied a completed result for the current tip.
- PR [#378](https://github.com/mearvk/SLeeLa/pull/378) is still open/unmerged. Its head `6f7fc49723eed52dfbbe9827c047944c1787e43c` is now **4 commits ahead / 14 behind** current master. The four ahead commits change only the four workflow files `.github/workflows/java-convert.yml`, `.github/workflows/jdk28-validation.yml`, `.github/workflows/regression-set.yml`, and `.github/workflows/root-build-install.yml`. Its Java, JDK 28, regression, portability and root-build runs remain queued/pending on that stale PR head. Do not treat them as current-master results or merge without reconciling the 14-commit divergence.

### Overall decision

**HOLD — production qualification is not complete.** Gate 1 is confirmed. Gate 2 passes only the manifest change-impact review; direct digest verification remains unexecuted. Gates 3–8 lack completed current-master evidence. The repository connector does not provide a local shell/checkout or workflow-dispatch action in this session, so no local build, verifier, converter, or representative program execution is claimed.
