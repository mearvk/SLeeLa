# SLeeLa Regex Symbol Exhaustiveness Reference

Version: 1.2.0-dev

This is the normative verification reference for proving that the SLeeLa Regex Natural Form symbol set is exhaustive for matching, grouping, quantification, alternation, and anchoring.

## 1. Exhaustive Semantic Universe

The current Natural Form contract defines **19 semantic symbol entries**:

| Domain | Semantic symbol | Required surface spellings | Matching / grouping obligation |
|---|---|---|---|
| Atom | any | `any`, `·` | Match exactly one character. |
| Atom | digit | `digit`, `#` | Match exactly one decimal digit. |
| Atom | letter | `letter`, `@` | Match exactly one letter. |
| Atom | space | `space`, `_` | Match exactly one whitespace character. |
| Atom | literal | quoted text | Match the quoted literal exactly. |
| Atom | charset | `[abc]` | Match one character in the set. |
| Atom | negated charset | `[^abc]` | Match one character outside the set. |
| Quantity | one | `one`, `=` | Require exactly one occurrence. |
| Quantity | optional | `optional`, `?` | Permit zero or one occurrence. |
| Quantity | some | `some`, `+` | Require one or more occurrences. |
| Quantity | many | `many`, `*` | Permit zero or more occurrences. |
| Quantity | exact | `{n}` | Require exactly n occurrences. |
| Quantity | range | `{n,m}` | Permit n through m occurrences. |
| Group | capture | `(...)` | Create a positional capture around a pattern. |
| Group | named capture | `<name: ...>` | Create a named capture around a pattern. |
| Group | non-capturing | `(?:...)` | Group a pattern without creating a capture. |
| Logic | alternative | `|`, `or` | Match one of the alternatives. |
| Anchor | begin | `begin`, `^` | Require the beginning of input. |
| Anchor | end | `end`, `$` | Require the end of input. |

This table is the **semantic completeness baseline**. A new Natural Form symbol is not complete merely because it has parser code; it must be added here and tested.

## 2. Surface-Spelling Exhaustiveness

Verification covers both each semantic concept and every standardized spelling or alias. Parameterized forms such as `quoted text`, `[...]`, `[^...]`, `{n}`, and `{n,m}` are verified with representative valid and invalid cases rather than attempting infinite enumeration.

## 3. Matching Proof Obligations

For every atom, quantifier, alternative, and anchor:

1. A positive case matches when its stated condition is satisfied.
2. A negative case fails when that condition is violated.
3. Each alias has the same semantic result as its canonical spelling.
4. Boundary behavior is tested where a construct can consume zero characters.
5. Invalid syntax produces a diagnostic rather than silent reinterpretation.
6. Each test identifies the semantic symbol it exercises.

Minimum matching coverage includes all primitive atoms and aliases, literals, positive and negated sets, all quantity operators and aliases, exact/range quantities, alternatives, and both anchor forms.

## 4. Grouping Proof Obligations

Grouping is exhaustive only when all three group forms are covered.

### Capture group — `(...)`

Evidence must show successful matching, populated positional capture, correct boundaries, and preservation of the outer match.

### Named capture — `<name: ...>`

Evidence must show successful matching, retained name, correct captured value/boundaries, and diagnostics for invalid or duplicate names where required by the contract.

### Non-capturing group — `(?:...)`

Evidence must show grouped matching while creating no ordinary positional capture.

Grouping tests must also cover nested groups, groups containing alternatives, groups containing quantifiers, alternatives containing groups, and permitted zero-width cases.

## 5. Exhaustiveness Relation

The proof target is:

`RequiredSymbols = DocumentedSymbols = TestedSymbols = AcceptedCoreSymbols`

Here, `DocumentedSymbols` is the finite semantic inventory above; `TestedSymbols` covers every semantic entry and finite alias; and `AcceptedCoreSymbols` means symbols intentionally accepted by the Natural Form parser. Engine-specific syntax is outside the core set and must be rejected or explicitly classified as an extension.

A test count alone is not proof. The automated suite must compare inventories and fail when a required symbol has no test obligation.

## 6. Change-Control Rule

When a symbol is added, removed, renamed, or aliased, update:

1. `natural/SYMBOLS.md`
2. `natural/GRAMMAR.md` when grammar changes
3. this reference
4. the symbol-exhaustiveness test manifest
5. positive and negative behavioral tests
6. parser/compiler/loader handling
7. compatibility/version documentation

## 7. Evidence Standard

A release may claim **symbol inventory exhaustiveness** only when the automated inventory test passes and the behavioral suite demonstrates the required matching/grouping obligations.

A release may claim **implementation exhaustiveness** only after compiler/loader and runtime paths have been inspected or tested against the same inventory.

An exhaustive specification is therefore not, by itself, proof that every implementation layer is correct.

SLeeLa — MEARVK LLC — 2026
