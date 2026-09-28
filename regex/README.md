<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Regex

Version: 1.1.0-dev

Cross-platform regular-expression subsystem for SLeeLa with a stable C ABI, C++ typed API, H/HPP public interfaces, tests, SLeeLa library objects, and the new Natural Form language.

## Natural Form

Natural Form is the user-facing regex contract. It uses a finite vocabulary of matching, grouping, quantity, logic, and anchor constructs. Readable names and compact symbolic aliases are equivalent where defined.

Examples:

```text
begin <name: letter (letter | digit | "_")*> end
begin <area: digit{3}> "-" <number: digit{3}> "-" digit{4} end
("cat" or "dog" or "bird")
```

See `natural/SYMBOLS.md`, `natural/GRAMMAR.md`, and `natural/EXAMPLES.md` for the normative language.

## Implementations

- C: `include/sleela_regex_natural.h` and `src/sleela_regex_natural.c`
- C++: `include/sleela_regex_natural.hpp` and `src/sleela_regex_natural.cpp`
- Java: `java/SleelaRegexNatural.java`
- SLeeLa: `../lib/regex/RegexNatural*.sleela`

The Natural Form front-end is deliberately separate from the POSIX ERE and C++ `std::regex` backends. Unsupported engine features must be diagnosed rather than silently entering the language.

SLeeLa — MEARVK LLC — 2026