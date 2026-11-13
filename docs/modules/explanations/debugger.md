<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# debugger: Explanation

| | |
|---|---|
| Nature | tool (`bin/sleelactl help debugger`) |
| Registry summary | SLeeLa debugger |
| Lives in | `debugger/` (binary `debugger/sleela-debugger`, built in place) |
| Writeup type | Explanation, because the shipped CLI has no mode that launches, steps or inspects a program; it only runs a self-test and writes a report, so no end-to-end debugging session exists to teach. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl build debugger` OK; `--self-test` exit 0; `--report` OK; `make test` FAILS (missing files); the test script run from `debugger/tests/` passes five suites and aborts in the sixth. |

## What it is

The debugger is a C++17 (plus C API) library of debugging concepts: sessions, breakpoints,
watchpoints, events, stack frames, actions, diagnostics, conformance checks and a line/voice command
layer. `debugger/README.md` calls it a "development-time diagnostic layer" that "records and
analyzes events" and does not execute arbitrary payloads. It is backend-neutral: a backend
interface (`debugger_backend.hpp`) has per-OS implementations. `debugger/NATIVE_EXECUTION.md` lists
real native execution (breakpoints, registers, DWARF, LLDB, Windows Debug API) as a plan, and
`debugger/TUTORIAL.md` warns that the model capabilities are not a finished native debugger.

## How it works

`debugger/DEBUGGER.ARCHITECTURE.md` and `debugger/TUTORIAL.md` describe the flow as target, native
backend, `DebugEngine`, `DebugSession` and event bus, actions, diagnostics and evidence, tests.

- `DebugSession` (`debugger.hpp`, `debugger.cpp`) stores breakpoints, watchpoints and `DebugEvent`s and can write a text report.
- `DebugEngine` (`debug_engine.hpp`, `debug_engine.cpp`) sits above a backend and records evidence.
- `debugger_backend.hpp` is the interface (launch, attach, resume, step, read/write memory, registers). `debugger_backend_linux.cpp` implements it with `ptrace` (`PTRACE_TRACEME`, `PTRACE_CONT`, `PTRACE_SINGLESTEP`, `PEEKDATA`/`POKEDATA`, `GETREGS`; register snapshot is only rip/rsp/rbp). The macOS and Windows files are separate. Nothing in the CLI calls these.
- `debug_conformance.*` and `debug_diagnostics.*` check the model's contracts. `debugger/CONFORMANCE.md` and the Makefile comment say conformance does not certify native Linux, macOS or Windows execution.
- `debugger_c.*`, `debug_engine_c.*` and `debugger_line.*` expose the same ideas through a C API and a command-line/voice grammar (`COMMAND_LANGUAGE.md`, `LINE_AND_VOICE_CONTROL.md`).

The CLI (`debugger/main.cpp`) is 18 lines of logic. It accepts exactly two forms:

```
sleela-debugger --self-test         # adds a breakpoint and watchpoint to a DebugSession, emits one event, checks the report text
sleela-debugger --report FILE       # writes the report of a session containing one "empty diagnostic session" event
```

Anything else prints `Usage: sleela-debugger --self-test | --report FILE` and exits 2.

## What was run

```sh
bin/sleelactl build debugger
```

```
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp debugger.cpp ... -o sleela-debugger
g++ ... debug_engine_test.cpp debug_engine.cpp -o debug-engine-test
g++ ... debug_diagnostics_test.cpp debug_diagnostics.cpp -o debug-diagnostics-test
g++ ... debug_conformance_test.cpp debug_conformance.cpp -o debug-conformance-test
  ✓ debugger built
```

```sh
bin/sleelactl run debugger --self-test            # exit 0, no output
bin/sleelactl run debugger --report /tmp/report.txt
cat /tmp/report.txt
```

```
SLeeLa Debugger Diagnostic Report
events=1
[info] info: empty diagnostic session
SLeeLa Debug Evidence
records=0
```

`./debug-engine-test` and `./debug-conformance-test` (and `make conformance`) exit 0.

## Where it lives

| File | Role |
|---|---|
| `debugger/main.cpp` | The CLI: `--self-test`, `--report FILE` |
| `debugger/debugger.{hpp,cpp}` | `DebugSession`, breakpoints, watchpoints, events, report |
| `debugger/debug_engine.{hpp,cpp}` | `DebugEngine` and evidence records |
| `debugger/debugger_backend*.{hpp,cpp}` | Backend interface; Linux (`ptrace`), macOS, Windows implementations |
| `debugger/debugger_actions.*`, `debugger_line.*`, `debugger_c.*`, `debug_engine_c.*` | Actions, line/voice commands, C APIs |
| `debugger/debug_conformance.*`, `debug_diagnostics.*` | Contract checks |
| `debugger/tests/` | Six test sources and `run-tests.sh` |
| `debugger/Makefile` | Targets `all test conformance clean` |

## Status and limits

- **No operable debugging path.** The only CLI modes are the two above; there is no option to name a target program. `debugger/example-report.txt`, used as the argument in `debugger/README.md`, is only an output path and does not exist.
- **`make test` fails.** `make -C debugger test` ends with `cc1plus: fatal error: debugger_core_test.cpp: No such file or directory` and `../debugger.cpp: No such file or directory`, then `make: *** [Makefile:27: test] Error 1`. Cause: `debugger/tests/run-tests.sh` compiles `debugger_core_test.cpp ../debugger.cpp -I..` and so must run with `debugger/tests/` as the working directory, but the Makefile runs `./tests/run-tests.sh` from `debugger/`. As a result `./debug-engine-test`, `./debug-diagnostics-test`, `./debug-conformance-test` and `--self-test` are never reached by `make test`.
- **A test fails even from the right directory.** Running `./run-tests.sh` inside `debugger/tests/`: the core, line, backend, actions and C-API suites pass; `debugger-line-c-test` aborts with `debugger_line_c_test.c:29: main: Assertion 'sleela_debugger_voice_command(s, "not a debugger command", ...) == SLEELA_DEBUGGER_INVALID_ARGUMENT' failed` (exit 134). The suites after it in the script do not run.
- **Side effect:** the script's cleanup trap also deletes `debug-diagnostics-test` from `debugger/`, so after `make test` that binary is gone.
- **Native backends not exercised.** The Linux `ptrace` backend was not driven by anything here. `NATIVE_EXECUTION.md` and `TUTORIAL.md` describe native execution as still to be verified; this page makes no claim that it works.
- `debugger/README.md` describes a "Corrections" note about build-blocking defects fixed on 2026-10-03; the build indeed succeeds with `-Wall -Wextra -pedantic`.

## Related

- [`debugger/README.md`](../../../debugger/README.md), [`debugger/TUTORIAL.md`](../../../debugger/TUTORIAL.md) (a ten-step conceptual tutorial; its step 2 `make` / `make test` commands are the ones that fail above)
- [`debugger/DEBUGGER.ARCHITECTURE.md`](../../../debugger/DEBUGGER.ARCHITECTURE.md), [`debugger/CONFORMANCE.md`](../../../debugger/CONFORMANCE.md), [`debugger/NATIVE_EXECUTION.md`](../../../debugger/NATIVE_EXECUTION.md)
- [`decompiler`](../tutorials/decompiler.md) for binary analysis that does work end to end

## Sources

Read: `bin/sleelactl.d/registry.conf` (line 71), `debugger/README.md`, `debugger/TUTORIAL.md` (steps 1-3), `debugger/main.cpp`,
`debugger/NATIVE_EXECUTION.md`, `debugger/Makefile`, `debugger/tests/run-tests.sh`, `debugger/tests/debugger_line_c_test.c` (lines 20-31),
`debugger/debugger_backend_linux.cpp` (grep for `ptrace`). Ran: `bin/sleelactl help|info|inspect|build|run debugger`,
`make -C debugger test`, `make -C debugger conformance`, `debugger/tests/run-tests.sh` from `debugger/tests/`,
`debug-engine-test`, `debug-conformance-test`. Build outputs left in `debugger/`: `sleela-debugger`, `debug-engine-test`, `debug-conformance-test` (untracked, not gitignored).
