# SLeeLa Production Release Qualification Report

**Assessment date:** 2026-11-18  
**Repository:** `mearvk/SLeeLa`  
**Baseline:** `master` at `c00fd2f3ed760a2a840cecd02a02daec032b6453` (JDK 28 signature-index and validation-campaign merge)  
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
