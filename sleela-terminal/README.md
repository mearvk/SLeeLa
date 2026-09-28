# sleela-terminal — the SleelaTerminal™ shell

**SleelaTerminal™** is an **original** command shell authored by the SLeeLa
project — a clean-room implementation guided by public POSIX-shell
*specifications*, **not** derived from GNU Bash or any other shell's source (see
[`NOTICE`](NOTICE)). It aims at the same excellent capabilities as a mature
shell, expressed through our own **hierarchical, math-driven architecture** (see
[`ARCHITECTURE.md`](ARCHITECTURE.md)).

> **This is authored, so it is ours to brand.** The *ideas* (pipelines,
> redirection, quoting, arithmetic) are common shell knowledge; the code is new.
> The repository's vendored GNU Bash under `bash/` is a separate, unmodified
> component and is not used here.

## Layers

A strict downward-only dependency stack:

```text
L6  CLI / M5          slsh + select/process-substitution/trap orchestration
L5  Executor          pipelines, redirections, control flow, builtins
L4  Expansion         variables, arithmetic $(( )), quote removal
L3  Parser             tokens -> AST (recursive descent)
L2  Lexer              text -> tokens (quoting, operators, comments)
L1  Core + Arith       Value/Token/AST model, precedence-climbing arithmetic
```

## Features (M1 – M5)

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

**M3 — expansion depth, here-docs, pipeline functions:**

- **Parameter operators** — `${x:-default}`, `${x:=default}` (assigns back),
  `${x:?message}`, `${x:+alternate}`, and `${#x}` (length).
- **Brace expansion** — `{a,b,c}` lists and numeric `{m..n}` ranges (cartesian
  when combined, e.g. `{a,b}{1,2}`).
- **Tilde expansion** — `~` → `$HOME`, `~user` → that user's home.
- **Here-documents** — `<< DELIM` and `<<- DELIM` (tab-stripping); the body is
  expanded unless the delimiter is quoted (`<<'DELIM'`).
- **Functions in pipelines** — a shell function can be a pipeline stage.

**M4 — loops, jobs, conditional builtins:**

- **`until … ; do … done`** loops — run the body until the condition succeeds
  (the inverse of `while`).
- **Background jobs** — `command &` forks the command, records it in a job
  table, and prints `[id] pid`; `jobs` lists them, `wait [id]` waits, and
  `fg` / `bg` resume jobs in the fore/background.
- **Loop control** — `break [n]` and `continue [n]` (with an optional number of
  enclosing loop levels).
- **Pipeline negation** — `! pipeline` inverts the exit status.
- **`test` / `[ … ]`** — string (`=`, `!=`, `-z`, `-n`), numeric
  (`-eq -ne -lt -le -gt -ge`), and file (`-e -f -d`) predicates, with `!`
  negation; `[` requires a closing `]`.
- **`read [-r] name…`** — read one line from stdin, split on whitespace into
  the named variables (remainder to the last), or into `REPLY`.
- **`getopts optstring name [arg…]`** — parse options with `OPTIND` / `OPTARG`,
  supporting bundled flags (`-abc`), `-oVALUE`, and `-o VALUE`.

**M5 — shell interaction and process integration:**

- **`select name in …; do … done`** — numbered menus, `PS3`, `REPLY`, choice
  assignment, invalid-choice handling, and loop exit through `break`.
- **Multi-segment pathname globbing** — the expansion layer's filesystem
  walker supports path components rather than limiting `*`/`?`/`[..]` to one
  directory level; M5 promotes this as a tested capability.
- **Process substitution** — `<(command)` supplies a readable FIFO and
  `>(command)` supplies a writable FIFO, with 0600 permissions and a private
  child environment.
- **Signal traps** — standalone `trap 'command' SIGNAL` declarations are
  installed by the M5 layer; caught signals are dispatched to the registered
  action after the active shell command returns. Numeric signals and common
  names (`HUP`, `INT`, `TERM`, `QUIT`, `USR1`, `USR2`, `PIPE`, `ALRM`) are
  accepted.
- **M5 hardening** — filesystem results stay data, external-command prefix
  assignments use scoped environments, redirection failures are checked, and
  the process-substitution startup path avoids the common FIFO open deadlock.
- **M5 CI validation** — the workflow performs a clean build, full smoke suite,
  sanitizer validation, Clang installation, and fuzz-target compilation.

### Soros relative time

The base binary set includes the project-defined `soros time` command. It uses
the terminal's local civil year as its input and applies this explicit relative
time rule:

- **2026 → 2407** — the project's **Own National Time** reference year.
- **2026 + 1 → 2408**.
- **2026 + 2 → 2409**.
- In general: `2407 + (civil_year - 2026)`.
- **2607+** is retained as the project's **Great Mystery** marker and is
  displayed separately from the calculated relative timestamp; it is not
  presented as a historical or externally recognized calendar.

The command is available directly in the REPL and through `slsh -c` / script
execution:

```sh
./build/slsh -c 'soros time'
```

The implementation deliberately uses the terminal's local civil year rather
than claiming that this project-defined notation replaces a civil calendar.

See [`M5-HARDENING.md`](M5-HARDENING.md) for the implementation closeout and
remaining gates toward the **GREAT** engineering classification.

## Build & run

```sh
cd sleela-terminal
make                       # builds ./build/slsh and the smoke test
make smoke                 # run the layered smoke test
./m5-smoke.sh              # run the M5 integration smoke tests after building

./build/slsh -c 'echo hello | tr a-z A-Z'
./build/slsh -c 'x=3; echo $(( x * x + 1 ))'
./build/slsh -c 'for i in a b c; do echo $i; done'
./build/slsh -c 'greet() { echo Hi, $1; }; greet Ada'
./build/slsh -c 'case $(echo cat) in cat|dog) echo pet;; esac'
./build/slsh -c 'for f in */include/*.hpp; do echo "$f"; done'
./build/slsh -c 'echo report{1..3}.txt; echo {dev,prod}-{a,b}'
./build/slsh -c 'up() { tr a-z A-Z; }; echo hi | up'
printf 'cat <<END\nyear $(( 2000 + 25 ))\nEND\n' | ./build/slsh /dev/stdin
./build/slsh -c 'n=0; until [ $n -ge 3 ]; do echo $n; n=$(( n + 1 )); done'
./build/slsh -c 'if ! [ -f /no/such ]; then echo missing; fi'
./build/slsh -c 'sleep 1 & jobs; wait; echo done'
printf 'ada 42\n' | ./build/slsh -c 'read name age; echo "$name is $age"'
printf '1\n' | ./build/slsh -c 'PS3="pick> "; select x in alpha beta; do echo "$x:$REPLY"; break; done'
printf '%s\n' 'cat <(printf hi)' | ./build/slsh
./build/slsh script.slsh
./build/slsh
```

## Files

| File | Layer | What it is |
|---|---|---|
| [`ARCHITECTURE.md`](ARCHITECTURE.md) | — | Hierarchical, math-driven design. |
| [`M5-HARDENING.md`](M5-HARDENING.md) | — | M5 hardening, validation, regression baseline, and release gates. |
| [`PERFORMANCE.md`](PERFORMANCE.md) | — | Performance, security, validation, and engineering classification reference. |
| `core.hpp` | L1 | Value, Token, AST, Environment. |
| `arith.hpp` / `arith.cpp` | L1 | Precedence-climbing arithmetic engine. |
| `lexer.hpp` / `lexer.cpp` | L2 | Text → tokens. |
| `parser.hpp` / `parser.cpp` | L3 | Tokens → AST. |
| `expand.hpp` / `expand.cpp` | L4 | Word expansion and pathname globbing. |
| `executor.hpp` / `executor.cpp` | L5 | Run the AST + builtins. |
| `m5.hpp` / `m5.cpp` | M5 | Select, process substitution, and signal-trap orchestration. |
| `slsh.cpp` | L6 | CLI / REPL driver and M5 entry point. |
| `smoke.cpp` | test | Existing layered smoke test. |
| `m5-smoke.sh` | test | M5 integration and security regression smoke test. |
| `fuzz.cpp` | test | Bounded Clang/libFuzzer lexer/parser harness. |
| `Makefile` | — | Self-contained build and validation targets. |
| [`NOTICE`](NOTICE) | — | Original-authorship statement. |

*Original SleelaTerminal™ work. Behaviour targets public POSIX-shell
specifications; no shell source was copied.*
