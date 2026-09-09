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
the core for execution. Sleela source is never interpreted directly.

```
Wrapper™ (.sleela source)
        │
        ▼
  [ Sleela front end — C++ ]
     version check → lexer → parser → AST → compiler (codegen)
        │   emits opcodes and drives the core via the exchange API
        ▼
  [ Sleela Core — C, stable ABI ]
     value model · operand stack · call frames · opcode dispatch loop
     ---- slcore_exchange(): the executable "exchange" API ----
```

The compiler lives in [`impl/frontend/`](impl/frontend/) and owns **no execution
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
| **Compilation / codegen** | `compiler.{h,cpp}` | Lower the AST to core bytecode: declare a global per class field, flatten methods into the core function table, assign local slots, emit opcodes, and resolve built-ins. Throws on semantic errors (unknown variable/function, duplicate field, missing `main`). |
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

### The `#sleela` pragma

A Wrapper™ declares its **syntax version** with a pragma on the first
non-blank, non-comment line:

```java
#sleela 1.0
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
| `1.0`     | **accepted** | within the supported range |
| `1.9`     | **rejected** (too new) | MINOR ahead of the supported max |
| `2.0`     | **rejected** (too new) | MAJOR ahead → breaking grammar unsupported |
| `0.9`     | **rejected** (too old) | below the supported floor |
| `one.zero`| **rejected** (malformed) | not a numeric `MAJOR.MINOR` |
| *(none)*  | **accepted, with warning** | assumed `1.0` for backward compatibility |

A rejected source exits non-zero with a descriptive diagnostic, e.g.:

```
sleela: prog.sleela: error: source declares Sleela syntax 2.0, which exceeds
this compiler's supported range (1.0 .. 1.0). Upgrade the compiler or lower
the #sleela pragma.
```

### Where the supported range lives

The range is defined in [`impl/frontend/version.h`](impl/frontend/version.h):

```cpp
inline SyntaxVersion minSupportedSyntax() { return SyntaxVersion{1, 0}; }
inline SyntaxVersion maxSupportedSyntax() { return SyntaxVersion{1, 0}; }
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
| **Sleela toolchain / implementation** (`sleela` CLI) | **0.1.2** | The C/C++ front end + core in `impl/`. Pre-1.0. | `impl/frontend/driver.cpp` |
| **Sleela language syntax** | **1.0** (range `1.0 .. 1.0`) | The grammar version a `.sleela` file declares via `#sleela`. | `impl/frontend/version.h` |
| **Nordshrift** (`.sst` transpiler driver) | **1.0** | Reuses this front end; enforces the same syntax rules. | `impl/nordshrift/nordshrift.cpp` |
| **NS-SST-0001** (`.sst` format spec) | **1.0.0** (Normative) | The `.sst` control-sheet format. | `SST.model` |
| **SL-META-0001** (metadocument) | **1.0.0** (Pre-Normative) | The governing language metadocument, incl. §4.4. | `src/Sleela.manifest` |

> **In short:** the compiler here is the **0.1.2** toolchain, implementing
> **Sleela language syntax 1.0**. Syntax is versioned independently of the
> implementation: the `0.1.2` toolchain implements syntax `1.0`.

Query the live values:

```sh
./build/sleela version
#  Sleela 0.1.2 (C/C++ core; SHEET.sheet conducted methods; .xclass input)
#    supported .sleela syntax: 1.0 .. 1.0 (declare per-file with '#sleela 1.0')
```

---

## 5. Using the compiler

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

---

*The Sleela compiler turns a Wrapper™ into core bytecode, and it is version
aware: it enforces the `#sleela` syntax-version pragma so a program always
states — and the compiler always checks — the grammar it was written against.*
