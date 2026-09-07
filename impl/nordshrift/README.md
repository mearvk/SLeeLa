# Nordshrift

**Nordshrift** is an interpreter/transpiler that **addresses system components
and combines them**. A Nordshrift program is a **Style Sheet** — *structural
input* (components and their properties) with some *functional attach*
(behavior bound to lifecycle points). The file format is **`.sst`**.

Nordshrift reads one `.sst` source, builds a **component graph**, and then
**runs into** one of three backends — the **triple input manifold**:

```
                       example.sst   (one Style-Sheet source)
                              │
                   ┌──────────┴──────────┐
                   │ Nordshrift front end │  lexer → parser → graph → sema
                   │        (C++)         │  (resolve uses, topo-order, checks)
                   └──────────┬──────────┘
              ┌───────────────┼────────────────────┐
              ▼               ▼                     ▼
        Java emitter    Sleela emitter (.sleela)  C/C++ emitter
        → .java         → runs on the C core       → .c
        (javac + run)   (executes in-process)      (gcc + run)
```

For the same `.sst`, the three emissions are **behaviorally equivalent**: the
deterministic parts of the program produce identical output on Java, on Sleela
(executed by the C core in [`../core`](../core)), and on compiled C.

## Build & run

```sh
cd impl
make                                   # builds build/sleela and build/nordshrift

# Emit each target:
./build/nordshrift emit --target=sleela nordshrift/examples/counter.sst
./build/nordshrift emit --target=java   nordshrift/examples/counter.sst
./build/nordshrift emit --target=c      nordshrift/examples/counter.sst

# Run through the Sleela C core (in-process):
./build/nordshrift run nordshrift/examples/counter.sst
# -> factorial of 6 = 720

make test-nordshrift                   # runs every .sst example via the Sleela core
```

Emitting and actually compiling all three for `counter.sst`:

```sh
mkdir -p out
./build/nordshrift emit --target=c      nordshrift/examples/counter.sst > out/p.c
gcc -std=c11 -pthread out/p.c -o out/p_c && ./out/p_c          # factorial of 6 = 720

./build/nordshrift emit --target=java   nordshrift/examples/counter.sst > out/NordshriftProgram.java
javac -d out out/NordshriftProgram.java && java -cp out NordshriftProgram   # factorial of 6 = 720

./build/nordshrift run nordshrift/examples/counter.sst          # factorial of 6 = 720
```

## The `.sst` Style-Sheet language

A program is a set of **components**. A component holds three kinds of members:

```
component Logger {
    level: "info";                 // structural property (data)

    banner: {                      // functional attach (a callable method)
        print("[" + prop(level) + "] logger ready");
    }
}

component Server {
    port: 8080;
    threads: 2;                    // activation runs on 2 workers
    repeat: 3;                     // `tick` runs 3 times per activation
    uses: Logger;                  // combination: Server depends on Logger

    on-start: {                    // lifecycle attach: runs when activated
        print("server starting on port " + prop(port));
        call Logger.banner();      // address + invoke a sibling component
    }

    tick: { print("tick"); }

    main: {                        // program entry (exactly one component has it)
        let i = 0;
        while (i < 5) { i = i + 1; }
        print("server done");
    }
}
```

- **Structural properties** (`name: value;`) are plain data, read inside
  attaches with `prop(name)`. Well-known ones: `threads`, `repeat`.
- **Functional attaches** (`name: { ... }`) are blocks of behavior. Reserved
  names: `on-start` (run on activation), `tick` (run `repeat` times), `main`
  (the entry point). Any other name is a callable method.
- **`uses: A, B;`** records dependency edges; components are activated in
  dependency-first order (topologically sorted). Cyclic `uses` is a compile
  error.
- The attach language is **Turing-complete**: `let`/assignment, `if/else`,
  `while`, arithmetic and comparisons, `&& || !`, string concatenation with
  `+`, `print(...)`, and `call C.f();`.

## Thread model (thread-friendly)

Components are **isolated**: an attach touches only its own component's
properties and interacts with siblings solely through explicit `call`. With no
shared mutable state at the language level, the emitters are free to run
components concurrently. `threads: N` renders as:

| Target | Rendering |
|--------|-----------|
| Java   | a fixed `ExecutorService(N)`, joined before continuing (real threads) |
| C/C++  | `N` `pthread`s, joined before continuing (real threads) |
| Sleela | `N` × `spawn(Comp_activate)` + `join()` on the core's threading model (real threads, up to 128) |

All three now run threaded activations on **real threads**. Because the
components are isolated, the program's **result** is consistent across targets;
only the interleaving of a threaded activation's output differs (visible when
`server.sst` runs on multiple threads, which is expected and correct). The
Sleela core keeps each `print` line atomic.

## Files

```
nordshrift/
  NORDSHRIFT.md          full design: grammar, semantics, thread model, lowering
  README.md              this file
  sst_lexer.{h,cpp}      .sst tokenizer
  sst_ast.h              component graph + functional-attach IR
  sst_parser.{h,cpp}     recursive-descent parser
  sst_sema.{h,cpp}       resolve uses, cycle check, topological order, validation
  emit_java.cpp          Java emitter
  emit_sleela.cpp        Sleela emitter (runs on the C core)
  emit_c.cpp             C/C++ emitter
  nordshrift.cpp         the `nordshrift` CLI
  examples/*.sst         counter, server
```

See [`NORDSHRIFT.md`](NORDSHRIFT.md) for the complete grammar and the per-target
lowering table.
