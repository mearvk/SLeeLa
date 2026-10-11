<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Native Compiler

Max Rupplin - MEARVK LLC - 2026

This top-level /compiler directory is a separately buildable C++20 compiler prototype. It complements the SLeeLa source framework in /lib/compiler and does not replace the production frontend in /impl.

## Build and test

Requires CMake 3.20+ and a C++20 compiler.

    cmake -S compiler -B build/compiler
    cmake --build build/compiler --config Release
    ctest --test-dir build/compiler --output-on-failure

Try it:

    ./build/compiler/sleelac --check compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --emit-ir compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --run compiler/examples/arithmetic.sleela
    ./build/compiler/sleelac --emit-bytecode /tmp/arithmetic.slbc compiler/examples/arithmetic.sleela

## Implemented subset

Lexer with source locations, integer literals, identifiers and line comments; recursive-descent expression parser; undeclared/duplicate-local checks; stack bytecode lowering, disassembly and interpreter; development SLBC writer. Statements: let name = expression;, print expression;, return expression;. Expressions support integers, locals, parentheses, unary minus, +, -, *, /.

## Scope

This is an experimental subset, not full SLeeLa 1.10 support. Classes, methods, inheritance, imports, strings, unsigned integer families, character sets, and the full grammar are not implemented here. The authoritative grammar and compatibility gate remain SLEELA.syntax and impl/frontend/version.{h,cpp}. Do not replace sleelvac with this prototype before feature-parity tests. See ARCHITECTURE.md and API.md.