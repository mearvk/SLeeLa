# Native Compiler Architecture

Source -> Lexer -> recursive-descent parser and AST -> semantic checks and lowering -> stack bytecode -> interpreter, disassembler, SLBC emitter, or native source emitter.

The public C++ API is in include/sleela/compiler.hpp. Implementation stages are private to src/compiler.cpp.

Diagnostics include source line and column. Compilation fails for invalid characters, malformed syntax, unknown locals, or duplicate declarations. The interpreter and emitted native interpreter check stack underflow, local-slot bounds, division by zero, and signed arithmetic overflow.

The C11/C++20 source backend serializes the validated bytecode as a static instruction array and emits a small checked runtime. It never executes the generated artifact. A separate native toolchain invocation and a separate explicit run are required. CI verifies both generated source forms compile with warnings treated as errors and that the resulting example prints the expected result.

The current SLBC writer uses host-endian integer fields and is a development artifact, not a portable stable ABI. Define byte order, size limits, checksums, and versioning before external use.

This experimental subset does not override SLEELA.syntax, the production version gate, /impl, or SLVM.
