# Native Compiler Architecture

Source -> Lexer -> recursive-descent parser and AST -> semantic checks and lowering -> stack bytecode -> interpreter, disassembler, or SLBC emitter.

The public C++ API is in include/sleela/compiler.hpp. Implementation stages are private to src/compiler.cpp.

Diagnostics include source line and column. Compilation fails for invalid characters, malformed syntax, unknown locals, or duplicate declarations. The interpreter checks stack underflow, local-slot bounds, division by zero, and signed arithmetic overflow.

The current SLBC writer uses host-endian integer fields and is a development artifact, not a portable stable ABI. Define byte order, size limits, checksums, and versioning before external use.

This experimental subset does not override SLEELA.syntax, the production version gate, /impl, or SLVM.
