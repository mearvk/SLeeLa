# SLeeLa Regex Natural Form — VERSION

Current Version: **1.1.0-dev**

## Release Identity

- Product: SLeeLa Regex Natural Form
- Version: 1.1.0-dev
- Status: Development
- Language Contract: Finite Natural Form grammar
- Native Implementations: C, C++
- Tooling Implementation: Java
- Language Library: SLeeLa
- Year: 2026

## 1.1.0-dev

This version establishes the initial cross-language Natural Form contract.

### Included

- Finite normative matching vocabulary
- Finite grouping vocabulary
- Finite quantity vocabulary
- Named groups
- Alternative expressions
- Beginning and ending anchors
- Literal and character-set forms
- Natural-word aliases for compact regex symbols
- Formal grammar specification
- Compatibility and backend rules
- C validation API
- C++ validation API
- Java validation/parser API
- SLeeLa Natural Form library objects
- C, C++, and Java conformance tests

### Compatibility Rule

Natural Form is the SLeeLa-level contract. POSIX ERE, C++ std::regex, Java regex, and future engines are implementation backends. Backend-specific features are not part of the core language unless explicitly added to the normative symbol and grammar documents.

### Versioning Policy

The Natural Form version changes when the language contract changes. Documentation-only changes do not require a language-version increment. Breaking grammar or symbol changes require a major-version review; new backward-compatible core constructs require a minor-version increment; corrective changes use the patch component.

SLeeLa — MEARVK LLC — 2026
