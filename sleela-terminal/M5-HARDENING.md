# SLeeLa Terminal — M5 Hardening and Validation Record

**Scope:** M1–M5 closeout review  
**Status:** Development-grade hardening complete; validation infrastructure in place  
**Engineering classification:** **BETTER+**

## 1. Purpose

This document records the changes made during the M5 hardening review. The goal was not simply to add shell features, but to make the feature boundaries safer, more explicit, and easier to validate.

The review follows the engineering priority:

```text
semantic correctness > security > resource correctness > performance > micro-optimization
```

## 2. Changes implemented

### Multi-segment pathname globbing

- M5 source-text glob rewriting was removed.
- Pathname expansion is performed in the normal expansion layer.
- Multi-component paths are traversed component by component.
- Filesystem matches remain pathname/argv data and are never re-lexed as shell source.
- The regression suite includes filenames containing shell metacharacters.

This closes the most important code/data-boundary concern identified in the M5 review.

### Process substitution

- Both `<(...)` and `>(...)` are supported by the M5 orchestration layer.
- Temporary FIFOs use restrictive `0600` permissions.
- Substitution commands execute with a private environment copy.
- FIFO startup uses a read/write descriptor strategy in the child to avoid the classic producer/consumer open deadlock.

Remaining hardening work includes explicit lifecycle/cleanup tests and broader failure/status conformance testing.

### Quoted assignment handling

- The lexer now distinguishes quotes inside an assignment value from quoting of the assignment name.
- Regression coverage protects values such as `PS3="pick> "` from being misclassified.

### External-command assignment scope

- Prefix assignments for external commands use a scoped environment.
- The parent shell environment is therefore not unintentionally modified by an external command prefix assignment.

Shell-function and special-builtin assignment semantics remain an area for broader POSIX conformance work.

### Redirection error handling

- `dup2()` failures are checked for redirections and heredoc setup.
- Descriptor failures now produce diagnostics and propagate an error instead of being silently ignored.

## 3. Validation infrastructure

The M5 workflow now performs the following sequence on relevant changes:

1. clean build;
2. full smoke/regression suite;
3. AddressSanitizer and UndefinedBehaviorSanitizer validation;
4. Clang installation;
5. libFuzzer target compilation.

The fuzz harness is bounded to 64 KiB input and exercises lexer input first, then parser input when lexing succeeds.

**Important:** the presence of these workflow gates is evidence of repeatable validation infrastructure, not evidence that every future workflow run has passed. GitHub Actions results must be inspected separately.

## 4. Regression baseline

The M5 smoke suite covers:

- `select` menus and `PS3`;
- invalid and valid selection behavior;
- `<(...)` process substitution;
- `>(...)` process substitution;
- signal trap registration and dispatch;
- multi-segment pathname globbing;
- filenames containing shell metacharacters;
- quoted assignment values.

The main test target now includes the M5 integration suite.

## 5. Remaining gates toward GREAT

The current classification remains **BETTER+** because the following evidence is still required:

- Linux process-group and terminal job-control integration tests;
- Windows-native terminal/PTY validation where Windows support is targeted;
- descriptor-leak tests across pipelines, substitutions, and redirections;
- signal-burst and blocking-input trap stress tests;
- complete command-substitution and pipeline exit-status conformance tests;
- symlink, permission, inaccessible-directory, and filesystem-race testing;
- long-running resource-limit tests;
- repeatable performance benchmarks;
- retained fuzz corpus and sustained fuzz execution in CI;
- broader POSIX behavior matrices.

## 6. Engineering conclusion

The M5 implementation has moved from feature addition toward explicit hardening and repeatable validation. The most important architectural improvement is that shell syntax and filesystem data remain separate: filesystem results are produced as data by the expansion layer rather than being converted back into shell source.

The current engineering judgment is:

**SLeeLa Terminal M1–M5: BETTER+.**

**Target:** advance to **GREAT** only after the remaining behavioral, portability, resource, fuzzing, and performance gates are demonstrated by repeatable evidence.

This is an engineering classification, not a formal security certification.
