<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Regex

Version: 1.2.0-dev

Cross-platform regular-expression subsystem for SLeeLa. The subsystem has three layers: the SLeeLa language library, the Natural Form front-end, and native/backend adapters for C, C++, and Java.

## Current Library Inventory

The SLeeLa-facing regex library currently contains **27 separate `.sleela` objects** under `lib/regex/`. This includes the core regex objects, matching/search objects, replacement and splitting objects, capability/configuration objects, diagnostics, and the complete Natural Form family.

See `../lib/regex/REGEX.INDEX.md` for the authoritative object inventory.

## Natural Form

Natural Form is the SLeeLa-level, user-facing regex contract. Its language definition is intentionally finite and versioned independently at **1.1.0-dev**. Readable names and compact symbolic aliases are equivalent where defined.

Examples:

```text
begin <name: letter (letter | digit | "_")*> end
begin <area: digit{3}> "-" <number: digit{3}> "-" digit{4} end
("cat" or "dog" or "bird")
```

See `natural/SYMBOLS.md`, `natural/GRAMMAR.md`, `natural/EXAMPLES.md`, and `natural/COMPATIBILITY.md`.

Unknown Natural Form words are rejected. Backend-specific syntax is not silently added to the core language.

## Implementations

- C: `include/sleela_regex_natural.h` and `src/sleela_regex_natural.c`
- C++: `include/sleela_regex_natural.hpp` and `src/sleela_regex_natural.cpp`
- Java: `java/SleelaRegexNatural.java` and `java/SleelaRegexNaturalParser.java`
- SLeeLa: `../lib/regex/Regex*.sleela`

## Verification

`test-suites/run-all.sh` builds and executes the C, C++, and Java Natural Form tests and verifies the complete 27-object SLeeLa regex source inventory. The Makefile exposes the same verification as `make test`.

SLeeLa — MEARVK LLC — 2026
