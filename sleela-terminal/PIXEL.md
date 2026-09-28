# PIXEL.md — SleelaTerminal™ Project Detail & Presentation Guide

## Purpose

`PIXEL.md` is the presentation-level record for SleelaTerminal™. It answers two practical questions: **What's Made** and **What's Included**. It also explains how to write this kind of project material accurately.

# 1. What's Made

## 1.1 SleelaTerminal™ Shell

SleelaTerminal™ includes the original C++17 command shell `slsh`. It is a clean-room implementation guided by public shell specifications and common shell behavior, not a fork of another shell.

The shell is organized as six layers:
- L1 — Core model and arithmetic
- L2 — Lexer
- L3 — Parser
- L4 — Expansion
- L5 — Executor
- L6 — CLI / M5 orchestration

Implemented shell capabilities include variables and assignments, quoting, parameter and arithmetic expansion, pathname and brace expansion, tilde expansion, command substitution, pipelines, redirections, here-documents, functions, `if`, `while`, `until`, `for`, `case`, background jobs, `jobs`, `wait`, `fg`, `bg`, `break`, `continue`, `test`, `read`, `getopts`, `select`, process substitution, signal traps, builtins, and external commands.

## 1.2 M1–M5 Development

**M1** establishes the core model, arithmetic engine, lexer, parser, executor, pipelines, redirections, and initial builtins.

**M2** adds `for`, `case`, functions, positional parameters, command substitution, pathname globbing, and brace groups.

**M3** adds parameter operators, brace expansion and ranges, tilde expansion, here-documents, and functions in pipelines.

**M4** adds `until`, background jobs, job control commands, loop control, pipeline negation, `test`, `read`, and `getopts`.

**M5** adds `select`, multi-segment pathname globbing, process substitution, signal traps, scoped external-command environments, checked redirection failures, filesystem-data/code separation, FIFO startup hardening, sanitizer infrastructure, and fuzz-target infrastructure.

## 1.3 Graphical Terminal

The project includes a GTK 4 / VTE graphical terminal named `SleelaTerminal`.

It provides a PTY-backed `slsh` session, terminal scrollback, configurable font and foreground color, keyboard copy/paste, text selection, select-all and clear-selection, a right-click terminal menu, local settings, child-process lifecycle handling, SIGHUP handling, status/version presentation, and an integrated software-center surface.

## 1.4 Local Configuration

The GUI stores human-readable local configuration under the user's normal configuration directory. Current settings include font, foreground color, language orientation, and declarative orientation/source switches.

These settings are documented as local configuration. A configuration flag does not itself establish identity, authority, truth, or legal status.

## 1.5 Software Integration

The GUI provides an integration surface for the project's Java/software tooling. It can scan the configured release source, distinguish final and pre-release states where available, prepare SecureJDK 28, CMD, Asysma, or all supported products, and display installer output.

## 1.6 Validation

The project contains normal builds, smoke tests, M5 integration tests, sanitizer builds, and a Clang/libFuzzer target.

Validation infrastructure must not be confused with a passing validation result. A test target being present proves that a test can be run; it does not prove that the test has passed in the current environment.

# 2. What's Included

## 2.1 Source

The `sleela-terminal/` tree contains the shell core, arithmetic engine, lexer, parser, expansion layer, executor, M5 orchestration, command-line entry point, GUI entry point, headers, tests, and fuzzing harness.

## 2.2 Build System

The Makefile provides normal build, smoke, M5, GUI, sanitizer, fuzz, install, uninstall, and clean targets. The GUI build detects GTK 4 and VTE development packages through `pkg-config`.

## 2.3 Executables

A normal build produces `build/slsh` and `build/smoke`; when GTK 4/VTE development dependencies are available it also produces `build/SleelaTerminal`.

Artifact names are build outputs, not certifications or release claims.

## 2.4 Tests and Documentation

The component includes smoke/regression coverage, M5 hardening documentation, architecture documentation, authorship/provenance documentation, build instructions, and version information.

## 2.5 Desktop and Presentation Assets

The component includes desktop integration and GUI presentation assets, including the `SleelaTerminal.desktop` launcher and graphical controls used by the terminal.

# 3. How to Write This Kind of Stuff

## 3.1 Separate the Questions

Use **What's Made** for implemented functionality. Use **What's Included** for files, tools, assets, documentation, and deliverables. Use a separate section for architecture, validation, or future work.

## 3.2 Describe Concrete Objects

Prefer names and observable behavior: `slsh`, `SleelaTerminal`, GTK 4, VTE, PTY, lexer, parser, executor, Makefile, smoke tests, fuzz target, and configuration file.

Avoid vague claims such as 'complete security', 'perfect shell', or 'production ready' unless the project defines measurable criteria and supplies evidence.

## 3.3 Keep Implementation and Evidence Separate

Say that a feature is **implemented** when the source provides it. Say that validation **exists** when the repository contains the test infrastructure. Say that validation **passed** only when an actual run provides that evidence.

## 3.4 Keep Future Work Separate

Unfinished work belongs under **Remaining Work** or **Next Gates**, never under What's Made.

For SleelaTerminal, examples include native Windows PTY validation, Linux job-control integration testing, descriptor-leak tests, signal stress, filesystem edge cases, resource-limit testing, performance benchmarks, sustained fuzzing, and broader POSIX behavior matrices.

## 3.5 Use Version Language Carefully

Use the recorded component version for release identity and milestone names for development stages. Do not increment a version merely because documentation changed.

## 3.6 Write UI Details as Observable Behavior

Describe what the user can see and operate separately from how it is implemented. For example: 'the terminal provides copy/paste and settings' is user-visible behavior; 'the terminal uses GTK 4, VTE, and a PTY' is implementation detail.

## 3.7 Keep Claims Source-Respecting

Before writing a claim, ask: Does the source implement it? Is it present on the documented branch? Is the version correct? Is it tested? Has the test actually been executed? Is the statement implementation, infrastructure, or roadmap?

# 4. Recommended PIXEL Pattern

Future component PIXEL documents can use:

1. Purpose
2. What's Made
3. What's Included
4. How to Write This Kind of Stuff
5. Validation
6. Remaining Work
7. Maintenance Rule

This creates a repeatable presentation format without forcing every component to use identical wording.

# 5. Current SleelaTerminal™ Snapshot

| Property | Current record |
|---|---|
| Component | SleelaTerminal™ |
| Version | `1.0.0` |
| Shell executable | `slsh` |
| Graphical executable | `SleelaTerminal` |
| Primary language | C++17 |
| GUI | GTK 4 |
| Terminal backend | VTE |
| Development hardening | M1–M5 |
| Documented engineering classification | BETTER+ |
| Build system | Make |
| Validation infrastructure | smoke, M5, sanitizers, fuzz target |

The snapshot is a documentation summary, not a substitute for source code, tests, or formal specifications.

# 6. Maintenance Rule

When SleelaTerminal materially changes, update the implementation, applicable tests, component version when warranted, `VERSION.md` where required, development documentation, and this `PIXEL.md` when the change alters What's Made or What's Included.

If a feature is planned but not implemented, say so. If it is implemented but not validated, say so. If validation infrastructure exists but has not been executed, document the infrastructure without claiming a passing result.

That distinction keeps PIXEL.md useful as both a project presentation document and an engineering accountability record.