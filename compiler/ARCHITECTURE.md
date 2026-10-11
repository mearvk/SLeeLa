# Native Compiler Architecture

Source -> Lexer -> recursive-descent parser and AST -> semantic checks and lowering -> stack bytecode -> interpreter, disassembler, SLBC emitter, or native source emitter.

The public C++ API is in include/sleela/compiler.hpp. Implementation stages are private to src/compiler.cpp.

Diagnostics include source line and column. Compilation fails for invalid characters, malformed syntax, unknown locals, duplicate declarations, and out-of-range positive integer literals. The interpreter and emitted native interpreter enforce a 65,536-value operand-stack limit and 65,536-local limit, check stack underflow and local-slot bounds, reject invalid opcodes, and diagnose signed arithmetic overflow and division by zero. The emitted runtime uses static storage for its bounded stack and local arrays to avoid exhausting small default process stacks.

The C11/C++20 source backend serializes the validated bytecode as a static instruction array and emits a small checked runtime. It never executes the generated artifact. A separate native toolchain invocation and a separate explicit run are required. CI builds and unit-tests on Linux, Windows, and macOS; Linux additionally compiles emitted C/C++ with warnings treated as errors and executes normal and arithmetic-fault fixtures.

CMake uses GNUInstallDirs so the install layout follows the target platform's conventions. A developer or installer can choose a relative staging prefix, for example `cmake --install build/compiler --prefix build/compiler/stage`, or choose a system-wide prefix explicitly. Do not assume a system install is needed.

The current SLBC writer uses host-endian integer fields and is a development artifact, not a portable stable ABI. Define byte order, size limits, checksums, and versioning before external use.

## Production integration gate

The root `/compiler` is intentionally an experimental subset compiler. The production frontend and SLVM remain authoritative. The `/lib/compiler` Sleela-facing bridge contracts are not proof that a working adapter exists. Do not route production compilation through this prototype until a reviewed adapter consumes the real production frontend's typed/verified IR, preserves diagnostics and language-version rules, and passes parity tests against the production compiler.

Required integration gates:
1. Identify the production frontend's stable typed-IR interface and ownership/lifetime rules.
2. Add an adapter without changing default production dispatch.
3. Compare acceptance/rejection, diagnostics, arithmetic behavior, and generated artifacts on shared fixtures.
4. Run existing compiler, VM, library, and OS build suites.
5. Enable opt-in routing only after parity is demonstrated; keep an immediate rollback to the established frontend.

This experimental subset does not override SLEELA.syntax, the production version gate, /impl, or SLVM.
