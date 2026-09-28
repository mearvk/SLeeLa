# IDE Language Conformance Matrix

Version: 0.2.0-dev

| Domain | Recognition | Semantic source | Diagnostics | Index | Build | Run | Test | Debug |
|---|---|---|---|---|---|---|---|---|
| SLeeLa | required | SLeeLa compiler | required | required | required | required | required | required |
| C | required | native C toolchain/LSP | required | required | required | required | required | required |
| C++ | required | native C++ toolchain/LSP | required | required | required | required | required | required |
| Java | required | Java/IntelliJ platform | required | required | required | required | required | required |

For SLeeLa, the test corpus must verify that IDE parsing agrees with compiler parsing. For C/C++, compile-command fixtures must verify that include paths, macros, target, and language standard are preserved.

The matrix is a framework contract; it does not claim that every row is production-complete yet.