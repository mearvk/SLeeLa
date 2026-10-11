<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Native Compiler

Max Rupplin - MEARVK LLC - 2026

This top-level /compiler directory is a separately buildable C++20 compiler prototype. It complements the SLeeLa source framework in /lib/compiler and does not replace the production frontend in /impl.

## Build and test

Requires CMake 3.20+ and a C++20 compiler.

    cmake -S compiler -B build/compiler -DCMAKE_BUILD_TYPE=Release
    cmake --build build/compiler --config Release
    ctest --test-dir build/compiler --build-config Release --output-on-failure

Try it:

    ./build/compiler/sleelac --check compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --emit-ir compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --run compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --emit-bytecode /tmp/arithmetic.slbc compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --emit-c /tmp/arithmetic.c compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --emit-cpp /tmp/arithmetic.cpp compiler/examples/arithmetic.sleela
    cc -std=c11 -Wall -Wextra -Werror /tmp/arithmetic.c -o /tmp/arithmetic-c
    c++ -std=c++20 -Wall -Wextra -Werror /tmp/arithmetic.cpp -o /tmp/arithmetic-cpp

## Implemented subset

Lexer with source locations, integer literals, identifiers and line comments; recursive-descent expression parser; undeclared/duplicate-local checks; stack bytecode lowering, disassembly and interpreter; development SLBC writer. Statements: `let name = expression;`, `print expression;`, `return expression;`. Expressions support integers, locals, parentheses, unary minus, +, -, *, /.

## Scope

This is an experimental subset, not full SLeeLa 1.10 support. Classes, methods, inheritance, imports, strings, unsigned integer families, character sets, and the full grammar are not implemented here. The authoritative grammar and compatibility gate remain SLEELA.syntax and impl/frontend/version.{h,cpp}. Do not replace sleelvac with this prototype before feature-parity tests. See ARCHITECTURE.md and API.md.

## Native C/C++ source emission (experimental subset)

`--emit-c` and `--emit-cpp` generate standalone C-compatible source containing validated-subset bytecode and a small checked interpreter. Compile generated output with a C11 or C++20 toolchain. This is a usable native build path for the prototype's supported subset, not a full SLeeLa-to-native compiler or replacement for `/impl`.

The emitter does not execute generated programs. Source compilation, native compilation, and execution remain separate developer-controlled steps. Unsupported source syntax is rejected before emission; inspect generated source and run tests before using artifacts. The interpreter enforces a 65,536-value stack limit and a 65,536-local limit, and rejects arithmetic overflow, division by zero, invalid opcodes, and invalid local slots. Generated runtime arrays use static storage rather than consuming the process's default stack.

CMake uses platform-aware install directories. To stage a user-controlled install without modifying system paths:

    cmake --install build/compiler --config Release --prefix build/compiler/stage

Use an explicit system prefix only when the installer or administrator intends a system-wide installation. See ARCHITECTURE.md and PRODUCTION-INTEGRATION-GATE.md for the production-integration gate.
