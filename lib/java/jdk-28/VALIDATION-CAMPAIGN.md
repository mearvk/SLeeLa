# JDK 28 validation campaign

Run from the repository root:

    make -C impl build/sleela
    SLEELA_BIN="$PWD/impl/build/sleela" bash lib/java/jdk-28/tools/run_validation_campaign.sh

The campaign separates these evidence classes instead of treating an API
envelope as an implementation:

1. JDK 28 inventory and index integrity.
2. Generic signature-index unit tests and an audit of type variables,
   wildcards, varargs, erasure collisions, and unsupported spellings.
3. The /lib/java manifest and JDK 28 API identity/envelope contract.
4. Compiler checking of every .sleela file in lib/java/jdk-28.
5. Java conformance and JDK differential programs.
6. Native converter repeat-run/provenance checks and the explicit
   per-member-stub regression.
7. A JDK 25 reflected-API coverage comparison against the JDK 28 tree. This
   is a cross-tier comparison, not a claim that JDK 25 is the JDK 28 API.
8. A separate functional-equivalence diagnostic.

The functional-equivalence diagnostic may exit with status 2. The campaign
records that outcome as PARTIAL, not PASS: the JDK 28 library intentionally
contains exact API declarations whose bodies still throw or return defaults.
The signature index preserves generic declarations as @generic metadata and
uses the same SLeeLa runtime type table as the primary Java API generator for
overload indexing. One-dimensional primitive and String arrays retain their
typed array forms; multidimensional and unsupported reference arrays erase to
Any, consistently across both generators. When multiple Java
overloads collapse to the same target signature, the audit reports the
collision rather than claiming that each overload is separately represented.
Implemented superclass and interface methods are included in stub suppression;
private, abstract, and interface-static declarations are not treated as inherited implementations. Constructor suppression
uses parameter-kind compatibility, not arity alone.

The /lib/java/converter policy remains per-member fail-loud stubs. A member
that cannot be lowered does not discard supported siblings in the same type.
tests/per-member-stub-policy.sh verifies that behavior and that the conversion
report records the stub. This policy is distinct from the sleelactl
java-convert converter's fail-closed per-file contract.

CI uploads detailed logs, the summary, and the generated-output diff preview as
the jdk28-validation-reports artifact. Generated files are not auto-committed;
the patch is reviewed separately before any repository update. A successful campaign means its executable gates passed; it does
not mean every JDK 28 API member has working behavior.

Important baseline limitation: the current JDK 28 generator reads member and
method signatures from the JDK 25 reflected JSONL index, while the JDK 28 docs
mirror supplies the type inventory and source URLs. Therefore it does not yet
prove complete JDK 28 member-level API coverage, and it cannot discover methods
added to existing types after JDK 25. The audit labels the two sets separately
and reports JDK 28 types without matching JDK 25 records or signature records.
