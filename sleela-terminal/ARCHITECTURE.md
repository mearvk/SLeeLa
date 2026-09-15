# SleelaTerminal™ shell — architecture

SleelaTerminal™ is an **original** command shell (see [`NOTICE`](NOTICE)): a
clean-room implementation, guided by public POSIX-shell *specifications* and the
*ideas* common to shells, not by copying any existing shell's source. It aims at
the same excellent capabilities as a mature shell — pipelines, redirections,
quoting, expansion, control flow, functions, builtins, arithmetic — expressed
through our own hierarchical, math-driven architecture.

## Design goals

1. **Hierarchical.** A strict layer stack; each layer depends only on the ones
   below it. No cycles, clean interfaces, testable in isolation.
2. **Math-driven.** Parsing and evaluation are grounded in explicit formal
   models: a token grammar, a precedence-climbing arithmetic evaluator, and an
   AST with well-defined evaluation rules — not ad-hoc string munging.
3. **Well-structured C++ (C++17).** Value types for the model, `std::variant`/
   enums for node kinds, small focused translation units, no external deps.
4. **Original + attributed.** Ours to brand SleelaTerminal™; the ideas are
   POSIX-shell common knowledge, the code is new.

## The layer stack

```text
        ┌─────────────────────────────────────────────┐
   L6   │  CLI / M5 orchestration                     │
        │    · select menus, process substitution     │
        │    · signal trap registration/dispatch      │
        ├─────────────────────────────────────────────┤
   L5   │  Executor                  run the AST       │
        │    · pipelines, redirections, exit status    │
        │    · control flow (if/while/until/for/case)  │
        │    · background jobs (&) + job-control        │
        │    · builtins registry                       │
        ├─────────────────────────────────────────────┤
   L4   │  Expansion                 words → strings   │
        │    · parameter/variable, quote removal        │
        │    · arithmetic $(( … )) via the Arith engine │
        │    · pathname globbing across path segments  │
        ├─────────────────────────────────────────────┤
   L3   │  Parser                    tokens → AST      │
        │    · recursive descent, one grammar per node  │
        ├─────────────────────────────────────────────┤
   L2   │  Lexer                     text → tokens     │
        │    · words, quoting, operators, comments      │
        ├─────────────────────────────────────────────┤
   L1   │  Core model                shared types      │
        │    · Token, Word, AST nodes, Value            │
        │    · Arith engine (own precedence climber)    │
        │    · Environment (variables + exit status)    │
        └─────────────────────────────────────────────┘
```

Dependencies point **downward only** for the core shell layers. The M5 driver is
an orchestration boundary at L6: it can allocate operating-system resources
(FIFOs/processes) and register signal handlers before delegating ordinary shell
text to the established L1–L5 runner. Nothing in the parser or expansion layer
depends on M5.

## L1 — Core model

- **`Value`** — a shell value (string with an integer view for arithmetic).
- **`Token`** — `{ kind, text }`; kinds: `Word`, `Assign`, `Pipe`, `Semi`,
  `Newline`, `Less`, `Great`, `DGreat`, `LParen`, `RParen`, `And`, `Or`,
  keywords (`If/Then/Elif/Else/Fi/While/Do/Done`), `Eof`.
- **AST nodes** (a small, closed set):
  - `SimpleCommand` — assignments + words + redirections.
  - `Pipeline` — a list of commands joined by `|`.
  - `AndOr` — pipelines joined by `&&` / `||`.
  - `List` — commands separated by `;` / newline.
  - `IfClause`, `WhileClause` — control flow with body `List`s.
  - `Redirection` — `{ op, fd, target }`.
- **`Environment`** — variable map, exit status `$?`, working directory.

## L1 — the arithmetic engine (math-driven core)

A self-contained integer expression evaluator used by `$(( … ))` and the shell's
own numeric needs. Two stages:

1. **Arith lexer** — numbers, identifiers, and operators
   `+ - * / % ( ) < <= > >= == != && || ! unary±`.
2. **Precedence-climbing evaluator** — a single recursive routine parameterised
   by binding power, giving correct precedence and associativity without a
   hand-written cascade of grammar functions. Precedence tiers (low→high):

   | Tier | Operators | Assoc |
   |---|---|---|
   | 1 | `\|\|` | left |
   | 2 | `&&` | left |
   | 3 | `== !=` | left |
   | 4 | `< <= > >=` | left |
   | 5 | `+ -` | left |
   | 6 | `* / %` | left |
   | 7 | unary `+ - !` | right |
   | 8 | `( )`, integer literal, variable | — |

   Division/modulo by zero is a reported error, not undefined behaviour.

## L2 — Lexer

Text → `Token` stream. Handles single quotes (literal), double quotes
(expansion-eligible span), operator recognition with maximal munch
(`>` vs `>>`, `&` vs `&&`, `|` vs `||`), comments (`#` to end of line), and
`name=value` assignment recognition at command-word position. Line/column
tracking for diagnostics.

## L3 — Parser

Recursive descent, one function per grammar production, producing the AST:

```text
list      := andor ( (';' | '\n') andor )*
andor     := pipeline ( ('&&' | '||') pipeline )*
pipeline  := command ( '|' command )*
command   := if_clause | while_clause | simple_command
simple    := (assignment)* (word | redirection)+
if        := 'if' list 'then' list ('elif' list 'then' list)* ['else' list] 'fi'
while     := 'while' list 'do' list 'done'
```

Parse errors are structured (`{message, line, col}`), never crashes.

## L4 — Expansion

Turns parsed words into final argument strings: variable/parameter expansion
(`$name`, `${name}`), arithmetic expansion (`$(( expr ))` via the Arith
engine), and quote removal — each as a distinct, ordered pass. Pure and
testable: `(word, environment) → string`.

Pathname globbing is implemented as a filesystem walk over path components.
A pattern such as `src/*/include/*.hpp` is therefore resolved one component at
a time rather than treating the entire path as one basename match. If no path
matches, the original pattern is preserved (nullglob-off behaviour).

## L5 — Executor

Evaluates the AST against an `Environment`:

- **Simple command:** apply assignments, expand words, then dispatch to a
  **builtin** (registry lookup) or an **external** program (via `posix_spawn`/
  `fork`+`execvp`), applying redirections.
- **Pipeline:** wire stdout→stdin across children; exit status is the last.
- **AndOr:** short-circuit on `$?`.
- **Control flow:** `if`/`while` evaluate their condition `List` and branch on
  exit status (0 = true), matching shell truth semantics.
- **Exit status** is a first-class value threaded through everything (`$?`).

## L6 — M5 orchestration

`m5.cpp` is deliberately outside the normal grammar. It recognizes constructs
that need operating-system resources or interactive state before ordinary
lexing/parsing:

### `select`

`select name in item...; do body; done` is handled by a small quote-aware M5
recognizer. The implementation prints a numbered menu to stderr, uses `PS3` as
the prompt (default `#? `), stores the raw response in `REPLY`, assigns the
selected item to `name`, and executes the body through the normal shell runner.
`break` uses the existing M4 loop-control signal to leave the M5 loop.

### Process substitution

`<(command)` and `>(command)` are rewritten to private FIFOs in `/tmp` with
mode `0600`. A child shell executes the substitution command with a private
`Environment` copy. For `<(...)`, child stdout feeds the FIFO; for `>(...)`,
child stdin reads the FIFO. The main command then sees an ordinary pathname,
so the existing parser/executor can handle it without a new redirection AST
node. Child processes are waited for after the containing command completes.

### Signal traps

Standalone `trap 'command' SIGNAL` declarations are consumed by M5 and mapped
to a small signal-state table. The POSIX signal handler only records the signal
number in `sig_atomic_t`; it never executes shell code from the asynchronous
signal context. After the active shell command returns, M5 runs the registered
trap action through the normal shell runner. Common signal names and numeric
signals are accepted.

M5 is intentionally conservative: it does not attempt to execute arbitrary
shell code from a signal handler, and it does not replace the L1–L5 parser with
a second general-purpose shell grammar.

## CLI / REPL

`slsh` entry point: `-c "<script>"`, a script file argument, or an interactive
read-eval-print loop. The driver enters M5 first and then delegates ordinary
text to `lex → parse → execute`.

## Testing

The existing `smoke` target exercises the M1–M4 layers. `m5-smoke.sh` adds
integration checks for a `select` menu, `<(...)`, `>(...)`, and a `USR1` trap.
The GitHub Actions workflow `.github/workflows/sleela-terminal-m5.yml` builds
the shell and runs both smoke layers on changes to `sleela-terminal`.

## Milestones

- **M1:** Core model + Arith engine + Lexer + Parser (simple commands,
  pipelines, and-or lists, redirections, `if`, `while`, assignments) + Executor
  with a starter builtin set + CLI + smoke test.
- **M2 (done):** `for … in`, `case … esac` (glob patterns, `|` alternation),
  shell **functions** (`name() { … }`) with positional parameters (`$1`, `$@`,
  `$#`), **command substitution** `$( … )`, and **pathname globbing**
  (`*`, `?`, `[..]`, `[!..]`) with field splitting. Brace groups `{ … }` too.
- **M3 (done):** **parameter operators** `${x:-w}` `${x:=w}` `${x:?m}` `${x:+w}`
  and `${#x}`; **brace expansion** `{a,b,c}` and numeric `{m..n}` (cartesian);
  **tilde expansion** `~` / `~user`; **here-documents** `<<` and `<<-` (body
  expanded unless the delimiter is quoted); and **functions inside pipelines**.
- **M4 (done):** **`until` loops** (the inverse of `while`); **background jobs**
  (`&`) with an `Environment` job table and the `jobs` / `fg` / `bg` / `wait`
  builtins; **loop control** `break` / `continue` (with optional level count);
  **pipeline negation** `! pipeline`; and the builtins `test` / `[` (string,
  numeric, and file predicates with `!` negation), `read` (line → variables,
  or `REPLY`), and `getopts` (option parsing via `OPTIND` / `OPTARG`).
- **M5 (implemented baseline):** `select`, multi-segment pathname globbing,
  process substitution, and signal traps. The M5 orchestration layer keeps
  signal handlers async-safe and delegates ordinary command execution back to
  the established runner.
- **M6+ (future):** broader POSIX compatibility, richer job-control semantics,
  process groups/terminals, traps integrated directly into the AST, and further
  expansion/quoting fidelity.

### M2 layer touch-points

| Layer | M2 addition |
|---|---|
| L1 core | `NodeKind::{For,Case,FunctionDef}`, `CaseItem`, function table + positional params on `Environment` |
| L2 lexer | `for/in/case/esac` keywords; `( ) { }` tokens; `$( … )` command-sub word spans; structural-keyword recognition |
| L3 parser | `parseFor`, `parseCase`, `parseFunctionDef`, `parseBraceGroup` |
| L4 expansion | command substitution (via a `CommandRunner` callback → no L4→L5 cycle), field splitting, `globPattern`/`globMatch`, `$1..`/`$@`/`$#` |
| L5 executor | `execFor`, `execCase`, `execFunctionDef`, `callFunction` (scoped positionals), `captureCommand` for `$( … )`, multi-field argv |

### M3 layer touch-points

| Layer | M3 addition |
|---|---|
| L1 core | `RedirOp::Heredoc` + `Redirection.body`/`expand_body`; `Tok::{DLess,DLessDash,HeredocBody}` |
| L2 lexer | `<<` / `<<-` here-doc capture (delimiter + body lines, tab-stripping, quoted-delimiter flag); `{`/`}` only structural when standalone (so `x{1,2}` stays one word) |
| L3 parser | here-doc redirection (delimiter + body token); strips the expand/literal flag |
| L4 expansion | `braceExpand` ( `{a,b}` / `{m..n}`, cartesian ), `tildeExpand` (`~`/`~user` via `getpwnam`), parameter operators `${x:-/:=/:+/:?}` and `${#x}` (env is now mutable for `:=`) |
| L5 executor | here-doc body piped to stdin (expanded per `expand_body`); functions/builtins run correctly as pipeline stages |

### M4 layer touch-points

| Layer | M4 addition |
|---|---|
| L1 core | `NodeKind::Until`; `Tok::{Amp,Bang}`; `Node.child_async` (per-list-child `&` flag) and `Node.negated` (pipeline `!`); `Environment::Job` + job table (`addJob`/`jobs`/`findJob`/`removeJob`); loop-control signal (`requestBreak`/`requestContinue`/`consumeLoopSignal`) |
| L2 lexer | `until` opener; `&` → `Tok::Amp` (background); bare `!` at command-start → `Tok::Bang`; both are command-start positions |
| L3 parser | `parseUntil`; `parseList` treats `&` as an async separator; `parsePipeline` consumes a leading `!` (wrapping a lone command in a negated `Pipeline`); newline allowed after `do` |
| L4 expansion | (unchanged — M4 adds no new word syntax) |
| L5 executor | `execUntil`; async children in `execList` (fork + register job + `[id] pid`); pipeline negation; loop-signal consumption in `execWhile`/`execUntil`/`execFor`/`execList`; builtins `jobs`/`fg`/`bg`/`wait`, `test`/`[`, `read`, `getopts`, `break`/`continue` |

### M5 layer touch-points

| Layer | M5 addition |
|---|---|
| L6 orchestration | `m5.hpp` / `m5.cpp`; M5 callback boundary; select loop; FIFO process substitution; async-safe signal capture |
| L4 expansion | Existing component-by-component filesystem globbing is treated as the M5 multi-segment pathname capability |
| L5 execution boundary | M5 delegates nested commands back through the normal `lex → parse → execute` runner; no duplicate executor is introduced |
| Tests | `m5-smoke.sh` and `.github/workflows/sleela-terminal-m5.yml` |
