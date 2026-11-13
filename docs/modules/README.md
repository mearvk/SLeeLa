<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa Modules: Tutorials and Explanations

One writeup per module. The modules are the 44 products registered in
[`bin/sleelactl.d/registry.conf`](../../bin/sleelactl.d/registry.conf) and controlled by
[`sleelactl`](../../bin/SLEELACTL.md), plus the 3 Advantage modules in
[`lib/advantage/modules`](../../lib/advantage/modules/README.md): **47 writeups, 30 tutorials and 17 explanations.**

Every writeup was produced by reading the module's own documents and code and **running** it in this
checkout on 2026-11-13 (`master` at `e048707f`, `impl/build/sleela` built from that commit). Each one
says at the top what was run and what happened.

## Tutorial or explanation

The split follows the [Diataxis](https://diataxis.fr/) distinction and is decided per module:

- **Tutorial** (learning by doing): the module is something you operate or program against, there is a
  path that works end to end in this checkout, and **every step was run successfully** for the writeup.
  The output shown under each step is the real output.
- **Explanation** (understanding): the module is a model, contract, specification or generator whose
  value is understanding, **or** it cannot be operated end to end here (no build target, needs a service
  manager, root, radio hardware or `nmcli`, or it does not build). The explanation says which, with the
  exact command and error.

A tutorial is not a claim that a module is complete: the **State** column and each writeup list what does
not work. An explanation is not a verdict that a module is bad: several are explanations only because they
need a real host (systemd, `nmcli`, radio hardware) that the check could not use.

## How to read the State column

| State | Meaning |
|---|---|
| works | the documented path runs and its results were checked |
| works (self-test) | only the module self-tests exist to run, and they pass |
| partial | the core runs, and at least one documented part fails or cannot run here |
| broken | the module does not build, or its main entry point fails |
| contract only | a data contract with nothing to run |

## The modules

### Daemons

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `server-edition` | [Explanation](explanations/server-edition.md) | partial | `sleelas` transport server builds and runs; `moral/2/serverd` has a shell syntax error (line 34); `server-edition/http` cannot build `httpctl` |
| `server-edition3` | [Explanation](explanations/server-edition3.md) | partial | `serverd --preflight` passes; a full run stops at `PORTCTL: unbound variable`; the shipped example config is rejected |
| `antivirus` | [Explanation](explanations/antivirus.md) | partial | native scanner detects the test sample; `make test` has 1 failing test (closes fd -1); `antivirus/README.md` is a copy of the Makefile |

### Registries

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `codecs` | [Tutorial](tutorials/codecs.md) | works | demo registers 20 codecs, matching `handlers=20`; the demo is bookkeeping, not encoding |
| `compression` | [Tutorial](tutorials/compression.md) | works | demo registers 15 algorithms, matching `handlers=15` |
| `video` | [Tutorial](tutorials/video.md) | works | demo registers 12 codecs, matching `handlers=12`; `lib/video/README.md` is two lines |

### Data sources

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `database` | [Explanation](explanations/database.md) | broken | 16 placeholder classes with no database behind them; `close()` fails at run time (reserved Munction name) |
| `db-connector` | [Tutorial](tutorials/db-connector.md) | works | C library builds, `make test` passes, a 20-line C program creates and reads a table |
| `sleela-sql` | [Tutorial](tutorials/sleela-sql.md) | partial | CLI works in both dialects; `SqlModel.sleela` fails with a syntax error; docs disagree on transactions |
| `ledger` | [Explanation](explanations/ledger.md) | partial | `lib/ledger` holds only a package facade; the working tool is `ledger/` (`ledger.py`, `ledger_tool`) and has no append command |
| `registry-store` | [Tutorial](tutorials/registry-store.md) | partial | `SLRegistry.sleela` runs and the native store builds; a saved registry reloads as `[BROKEN]` |

### Protocols: HTTP family

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `http-1.0` | [Explanation](explanations/http-1.0.md) | broken | does not compile: `http1_download_should_resume` defined twice in `http1_server.c` |
| `http-2.0` | [Tutorial](tutorials/http-2.0.md) | works | self-test and Python session pass; `STATUS.md` still calls finished parts stubs |
| `http-3.0` | [Tutorial](tutorials/http-3.0.md) | works | pipeline demo passes with a fresh baseline; a stale `.http3_intactx_baseline.demo` makes it fail |
| `http-4.0` | [Explanation](explanations/http-4.0.md) | works (self-tests) | 5 self-tests pass; specification and frame library, no demo |
| `http-5.0` | [Explanation](explanations/http-5.0.md) | works (self-test) | self-test passes; no demo |
| `http-6.0` | [Explanation](explanations/http-6.0.md) | partial | self-test passes; `http6_consolidated_pack.hpp` is never compiled |
| `http-7.0` | [Tutorial](tutorials/http-7.0.md) | works | C and C++ demos agree; server self-test and logger test pass |
| `http-8.0` | [Tutorial](tutorials/http-8.0.md) | works | demos and self-test pass; `make test` overwrites the tracked binary `http-8.0/http8_server` |
| `http-9.0` | [Explanation](explanations/http-9.0.md) | partial | metadata API passes its syntax test; `make demo` fails to link (no `main`) |

### Protocols: radio and telephony

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `cdma` | [Tutorial](tutorials/cdma.md) | partial | `cdma-sim` builds and runs; `make -C cdma/build test` fails (`test_link.c` missing) |
| `ofdma` | [Tutorial](tutorials/ofdma.md) | works | builds, tests pass, demo prints the resource-grid metrics |
| `wifi-common` | [Explanation](explanations/wifi-common.md) | partial | policy core and tests work; the real backend needs `nmcli` |
| `wi-fi-4` | [Explanation](explanations/wi-fi-4.md) | partial | 802.11n profile demos (C, C++, Java) agree; manager example needs `nmcli` |
| `wi-fi-5` | [Explanation](explanations/wi-fi-5.md) | partial | 802.11ac profile demos agree; manager example needs `nmcli` |
| `wi-fi-6` | [Explanation](explanations/wi-fi-6.md) | partial | 802.11ax profile demos agree; the band mask has no 6 GHz bit although the docs mention 6E |
| `telephony-skya` | [Tutorial](tutorials/telephony-skya.md) | partial | native engine, CLI and 54 drivers build and run; stopping a running server hangs; 15 of 19 `.sleela` files fail `check` |

### Protocols: routing and transport

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `resolver` | [Tutorial](tutorials/resolver.md) | works | CLI and self-test work as `RESOLUTION.md` describes |
| `route` | [Explanation](explanations/route.md) | contract only | JSON contract and C helper; nothing in the repo consumes `route_data.h` |
| `connector` | [Explanation](explanations/connector.md) | broken | its Makefile cannot compile: 14 `javac` errors with a local servlet jar |
| `preferred-routers` | [Tutorial](tutorials/preferred-routers.md) | partial | C selector works; the C++ file lacks a function the C file has; the data files are never read |

### Tools

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `compiler` | [Tutorial](tutorials/compiler.md) | works | `sleelac` checks, runs and emits C/C++ that compiles with `-Werror` (its own small subset) |
| `decompiler` | [Tutorial](tutorials/decompiler.md) | partial | `inspect`/`disassemble`/`cfg`/`decompile` run; symbols are not read from `.symtab`, the C output is not valid C |
| `debugger` | [Explanation](explanations/debugger.md) | partial | only `--self-test` and `--report`; `make -C debugger test` fails |
| `native-drive` | [Tutorial](tutorials/native-drive.md) | works | C build, run, pipe and `@native` plugin paths work; the systemd part is not operable here |
| `java-convert` | [Tutorial](tutorials/java-convert.md) | works | converted programs match `java`; suite 32 of 32; 420 of 884 JDK files convert |

### Native subjects

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `math` | [Tutorial](tutorials/math.md) | works | `run math` matches its fixture; `math.pow` is wrong for bases of zero or below (`pow(-2,3)` gives 1) |
| `physics` | [Tutorial](tutorials/physics.md) | works | demo matches its fixture; `ideal_gas_pressure(n,R,T)` has no volume argument |
| `chemistry` | [Tutorial](tutorials/chemistry.md) | works | demo matches its fixture; the linked C++ chemistry model is unreachable from SLeeLa and disagrees with the SLeeLa one |
| `astrophysics` | [Tutorial](tutorials/astrophysics.md) | works | demo matches its fixture; 6 of 11 C functions are bound |
| `sociology` | [Tutorial](tutorials/sociology.md) | works | demo matches its fixture; 4 of 11 C functions are bound |
| `economics` | [Tutorial](tutorials/economics.md) | works | 12 checks pass; a zero rate gives `-nan` in the annuity functions |
| `inference` | [Tutorial](tutorials/inference.md) | works | 15 checks pass; regression and correlation match python3 |
| `financial` | [Tutorial](tutorials/financial.md) | works | 15 checks pass; complex roots and singular systems silently return 0 |

### Advantage modules (not in the sleelactl registry)

| Module | Writeup | State | What the check found |
|---|---|---|---|
| `advantage-modules` | [Tutorial](tutorials/advantage-modules.md) | works | host demo 46 of 46; a user module plugs in through the bridge |
| `high-speed-trader` | [Tutorial](tutorials/high-speed-trader.md) | works | demo 31 of 31; a model on a synthetic stream, not trading advice |
| `moral-evaluator` | [Tutorial](tutorials/moral-evaluator.md) | works | demo 39 of 39; fixed-rule verdicts on made-up inputs, not moral judgements |

## Problems that cut across modules

Found while writing the module pages; each page has the details and the evidence. None was fixed.

1. **`sleelactl negotiate` and `grade` (all 20 protocol modules)** point to a "negotiate/grade section" of the
   module README that no README has, and exit 3. `simulate` exits 3 unless the registry sets `sim=` (only `cdma`).
2. **`sleelactl validate`** exits 3 ("nothing to run") for `wi-fi-4`, `wi-fi-5`, `wi-fi-6`, `telephony-skya`,
   `route` and `cdma`, which have no `validate`/`syntax`/`test` target; for `http-1.0` it fails because the module
   does not compile.
3. **Option order in `sleelactl`**: options must come before the verb. `bin/sleelactl start server-edition --dry-run`
   passes `--dry-run` to the action and runs the real `systemctl` (harmless here, it failed). Argument forwarding
   also trips: `sleelactl run compiler -- --run f` fails with `unknown option: --`, and `sleelactl run native-drive run ...`
   doubles the `run`.
4. **Build outputs not ignored by git**: `debugger/{sleela-debugger,debug-engine-test,debug-conformance-test}`,
   `server-edition/{sleelas,server_protocol_test}`, `wifi-common/wifi-manager-test`,
   `telephony-skya/native/{skya,sleela-skya}` and `cdma/tools/cdma-sim` show as untracked after a normal build, and
   `http-8.0/http8_server` is a tracked binary that `make test` overwrites (so `make clean` deletes a tracked file).
5. **Library code that does not run**: `lib/database` (`close()` collides with a reserved built-in),
   `lib/sleela-sql/SqlModel.sleela` (syntax error), 15 of 19 `telephony-skya` `.sleela` files fail `sleela check`.
6. **Documents that contradict their code**: `antivirus/README.md` is a copy of its Makefile; `http-2.0/STATUS.md`
   calls finished parts stubs; `decompiler/README.md` says ELF symbols are parsed; `lib/registry/README.md` counts five
   seeded keys (there are six); `server-edition` READMEs say `sleelas` reads `SLEELA_SERVER_LEVEL` (it does not).
7. **Silent numeric errors in subjects**: `math.pow` returns 1 for `pow(0,2)` and `pow(-2,3)` (it is built as
   `exp(y*log(x))`), and `sqrt`/`log` of invalid input return 0; `economics`, `financial` and `inference` inherit this.

## Keeping this page true

- A writeup records one run on one date. When a module changes, re-run its steps and update the writeup's
  **Verified** row; if a tutorial step stops working, fix the module or turn the page into an explanation.
- Never add a tutorial step that was not run. Never describe a project design as a published standard: each
  protocol page separates what the module's files say about IEEE/IETF standards from the project's own designs.
- The Java converter has its own measured status page: [`JAVA.CONVERSION.STATUS.md`](../../JAVA.CONVERSION.STATUS.md).
