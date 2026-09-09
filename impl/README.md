# Sleela

**Sleela** is a small, **Java-like** programming language that runs on top of a
purpose-built **C/C++ execution core**. The core is a Turing-complete,
stack-based bytecode virtual machine with a stable C ABI. The Sleela language
sits *above* the core and drives it entirely through the core's single dispatch
entry point — the **exchange** function (`slcore_exchange`). Sleela source is
never interpreted directly: it is lexed, parsed, and compiled down to core
bytecode, then executed by the C/C++ engine underneath.

> **Wrapper™** — the name of the `.sleela` file type. A **Wrapper™** is a Sleela
> source file: the program unit that carries the metadocument addend (it is
> governed by the Sleela Language Metadocument, SL-META-0001). "`.sleela` file",
> "Sleela source file", and "Wrapper™" are used interchangeably below.

```
Sleela source — a Wrapper™ (.sleela)
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
./build/sleela run examples/hello.sleela      # run a Wrapper™ (.sleela file)
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

### Conducted methods (SHEET.sheet)

Sleela draws its system-object vocabulary from [`../SHEET.sheet`](../SHEET.sheet),
the catalog of common system objects (129 objects across 16 role categories,
carrying the `System` root and its invariants — depth **3024**, complexity
degree **4**). These are *conducted methods*: sheet-backed grooves the compiler
resolves at compile time, giving a program method control, routing, and insight.

| Built-in              | Meaning                                                       |
|-----------------------|---------------------------------------------------------------|
| `conduct("Name")`     | `true` if `Name` is a conducted (catalogued) method           |
| `role("Name")`        | the object's conduct role (e.g. `piping`, `smart-move`)       |
| `insight("Name")`     | the object's insight/gloss (an insight about the object/gain) |
| `congruent("A","B")`  | `true` if A and B route to a known congruence                 |
| `route("A","B")`      | `"A -> B"` when congruent, else `""` (no route)               |
| `sysdepth()`          | the relevant system depth (`3024`)                            |
| `degreemax()`         | the max complexity degree (`4`)                               |

```java
class Conduct {
    void main() {
        print("depth " + sysdepth() + ", degree " + degreemax());
        print(role("Pipeline"));                     // piping
        print(insight("Reward"));                    // accrued value guiding ...
        print(route("Pipeline", "Stream"));          // Pipeline -> Stream
    }
}
```

Run with the sheet located (via `$SLEELA_SHEET` or a nearby `SHEET.sheet`):
`SLEELA_SHEET=../SHEET.sheet ./build/sleela run examples/conduct.sleela`.
The catalog is parsed by the shared `catalog/` module, which Nordshrift also
uses for its object-compatibility list.

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
  examples/             sample Wrapper™ (.sleela) programs (incl. conduct.sleela)
  catalog/              shared SHEET.sheet parser (conduct + object compat)
    sheet_catalog.{h,cpp}  Catalog model: objects, roles, congruence, invariants
  nordshrift/           Nordshrift: the .sst transpiler driver (NS-SST-0001)
    sst_lexer/parser, sheet_model, source_resolve, sleela_emit,
    object_compat (compatibility list + relevance), nordshrift CLI
  Makefile
  DESIGN.md
  README.md
```

> **Note on the repository.** This `impl/` tree is the current C/C++-backed
> implementation. The older 2019 Java "Nordshrift" exploration remains under
> `src/` as project history and is untouched.

## Nordshrift (the `.sst` transpiler driver)

Layered on top of Sleela is **Nordshrift** — the transpiler *driver* defined by
the normative spec **NS-SST-0001** (`/SST.model`). It reads a **`.sst` control
sheet** (indentation-significant, pragma-first) that names Sleela source files
and selects a target, (each a **Wrapper™**) then drives their transpilation into the **triplet** —
**Java**, **Sleela** (executed on the C core here), or **C** — chosen by the
sheet's `target-language` directive.

```sh
./build/nordshrift check nordshrift/examples/demo/build.sst   # validate; NSS-* diagnostics
./build/nordshrift build nordshrift/examples/demo/build.sst   # transpile + (sleela) run on core
```

Nordshrift also carries the [`../SHEET.sheet`](../SHEET.sheet) objects in an
**object compatibility list** and converts each into a per-target *relevance* —
either **direct** (a concrete target construct, e.g. `Thread -> pthread_t`) or
**model** (an abstract role/pattern) — for eventual OS-executable compilation:

```sh
./build/nordshrift objects                       # the 129-object compatibility list
./build/nordshrift relevance --target=c Thread   # direct -> pthread_t
./build/nordshrift relevance --target=java       # whole-catalog table + direct/model tally
```

See [`nordshrift/README.md`](nordshrift/README.md) and
[`nordshrift/NORDSHRIFT.md`](nordshrift/NORDSHRIFT.md).

## The Constitution — ordained constraints, in order

This is the ordered constitution of governing constraints ("congrains") the
system honors. Every rule here is enforced or expressible today; each is drawn
from `SHEET.sheet` (the `congrains` and `limits` sections and the `system`
invariants) or from the NS-SST-0001 diagnostics. Constraints are organized into
named **groupings**. Two of the groupings — **Health** and **IQ** — are
*system* metric groupings (system health and system insight/quality); they
describe the software system's own condition, nothing about people.

### Article I — System invariants (ordained, always in force)

In order of precedence:

1. **Depth** — the relevant system depth is **3024**: the maximum
   nesting/reachability from `System` to any leaf object (`SHEET.sheet` →
   `system.depth`; the `DepthLimit` congrain).
2. **Connectivity** — up to **3024** congruent-linear systems may be connected
   (`system.congruent-linear-systems-max`).
3. **Complexity degree** — the connection order between systems must be **≤ 4**
   (`system.complexity-degree-max`; the `ComplexityDegree` congrain). Degrees:
   `1` direct · `2` mediated · `3` brokered · `4` federated.
4. **The connect rule** — `connect(n)` is permitted iff `n ≤ 3024` **and**
   `degree(topology) ≤ 4`. These are surfaced in Sleela as `sysdepth()` (3024)
   and `degreemax()` (4).

### Article II — Congrains grouping (the binding constraints)

In `SHEET.sheet` order (`section congrains`):

| # | Constraint      | Ordinance                                                        |
|---|-----------------|-------------------------------------------------------------------|
| 1 | `Invariant`     | a condition that must always hold for an object                   |
| 2 | `Precondition`  | what must be true before an operation runs                        |
| 3 | `Postcondition` | what the operation promises afterward                             |
| 4 | `Constraint`    | a restriction on legal object states/relations                    |
| 5 | `Dependency`    | a binding one object has on another (co-grained)                  |
| 6 | `Coupling`      | strength of inter-object binding (keep it loose)                  |
| 7 | `Congruence`    | structural sameness enabling congruent-linear connection          |
| 8 | `Contract`      | the bundle of congrains an object honors                          |

### Article III — Limits grouping (the bounding constraints)

In `SHEET.sheet` order (`section limits`):

| # | Limit             | Ordinance                                                      |
|---|-------------------|-----------------------------------------------------------------|
| 1 | `Bound`           | an inclusive/exclusive extent on a value                        |
| 2 | `Quota`           | a permitted amount over a scope/time                            |
| 3 | `RateLimit`       | cap on operations per unit time                                 |
| 4 | `Timeout`         | a limit on how long an operation may take                       |
| 5 | `Capacity`        | the maximum a container/resource may hold                       |
| 6 | `Threshold`       | a boundary value that changes behavior when crossed             |
| 7 | `DepthLimit`      | maximum reachable depth — here the System depth is **3024**     |
| 8 | `ComplexityDegree`| the connection order between systems — capped at **4**          |

### Article IV — Health grouping (system-health metrics)

System **Health** is the grouping of runtime-condition constraints the system
watches (drawn from the catalog's `attention` and `gain` roles). A system is
*healthy* while all hold:

| Metric        | Constraint                                                          |
|---------------|--------------------------------------------------------------------|
| `Watchdog`    | no line of execution is stalled (heartbeat within its `Timeout`)   |
| `Monitor`     | observed metrics stay within their `Threshold`s                    |
| `Throughput`  | work completed per unit time stays above its floor                 |
| `Capacity`    | no `Resource` exceeds its `Capacity` / high-water mark             |
| `Exception`   | unhandled exceptions do not cross a system boundary                |

### Article V — IQ grouping (system insight / quality)

System **IQ** is the grouping of insight/quality constraints — how well the
system *understands and expresses itself* (drawn from the `gain` and
`congruence` roles and Sleela's conducted-method insights). Higher IQ = more of
these satisfied:

| Metric        | Constraint                                                          |
|---------------|--------------------------------------------------------------------|
| `Insight`     | every conducted object resolves an `insight(...)` (a known gloss)  |
| `Congruence`  | related objects `route(...)` to a known congruence (no dead ends)  |
| `Coupling`    | inter-object coupling stays loose (Article II·6)                   |
| `Contract`    | each object's pre/post/invariant `Contract` is stated              |
| `Optimization`| a defined objective (`gain`) is being measured and improved        |

Query these from Sleela via the conducted methods (`insight`, `role`,
`congruent`, `route`, `sysdepth`, `degreemax`) and from Nordshrift via
`nordshrift objects` / `nordshrift relevance`.

## Status

Version 0.1.0. A working end-to-end pipeline (lex → parse → compile → execute on
the C core) with recursion, loops, arithmetic, strings, and I/O. Next candidates:
short-circuit `&&`/`||`, arrays, a REPL, and a static type checker on top of the
Java-like surface.
