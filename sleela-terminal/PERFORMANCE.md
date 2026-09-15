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

## 16. Seeded M5 improvements

The current M5 baseline incorporates these architectural improvements:

- multi-segment globbing is integrated into the normal L4 expansion path;
- filesystem matches remain structured argument data rather than generated shell source;
- process-substitution FIFO startup uses a deadlock-resistant child open strategy;
- the quoted-assignment lexer path recognizes quotes in assignment values without treating them as quoted assignment names;
- external-command prefix assignments use a scoped environment;
- redirection and heredoc `dup2()` failures are checked;
- `make test` includes the M5 integration suite;
- M5 smoke coverage includes a metacharacter-bearing filename regression.

These improvements reduce semantic duplication and close the highest-risk M5 boundaries identified during review. They do not by themselves constitute a production security certification.

## 17. Reference principles

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

## 18. M1–M5 Review Record — Improvement Closeout

### Previous-state assessment

The previous implementation was a **Good-to-Better development-grade shell framework**: the layered architecture was clear and functional, but several semantics and resource boundaries remained deliberately simplified. The principal concerns were duplicated M5 glob logic, source-text glob rewriting, command-prefix assignment leakage into the parent environment, limited job-control semantics, and incomplete production-level negative testing.

### Improvements seeded in the closing review

- M5 multi-component globbing remains in the expansion layer and returns pathname data directly.
- The obsolete source-rewriting glob path has been removed from M5 execution.
- Process-substitution FIFO startup uses a deadlock-resistant open strategy.
- Quoted assignment values are covered by a lexer regression test.
- External-command prefix assignments now use a scoped environment instead of mutating the parent shell.
- Redirection descriptor failures are checked.
- The main smoke test includes regression coverage for filesystem metacharacters.
- `make test` includes the M5 integration suite.
- README and architecture documentation describe the hardened M5 boundary.

### Ending-state assessment

The resulting M1–M5 framework is best classified as **BETTER+**, approaching **GREAT** in architecture and security discipline, but not yet **SUPER** or production-certified.

**BETTER+** means the software has a coherent layered design, meaningful shell functionality, explicit OS/resource boundaries, seeded security regressions, and repeatable validation infrastructure, while still requiring broader conformance testing, platform validation, and deeper job-control/process semantics before a higher classification is justified.

### Classification scale

| Grade | Meaning |
|---|---|
| Good | Functional prototype with basic organization and tests |
| Average | Works in common cases but has material structural or correctness gaps |
| Better | Strong development framework with meaningful security and regression discipline |
| Great | Broadly validated, hardened, portable, and suitable for serious release qualification |
| Super | Exceptional, extensively verified, production-grade implementation with strong conformance, security, portability, and performance evidence |

### Review conclusion

**SLeeLa Terminal M1–M5: BETTER+.**

The next advancement toward **GREAT** should come from test depth and semantic completeness rather than adding features alone: POSIX/Linux behavior matrices, Windows-specific I/O/runtime behavior where supported, process-group job control, complete trap semantics, command-substitution status propagation, descriptor-leak testing, fuzzing, sanitizers, and repeatable performance benchmarks.

This classification is an engineering review judgment, not a formal security certification.

## 19. Next-step validation and hardening

The review now advances from feature completion toward **validation depth**.

### Improvements implemented

1. **Redirection failure handling**
   - `dup2()` failures are now checked rather than silently ignored.
   - Heredoc descriptor setup also reports and propagates descriptor errors.

2. **Sanitizer target**
   - `make sanitize` builds the terminal and smoke test with AddressSanitizer and UndefinedBehaviorSanitizer.
   - Leak detection is enabled for the smoke run.

3. **Lexer/parser fuzz target**
   - `fuzz.cpp` supplies a bounded libFuzzer harness.
   - Inputs exercise lexing first and parsing when lexical analysis succeeds.
   - A 64 KiB input ceiling prevents the harness from becoming an uncontrolled resource test.
   - Clang/libFuzzer is now the explicit reference toolchain for the fuzz target.

### CI validation added

The M5 workflow now performs:

1. clean build;
2. complete `make test` suite;
3. sanitizer validation;
4. Clang installation;
5. fuzz-target compilation.

This is validation infrastructure, not a claim that the new workflow has already passed. The current GitHub status query has no reported status entries for the latest inspected commit.

### What remains before GREAT

The following remain explicit gates:

- Linux job-control/process-group integration tests;
- Windows-native terminal/PTY implementation and tests where Windows support is targeted;
- descriptor-leak tests across pipelines, substitutions, and redirections;
- signal/trap stress tests;
- command-substitution and pipeline exit-status conformance tests;
- filesystem race/symlink test matrix;
- long-running resource-limit tests;
- repeatable performance benchmarks;
- fuzz corpus retention and sustained fuzz execution in CI.

### Current engineering grade

**BETTER+**

The framework has moved beyond a basic development assessment through seeded hardening and validation infrastructure. It is still not **GREAT** because the remaining gates require demonstrated cross-platform and long-running behavioral evidence, not merely source-level design.

**Target:** GREAT after the validation gates above pass consistently.
