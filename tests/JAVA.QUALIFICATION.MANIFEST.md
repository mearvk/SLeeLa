# Java Qualification Manifest

The qualification manifest coordinates the Java/SLeeLa source/API congruence layers as one deterministic system.

Layers: source equivalence; expression equivalence; normalized declaration/signature comparison; negative/constraint corpus; definite assignment/reachability; checked exceptions; Java API dependency closure and transitive counterpart graph; platform/reproducibility observation.

PASS means the invoked child process returned zero. FAIL means non-zero. The aggregate result is PASS only when every configured layer passes. Platform observation does not assert that remote operating systems were executed. Java SE 27 is the released specification baseline; Java 28 remains a forward compatibility target. No JVM, bytecode, SLVM, or runtime equivalence is implied.

Run: python3 tests/java_qualification_manifest.py --json qualification.json


## 0.3.21 Development Upgrade — Java Flow Qualification Closure

This upgrade makes Java control-flow qualification a first-class closure task. Qualification now requires a source fixture, normalized representation, semantic rule, positive or negative result, and unified-manifest evidence.

Required next coverage: boolean-path-sensitive `&&`, `||`, `!`, and `?:`; constant boolean expressions; break/continue joins and labels; while/do/for completion; switch completion; try/catch/finally abrupt paths; constructor and blank-final definite-unassignment; lambda capture boundaries; and checked-exception propagation.

The work remains source/API qualification and does not imply JVM, bytecode, SLVM, or runtime equivalence.


## 0.3.22-dev — Java API dependency graph

The API dependency layer now includes a deterministic graph artifact with direct
and transitive type dependencies, nested-type ownership, explicit/static imports,
fully-qualified references, wildcard package edges, package/module metadata,
unresolved references, and duplicate counterpart paths. Its isolated regression
suite is `tests/java_api_dependency_graph_suite.py` and is also invoked by the
existing API dependency suite. Graph resolution is not behavioral or member-level
equivalence evidence.


## 0.3.23-dev — Member-level API dependencies

The member dependency auditor now extracts field, method return/parameter,
constructor parameter, generic-bound, inheritance, declared-exception, and
declaration-annotation type references. Every dependency edge records the
declaring source type, member, kind, role, target, and line. Missing explicit
Java namespace imports are reported.

Regression coverage is provided by
`tests/java_api_member_dependencies_suite.py`, invoked through the existing
Java API dependency suite and directly by
`make -C tests java-api-member-dependencies`.

This remains static dependency evidence. Effective inherited-member contract
comparison, generic substitution, override compatibility, and behavioral
equivalence remain outstanding.


## 0.3.24-dev — Visibility and constructor inheritance

The visibility auditor classifies declared members and computes the
source-level inherited member set using public, protected, package-private,
and private rules. The constructor report explicitly distinguishes
non-inheritance from accessible superclass constructor invocation options;
implicit default constructors are modeled when no constructor is declared.

Regression suite: `tests/java_api_visibility_constructor_suite.py`.
Direct target: `make -C tests java-api-visibility-constructor`.
The suite is also invoked by `tests/java_api_dependency_suite.py`.

This is conservative source-level evidence, not complete JLS access-control
or behavioral equivalence proof.


## Java source implementation depth (0.3.25-dev)

A separate implementation-depth audit is available at
`tests/java_source_implementation_audit.py`, with synthetic regression
coverage in `tests/java_source_implementation_audit_suite.py`.

It classifies method bodies into generic dispatch placeholders, empty bodies,
constant placeholders, accessor/field-copy bodies, and substantive-looking
candidates. This provides a measurable baseline before translating API
representations into executable SLeeLa behavior.

Direct target: `make -C tests java-source-implementation-audit`.

The audit is deliberately not treated as behavioral equivalence evidence.
Substantive-looking methods still require semantic review and Java-contract
conformance tests. The source implementation program begins with foundational
`java.lang` and `java.util` behavior and expands by dependency order.


## SLeeLa-native java.lang implementation slice (0.3.26-dev)

Added implementation sources:

- `lib/java/implementations/java/lang/JavaObjectIdentity.sleela`
- `lib/java/implementations/java/lang/JavaStringValue.sleela`

Added executable VM fixtures and runners:

- `tests/java_object_identity_native.sleela`
- `tests/java_object_identity_native_suite.py`
- `tests/java_string_native.sleela`
- `tests/java_string_native_suite.py`

Direct target: `make -C tests java-lang-native`.

These are intentionally partial implementation units. Object identity depends
on stable VM heap/object handles and does not yet implement monitor operations,
wait/notify, clone, or default toString. The String slice implements scalar
value equality, concatenation, non-negative repetition, and array joining;
UTF-16 indexing, code-point behavior, Java hashCode, regex, normalization,
case mapping, and exception semantics remain unimplemented. Runtime suites
were added but not executed in this change.
