---
inclusion: always
---

# SLeeLa build status (verified)

The `impl/` C/C++ tree now **builds end to end**: `cd impl && make` produces the
`sleela` and `nordshrift` binaries and stages the runtime. The `sleela` binary
compiles and executes `.sleela` programs, including the Java-style control-flow
statements (`if/else`, `while`, `do/while`, `for`, `switch/case/default`,
`break`, `continue`, `return`, `throw`, `assert`, `yield`, `synchronized`,
`try/catch/finally`) and expression forms (assignment/compound-assignment,
ternary `?:`, `instanceof`, cast, `this`/`super`, array access, method
reference) — verified by compiling and running a program that exercises them.

## Build command

The build is gated by a fail-closed SHA-256 manifest. After editing any verified
source file, regenerate the manifest, then build with it set:

```sh
python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json
cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make
```

## Known remaining issues (pre-existing, NOT frontend-build blockers)

These are independent of the compiler front end and were latent while the tree
never built. `make test` still fails on them:

- **Version floor vs. test corpus.** `minSupportedSyntax()` is `{1,3}` (per
  `VERSION.md`, range 1.3..1.6), but `impl/examples/*.sleela` and
  `impl/tests/subjects/*_values.sleela` declare `#sleela 1.0`, so they are
  rejected as "too old." The `tests/version/run_version_tests.sh` harness also
  disagrees with `VERSION.md` (it expects 1.4 to be rejected as "too new").
- **Type-name convention.** Subject test sources use Java `String`, but the
  semantic analyzer's known-type set uses lowercase `string`.
- **`test-java-runtime`.** A generated harness C file trips
  `-Werror=missing-field-initializers`.
- **Nordshrift `.sst` examples.** `test-nordshrift` reports stale example
  content / `source.root` path issues.

Reconcile the corpus (and the version-test contract) with `VERSION.md` before
claiming `make test` is green.

## History note — the front end never compiled before

Before this work, `impl/frontend/parser.cpp` targeted ~16 Java statement/
expression AST nodes that were never added to `ast.h` and never handled by
`semantic.cpp`/`compiler.cpp`. Those nodes were added to `ast.h` and wired
through the analyzer and the bytecode compiler; the duplicate/old statement
parser was removed; and a latent `Binary` constructor bug (`lhs(std::move(r))`
with `rhs` left uninitialized) was fixed. The earlier build-blocker fixes
(Makefile, HTTP servers, system monitor, lexer, parser.h) are below.

## How to build

The build is gated by a fail-closed SHA-256 manifest. After editing any verified
source file, regenerate the manifest, then build with it set:

```sh
python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json
cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make
```

The committed manifest goes stale whenever source changes; regenerate it rather
than editing entries by hand.

## Build blockers found and FIXED in this review

All compile cleanly in isolation now:

1. `impl/Makefile` — linked three Skya objects whose sources were deleted
   (`skya_policy.{cpp,h}`, `skya_sleela_bridge.{cpp,h}`) and the engine object
   that nothing calls anymore. The `sleela` driver has no `skya` subcommand, so
   the Skya objects were removed from the `sleela` link line and their compile
   rules deleted. Also fixed a wrong prerequisite path for the core object
   (`../../runtime/garbage_collector.h` → `../runtime/garbage_collector.h`; the
   `#include` in `sleela_core.h` was already correct).
2. `http-servers/common/http_server.cpp` — the HTTP token-separator string
   literal was mis-escaped (`"...\\"/[]?={}..."`), truncating the string and
   breaking the file. Restored the correct `"()<>@,;:\\\"/[]?={} \t"`.
3. `http-servers/2/http2/http2_server.cpp` — three real bugs: a non-existent
   nghttp2 constant (`NGHTTP2_ERR_ENHANCE_YOUR_CALM`), one extra `}` closing
   `respond()` early, and a `nghttp2_data_provider` brace-init that assigned a
   `Data*` into the union's `int fd`. Set `.source.ptr`/`.read_callback` instead.
4. `impl/core/sleela_system_monitor.{c,h}` — the `.c` reads/writes `m->level`
   but `SLSystemMonitor` had no `level` member; added `SLHSMLevel level;`.
5. `impl/frontend/lexer.cpp` — duplicate `case` labels (`'+'`,`'-'`,`'*'`,`'/'`,
   `'%'` in `tokenize`, and `KwIf/KwElse/.../KwNew` in `tokName`), plus a bogus
   `=` → `->` arrow branch. Removed the simple duplicate cases, keeping the
   fuller operator handling.
6. `impl/frontend/parser.h` — line of declarations had literal `\n` text instead
   of newlines, and `parseStatement()`/`parseBlock()` were declared twice with
   conflicting return types. De-duplicated; `parseBlock()` returns
   `std::unique_ptr<Block>`.

## Remaining build blocker — frontend AST is incomplete (NOT yet fixed)

`impl/frontend/parser.cpp` is written to build ~16 Java-statement/expression AST
nodes that **were never added to `ast.h`** and are **not handled by
`semantic.cpp` or `compiler.cpp`**: `ArrayAccess`, `AssertStmt`,
`AssignmentExpr`, `CastExpr`, `ConditionalExpr`, `DoStmt`, `InstanceOfExpr`,
`MethodReferenceExpr`, `SuperExpr`, `SwitchStmt`, `SynchronizedStmt`, `ThisExpr`,
`ThrowStmt`, `TryStmt`, `YieldStmt`, plus `JavaTypeMetadata.typeParameters`.
History confirms these node types never existed in `ast.h`. The "Sync Java
statement AST to master" work added the parser half without the AST/semantic/
compiler halves, so `build/parser.o` fails with ~91 errors and the whole tree
has never compiled.

Finishing this means designing those nodes with correct fields and implementing
their semantic checks and lowering end to end — a feature-completion task, not a
typo fix. Until it is done, treat `impl/` as "not building."
