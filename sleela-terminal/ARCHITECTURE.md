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
   L6   │  CLI / REPL driver         (main, -c, file)  │
        ├─────────────────────────────────────────────┤
   L5   │  Executor                  run the AST       │
        │    · pipelines, redirections, exit status    │
        │    · control flow (if / while / for / case)  │
        │    · builtins registry                       │
        ├─────────────────────────────────────────────┤
   L4   │  Expansion                 words → strings   │
        │    · parameter/variable, quote removal        │
        │    · arithmetic $(( … )) via the Arith engine │
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

Dependencies point **downward only**: the Executor uses Expansion, which uses
the Arith engine and Core; the Parser uses the Lexer and Core; the Lexer uses
Core. Nothing lower includes anything higher.

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

## L6 — CLI / REPL

`slsh` entry point: `-c "<script>"`, a script file argument, or an interactive
read-eval-print loop. Thin — it only wires stdin/args into the pipeline
`lex → parse → execute`.

## Testing

A `smoke` target exercises each layer: the arithmetic engine (precedence,
associativity, div-by-zero), the lexer (quoting/operators), the parser
(pipelines/redirs/if/while), expansion, and end-to-end execution with builtins.

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
- **M4+:** job control (`&`, `jobs`, `fg`/`bg`), `until` loops, `select`, richer
  builtins (`read`, `test`/`[`, `getopts`), and multi-segment path globbing.

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
| L4 expansion | `braceExpand` ( `{a,b}` / `{m..n}`, cartesian ), `tildeExpand` (`~`/`~user` via `getpwnam`), parameter operators `${x:-/:=/:?/:+}` and `${#x}` (env is now mutable for `:=`) |
| L5 executor | here-doc body piped to stdin (expanded per `expand_body`); functions/builtins run correctly as pipeline stages |
