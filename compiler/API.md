# Native Compiler API

Public header: include/sleela/compiler.hpp.

- compile(source): lex, parse, validate local names, and lower to bytecode.
- CompileResult::ok(): true when there are no diagnostics.
- run(program): execute and capture printed integers, return value, or runtime error.
- disassemble(program): render bytecode instructions.
- write_bytecode(program, path, error): write the development SLBC container; rejects failed compilation.
- read_text_file(path, error): read a source file.

Opcodes: PUSH_INT, LOAD_LOCAL, STORE_LOCAL, ADD, SUB, MUL, DIV, NEG, PRINT, RETURN, HALT. The API and file format are experimental; compatibility is not promised.
