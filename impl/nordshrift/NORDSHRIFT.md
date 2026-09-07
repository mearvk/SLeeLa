# Nordshrift — the Style-Sheet system language

Nordshrift is an **interpreter/transpiler that addresses system components and
combines them**. A Nordshrift program is not a list of statements; it is a
**Style Sheet** — *structural input* (components and their properties) with some
*functional attach* (behavior bound to lifecycle points). The file format is
**`.sst`**.

Nordshrift reads one `.sst` source, builds a **component graph**, and then
**runs into** one of three backends — the *triple input manifold*:

```
                       example.sst   (one Style-Sheet source)
                              │
                              ▼
                 ┌────────────────────────────┐
                 │  Nordshrift front end (C++) │
                 │  lexer → parser → graph     │
                 │  + semantics (uses, order,  │
                 │    thread annotations)      │
                 └──────────────┬─────────────┘
                                │  one graph, three emitters
              ┌─────────────────┼──────────────────────┐
              ▼                 ▼                        ▼
        Java emitter      Sleela emitter (.sleela)   C/C++ emitter
        → .java           → runs on the C core        → .c
        (javac + run)     (sleela run)                (gcc + run)
```

The three emissions are **behaviorally equivalent**: the same `.sst` produces
the same observable output on Java, on Sleela (executed by the C core built in
`../core`), and on C/C++.

## Why a style sheet

A style sheet is the natural notation for *"address a component, then describe
it."* CSS rules are `selector { property: value; }`. Nordshrift keeps that
shape and adds **functional attaches** — property values that are `{ ... }`
blocks of behavior — plus **combination** via `uses:`, which wires one component
to others (the "addresses system components and combines" part).

## `.sst` grammar

```
program        := item*
item           := component

component      := "component" IDENT "{" member* "}"

member         := property | attach | uses
property       := IDENT ":" value ";"
uses           := "uses" ":" IDENT ("," IDENT)* ";"
attach         := ("on" "-")? IDENT ":" block   // e.g. on-start:, tick:, main:
                  // any property whose value is a block is a functional attach

value          := STRING | NUMBER | BOOL | IDENT
block          := "{" stmt* "}"

// ---- functional attach language (Turing-complete) ----
stmt           := "let" IDENT "=" expr ";"
                | IDENT "=" expr ";"
                | "if" "(" expr ")" stmt ("else" stmt)?
                | "while" "(" expr ")" stmt
                | "print" "(" expr ")" ";"
                | "call" IDENT "." IDENT "(" args? ")" ";"   // invoke sibling attach
                | block
expr           := or
or             := and ("||" and)*
and            := eq  ("&&" eq)*
eq             := rel (("=="|"!=") rel)*
rel            := add (("<"|"<="|">"|">=") add)*
add            := mul (("+"|"-") mul)*
mul            := un  (("*"|"/"|"%") un)*
un             := ("-"|"!") un | primary
primary        := NUMBER | STRING | BOOL | IDENT
                | "prop" "(" IDENT ")"           // read this component's property
                | "(" expr ")"
args           := expr ("," expr)*
```

Comments are `// line` and `/* block */`.

### Reserved lifecycle attaches

| Attach       | Meaning                                                        |
|--------------|----------------------------------------------------------------|
| `on-start`   | run once when the component is activated                      |
| `main`       | the program entry attach (exactly one component should have it)|
| `tick`       | body run `repeat:` times (a simple bounded loop hook)         |

Any other `name: { ... }` is a **named attach** (a method) callable via
`call Component.name();`.

### Structural properties (well-known)

| Property   | Type   | Effect                                                             |
|------------|--------|--------------------------------------------------------------------|
| `threads`  | number | component runs its `on-start`/`tick` on N worker threads           |
| `repeat`   | number | how many times `tick` runs                                         |
| `port`,... | any    | plain data, readable in attaches via `prop(port)`                  |

`uses: A, B;` records dependency edges; the semantic pass **topologically
orders** components so a component's dependencies start first, and reports a
clear error on a cycle.

## Example

```
// server.sst
component Logger {
    level: "info";
    log: {
        print("[" + prop(level) + "] " + msg);
    }
}

component Server {
    port: 8080;
    threads: 2;
    uses: Logger;

    on-start: {
        print("server starting on port " + prop(port));
    }

    main: {
        let i = 0;
        while (i < 3) {
            print("request " + i);
            i = i + 1;
        }
        print("server done");
    }
}
```

## Thread model (thread-friendly, modern sense)

Nordshrift components are **isolated units**: an attach reads its own component's
properties and only interacts with siblings through explicit `call`. There is no
shared mutable global state at the language level, so the emitters are free to
run components concurrently:

- **`threads: N`** on a component means its activation body is dispatched onto
  `N` workers.
- **Java** emitter: an `ExecutorService` (fixed thread pool) per threaded
  component; `main` joins.
- **C/C++** emitter: `pthread_create` workers joined before continuing.
- **Sleela** emitter: the current Sleela core is single-threaded, so threaded
  components are emitted as a deterministic sequential fold (N sequential
  passes) — same observable output, honoring the isolation contract. (When the
  core gains threads this becomes a real fan-out.)

Turing-completeness comes from the attach language: unbounded `while` +
mutable `let` bindings + conditionals + arithmetic.

## Lowering summary (per target)

| Nordshrift            | Java                          | Sleela                         | C/C++                        |
|-----------------------|-------------------------------|--------------------------------|------------------------------|
| component `C`         | `class C`                     | `class C` methods              | struct `C` + funcs           |
| property `p: v`       | field / constant              | local/const in accessor        | `#define`/const / struct field |
| attach `name: {..}`   | method `name()`               | method `name()`                | function `C_name()`          |
| `prop(p)`             | field read                    | inlined constant               | field/const read             |
| `uses: A`             | reference / start order       | call order                     | call order                   |
| `call A.f()`          | `A.f()`                       | `A_f()`                        | `C_f()`                      |
| `threads: N`          | `ExecutorService(N)`          | N sequential passes            | N `pthread`s                 |
| `print(e)`            | `System.out.println`          | `print`                        | `printf`                     |

## CLI

```
nordshrift emit --target=java   file.sst   # writes/prints Java source
nordshrift emit --target=sleela file.sst   # writes/prints Sleela source
nordshrift emit --target=c      file.sst   # writes/prints C source
nordshrift run file.sst                     # emit Sleela + execute on the C core
```

## Layout

```
impl/nordshrift/
  NORDSHRIFT.md          this document
  sst_lexer.{h,cpp}      .sst tokenizer
  sst_ast.h              component graph + attach IR
  sst_parser.{h,cpp}     .sst parser
  sst_sema.{h,cpp}       resolve uses, cycle check, topo order
  emit_java.{h,cpp}      Java emitter
  emit_sleela.{h,cpp}    Sleela emitter (runs on the C core)
  emit_c.{h,cpp}         C/C++ emitter
  nordshrift.cpp         the CLI driver
  examples/*.sst
```
