# Regex Feature Matrix

Version: 1.2.0-dev

## SLeeLa Layer

The SLeeLa library defines the object-level regex surface:

- pattern construction and literals
- compilation and validation
- matching and search
- captures and results
- scanning and iteration
- replacement and splitting
- flags, options, dialects, capabilities, and engines
- diagnostics and error reporting
- Natural Form grammar, symbols, groups, and parsing

## Natural Form

Natural Form is the normative SLeeLa-facing syntax contract. Its current language version is **1.1.0-dev**.

The contract covers:

- literals and quoted text
- `any`, `digit`, `letter`, and `space`
- character sets
- `one`, `optional`, `some`, and `many`
- exact and bounded repetition
- capture and non-capture groups
- named captures
- alternation
- beginning and ending anchors
- compact symbolic aliases

Unknown Natural Form words must be rejected rather than interpreted as literals.

## Backends

C, C++, and Java are implementation layers. POSIX ERE, C++ `std::regex`, and Java regex are adapters/backends, not the definition of Natural Form.

Lookaround, recursion, atomic groups, engine-specific backtracking controls, and other backend-specific extensions are outside the core contract until explicitly standardized and added to the Natural Form grammar and symbol inventory.

## Verification

The regex test suite now verifies:

1. C Natural Form compilation and validation.
2. C++ Natural Form compilation and validation.
3. Java Natural Form parsing and validation.
4. The complete 27-object `lib/regex` SLeeLa source inventory.
5. Missing or unexpected regex library objects.

SLeeLa — MEARVK LLC — 2026
