# SLeeLa Terminal — Performance Reference

**Scope:** M1–M5  
**Purpose:** Reference structure for performance, correctness, and security.

## 1. Runtime model

```
Source → Lexer → Parser/AST → Expansion → Redirections/IPC → Executor → OS
```

M5 adds `select`, multi-component pathname globbing, process substitution, and traps/signals. These must integrate with the existing AST, expansion, executor, and OS abstraction rather than introduce parallel shell semantics.

## 2. Performance hierarchy

Correctness and security take precedence over optimization:

```
semantic correctness > security > resource correctness > performance > micro-optimization
```

Avoid unnecessary parsing, filesystem calls, process creation, descriptor duplication, and repeated environment/AST traversal.

## 3. M1 — Core

**Structure:** lexer → parser → executor.

**Performance:** target linear lexical scanning; avoid repeated string concatenation and reparsing.

**Security:** preserve token boundaries, reject malformed syntax deterministically, and never invoke a host shell merely to execute SLeeLa commands.

## 4. M2 — Expansion

Expansion is a data boundary:

```
shell syntax ≠ expanded data ≠ argv elements
```

Expanded values must remain data. In particular, filesystem results must never be converted into shell source and reparsed.

## 5. M3 — Processes and substitution

Minimize `fork()` and helper processes where semantics permit. Close unused descriptors promptly and preserve child exit status.

Child environments and descriptors must be explicit. Command substitution must preserve stdout behavior, status, environment semantics, and descriptor ownership.

## 6. M4 — Jobs and terminal control

Interactive jobs should use explicit process-group and controlling-terminal semantics. Use OS child-state notifications instead of aggressive polling.

Foreground/background transitions and signals must respect terminal ownership and must not leak shell-owned descriptors.

## 7. M5 — select

`select` is an interactive loop:

```
menu → stdin → selection → variable/REPLY → body → repeat
```

Input and menu values are data. They must never be reparsed as shell source.

Quoted assignments such as `PS3="pick> "` must retain assignment semantics.

## 8. M5 — Multi-segment pathname globbing

The required model is:

```
pattern → path components → filesystem traversal → matched pathnames → argv fields
```

**Forbidden model:**

```
pattern → filename substitution into source text → re-lex/re-parse
```

Filenames containing spaces, quotes, shell metacharacters, newlines, or Unicode must remain a single pathname/argv field.

Traversal should prune nonmatching components early and provide deterministic ordering where shell semantics require it.

Symlinks, permissions, inaccessible directories, and filesystem races are untrusted conditions.

## 9. M5 — Process substitution

`<(command)` and `>(command)` expose IPC endpoints to the consuming command.

Temporary FIFOs or equivalent IPC objects must be unpredictable, restrictive, short-lived, and cleaned up on success, failure, interruption, and shell exit.

Producer/consumer startup must not deadlock. Child failures and relevant statuses must not be silently converted to success.

## 10. M5 — Traps and signals

Signal handlers must remain async-signal-safe:

```
OS signal → minimal handler → pending state → normal shell context → trap action
```

Handlers must not execute arbitrary shell code directly. Pending signals must not be accidentally collapsed when multiple signals arrive.

Trap state must have defined inheritance and restoration semantics for child processes and subshells.

## 11. File-descriptor discipline

Every descriptor has an owner and lifecycle:

```
create → secure/close-on-exec as appropriate → transfer → use → close
```

Audit pipeline pipes, here-document resources, process-substitution FIFOs, terminal descriptors, and captured stdout/stderr for leaks and blocking behavior.

## 12. Resource limits

M5 should impose deliberate bounds for command size, glob traversal/matches, simultaneous process substitutions, open descriptors, background jobs, and pending signal work.

Failures should be deterministic and diagnostic rather than causing uncontrolled memory/process growth.

## 13. Security test baseline

Required negative tests include:

- glob matches containing spaces, tabs, quotes, `;`, `&`, `|`, redirections, parentheses, newlines, and Unicode;
- symlink and inaccessible-directory cases;
- producer/consumer process-substitution failure;
- early exit and interruption;
- multiple simultaneous substitutions;
- FIFO cleanup;
- signal bursts and signals during blocking input;
- nested traps;
- `select` EOF, invalid input, empty input, and quoted `PS3`.

## 14. Measurement baseline

Record before optimization:

- parse latency;
- expansion latency;
- glob traversal time;
- process creation/startup latency;
- pipeline startup time;
- command-substitution latency;
- process-substitution startup latency;
- signal-to-trap latency;
- resident memory;
- open-descriptor count.

Measure simple commands, long pipelines, large glob candidate sets, nested substitutions, background jobs, and repeated interactive loops.

## 15. Release classification

| Level | Requirement |
|---|---|
| Experimental | Feature exists; tests incomplete |
| Development | Implementation actively validated |
| Hardened | Security and negative tests substantially complete |
| Release candidate | Functional, security, and performance gates pass |
| Production | RC plus platform-specific validation |

A feature is not production-ready merely because its parser or executor path exists.

## 16. Reference principles

SLeeLa Terminal should favor:

- structured AST execution;
- strict code/data separation;
- explicit OS boundaries;
- least-privilege IPC;
- deterministic resource ownership;
- fail-closed security checks;
- bounded resource consumption;
- comprehensive negative testing;
- measured performance;
- platform-aware implementations.

**Reference rule:** a performance optimization that changes shell-visible semantics is a regression.

**End of PERFORMANCE.md**
