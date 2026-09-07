# Sleela

**Sleela** is a small, **Java-like** programming language that runs on top of a
purpose-built **C/C++ execution core**. The core is a Turing-complete,
stack-based bytecode virtual machine with a stable C ABI. The Sleela language
sits *above* the core and drives it entirely through the core's single dispatch
entry point — the **exchange** function (`slcore_exchange`). Sleela source is
never interpreted directly: it is lexed, parsed, and compiled down to core
bytecode, then executed by the C/C++ engine underneath.

```
Sleela source (.sleela)
        |
        v
  [ Sleela front end — C++ ]
     lexer -> parser -> AST -> compiler
        |
        |  emits opcodes + drives the core via the exchange API
        v
  [ Sleela Core — C, stable ABI ]
     value model | operand stack | call frames | opcode dispatch loop
     ---- slcore_exchange(): the executable "exchange" API ----
```

## Why this shape

The request was for a **C/C++-backed interpreter** where the C/C++ layer provides
a complete, Turing-responsible execution API (an "exchange" function), and the
Sleela language sits above it and calls that exchange function. That is exactly
the boundary here:

- **The core** (`core/`) knows nothing about Sleela. It is an embeddable VM that
  understands *opcodes and values*. Everything an embedder needs — assembling a
  program and running it — is reachable through the one symbol
  `slcore_exchange()`. (Typed builder helpers such as `slvm_emit` are thin
  convenience wrappers over that same dispatch.)
- **The language** (`frontend/`) is a compiler front end. It turns Java-like
  source into core bytecode and hands it to the core.

Any language could target this core; Sleela is the first.

## Building

Requires a C11 compiler and a C++17 compiler (gcc/clang).

```sh
cd impl
make            # produces build/sleela
```

## Running

```sh
./build/sleela run examples/hello.sleela
./build/sleela version
make test       # runs every example under examples/
```

## The language (first pass)

Sleela looks like a small Java. A program is one or more classes; execution
starts at `main()`.

```java
class Factorial {

    int fact(int n) {
        if (n <= 1) {
            return 1;
        }
        return n * fact(n - 1);
    }

    void main() {
        int i = 1;
        while (i <= 10) {
            print("fact(" + i + ") = " + fact(i));
            i = i + 1;
        }
    }
}
```

Supported today:

| Feature        | Notes                                                        |
|----------------|--------------------------------------------------------------|
| Classes        | `class Name { ... }`; methods are flattened into core funcs  |
| Types          | `int`, `double`, `boolean`, `String`, `void` (parsed; core is dynamic) |
| Methods        | params, recursion, calls; `main()` is the entry point        |
| Locals         | `int x = ...;` typed declarations                            |
| Control flow   | `if / else`, `while`, `for`                                  |
| Operators      | `+ - * / %`, `== != < <= > >=`, `&& || !`, unary `-`         |
| Strings        | literals with escapes; `+` concatenates (Java-style)         |
| I/O            | `print(expr)` built-in                                       |
| `return`       | with or without a value                                      |
| Fields         | class-level `int total = 0;` — shared state (compile to globals) |
| Threading      | `spawn(m)`, `join()`, `lock(n)`/`unlock(n)`, `send(slot,v)`/`recv(slot)` |

See `examples/` for `hello`, `factorial`, `fibonacci`, `fizzbuzz`, and
`threads` (spawns 8 workers, guards a shared field with a lock, and coordinates
via the 2-tuple mailbox).

### Threading built-ins

The core ships a clean, bounded threading model (up to **128** threads); the
Sleela surface exposes it as built-ins:

| Built-in            | Meaning                                                    |
|---------------------|-------------------------------------------------------------|
| `spawn(method)`     | run a no-arg method on a new thread; yields a thread id     |
| `join()`            | wait for all spawned threads to finish                      |
| `lock(n)`/`unlock(n)`| acquire/release lock-table slot `n` (a compile-time int)   |
| `send(slot, value)` | send the 2-tuple `(slot, value)` onto the mailbox line      |
| `recv(slot)`        | block until a tuple arrives on `slot`; yields its value     |

Class **fields** are shared across threads (they compile to the core's
thread-safe globals); method **locals** are per-thread. `print` output is
atomic per line. See [`DESIGN.md`](DESIGN.md) for the full model.

## The exchange API (core C ABI)

Declared in [`core/sleela_core.h`](core/sleela_core.h). The whole core can be
driven through one symbol:

```c
SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg);
```

`SLExchangeOp` selects the operation (reset, add-const, declare-global,
begin/end-func, emit, patch, set-entry, run, get-result); `SLExchangeArg` is the
tagged in/out packet. `core/exchange_smoke.c` assembles and runs
`main() { print(6 * 7); }` using *only* `slcore_exchange` — no builder helpers —
as a proof that the exchange function alone is sufficient to program the core.

Build and run that proof:

```sh
gcc -std=c11 -O2 core/exchange_smoke.c core/sleela_core.c -o /tmp/smoke && /tmp/smoke
# prints: 42
```

### VM at a glance

- **Value model** (`SLValue`): tagged union of `null`, `int` (`int64`),
  `double`, `bool`, and interned `String`.
- **Stack machine**: operand stack, constant pool, globals, and call frames
  (`CALL` / `RET` with a locals region per frame). Unbounded conditional jumps
  plus recursion make it Turing-complete.
- **Opcodes**: `CONST/POP/DUP`, `LOADG/STOREG/LOADL/STOREL`, arithmetic,
  comparisons, logic, `JMP/JMPF`, `CALL/RET`, `PRINT`, `HALT`.

Full details are in [`DESIGN.md`](DESIGN.md).

## Layout

```
impl/
  core/                 C/C++ execution core (the exchange API + VM)
    sleela_core.h       public C ABI
    sleela_core.c       stack VM + dispatch loop
    exchange_smoke.c    proof the core is drivable via slcore_exchange alone
  frontend/             Sleela language layer (C++)
    lexer.{h,cpp}       Java-like tokenizer
    ast.h               AST nodes
    parser.{h,cpp}      recursive-descent parser
    compiler.{h,cpp}    AST -> core bytecode
    driver.cpp          the `sleela` CLI
  examples/             sample .sleela programs
  nordshrift/           Nordshrift: the .sst Style-Sheet system language
    sst_lexer/parser/sema, emit_java/sleela/c, nordshrift CLI, examples/*.sst
  Makefile
  DESIGN.md
  README.md
```

> **Note on the repository.** This `impl/` tree is the current C/C++-backed
> implementation. The older 2019 Java "Nordshrift" exploration remains under
> `src/` as project history and is untouched.

## Nordshrift (the Style-Sheet system language)

Layered on top of Sleela is **Nordshrift** — an interpreter/transpiler that
addresses and combines *system components* described as **Style Sheets**
(`.sst`: structural properties + functional attaches). One `.sst` source runs
into a **triple manifold** of targets — **Java**, **Sleela** (executed on the C
core here), and **C/C++** — with behaviorally equivalent output. It is
Turing-complete via the attach language and thread-friendly via isolated,
optionally-threaded components.

```sh
./build/nordshrift run nordshrift/examples/counter.sst      # runs on the C core
./build/nordshrift emit --target=java nordshrift/examples/server.sst
```

See [`nordshrift/README.md`](nordshrift/README.md) and
[`nordshrift/NORDSHRIFT.md`](nordshrift/NORDSHRIFT.md).

## Status

Version 0.1.0. A working end-to-end pipeline (lex → parse → compile → execute on
the C core) with recursion, loops, arithmetic, strings, and I/O. Next candidates:
short-circuit `&&`/`||`, arrays, a REPL, and a static type checker on top of the
Java-like surface.
