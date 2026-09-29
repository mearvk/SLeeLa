# SLeeLa Regex — VERSION

Current Version: **1.2.0-dev**

## Release Identity

- Product: SLeeLa Regex
- Version: 1.2.0-dev
- Status: Development
- SLeeLa library objects: 27
- Natural Form language version: 1.1.0-dev
- Native implementation layers: C, C++, Java
- Year: 2026

## 1.2.0-dev

This development version records the expanded SLeeLa regex object family as the authoritative library inventory and brings the test suite and documentation into alignment with that inventory.

### Included

- 27 independent SLeeLa regex source objects
- Complete Natural Form object family
- Regex compiler, matcher, scanner, iterator, replacement, splitter, validator, result, and diagnostic objects
- Capability, dialect, engine, flags, options, and literal objects
- C, C++, and Java Natural Form implementations
- Complete source-inventory verification
- Compiler/loader-facing object discovery requirement
- Documentation aligned with the current `lib/regex` contents

### Compatibility

Natural Form remains a separate language contract. Backend-specific constructs are not automatically promoted into SLeeLa syntax.

See `natural/VERSION.md` for the Natural Form language-version policy.

SLeeLa — MEARVK LLC — 2026
