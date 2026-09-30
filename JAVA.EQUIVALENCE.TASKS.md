# JAVA.EQUIVALENCE.TASKS.md

## Java Source -> SLeeLa Equivalence Task Ledger

Scope: known Java source represented congruently at the SLeeLa source/API level. Not required: JVM, Java bytecode execution, SLVM, or JVM runtime internals.

The official Java SE specification page currently identifies Java SE 27 as released in September 2026; the project's Java 28 target is therefore a forward compatibility target, not a released Java specification. citeturn0search0

### Eight source-equivalence pieces

| # | Piece | Required capability | Status |
|---|---|---|---|
| 1 | Lexical | Java-compatible tokens, identifiers, literals, operators, annotations, generic punctuation, source metadata | Foundation expanded |
| 2 | Type system | Primitive/reference/array/null types; generics; wildcards; bounds; conversions; inference | Type model created |
| 3 | Declarations | Class/interface/enum/record/annotation; fields/methods/constructors/nested types | Metadata + model expanded |
| 4 | Expressions | Names, access, calls, creation, arrays, operators, casts, patterns, lambdas, method references, conditionals | Expression inventory types created |
| 5 | Statements | Blocks, declarations, control flow, switch/yield, exceptions, synchronized, try-with-resources | Statement inventory types created |
| 6 | Semantic constraints | Resolution, access, conversions, overload/override, generics, definite assignment, exceptions | Rule taxonomy created |
| 7 | API counterparts | Exact Java package/type/member/signature/annotation counterparts | Existing JDK API envelopes; dependency closure next |
| 8 | Equivalence testing | Java-to-SLeeLa inventory, normalized comparison, deterministic diagnostics | First deterministic driver created |

### Next major piece: Java Source Equivalence Driver

Implemented:
- tests/java_source_equivalence.py
- paired Java/SLeeLa fixture under tests/java_equivalence/
- tests/Makefile target java-source-equivalence

The driver reports structural, semantic, and functional-source states separately. It does not claim behavioral equivalence from API envelopes.

### Remaining major implementation queue

1. Expand Java expression AST and parser. **Completed in 0.3.6-dev: expression node taxonomy, operator lexing, precedence parsing, assignments, conditionals, casts, array access, postfix increment/decrement, method references, and constructor arguments.**
2. Expand Java statement AST and parser. **Next major task.**
3. Implement Java type/conversion semantics.
4. Implement overload and override resolution.
5. Implement definite-assignment and reachability rules.
6. Implement checked-exception analysis.
7. Implement dependency-driven Java API counterpart closure.
8. Expand negative and constraint fixtures.
9. Add normalized AST/signature comparison beyond the inventory driver.
10. Add Linux, Windows 10+, and macOS qualification records.

### Non-goals for this qualification

Do not add JVM execution requirements to this ledger. Java source congruence is the target; JVM/SLVM interoperability is a separate future project.

**MEARVK LLC — 2026**
