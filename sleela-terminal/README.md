# sleela-terminal — the SleelaTerminal™ shell

**SleelaTerminal™** is an **original** command shell authored by the SLeeLa
project — a clean-room implementation guided by public POSIX-shell
*specifications*, **not** derived from GNU Bash or any other shell's source (see
[`NOTICE`](NOTICE)). It aims at the same excellent capabilities as a mature
shell, expressed through our own **hierarchical, math-driven architecture** (see
[`ARCHITECTURE.md`](ARCHITECTURE.md)).

> **This is authored, so it is ours to brand.** The *ideas* (pipelines,
> redirection, quoting, arithmetic) are common shell knowledge; the *code* is
> new. The repository's vendored GNU Bash under `bash/` is a separate,
> unmodified component and is not used here.

## Layers

A strict downward-only dependency stack:

```text
L6  CLI / REPL          slsh: -c "script" | file | interactive
L5  Executor            pipelines, redirections, control flow, builtins
L4  Expansion           variables, arithmetic $(( )), quote removal
L3  Parser              tokens -> AST (recursive descent)
L2  Lexer               text -> tokens (quoting, operators, comments)
L1  Core + Arith        Value/Token/AST model, precedence-climbing arithmetic
```

## Features (M1 + M2)

**M1 — the core:**

- **Core model** — `Value`, `Token`, AST nodes, `Environment`.
- **Arithmetic engine** — an original precedence-climbing integer evaluator
  (used by `$(( … ))`), with correct precedence/associativity and div-by-zero
  reporting.
- **Lexer** — words, single/double quoting, operators (`| < > >> ; && ||`),
  comments, `name=value` assignments.
- **Parser** — simple commands, pipelines, and-or lists, redirections, `if`,
  `while`, assignments.
- **Executor** — runs the AST: assignments, expansion, builtin/external
  dispatch, pipelines, redirections, control flow, and `$?` exit status.
- **Builtins** — `cd`, `pwd`, `echo`, `export`, `unset`, `true`, `false`,
  `exit`, `set`, `:`.

**M2 — control, functions, expansion:**

- **`for … in … ; do … done`** loops (iterate a word list; falls back to `$@`).
- **`case … in pat) … ;; esac`** with glob patterns and `|` alternation.
- **Shell functions** `name() { … }` with positional parameters `$1`, `$2`, …,
  `$@`, `$#` (scoped per call).
- **Command substitution** `$( … )` — captures a command's stdout.
- **Pathname globbing** — `*`, `?`, `[a-z]`, `[!..]`, with field splitting.
- **Brace groups** `{ … }` run in the current shell.

## Build & run

```sh
cd sleela-terminal
make                       # builds ./build/slsh and the smoke test
make smoke                 # run the layered smoke test

./build/slsh -c 'echo hello | tr a-z A-Z'   # (external tr via PATH)
./build/slsh -c 'x=3; echo $(( x * x + 1 ))'
./build/slsh -c 'for i in a b c; do echo $i; done'
./build/slsh -c 'greet() { echo Hi, $1; }; greet Ada'
./build/slsh -c 'case $(echo cat) in cat|dog) echo pet;; esac'
./build/slsh -c 'for f in *.md; do echo $f; done'   # globbing
./build/slsh script.slsh                    # run a script file
./build/slsh                                # interactive REPL
```

## Files

| File | Layer | What it is |
|---|---|---|
| [`ARCHITECTURE.md`](ARCHITECTURE.md) | — | The hierarchical, math-driven design. |
| `core.hpp` | L1 | Value, Token, AST, Environment. |
| `arith.hpp` / `arith.cpp` | L1 | The precedence-climbing arithmetic engine. |
| `lexer.hpp` / `lexer.cpp` | L2 | Text → tokens. |
| `parser.hpp` / `parser.cpp` | L3 | Tokens → AST. |
| `expand.hpp` / `expand.cpp` | L4 | Word expansion. |
| `executor.hpp` / `executor.cpp` | L5 | Run the AST + builtins. |
| `slsh.cpp` | L6 | CLI / REPL driver. |
| `smoke.cpp` | test | Layered smoke test. |
| `Makefile` | — | Self-contained build. |
| [`NOTICE`](NOTICE) | — | Original-authorship statement. |

*Original SleelaTerminal™ work. Behaviour targets public POSIX-shell
specifications; no shell source was copied.*
