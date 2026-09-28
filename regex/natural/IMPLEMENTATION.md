# Natural Form Implementation

Version: 1.1.0-dev

The implementation is layered:

1. **Lexical recognition** identifies literals, delimiters, quantities, aliases, and finite Natural Form words.
2. **Grammar parsing** creates a canonical AST.
3. **Capability validation** checks whether the selected backend can represent that AST.
4. **Backend emission** translates only after validation.

C and C++ provide the native front-end foundation. Java provides an independent front-end suitable for the Java/SLeeLa tooling layer. SLeeLa library objects describe the language contract and user-facing operations.

Implementations must reject unknown Natural Form words. They must not reinterpret unknown words as literals because that would make spelling mistakes silently change matching behavior.

The current validator intentionally performs structural validation first. Full AST emission, named-capture compilation, and backend-specific escaping are the next compiler layer and must retain source offsets for diagnostics.
