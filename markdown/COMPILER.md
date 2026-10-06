# The Sleela Compiler

This document describes the Sleela compiler: what it is, how it is structured,
how to use it, and how it is **version aware**. For the authoritative record of
every version number in the project, see [`VERSION.md`](VERSION.md); for the
`.sleela` filetype (**Wrapper™**) itself, see [`SLEELA.md`](SLEELA.md).

---

## 1. What the Sleela compiler is

Sleela is a **Java-like language** that runs on a small, Turing-complete
**C/C++ execution core**. The compiler is the front end that turns a
`.sleela` source file (a **Wrapper™**) into **core bytecode** and hands it to
the core for execution. Sleela source is not interpreted by a separate parallel language interpreter. The `sleela` CLI can execute textual `.sleela` directly by compiling it in memory and handing the resulting Core representation to the native C SLVM; it can also compile source to a persistent Core artifact for later SLVM loading.

```
Wrapper™ (.sleela source)
        │
        ▼
  [ Sleela front end — C++ ]
     version check → lexer → parser → AST + annotations → semantic analysis → compiler (codegen)
        │   emits opcodes and drives the core via the exchange API
        ▼
  [ Sleela Core — C, stable ABI ]
     value model · operand stack · call frames · opcode dispatch loop
     ---- slcore_exchange(): the executable "exchange" API ----
```

The current compiler compatibility line is **2.6-dev**. The compiler lives in [`impl/frontend/`](impl/frontend/) and owns **no execution
logic**: it walks the AST and drives the C core purely through the builder
helpers over `slcore_exchange`. The same front end is reused by **Nordshrift**
(the `.sst` transpiler driver) to parse the sources it transpiles.

---

## 2. The compilation pipeline

| Stage | Unit | Files | Responsibility |
|-------|------|-------|----------------|
| **Version resolution** | `version.{h,cpp}` | Resolve and enforce the `#sleela` syntax-version pragma (SL-META-0001 §4.4) *before* any parsing. |
| **Lexing** | `lexer.{h,cpp}` | Tokenize the Java-like surface (keywords, identifiers, literals, operators); skip whitespace, `//` and `/* */` comments, and the leading `#…` pragma line. |
| **Parsing** | `parser.{h,cpp}` | Recursive-descent parse of tokens into an AST (`ast.h`): classes, fields, methods, statements, and precedence-climbing expressions. |
| **Semantic analysis / compilation / codegen** | `compiler.{h,cpp}` | Lower the AST to core bytecode: declare a global per class field, flatten methods into the core function table, assign local slots, emit opcodes, and resolve built-ins. Throws on semantic errors (unknown variable/function, duplicate field, missing `main`). |
| **Execution** | Sleela Core (`core/`) | Load the emitted program into an `SLVM` and run it via the exchange/run API. |

### Language surface the compiler accepts

Classes with an entry `main()`; typed locals (`int`, `double`, `boolean`,
`String`, `void`); methods with recursion; `if/else`, `while`, `for`; the full
operator set (`+ - * / %`, comparisons, `&& || !`, unary `-`); Java-style string
concatenation with `+`; class **fields** (shared state, compiled to thread-safe
globals); a bounded **threading** model (`spawn`/`join`/`lock`/`unlock`/
`send`/`recv`); and **conducted methods** backed by `SHEET.sheet`
(`conduct`/`role`/`insight`/`congruent`/`route`/`sysdepth`/`degreemax`).

---

## 3. Version awareness (SL-META-0001 §4.4)

The Sleela compiler is **version aware**: it knows which syntax versions it can
accept and enforces that on every source it compiles, through **both** entry
points (`sleela` and Nordshrift).

### First-class annotations

A Wrapper may declare document annotations before its first import, struct, or class. The same annotation collection is carried through lexing, parsing, the Program AST, semantic validation, compilation, runtime installation, and Server Edition integration. `@next` is validated as a safe forwarding destination; annotation metadata never grants capability or bypasses SourceRouter policy.

See [`impl/frontend/ANNOTATION_PIPELINE.md`](impl/frontend/ANNOTATION_PIPELINE.md).

### The `#sleela` pragma

A Wrapper™ declares its **syntax version** with a pragma on the first
non-blank, non-comment line:

```java
#sleela 1.3
class Hello {
    void main() { print("Hello, Sleela!"); }
}
```

- Form: `#sleela MAJOR.MINOR` (a trailing `.PATCH` is tolerated and ignored,
  since a PATCH increment introduces no grammar changes).
- The pragma is located after any leading blank lines and `//` / `/* */`
  comments — exactly the "first non-blank, non-comment line" the metadocument
  requires.

### The rule the compiler enforces

> *"A compiler must reject files whose declared version exceeds the compiler's
> supported version range."* — SL-META-0001 §4.4

The compiler resolves a declared version to one of: **accept**, **warn**
(no pragma → assume the floor), or **reject**. On a compiler whose supported
range is `1.0 .. 1.0`:

| Declared `#sleela` | Result | Rationale |
|--------------------|--------|-----------|
| `1.3`     | **accepted** | current supported syntax |
| `1.4`     | **rejected** (too new) | MINOR ahead of the supported max |
| `2.0`     | **rejected** (too new) | MAJOR ahead → breaking grammar unsupported |
| `1.2`     | **rejected** (too old) | below the supported floor |
| `one.zero`| **rejected** (malformed) | not a numeric `MAJOR.MINOR` |
| *(none)*  | **accepted, with warning** | assumed `1.3` for backward compatibility |

A rejected source exits non-zero with a descriptive diagnostic, e.g.:

```
sleela: prog.sleela: error: source declares Sleela syntax 2.0, which exceeds
this compiler's supported range (1.3 .. 1.3). Upgrade the compiler or lower
the #sleela pragma.
```

### Where the supported range lives

The range is defined in [`impl/frontend/version.h`](impl/frontend/version.h):

```cpp
inline SyntaxVersion minSupportedSyntax() { return SyntaxVersion{1, 3}; }
inline SyntaxVersion maxSupportedSyntax() { return SyntaxVersion{1, 3}; }
inline SyntaxVersion defaultSyntaxVersion() { return minSupportedSyntax(); }
```

To widen support, raise `maxSupportedSyntax()` (and, when an old grammar is
finally dropped, `minSupportedSyntax()`), then bump the toolchain version and
record it in [`VERSION.md`](VERSION.md).

---

## 4. Versions

The compiler versions several things independently (per SL-META-0001 §8.3),
so they do not share one number. `VERSION.md` is the single source of record;
the current values are:

| Component | Version | Meaning | Source of truth |
|-----------|---------|---------|-----------------|
| **Sleela toolchain / implementation** (`sleela` CLI) | **0.3.0-dev** | The C/C++ front end + core in `impl/`. Pre-1.0. | `impl/frontend/driver.cpp` |
| **Sleela language syntax** | **1.3** (range `1.3 .. 1.3`) | The grammar version a `.sleela` file declares via `#sleela`. | `impl/frontend/version.h` |
| **Nordshrift** (`.sst` transpiler driver) | **2.6-dev** | Reuses this front end; enforces the same syntax rules. | `impl/nordshrift/nordshrift.cpp` |
| **NS-SST-0001** (`.sst` format spec) | **1.0.0** (Normative) | The `.sst` control-sheet format. | `SST.model` |
| **SL-META-0001** (metadocument) | **1.0.0** (Pre-Normative) | The governing language metadocument, incl. §4.4. | `src/Sleela.manifest` |

> **In short:** the compiler here is the **0.3.0-dev** toolchain, implementing
> **Sleela language syntax 1.3**. Syntax is versioned independently of the
> implementation: the `0.3.0-dev` toolchain implements syntax `1.3`.

Query the live values:

```sh
./build/sleela version
#  Sleela 0.3.0-dev (C/C++ core; SHEET.sheet conducted methods; .xclass input)
#    supported .sleela syntax: 1.3 .. 1.3 (declare per-file with '#sleela 1.3')
```

---

## 5. Runtime integration

The executable targets are linked with the SLVM runtime service layer: GarbageCollector, SecuritySupervisor, and the explicit Parameters contract. Class admission and resource reservation precede managed allocation; garbage collection governs object lifetime.

The Normal User profile specifies a software-capability threshold of **141+** and concurrent handling of **8** sociological subjects. These are runtime design parameters, not psychometric judgments.

## 6. Using the compiler

Build, then compile-and-run or validate a Wrapper™:

```sh
cd impl && make                                # produces build/sleela

./build/sleela run     examples/versioned.sleela   # compile + run on the C core
./build/sleela check   examples/versioned.sleela   # validate (version + lex + parse), do NOT run
./build/sleela version                             # version + supported syntax range
```

- `run` — the full pipeline: version check → lex → parse → compile → execute.
- `check` — everything up to parsing (including the version rule), without
  executing; prints the resolved syntax version.
- `version` — prints the toolchain version and the supported `.sleela` syntax
  range.

The compiler also accepts SecureJDK 28 `.xclass` input (`sleela run file.xclass`
/ `sleela xclass …`), reconstructing a program and running it through the same
core.

### Tests

Version behavior is covered by `make test` (target `test-version`, harness
`impl/tests/version/run_version_tests.sh`), which asserts that in-range versions
run and out-of-range / malformed versions are rejected.

## 7. Relationship to Slecompiler™

Sleela and Slecompiler are distinct products. The Sleela compiler consumes `.sleela`
Wrapper™ source and lowers it to the Sleela Core. Slecompiler consumes native artifacts
for static inspection, decoding, control-flow recovery, library analysis, and SLIR-based
analysis. Slecompiler does not become part of the Sleela execution path and does not
execute an analyzed artifact merely because it can decode or lift it.

See [`decompiler/API.md`](decompiler/API.md) and [`decompiler/TUTORIAL.md`](decompiler/TUTORIAL.md).

---

*The Sleela compiler turns a Wrapper™ into core bytecode, and it is version
aware: it enforces the `#sleela` syntax-version pragma so a program always
states — and the compiler always checks — the grammar it was written against.*


## Native source execution versus persistent SLVM artifacts

The `sleela` command-line program supports both forms without creating two language implementations.

### Textual `.sleela`

```sh
./build/sleela run program.sleela
```

This is the direct/native SLeeLa source path: the authoritative C++ frontend reads the source, resolves its syntax version, lexes, parses, performs semantic/compiler lowering, and executes the resulting Core program in the native C SLVM in the same process.

### Persistent `.sleela` Core artifact

```sh
./build/sleela compile program.sleela -o program.sleela
./build/sleela run program.sleela
```

The second invocation recognizes the persistent artifact representation, validates it, loads it through the Core artifact loader, and executes it in SLVM without reparsing source text.

These are **source mode** and **artifact mode**, respectively. They are not a native interpreter and a separate VM interpreter. Both terminate at the same Core/SLVM execution boundary.

See `sleela-virtual-machine/docs/COMMAND-LINE-EXECUTION.md` for the complete command contract.
