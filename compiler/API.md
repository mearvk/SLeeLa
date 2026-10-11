# Native Compiler API

Public header: include/sleela/compiler.hpp.

- `compile(source)`: lex, parse, validate local names, and lower to bytecode.
- `CompileResult::ok()`: true when there are no diagnostics.
- `run(program)`: execute and capture printed integers, return value, or runtime error. The interpreter enforces a 65,536-value operand-stack limit and validates opcodes and local slots.
- `disassemble(program)`: render bytecode instructions.
- `write_bytecode(program, path, error)`: write the development SLBC container; rejects failed compilation and counts that exceed the container's 32-bit instruction/local-count fields.
- `write_native_source(program, path, cpp, error)`: emit a standalone C-compatible native source file from successfully compiled subset bytecode. Use `cpp=false` for C output or `cpp=true` for a .cpp output. The generated runtime checks stack capacity, program-counter bounds, local slots, division by zero, and signed arithmetic overflow.
- `read_text_file(path, error)`: read a source file.

CLI native-output modes are `sleelac --emit-c OUTPUT.c SOURCE.sleela` and `sleelac --emit-cpp OUTPUT.cpp SOURCE.sleela`. Both output forms contain a small checked interpreter and the validated subset's bytecode. The C++ output is intentionally C-compatible, allowing one code generator to keep execution behavior consistent across both targets.

Opcodes: PUSH_INT, LOAD_LOCAL, STORE_LOCAL, ADD, SUB, MUL, DIV, NEG, PRINT, RETURN, HALT. The API and file format are experimental; compatibility is not promised. This prototype does not implement the complete SLeeLa grammar and is not a substitute for /impl.
