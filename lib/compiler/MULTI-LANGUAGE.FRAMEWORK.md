<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-compiler-logo-001.jpeg" alt="SLeeLa Compiler" width="100%"></p>

# SLeeLa Multi-Language Compiler Framework

Max Rupplin - MEARVK LLC - 2026

The `/lib/compiler` package provides a **modular framework** for building
compilers for any publicly known programming language. A developer adds a new
language by writing one small SLeeLa front end that extends a common contract;
many independent front ends live side by side and are registered into one shared
catalog. No per-language allow-list is baked into the compiler.

## Authority

Every language front end lowers *its own* language toward the common SLeeLa IR
and, from there, a VM-ready artifact. The authoritative path is unchanged:

```
<language> source  ->  <language> front end  ->  SLeeLa IR  ->  VM lowering  ->  SLVM/SLJVM
```

The framework does **not** create a parallel language, and it does **not** grant
itself VM, capability, security, resolver, memory, or certificate authority. A
produced artifact is only ever executed later across an explicit SLeeLa VM/OS and
security boundary. **Compilation is not execution.**

## The contract

A compiler for a language is an `SLLanguageCompiler` subclass. The base type
([`SLLanguageCompiler.sleela`](SLLanguageCompiler.sleela)) defines:

- **Identity** — `declare(name, family, version, extensions, defaultTarget)`.
- **Provided phases** — `provide(phase)` for each pipeline stage the front end
  implements, recorded as a bitmask.
- **Phase hooks** — `runSource`, `runLex`, `runParse`, `runResolve`,
  `runSemantic`, `runIR`, `runLower`, `runCodegen`. A subclass overrides the
  phases it provides; the base returns whether the phase is declared.
- **Registration & planning** — `register()` puts the front end in the shared
  catalog; `planCompile(request)` produces a fail-closed `SLCompilePlan`.

### Canonical pipeline phases

| # | Phase    | Role |
|---|----------|------|
| 1 | source   | load/normalize source units |
| 2 | lex      | tokenize |
| 3 | parse    | build the language syntax tree |
| 4 | resolve  | names, scopes, imports, symbols |
| 5 | semantic | types, capabilities, dependencies |
| 6 | ir       | lower to target-neutral SLeeLa IR |
| 7 | lower    | lower IR to VM-ready operations |
| 8 | codegen  | emit artifact |

### Targets

| Target | Meaning |
|---|---|
| `SLVM`   | SLeeLa VM artifact (authoritative native path) |
| `SLJVM`  | SLeeLa JVM-family artifact (used by JVM-family languages) |
| `SLIR`   | stop at SLeeLa IR (library/analysis, no emission) |
| `NATIVE` | native plan only; emission remains gated by the VM/OS boundary |

Phases 1–6 (source..IR) are the common minimum for any produced model; `SLVM`,
`SLJVM`, and `NATIVE` additionally require `lower` and `codegen`.

## Supporting types

- [`SLCompileRequest.sleela`](SLCompileRequest.sleela) — a single request:
  language, input path, requested target, and a `strict` fail-closed flag.
- [`SLCompilePlan.sleela`](SLCompilePlan.sleela) — the fail-closed plan/result:
  covered vs required phases, the first missing phase, resolved target, and a
  `vmReady` verdict. Nothing is guessed; an unresolved or reference-only path is
  surfaced.
- [`SLCompilerRegistry.sleela`](SLCompilerRegistry.sleela) — register many front
  ends, count them, test membership, and resolve a file name to a front end by
  extension.

## Modular front ends

Each language is an independent unit under
[`frontends/<language>/`](frontends/). The shipped examples span native,
JVM, and scripting families to show the pattern:

| Front end | File | Family | Default target |
|---|---|---|---|
| C | `frontends/c/CLanguageCompiler.sleela` | C-family | SLVM |
| C++ | `frontends/cpp/CppLanguageCompiler.sleela` | C-family | SLVM |
| Java | `frontends/java/JavaLanguageCompiler.sleela` | JVM | SLJVM |
| Python | `frontends/python/PythonLanguageCompiler.sleela` | scripting | SLVM |
| JavaScript | `frontends/javascript/JavaScriptLanguageCompiler.sleela` | scripting | SLVM |
| Rust | `frontends/rust/RustLanguageCompiler.sleela` | systems | SLVM |
| Go | `frontends/go/GoLanguageCompiler.sleela` | systems | SLVM |

These are reference front ends that declare identity and the full pipeline, with
phase hooks annotated for the language-specific behavior a developer plugs in
(e.g. Python's indentation-significant lexing, Rust's borrow analysis in the
semantic phase, C++'s template/overload resolution). The
[`LANGUAGE.FORMAT.REFERENCE.md`](LANGUAGE.FORMAT.REFERENCE.md) catalog is the
recognition data these front ends draw their identity from; it identifies, it
never authorizes execution.

## Adding a language

1. Create `frontends/<language>/<Name>LanguageCompiler.sleela`.
2. Extend `SLLanguageCompiler`; in `configure()` call `declare(...)` and
   `provide(...)` for each phase you implement.
3. Override the phase hooks (`runLex`, `runParse`, ...) with the language's
   behavior.
4. Register it: `registry.add(new <Name>LanguageCompiler())`.

The recursive `/lib` discovery used by the compiler and the Nordshrift loader
picks up the new directory automatically — no compiler allow-list change.

## Native boundary

C provides the stable ABI; C++ orchestrates. Neither bypasses the SLeeLa
capability, security, resolver, memory, certificate, or VM boundaries.

| File | Role |
|---|---|
| [`include/sleela_langc.h`](include/sleela_langc.h) | C ABI: module descriptor, registry, phase model, planning |
| [`src/sleela_langc.c`](src/sleela_langc.c) | C implementation (allocation-free, no I/O, no execution) |
| [`include/sleela_langc.hpp`](include/sleela_langc.hpp) | C++ orchestration facade (`LanguageCompiler`, `Registry`, `Plan`) |
| [`src/sleela_langc.cpp`](src/sleela_langc.cpp) | C++ TU + compile-time C/C++ congruence checks |
| [`include/sleela_lang_bridge.h`](include/sleela_lang_bridge.h) | `native` VM/OS bridge entry points the `.sleela` layer calls |
| [`src/sleela_lang_bridge.c`](src/sleela_lang_bridge.c) | flat-scalar registration + integer plan-handle adapter |
| [`tests/langc_selftest.c`](tests/langc_selftest.c) | behavioral self-test with real assertions |

## Build and test

```sh
make -C lib/compiler native     # compile the C/C++ framework objects
make -C lib/compiler selftest   # build and run the behavioral self-test
make -C lib/compiler all        # native + selftest + runtime + inventory + sanity
```

The self-test checks registration/replacement, case-insensitive lookup,
extension resolution, phase planning, and the fail-closed `strict` and
`reference-only` paths.

**SLeeLa — MEARVK LLC — 2026**
