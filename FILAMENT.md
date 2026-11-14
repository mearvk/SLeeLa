# FILAMENT — SLeeLa Build Assurance Register

**Casualty address to the United States for:**  
**ICC and DRR5 — Casualty Loss Leaders**

> The terms above are retained as the requested classification labels. They are not build results, failure diagnoses, or statements about the technical status of either module.

**Repository:** `mearvk/SLeeLa`  
**Branch recorded:** `master`  
**Status date:** 2026-11-14  
**Source/build status baseline:** `c05c4f020fa880badb2f0b36c441307b729f7ad0` (Makefile recipe audit)  
**VM inventory update commit:** `b615cc7098300121f62deba69bfa7ff960c11e76`

## Purpose and status rules

This register records the build entry points declared by the root `Makefile`, separately invoked module builds, and dedicated CI evidence visible at the status date. A Makefile target being present is not proof that it has passed on every supported operating system.

- **PASS (dedicated CI):** the latest inspected dedicated workflow completed successfully. This applies to that workflow's scope and commit only.
- **IN PROGRESS:** the latest inspected workflow is still running; no pass is claimed.
- **PRIOR FAILURE / RETEST PENDING:** the latest completed relevant run failed, and a newer retest is running or required.
- **WIRED — NOT QUALIFIED:** a build target exists, but no fresh passing end-to-end result for that target was established in this audit.
- **OPTIONAL — NOT IN DEFAULT ALL:** a separately invoked build target is intentionally outside the root `make all` dependency chain.

## Root build and install pipeline

The authoritative aggregate entry point is `make all`. The root Makefile currently dispatches the following targets:

| Module / build stage | Build entry point | Current status | Evidence / qualification |
|---|---|---|---|
| Native runtime and core | `make core` / `make -C impl all` | **IN PROGRESS — RETEST PENDING** | Native C/C++ Portability run **43629021139** is running on commit `07f9a410`. The preceding completed run **43622491714** failed on the Windows VM regression timeout; Linux and macOS jobs passed. The timeout was raised to 40 minutes; the new result is not yet known. |
| Java 28 bridge | `make java28` | **WIRED — NOT QUALIFIED** | Included in `make all`; no fresh complete root-build result establishing this target on all three OSes was found. |
| XLT compiler | `make xlt` | **WIRED — NOT QUALIFIED** | Included in `make all`; root dispatcher invokes `impl/xlt`. Platform-wide completion remains to be demonstrated by a fresh full build. |
| Regex | `make regex` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Compiler | `make compiler` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Decompiler | `make decompiler` | **OBJECT BUILD — TEST GAP** | C/C++ objects compile; sanity/tutorial recipes only print descriptions. No executable decompiler self-test is wired into this target. |
| VM | `make vm` | **PASS — CONFORMANCE WORKFLOW** | Complete SLeeLa VM and Language Conformance **43619118411** and VM Arithmetic Table Completion **43619118403** passed on commit `d5034d8f`. This is dedicated conformance evidence, not a fresh aggregate build. |
| Scripting | `make scripting` | **PRESENCE CHECK ONLY — NOT COMPILED** | Checks documentation/specification/config/example/test files exist; does not compile or execute the scripting language. |
| JetBrains integration | `make jetbrains` | **HELP/DOWNLOAD TARGET — NO IDE BUILD** | `all` invokes `help`; download targets acquire upstream IntelliJ source. No SLeeLa IDE artifact is compiled by this target. |
| Installer checks | `make install-check` | **PREFLIGHT ONLY — INSTALL TEST PENDING** | Validates installer inputs; staged install, launch, upgrade, and uninstall remain separate qualification steps. |
| Tutorial validation | `make tutorial-check` | **WIRED — NOT QUALIFIED** | Included in `make all`; no fresh aggregate result identified. |
| Repository tests | `make tests` | **COMPILE-ONLY AGGREGATE — EXECUTION NOT IMPLIED** | The tests Makefile default all/check path performs an OpenSSL probe and compiles native smoke/AI/recognition executables; it does not execute the complete set. Some individual AI/recognition targets have explicit run recipes, but root make tests does not call them. |
| Configuration | `make config` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Route | `make route` | **JSON VALIDATION ONLY** | Parses `route/ROUTE.DATA.json`; it is not a native module compile. |
| CommonRails | `make common-rails` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Codecs | `make codecs` | **STATIC LIBRARY BUILD — TESTS SEPARATE** | Compiles and archives `libsleela_codecs.a`. `make codecs-test` builds and executes the registry, manager, and FLAC-native test binaries. Root `make all` invokes only `codecs`. |
| Churn | `make churn` | **JAVA COMPILE + DATA CHECK — EXECUTION TEST SEPARATE** | Compiles five Java constraint sets and validates JSON/XML/CSV/TXT data. `make churn-test` additionally runs all five Java reports. Root `make all` invokes only `churn`. |
| Antivirus / Sleelavirin | `make antivirus` | **LIBRARY / TEST-BINARY BUILD — TEST EXECUTION SEPARATE** | Builds C/C++ libraries, native scanner, and test executables. `make antivirus-test` executes C/C++ scanner tests, native CLI tests, Python unit tests, and signature-definition checks. Root `make all` invokes only `antivirus`. |
| Registry 3.0 | `make registry` | **PASS — DEDICATED CI** | Cross-Platform Verification **43622485269** completed successfully on Linux, macOS, and Windows. |
| Premium SLVM runners | `make premium-vms` | **11 RUNNER BUILDS — SELF-CHECK SEPARATE** | Builds all eleven runnable SLVM generation executables against the Core archive. Each generation's `make test` runs its `--selfcheck`; the root `premium-vms` recipe invokes `make all`, not each generation's `test`. Dedicated conformance CI has executed the runners on its recorded commit. |
| Ballum native display library | `make ballum` | **PARTIAL / HOST-DEPENDENT** | Root target attempts the native build when host display dependencies are present and otherwise falls back to portable tests. The openSUSE package list now includes `libXext-devel`; the latest Native C/C++ Portability run is still in progress. No openSUSE package-install/build run is claimed. |

## Complete VM inventory and assurance scope

The VM inventory is larger than the single `make vm` package target. The repository contains three distinct selectable/implementation tracks. They must be tracked separately; the Standard registry is not the Premium registry, and neither registry should be mistaken for the native `/impl` execution core.

### VM families

| VM family | Inventory | Repository / source location | Build and test entry points | Current assurance status |
|---|---|---|---|---|
| **Native implementation VM (Core)** | One authoritative operational VM | `/impl`, principally `impl/core/sleela_core.c` and its runtime/platform modules | `make core`; `make -C impl all`; `make -C impl test`; `make -C impl test-platform` | **IN PROGRESS / RETEST PENDING.** Native C/C++ Portability run 43629021139 was running at the last recorded check. Prior Linux/macOS jobs passed; Windows VM regression previously timed out. The 40-minute retest must finish before cross-platform PASS is claimed. |
| **Standard selectable VM versions** | **10 registered versions: 001–010** (`SLVmVersion001` … `SLVmVersion010`) | `virtual-machine/vm-version/`, catalogued by `SLVmRegistry.sleela` | Registry/manifest validation; native VM selection and boot path; include in the root VM and integration test matrix | **REGISTERED — BOOT/BUILD QUALIFICATION REQUIRED.** The registry declares load, list, select, and boot for all ten. Registration alone is not evidence that each selected version builds and boots successfully. |
| **Premium SLVM generations** | **11 generations: SLVM/1–SLVM/11** (`SLVmPremiumVersion001` … `SLVmPremiumVersion011`) | `sleela-virtual-machine/`, catalogued by `SLVmPremiumRegistry.sleela`; generation source under `sleela-virtual-machine/001` … `011` | `make premium-vms`; premium manifest/admission checks; per-generation build, boot, and execution tests | **WIRED — CURRENT FULL BUILD RETEST REQUIRED.** The target is included in root `make all`. Earlier admission checks passed for all eleven, but that historical result does not establish a fresh build-and-boot pass on the current commit or all three OSes. |

### Standard VM version register (10)

| Version | Manifest / registry identity | Required assurance |
|---|---|---|
| Standard 001 | `SLVmVersion001` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 002 | `SLVmVersion002` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 003 | `SLVmVersion003` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 004 | `SLVmVersion004` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 005 | `SLVmVersion005` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 006 | `SLVmVersion006` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 007 | `SLVmVersion007` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 008 | `SLVmVersion008` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 009 | `SLVmVersion009` | Manifest load, configure, select, build, boot, and guest execution |
| Standard 010 | `SLVmVersion010` | Manifest load, configure, select, build, boot, and guest execution |

The Standard registry source explicitly loads and configures ten manifests, exposes them through its list/select methods, and returns a configured `SLVmRuntime` from `boot(number)`. The assurance gate must exercise those actual paths rather than only compile or inspect the registry.

### Premium VM generation register (11)

| Generation | Formal generation name | Repository directory | Required assurance |
|---|---|---|---|
| Premium SLVM/1 | Foundation | `sleela-virtual-machine/001` | Manifest, generation build, boot, guest execution |
| Premium SLVM/2 | Operator | `sleela-virtual-machine/002` | Manifest, generation build, boot, guest execution |
| Premium SLVM/3 | Specialist | `sleela-virtual-machine/003` | Manifest, generation build, boot, guest execution |
| Premium SLVM/4 | Supervisor | `sleela-virtual-machine/004` | Manifest, generation build, boot, guest execution |
| Premium SLVM/5 | Manager | `sleela-virtual-machine/005` | Manifest, generation build, boot, guest execution |
| Premium SLVM/6 | Director | `sleela-virtual-machine/006` | Manifest, generation build, boot, guest execution |
| Premium SLVM/7 | Administrator | `sleela-virtual-machine/007` | Manifest, generation build, boot, guest execution |
| Premium SLVM/8 | Executive | `sleela-virtual-machine/008` | Manifest, generation build, boot, guest execution |
| Premium SLVM/9 | Authority | `sleela-virtual-machine/009` | Manifest, generation build, boot, guest execution |
| Premium SLVM/10 | Principal | `sleela-virtual-machine/010` | Manifest, generation build, boot, guest execution |
| Premium SLVM/11 | Sovereign | `sleela-virtual-machine/011` | Manifest, generation build, boot, guest execution |

### VM-supporting modules and build concerns

These components are part of VM assurance even when they are not separate VM generations. They must be included in dependency, compile, integration, and regression checks as applicable.

| Supporting module / subsystem | Repository location / known entry | What must be verified |
|---|---|---|
| Native Core runtime and C ABI | `impl/core/`, `impl/runtime/`, `impl/Makefile` | Core dispatch, artifact loading, runtime services, C ABI, clean build, and platform abstraction |
| VM package / management layer | `lib/vm/`; root `make vm` → `make -C lib/vm all` | C and C++ manager/linking/challenge/report/dynamic-memory-guard objects; package checks and tutorial validation |
| VM Creator and generation catalog | `lib/vm/creator/` | Core and generation source resolution, build-plan construction, compiler invocation, artifact verification, packaging, boot and guest execution |
| Standard VM registry and manifests | `virtual-machine/vm-version/SLVmRegistry.sleela`; `SLVmVersion001..010` | All ten manifests load/configure/list/select/boot; reject out-of-range selections; verify returned runtime |
| Premium VM registry and manifests | `sleela-virtual-machine/SLVmPremiumRegistry.sleela`; `SLVmPremiumVersion001..011` | All eleven manifests load/configure/list/select/boot; reject out-of-range selections; verify returned runtime and native binary |
| VM runtime services | `lib/vm/runtime/`, `lib/vm/RUNTIME-SERVICES.md` | Garbage collection, memory management, security supervisor, resource ownership, lifecycle, and failure behavior |
| Dynamic Memory Guard | `lib/vm/src/sleela_vm_dynamic_memory_guard.*` and related runtime sources | HARD / SLOW_CAREFUL / AGGRESSIVE policies, hard limits, allocation failure behavior, and platform consistency |
| VM garbage collector | `lib/vm/runtime/garbage_collector.c`, `lib/vm/src/sleela_vm_gc.cpp` | Generational/incremental tracing, barriers, remembered sets, safepoints, and behavioral reclamation tests |
| Instruction set and opcode map | `lib/vm/InstructionSet.sleela`, `lib/vm/OPCODE-MAP.md` | Stable opcode identity, unsupported-opcode rejection, and generation-wide ISA compatibility |
| System-call bridge and OS adapters | `lib/vm/SleelaVMSystemCallBridge.sleela`, `impl/core/sleela_os.c`, platform adapters | Shared system-call behavior across generations; bounded process/resource handles; Linux, macOS, and Windows behavior |
| VM broker, capabilities, and security | `sleela-virtual-machine/001/` and related VM security sources | Capability enforcement, broker boundary, memory security, OS boundary, and fail-closed behavior |
| VM arithmetic and performance suites | VM arithmetic table/conformance workflows and `impl/tests/` | Arithmetic correctness, regression coverage, bounded resource behavior, and performance regressions |
| VM conformance and source-to-VM path | `sleela-virtual-machine/*/docs/SOURCE-TO-VM.md`; `markdown/VM.SUMMARY.md` | Compiler → Core artifact → selected VM → verified execution; generation compatibility and artifact integrity |
| VM tutorials and examples | `lib/vm/tutorials/`, `lib/vm/creator/examples/`, generation examples | Examples compile and run against the intended VM family; documentation claims match executable behavior |

### VM assurance rules

1. Treat the native `/impl` VM, the ten Standard selectable versions, and the eleven Premium SLVM generations as separate inventory lines.
2. For every registered Standard or Premium manifest, test **load → configure → list → select → build → boot → execute**. A manifest compiling or a registry reporting its count is not a complete VM pass.
3. Run VM tests from a clean build directory and verify that no VM silently consumes stale objects or artifacts from another generation.
4. Test shared ISA/opcode, system-call, runtime-service, memory, security, and platform contracts across the families. Preserve fail-closed behavior for unsupported opcodes and unauthorized capabilities.
5. Record per-generation and per-platform results. A pass for the native Core, one representative generation, a registry count, or an admission check must not be generalized into a pass for all 21 entries.
6. Keep the trusted SHA-256 verification gate active throughout compilation, packaging, and execution.

**Inventory count:** 1 native operational Core + 10 Standard registered versions + 11 Premium generations = **22 tracked VM entries**. The 11 Premium generations are the numbered architecture family described in `markdown/VM.SUMMARY.md`; the Standard registry is a distinct selectable catalogue. The count is an inventory count, not a claim that all 22 are independently implemented runnable interpreters or currently passing builds.

---

## Explicit and optional module builds

These targets are available but are not part of the default root `make all` chain unless stated otherwise.

| Module / build stage | Build entry point | Current status | Evidence / qualification |
|---|---|---|---|
| Native connectors | `make connector` | **OPTIONAL — NOT IN DEFAULT ALL** | Explicit target; requires external client-library dependencies. |
| Connector tests and dependency probe | `make connector-test`, `make connector-check-deps` | **OPTIONAL — NOT IN DEFAULT ALL** | Explicit test and prerequisite targets. |
| SLeeLa-facing Ballum package | `make lib-ballum` | **OPTIONAL — NOT IN DEFAULT ALL** | Separate `lib/ballum` integration target; not the same as the native `ballum/` backend. |
| Ballum portable tests | `make ballum-test` | **TARGET AVAILABLE** | Tests avoid display-library dependencies; a fresh result is not recorded in this register. |
| Ballum dependency check / install | `make ballum-check-deps`, `make ballum-deps` | **REPAIRED — HOST CHECK PENDING** | Linux X11/XRandR/Xext checks align with the default MIT-SHM configuration. openSUSE now provisions `libX11-devel libXrandr-devel libXext-devel`. |
| Ballum install / uninstall / clean | `make ballum-install`, `make ballum-uninstall`, `make ballum-clean` | **TARGET AVAILABLE** | Separate native library lifecycle targets; no fresh cross-platform install test is claimed. |
| API documentation | `make api-docs` | **PASS — DEDICATED CI** | API documentation build **43581713065** completed successfully on commit `89000c1d`; older than the current build fixes. |
| AI library integration | Workflow: `ai-library-integration.yml` | **PASS — DEDICATED CI** | Run **43619118388** completed successfully on commit `d5034d8f`. |
| Advantage Library | Workflow: `advantage-verification.yml` | **PASS — DEDICATED CI** | Run **43622491621** completed successfully; six Advantage demos passed and an artifact was generated. |
| Native Drive | Workflow: `native-drive.yml` | **PASS — DEDICATED CI** | Run **43622309880** completed successfully with three platform jobs. |
| GUI documentation | Workflow: `guidoc-ci.yml` | **PASS — DEDICATED CI** | Run **43622309907** completed successfully. |
| Heterogeneous value regression | Workflow: `heterogeneous-value-regression.yml` | **PASS — DEDICATED CI** | Run **43622309894** completed successfully. |
| Comparison standards conformance | Workflow: `comparison-conformance.yml` | **PASS — DEDICATED CI** | Run **43619118402** completed successfully. |
| Character Recognition C11 | Workflow: `character-recognition.yml` | **PASS — DEDICATED CI** | Run **43619118440** completed successfully. |
| Java conversion | Workflow: `java-convert.yml` | **PRIOR FAILURE — REPAIR / RETEST REQUIRED** | Latest inspected run **43613197880** failed on commit `e2e96b5c`. This workflow is distinct from proving the Java 28 root target. |

## Build-target semantics audit

This section distinguishes a target that compiles source from a target that only validates metadata, prints a checklist, or delegates work. These distinctions matter: a successful `make all` is meaningful only for the work its recipes actually perform.

| FILAMENT entry | What the current recipe actually does | Build-match assessment |
|---|---|---|
| `core` / `impl` | Runs toolchain preflight and trusted-manifest verification, then delegates to `impl all`. The portability workflow compiles the native runtime/static core and runs the VM regression suite. | **REAL NATIVE BUILD.** Linux and macOS passed in run `43629021139`; Windows native compilation passed, but its full VM regression remains in progress. |
| `java28` | Compiles Java sources, builds the native C port executable, and compiles the JNI shared bridge. | **REAL MULTI-LANGUAGE BUILD.** The `all` target compiles these artifacts; demo execution is a separate target and must be reported separately. |
| `xlt` | Builds the XLT executable through the C++ compiler. | **REAL BUILD.** The standalone `check` target also runs the supplied XLT example. |
| `regex` | Builds C, C++, natural-language C/C++, and Java components, then runs its test-suites target. | **BUILD + TEST.** |
| `compiler` | Compiles C and C++ compiler objects, builds/runs the compiler self-test, builds the runtime, inventories the library, and checks examples. Its `sanity` and tutorial targets print descriptive information. | **REAL BUILD + SELFTEST + EXAMPLE CHECK.** Do not count the printed sanity/tutorial descriptions as executable tests. |
| `decompiler` | Compiles C and C++ objects. Its `sanity` and tutorial targets print descriptive information only. | **REAL OBJECT BUILD; LIMITED VALIDATION.** No executable decompiler self-test is wired into this target at present. |
| `vm` package | Compiles C and C++ VM manager/runtime objects. The compiler-manager, memory-guard, GC, and tutorial check targets print contract descriptions. | **REAL OBJECT BUILD; CHECK TARGETS ARE DESCRIPTIVE.** The separate VM conformance workflow supplies behavioral VM evidence. |
| `scripting` | Checks that the specification, documentation, configuration, example, and test files exist. | **SOURCE-PACKAGE PRESENCE CHECK, NOT COMPILATION.** The current scripting Makefile does not compile or execute the scripting language. |
| `jetbrains` | The default `all` target invokes `help`; separate download targets acquire upstream IntelliJ source. | **NOT AN IDE BUILD.** The current target does not compile a SLeeLa IDE or produce an IDE artifact. |
| `install-check` | Delegates to installer preflight/manifest checks. | **INSTALLER VALIDATION, NOT AN INSTALLATION TEST.** A staged install, executable launch, uninstall, and upgrade cycle require separate evidence. |
| `tutorial-check` | Runs `scripts/tutorial-check.sh`. | **TUTORIAL VALIDATION.** It is not itself a module compiler. |
| `tests` | Delegates to `tests check`. | **TEST AGGREGATE.** Its result depends on the test suite's own declared scope. |
| `config` | Validates configuration data and builds/runs native configuration smoke programs. | **VALIDATION + COMPILE + SMOKE TEST.** |
| `route` | Parses `route/ROUTE.DATA.json` with Python's JSON parser. | **DATA-SYNTAX VALIDATION, NOT A NATIVE MODULE BUILD.** |
| `common-rails` | Builds/runs C and C++/Java heritage checks and checks SLeeLa/Nordshrift sources when their executables are present. Missing executables are reported as skips. | **MIXED BUILD + CHECK.** The root order builds Core first, but a standalone invocation can skip source-language checks. |
| `codecs` | Compiles C/C++ objects and archives `libsleela_codecs.a`. The executable tests are under the separate `codecs-test` target. | **REAL LIBRARY BUILD; TESTS ARE SEPARATE.** Root `make all` does not itself invoke `codecs-test`. |
| `churn` | Compiles each of five Java constraint sets and validates the JSON/XML/CSV/TXT data. The Java reports execute under the separate `churn-test` target. | **REAL JAVA COMPILE + DATA CHECK; EXECUTION TESTS ARE SEPARATE.** |
| `antivirus` | Builds C and C++ libraries, native/test executables, and the native core. | **REAL BUILD.** The test binaries are built by `all`; running the full behavioral suite is the separate `antivirus-test` target. |
| `registry` | The root target runs `build-registry.sh`; the Registry 3.0 workflow separately compiles and tests on Linux, macOS, and Windows. | **DEDICATED CROSS-PLATFORM PASS** for the Registry 3.0 workflow; do not infer that every registry export/integration target was exercised by that workflow. |
| `premium-vms` | Depends on Core and runs `make all` independently in each of the eleven generation directories, failing on the first error. | **REAL PER-GENERATION BUILD.** Dedicated VM conformance run `43619118411` built and executed all eleven on its tested commit. |
| `ballum` | Builds the native OS display library only when host display dependencies pass the check; otherwise it falls back to the portable test. | **HOST-CONDITIONAL.** A green root `make all` can mean the fallback test passed, not that the native display library was built. |
| `api-docs` | Runs the API documentation generation/build script. | **DOCUMENTATION BUILD**, not a runtime module build; latest recorded dedicated run is older than the current source baseline. |

### VM-specific CI evidence

The dedicated VM conformance run `43619118411` completed successfully with these steps:

- Built the Basic Native VM and compiler.
- Built all eleven Premium VM generations.
- Executed the Basic Native VM and all eleven Premium VMs.
- Ran each Premium VM admission self-check.
- Verified all Standard VM Modium manifests.
- Ran steady-state VM arithmetic benchmarks.

The VM arithmetic workflow `43619118403` also built the Basic Native VM and Premium generations 1–11, then ran a five-sample arithmetic matrix across all twelve targets. These are substantive build-and-execution results, but both runs predate the latest native portability source baseline. The current portability workflow `43629021139` independently confirms the Core build and the VM regression suite on Linux/macOS; Windows is still executing its full VM regression.

### Aggregate-build conclusion

**The repository has not yet demonstrated that every row in the root `make all` target compiles and completes perfectly on all three supported operating systems.** The root aggregate currently mixes real native builds, data/documentation checks, package-presence checks, and host-conditional fallbacks. Several separate test targets are not dependencies of `make all`. The current GitHub Actions evidence does not include a fresh, successful full root `make all` run on Linux, macOS, and Windows at the same source commit.

Do not mark the full repository build green until a clean-checkout aggregate build and the relevant separate test/install targets complete on each supported OS. In particular, the IDE and scripting entries need actual build/compile artifacts before they can be described as compiled modules.

---

## Platform qualification matrix

| Platform | Evidence at this status date | Overall assessment |
|---|---|---|
| Linux | Native portability jobs passed in the preceding run **43622491714**; the run as a whole failed later in Windows. | **Partial evidence; full root build not certified.** |
| macOS | Native portability jobs passed in the preceding run **43622491714**; the run as a whole failed later in Windows. | **Partial evidence; full root build not certified.** |
| Windows 10+ (MSYS2 UCRT64 / MinGW-w64) | Prior native portability run failed when the VM regression exceeded its 20-minute timeout. The Windows timeout has been raised to 40 minutes; run **43629021139** is in progress. | **Pending retest; do not mark green yet.** |

## Build repairs already reflected in the current Makefile history

- Root `make all` now includes XLT and its clean path.
- Windows root and direct `impl` builds select MinGW GCC/G++ defaults only when the caller has not overridden them.
- Windows build instructions explicitly require the POSIX shell and utilities used by the Makefiles.
- Test and codec Makefiles account for Windows `.exe` names; the C++ runtime smoke test links the POSIX thread runtime.
- Python-based connector and churn checks accept multi-token launchers such as `py -3`.
- OpenSSL preflight checks verify the SHA-256 header and actual symbol link.
- Ballum dependency checks include Xext when MIT-SHM is enabled; the openSUSE `zypper` package list now includes `libXext-devel`.
- The Windows VM regression timeout is now 40 minutes; its success remains pending until the current workflow finishes.

## Release / assurance gate

Do not label the repository's complete build matrix as green until a fresh run on the same release commit demonstrates:

1. Root `make all` success on Linux, macOS, and Windows 10+ (MSYS2 UCRT64 / MinGW-w64).
2. A successful root `make clean` and rebuild, with no reliance on stale artifacts.
3. Successful install checks and staged/local install validation on each supported OS.
4. Passing module-specific tests for optional targets that are part of the intended release.
5. A resolved Java conversion failure and a completed Windows VM regression retest.
6. Integrity verification against the trusted SHA-256 manifest without weakening or bypassing the verification gate.

**Current conclusion:** SLeeLa has a broad, wired Makefile build graph and several dedicated module workflows are passing. The full repository build is **not yet certified green** across Linux, macOS, and Windows. Pending and unqualified targets are deliberately shown as such rather than being inferred successful from the existence of Makefile rules.


## Recipe-level Makefile audit — 2026-11-14

This section records what the recipes do, rather than relying on target names or help text. The audit started from the exact master tree at c05c4f020fa880badb2f0b36c441307b729f7ad0. The repository tree contains **175 Makefiles / make fragments**, including nested platform, protocol, library, example, and per-generation build files. The table below covers every target dispatched by the root make all and the nested Makefiles those targets enter. A Makefile elsewhere in the tree is not thereby part of the root aggregate.

### Root make all: recipe truth table

| Root target | Delegated recipe / artifact | What a successful target proves | What it does not prove |
|---|---|---|---|
| core | autocheck, trusted SHA-256 verification, then impl all; builds sleela, sleelvac, nordshrift, libslcore.a, and stages the package | Toolchain preflight, manifest verification, native compile/link and package staging succeeded | It does not run the complete impl test suite or install the package |
| java28 | Java class compilation, C TCP-port executable, JNI shared library | Java/C/JNI compile and link succeeded for the detected host JDK | Demo channels are not executed; those are demo-port and demo-jni |
| xlt | impl/xlt all compiles and links xltc | XLT compiler executable was produced | The bundled example is not run by root xlt; that is make -C impl/xlt check |
| regex | C, C++, natural-language C/C++, Java and nested test suites | Builds and executes the regex C/C++/Java tests and nested C/C++/Java/SLeeLa suites | Does not prove other language/runtime consumers outside this Makefile |
| compiler | lib/compiler all: C/C++ objects, ABI self-test, runtime build, inventory and structural example check | Native front-end objects compile; the multi-language ABI self-test executes; inventory and example-structure scripts run | Tutorial and sanity targets only print descriptions; structural example checks are not the same as compiling every .sleela example |
| decompiler | lib/decompiler all: C and C++ objects | Decompiler source objects compile | Sanity/tutorial targets print descriptions; no executable decompiler self-test or decompilation fixture is run |
| vm | lib/vm all: C/C++ manager, linking, challenge, reports, memory-guard and GC objects | VM package native objects compile | cm-check, memory-guard-check, gc-check, and tutorial-check print contract descriptions; they are not behavioral tests |
| scripting | sleela-scripting all → file-presence assertions | Required spec, docs, config, example and test files exist | No scripting compiler, interpreter, or script test is built or run |
| jetbrains | jetbrains all → help | The helper target can print its source-acquisition instructions | No IntelliJ/SLeeLa IDE source is downloaded, compiled, packaged, or launched |
| install-check | Installer file/content checks, manifest-entry check and strict sleelactl registry check | Installer entry points and selected Native Drive/manifest/registry integration assumptions are present | No files are staged or installed; no installed executable is launched; no upgrade/uninstall lifecycle is exercised |
| tutorial-check | Runs scripts/tutorial-check.sh | The tutorial structure/evidence checker completed | Does not compile or execute every tutorial program |
| tests | tests check → OpenSSL probe and compilation of native smoke/AI/recognition executables | Dependencies are available and those test programs compile/link | The all/check path does not execute the compiled suite. Some individual AI/character-recognition targets execute their named programs, but root make tests does not invoke them |
| config | Python config-policy validation; builds and runs location and VM-config C smoke tests | Config data validation and both native smoke programs pass | Does not qualify every runtime configuration path or platform-specific installer setting |
| route | Python JSON parse of route/ROUTE.DATA.json | Route data is syntactically valid JSON | No native route engine is compiled or executed |
| common-rails | C heritage test compile/run; nested C++ and Java checks; SLeeLa and Nordshrift checks when executables exist | Available C/C++/Java checks execute; SLeeLa/SST sources are checked when their CLIs exist | Missing SLeeLa/Nordshrift executables are reported as SKIP, not failure; standalone invocation may therefore be partial |
| codecs | Compiles all declared C/C++ codec objects and archives libsleela_codecs.a | Codec static archive was produced | Does not run codec tests; codecs-test explicitly builds and executes registry, manager and FLAC-native tests |
| churn | Compiles five Java constraint sets and runs validate_data.py | Five sets compile and data files pass JSON/XML/CSV/TXT validation | Does not execute the five Java reports; churn-test does |
| antivirus | Builds C/C++ libraries, scanner, and four test executables | Native libraries, scanner and test binaries compile/link | Does not execute tests; antivirus-test runs C/C++/native/Python tests and definition checks |
| registry | Builds legacy and Registry 3.0 command-line executables | Both registry tools compile/link | Does not execute registry tests; registry-test builds and runs both C and C++ Registry 3.0 tests |
| premium-vms | Iterates generation directories 001–011 and invokes each make all | All eleven generation executables build against the Core archive | Root target does not invoke each generation's make test / --selfcheck; dedicated VM conformance workflow provides separate execution evidence |
| ballum | If host dependency check succeeds, builds the OS-specific static library and pkg-config file; otherwise runs portable tests | Either the native library build passed, or the portable fallback tests passed | A green root aggregate alone does not tell which branch ran; record the native dependency check and backend artifact separately |

### Corrected interpretation of test targets

The following root targets have actual execution recipes and should be kept distinct from compile-only or informational targets:

| Explicit target | Recipe-level behavior |
|---|---|
| make codecs-test | Executes three codec test binaries after building them |
| make churn-test | Executes all five Java constraint-report programs after compiling and validating data |
| make antivirus-test | Executes four native test binaries, native scanner signature/content checks, Python unit tests, and signature-definition validation |
| make registry-test | Executes the C and C++ Registry 3.0 test programs |
| make config | Validates configuration data and executes two native smoke tests |
| make tutorial-check | Executes the tutorial checker script |
| make common-rails | Executes the C, C++ and Java heritage checks; source-language checks run only when their executables are available |
| make -C impl test | Runs the broad Core/VM/runtime regression target declared by impl/Makefile; it is not a dependency of root make all |

### Root dispatcher and documentation drift

Two discrepancies were found in the root Makefile's human-facing descriptions:

1. The guess target's printed make all sequence omits xlt, although the actual all prerequisite list includes xlt immediately after java28.
2. The root help description says make tests builds and runs tests/; the actual tests/Makefile default all/check path compiles the programs but does not execute the full suite.

These are documentation mismatches, not evidence of a failing compiler. They should be corrected in the root Makefile in a separate source change and then reflected here after verification.

### Targets outside root make all

The following root targets are separately invocable and are not part of the default aggregate: connector, connector-test, connector-check-deps, lib-ballum, lib-ballum-check, lib-ballum-install, api-docs, server, sleeladoc, sleeladoc-jar, install/uninstall variants, editor integration targets, and dedicated module test targets (codecs-test, churn-test, antivirus-test, registry-test). Their existence or inclusion in the installer's module catalogue is not evidence that root make all compiled them.

Other independent Makefiles exist under Advantage, Audio, AutoCAD, CDMA, Compression, Crypto, Debugger, Drivers, HTTP 1.0–9.0, HTTP servers, Idea, Ledger, Native, OFDMA, Operating System, Philosophy, Resolver, Runtime, Server Edition, Signal Processing, SleelaTerminal, Sommeliers, Telephony/Skya, User Interface, Video, Web Server Technology, Wi-Fi, and the per-platform/per-example subtrees. These require their own clean build and test qualification. The presence of 175 Makefiles in the tree must not be read as 175 modules being built by the root aggregate.

### Aggregate status after recipe audit

**The root make all is a mixed build-and-validation aggregate, not a complete test or installation gate.** It compiles and links many real artifacts, executes some module tests, and performs several source/data/documentation checks; it also contains targets that only print descriptions or verify file presence. The explicit test targets and independent module Makefiles must be qualified separately. A repository-wide green status requires a clean build plus the relevant explicit tests and install lifecycle on Linux, macOS, and Windows at the same source commit.


## Per-module Make qualification — continuation 2026-11-14

This phase advances from recipe inspection to an explicit execution matrix. The Makefiles below were inspected on the current `master` tree after commit `68b1bb295b6b45ec9eef27b2d061f558b6c82e14`. This is a **static recipe audit**, not a claim that the commands were executed in this session. Results remain unqualified until clean builds/tests produce recorded exit codes and artifacts.

### Module-level commands and evidence gates

| Module | Clean build / test commands | What the commands actually qualify | Current FILAMENT status |
|---|---|---|---|
| Native Core / implementation VM | `make -C impl clean all`; then `make -C impl test`; also `make -C runtime clean test` | Core build produces the native executables and `libslcore.a`; Core test runs the declared VM/runtime regression list. The separate runtime test builds and executes GC/security and Java-bridge-injection tests. | **BUILD/TEST RETEST PENDING.** Current three-platform portability workflow is still running; do not infer the independent runtime suite from Core compilation. |
| Standard VM registry (001–010) | Build Core first; run the registry/manifest validation and actual CLI load/configure/list/select/boot/guest-execution checks for each manifest | Ten registry records and manifest files must be exercised individually; a registry count or parse is not a boot test. | **REGISTERED; INDIVIDUAL EXECUTION MATRIX REQUIRED.** Prior VM conformance evidence is on an earlier commit. |
| Premium VM generations (001–011) | `make premium-vms`; then for each `n`: `make -C sleela-virtual-machine/NN test` | Root target builds each generation; each generation's `test` target runs its executable with `--selfcheck`. Root `premium-vms` calls `all`, not `test`. | **BUILD TARGET WIRED; PER-GENERATION SELF-CHECK NOT IN ROOT TARGET.** Execute and record all eleven self-checks on the current baseline. |
| Java 28 bridge | `make -C java28 clean all`; then `make -C java28 demo` | `all` compiles Java classes, the native TCP-port driver, and JNI shared library; `demo` executes both port and JNI channels. Requires a full JDK/JNI headers. | **COMPILE AND DEMO QUALIFICATION REQUIRED.** Root aggregate currently builds only; record JDK version and JNI platform. |
| XLT compiler | `make -C impl/xlt clean all check` | Builds the C++ compiler and runs the bundled EPYC DDR2 example. | **BUILD + SMOKE COMMAND DEFINED; FRESH RESULT REQUIRED.** |
| Regex | `make -C regex clean all` | Compiles C/C++ and natural-language C/C++ binaries, executes those smoke binaries, runs the Java test, and delegates to nested test suites. | **BUILD + TEST COMMAND DEFINED; FRESH RESULT REQUIRED.** |
| Compiler package | `make -C lib/compiler clean all` | Compiles C/C++ front-end objects, runs the multi-language framework ABI self-test, builds the runtime compiler executable, inventories the library, and runs structural example checks. Sanity/tutorial recipes are descriptive output. | **BUILD + ABI SELFTEST; SOURCE-EXAMPLE EXECUTION STILL DISTINCT.** |
| Decompiler package | `make -C lib/decompiler clean all` | Compiles the C and C++ decompiler objects. Sanity/tutorial recipes print contract descriptions. | **OBJECT BUILD ONLY.** No executable decompiler self-test or fixture is wired; add one before calling the decompiler behavior qualified. |
| VM package | `make -C lib/vm clean all` | Compiles six C and seven C++ VM-management/runtime objects. `cm-check`, `memory-guard-check`, `gc-check`, and `tutorial-check` print descriptions; they do not execute behavioral tests. | **OBJECT BUILD ONLY.** Use Core/runtime behavioral suites for execution evidence; do not count the descriptive checks as tests. |
| Runtime GC/security services | `make -C runtime clean test` | Compiles and executes `test_gc_security` and `test_java_bridge_injection`; this is an independent Makefile and is not called by root `make all`. | **EXPLICIT TEST REQUIRED.** Include in the release matrix on Linux, macOS, and Windows where supported. |
| Repository tests aggregate | `make -C tests clean check` | Runs the OpenSSL probe and compiles the aggregate native smoke/AI/recognition executables; the default `check` path does not execute the complete suite. | **COMPILE/CHECK ONLY.** Run the intended individual behavioral targets separately and report their names. |
| Configuration | `make -C config clean test` | Validates configuration policy and compiles/runs location and VM-configuration smoke programs. | **BUILD + SMOKE COMMAND DEFINED; FRESH RESULT REQUIRED.** |
| CommonRails | `make -C common-rails clean check` | Executes C, C++, and Java checks; SLeeLa/Nordshrift checks are conditional on the relevant executables being present. | **MIXED CHECK.** Record conditional skips separately from passes. |
| Codecs | `make -C codecs clean all test` | Builds the static C/C++ codec archive and executes registry, manager, and FLAC-native tests. | **BUILD + TEST COMMAND DEFINED; ROOT ALL RUNS BUILD ONLY.** |
| Churn | `make -C churn clean test` | Compiles all five Java constraint sets, validates JSON/XML/CSV/TXT data, and executes all five reports. | **BUILD + TEST COMMAND DEFINED; ROOT ALL RUNS COMPILE/DATA CHECK ONLY.** |
| Antivirus / Sleelavirin | `make -C antivirus clean test` | Builds C/C++ libraries, scanner and test binaries, then executes native scanner, content-signature, C/C++, Python, and definition checks. | **BUILD + TEST COMMAND DEFINED; ROOT ALL DOES NOT RUN FULL TEST.** |
| Registry | `make -C registry clean all test` | Builds the registry library/CLI and executes the C and C++ Registry 3.0 test programs. | **BUILD + TEST COMMAND DEFINED; FRESH RESULT REQUIRED.** |
| Ballum | `make -C ballum clean test`; separately `make -C ballum check-deps all` and install/uninstall in a staged prefix | Portable tests execute without display dependencies. Native `all` depends on platform display libraries; dependency check and produced backend artifact must be recorded separately. | **PORTABLE TEST AND NATIVE BACKEND ARE DISTINCT GATES.** Root fallback is not evidence of a native backend build. |
| Scripting | `make -C sleela-scripting clean all test` | Current rules assert source/specification/documentation/configuration/example/test file presence; no scripting compiler or interpreter is built or run. | **PRESENCE CHECK ONLY.** Not a compiled module. |
| JetBrains / SLeeLa IDE | `make -C jetbrains all` | Current default target prints help; download targets acquire upstream IntelliJ sources. | **NO IDE BUILD.** No SLeeLa IDE artifact is produced or launched by this Makefile. |

### Newly confirmed GC build/test boundary

The native Core Makefile explicitly compiles `../runtime/garbage_collector.c` into the shared Core archive `impl/build/libslcore.a`. This confirms the collector is part of the Core build and the Premium runners' shared engine. Separately, `runtime/Makefile` has an executable `test` target that runs both GC/security and Java bridge-injection tests. The root `make all` does not invoke `make -C runtime test`, and the `lib/vm` package's `gc-check` is informational output only. Keep those three facts distinct in any GC pass claim.

### Current portability workflow snapshot

The latest inspected Native C and C++ Portability run is **43630812944**, on `master` commit `68b1bb295b6b45ec9eef27b2d061f558b6c82e14`. At the time of this record, its Windows 10+/MinGW-w64, macOS, and Ubuntu jobs were all **in progress**. Earlier run **43630413755** had Linux and macOS jobs completed successfully while Windows remained in progress. Neither run is a completed three-platform pass yet.

The workflow snapshot is available at: https://github.com/mearvk/SLeeLa/actions/runs/43630812944

### Next execution order

1. Wait for run 43630812944 to finish; inspect all three jobs and retain Windows regression logs if it fails or times out.
2. Execute clean Core build and Core regression tests on each supported OS; run the independent `runtime` test suite as a separate gate.
3. Execute the Standard 001–010 selection/boot matrix and Premium 001–011 build plus `--selfcheck` matrix on the same commit.
4. Execute Java 28 demos, XLT example, compiler ABI self-test, regex suites, configuration smoke tests, CommonRails, codecs, churn, antivirus, registry, and Ballum portable/native checks.
5. Keep decompiler, scripting, and JetBrains explicitly unqualified until their Makefiles produce and test the missing executable artifacts.
6. Update this register with exact commit SHA, command, platform, exit status, artifact, and test count. Do not mark a module PASS from a target name, a dry run, a help message, or a source-presence assertion.

**Execution status for this continuation:** recipe inspection completed; no module build command was executed by this GitHub repository inspection. All commands above are the next verification gates, not reported passes.


## Live build and CI result refresh — 2026-11-14 19:24 UTC

This section supersedes the earlier in-progress snapshot above. Results below were refreshed from GitHub Actions after commit `a7bc766c0bc260d3c824ee05aa8d8848e113ae68` updated this document.

### Latest portability run — current FILAMENT commit

Workflow: [Native C and C++ Portability, run 43631417081](https://github.com/mearvk/SLeeLa/actions/runs/43631417081)

- Tested commit: `a7bc766c0bc260d3c824ee05aa8d8848e113ae68`
- Run status at refresh: **in progress**
- macOS job: runtime build, frozen 155-opcode ISA check, OSLS native registry build, and OSLS VM compiler/runtime dispatch steps completed successfully; full VM regression suite was still running.
- Ubuntu job: same initial build/ISA/registry/dispatch steps completed successfully; full VM regression suite was still running.
- Windows 10+ / MinGW-w64 job: philosophy event-tree tests and trusted-source manifest regeneration completed successfully; native runtime/static Core build was still running.
- No final three-platform conclusion is available yet. Do not mark portability PASS until all three jobs finish successfully.

The preceding run, [43630812944](https://github.com/mearvk/SLeeLa/actions/runs/43630812944), tested commit `68b1bb295b6b45ec9eef27b2d061f558b6c82e14`. At refresh, macOS and Ubuntu jobs had completed successfully, while Windows was still in progress. This is partial two-platform evidence only.

### Investigated Windows failure — run 43622491714

Workflow: [Native C and C++ Portability, run 43622491714](https://github.com/mearvk/SLeeLa/actions/runs/43622491714)

- Tested commit: `d05501ff7459c0a9aec9c1eb46f6dcfb91575cca`
- Ubuntu: **PASS**
- macOS: **PASS**
- Windows 10+ / MinGW-w64: **FAIL — timeout**, not a compiler/linker error in the captured terminal section.
- The Windows defender verification passed for 18,668 items. Multiple source examples and regression checks passed, including class dispatch, lineage, object references, regex native foundation, and the version-awareness accepted/rejected cases.
- The job timed out after 20 minutes in **Run full VM regression suite**. The final captured diagnostic test reached malformed `#sleela` input, then emitted the interactive prompt `Terminate batch job (Y/N)?`; the runner sent Ctrl-C and terminated the step.

**Required repair:** make malformed-input/diagnostic regression invocations non-interactive on Windows. Ensure child processes have stdin redirected from a non-interactive source, invoke compiler/test processes without shell batch wrappers that prompt on Ctrl-C, and enforce a bounded subprocess timeout with captured stdout/stderr. A malformed source test must assert a nonzero compiler exit and expected diagnostic, not wait for a user response. Re-run the Windows job after repair. Do not classify the timeout as a source compiler failure without a reproducing compiler diagnostic.

### Additional completed workflow evidence

| Workflow | Run / tested revision | Result | Qualification |
|---|---|---|---|
| Registry 3.0 Cross-Platform Verification | [43622485269](https://github.com/mearvk/SLeeLa/actions/runs/43622485269), commit `6a969549ee2c` | **PASS** | Ubuntu, macOS, and Windows jobs all completed successfully. This is an earlier revision than the current FILAMENT commit. |
| Advantage Library Verification | [43622491621](https://github.com/mearvk/SLeeLa/actions/runs/43622491621), commit `d05501ff7459` | **PASS** | Runtime build, six demos, and documentation verification completed successfully. |
| Complete SLeeLa VM and Language Conformance | [43619118411](https://github.com/mearvk/SLeeLa/actions/runs/43619118411), commit `d5034d8f1b1d` | **PASS** | Windows, Ubuntu, macOS, arithmetic, and Basic Native VM + Premium SLVM 1–11 jobs passed on that revision. |
| VM Arithmetic Table Completion | [43619118403](https://github.com/mearvk/SLeeLa/actions/runs/43619118403), commit `d5034d8f1b1d` | **PASS** | Basic Native + Premium SLVM 1–11 arithmetic workflow passed on that revision. |
| Comparison Standards Conformance | [43619118402](https://github.com/mearvk/SLeeLa/actions/runs/43619118402), commit `d5034d8f1b1d` | **PASS** | Ubuntu, macOS, and Windows shared-vector jobs passed on that revision. |

These historical workflow passes are retained as evidence, but must not be promoted to current-commit status without rerunning them against the current tree.

### Current module qualification ledger

| Module / suite | Most recent evidence located | FILAMENT disposition |
|---|---|---|
| Native Core build | Current run 43631417081: macOS/Ubuntu native build step passed; Windows build still running | **PARTIAL — await all platforms** |
| Frozen 155-opcode ISA | Current run 43631417081: macOS/Ubuntu checks passed | **PARTIAL — Windows pending** |
| OSLS native registry | Current run 43631417081: macOS/Ubuntu build/test step passed; Windows pending | **PARTIAL — Windows pending** |
| OSLS VM compiler/runtime dispatch | Current run 43631417081: macOS/Ubuntu step passed; Windows pending | **PARTIAL — Windows pending** |
| Full VM regression suite | Current run 43631417081: macOS/Ubuntu running; previous Windows attempt timed out | **IN PROGRESS / WINDOWS HARNESS REPAIR REQUIRED** |
| Native VM + Premium SLVM 1–11 | Run 43619118411 passed on commit `d5034d8f1b1d` | **HISTORICAL PASS — rerun current revision** |
| VM arithmetic | Run 43619118403 passed on commit `d5034d8f1b1d` | **HISTORICAL PASS — rerun current revision** |
| Comparison vectors | Run 43619118402 passed on commit `d5034d8f1b1d` | **HISTORICAL PASS — rerun current revision** |
| Registry 3.0 | Run 43622485269 passed on commit `6a969549ee2c` | **HISTORICAL THREE-PLATFORM PASS — rerun current revision** |
| Advantage Library | Run 43622491621 passed on commit `d05501ff7459` | **HISTORICAL PASS — rerun current revision** |
| Independent runtime GC/security tests | No current-run result established by the workflow evidence inspected in this refresh | **SEPARATE TEST GATE — NOT YET QUALIFIED** |
| Remaining Makefile modules in the recipe matrix above | No fresh execution evidence inspected in this refresh | **PENDING — execute the listed clean build/test commands** |

### Next actions, ordered by dependency

1. Allow run 43631417081 to finish and refresh its final job conclusions.
2. Repair the Windows malformed-input regression harness so it cannot prompt for interactive batch termination; preserve diagnostic assertions and timeouts.
3. Re-run portability and verify Windows alongside macOS and Ubuntu.
4. Run the independent `make -C runtime clean test` suite; record GC/security and Java bridge-injection results independently of Core compilation.
5. Re-run Native VM + Premium SLVM 1–11, arithmetic, comparison, and Registry 3.0 workflows against the current commit.
6. Continue the module-by-module Make execution matrix. Update each module only with exact commit, platform, command, exit status, and artifact/test evidence.

**Refresh integrity note:** this update reports only the GitHub Actions states and logs inspected. It does not claim that local shell builds were run during this refresh, and it does not label pending or historical evidence as a current PASS.


## Root build and installation pipeline audit — 2026-11-14 15:28 EST

This section supersedes the earlier portability status snapshot where it conflicts with the completed runs below.

### Root dispatcher coverage

The root `Makefile` defines `all` as the following dependency chain:

`core → java28 → xlt → regex → compiler → decompiler → vm → scripting → jetbrains → install-check → tutorial-check → tests → config → route → common-rails → codecs → churn → antivirus → registry → premium-vms → ballum`.

Important distinctions:

- The current `Native C and C++ Portability` workflow does **not** invoke root `make all` or `make build`. It runs the native `impl` build and its regression suite, plus selected independent checks.
- Root `make all` includes `install-check`; this is an installer-wiring/presence check, not an installation.
- Root `make all` does not depend on `install`, `install-local`, `install-system`, or `install-all`. The guided `advanced-build` prompt is only part of `make build` after `all`, and is intentionally skipped without a TTY unless explicitly opted in.
- The installer Makefile has actual install/uninstall recipes, but the current portability workflow does not perform a staged install, inspect the staged tree, execute installed binaries, or test uninstall/rollback.
- Some root modules' default `all` recipes are build-only or presence/data checks. For example, the scripting default checks source/documentation/configuration/example/test presence; the JetBrains default target displays helper/help output and does not build a SLeeLa IDE. The root `tests` target calls `make -C tests check`, which is not equivalent to executing every independent test suite.
- Consequently, there is currently no fresh end-to-end CI qualification of the complete root build-and-install path on Linux, Windows, and macOS. Do not infer it from a successful native Core build.

### Latest portability run results

Workflow: [Native C and C++ Portability, run 43631952431](https://github.com/mearvk/SLeeLa/actions/runs/43631952431)

- Tested commit: `dd4cfb6c7354fd6ab198e4f67e86b8a3fa5eaaed`.
- Ubuntu: **PASS** — native Core build, ISA, OSLS registry, OSLS VM dispatch, full VM regression, AI/XOR, character recognition, Wi-Fi, and native platform smoke checks completed.
- macOS: **PASS** — same workflow stages completed successfully.
- Windows 10+ / MinGW-w64: **FAIL — full VM regression step timed out at 40 minutes**. Native Core build, ISA verification, OSLS registry, and OSLS VM dispatch all passed. Windows steps after the full VM regression were skipped.

### Corrected Windows timeout diagnosis

The new log disproves the earlier assumption that the malformed `#sleela` diagnostic test is the current hang:

- The version-awareness suite explicitly completed with `VERSION TESTS: PASS`, including malformed pragma rejection and the no-run check.
- Nordshrift compilation, semantic checks, end-to-end artifact execution, compatibility checks, CommonRails, and artifact ABI checks progressed.
- The final visible test output is `terminal-ok` from the Windows ConPTY child command. The C smoke program's subsequent `terminal platform: windows-conpty` and `terminal platform smoke: PASS` lines never appear.
- In `impl/core/terminal_platform_smoke.c`, those final lines are printed only after `slterminal_close()` returns. The evidence therefore localizes the current hang to the Windows terminal smoke's close/ConPTY teardown path, most likely inside `slterminal_close()`; it is not yet a proven single API call.
- The next isolated reproducer is `make -C impl test-terminal-platform` on Windows, with bounded diagnostics around process reap, drain-thread startup, `ClosePseudoConsole`, output-handle close, and thread joins. Keep the teardown bounded and ensure no worker thread or synchronous pipe read can outlive the handle it uses. Only after this isolated target passes should the full Windows regression suite be rerun.

### Root build/install qualification gates still required

| Gate | Required command / evidence | Current status |
|---|---|---|
| Root full build | `make clean all` on Linux, Windows (supported MinGW/MSYS environment), and macOS; capture every child target and artifact | **NOT RUN AS A ROOT PIPELINE** |
| Root tests | Execute each real test suite, not only root `tests: check`; record commands, counts, and exit codes | **PARTIAL / NOT ROOT-WIRED** |
| Installer entry-point checks | `make install-check` | Target is wired into `all`; fresh cross-platform result not established by the portability workflow |
| Staged user-local install | Run `make -C install install-local` with a temporary `PREFIX`/staging root and no sudo; inspect manifest and expected binaries/libraries | **NOT QUALIFIED** |
| Staged system install | Exercise `install-system` through `DESTDIR`/temporary staging; confirm no host system writes | **NOT QUALIFIED** |
| Install-all | Test all standard locations in an isolated container/VM, not on the host machine | **NOT QUALIFIED** |
| Installed-product smoke | Run installed `sleela`, `sleelvac`, and `nordshrift` from the staged prefix; validate library/data/config resolution | **NOT QUALIFIED** |
| Uninstall / rollback | Uninstall the staged install and verify only tracked files are removed and preserved config/backup behavior is correct | **NOT QUALIFIED** |
| Advanced guided installer | Test Linux, macOS, and Windows guided entry points with explicit non-interactive choices and isolated staging | **NOT QUALIFIED** |

**Immediate priority:** repair and isolate the Windows ConPTY teardown hang first. Then add a dedicated root-pipeline workflow that executes the root dispatcher and staged installer qualification per supported OS, while keeping optional dependency-dependent modules separately reported rather than silently counted as successful.

**Evidence integrity:** the run above is the first completed portability result on the current FILAMENT commit. It is a two-platform pass and a Windows regression failure; it is not a root `make all` or installer pass. No local shell build or installation was executed as part of this GitHub inspection.


## Root build and installation qualification — 2026-11-14 (current work)

This section records the root dispatcher changes and the dedicated end-to-end qualification path. Build status is **pending until the current Actions run completes**; no current root build or install is marked PASS solely because a workflow was added.

### Root dispatcher improvements

The root `Makefile` now makes these test stages part of `make all`:

| Root stage | Command actually invoked | What it qualifies |
|---|---|---|
| Codecs tests | `make -C codecs test` via `codecs-test` | Registry, manager, and FLAC-native executable tests |
| Churn tests | `make -C churn test` via `churn-test` | Compiles and runs all five Java constraint-report sets, plus data validation |
| Antivirus tests | `make -C antivirus test` via `antivirus-test` | Native scanner/test binaries and Python tool/test checks |
| Runtime tests | `make -C runtime clean test` via `runtime-test` | GC/security and Java bridge-injection executable tests |
| Premium VM tests | For each `001–011`, run `make all` then `make test` | Builds each Premium SLVM and runs its `--selfcheck` |
| Existing root test stage | `make -C tests check` | Builds and executes memory, reflection, Nordshrift, runtime, network-security, XOR neural-net, and character-recognition smoke binaries |

Additional dependency/order corrections:

- `common-rails` now explicitly depends on `core`.
- `registry` now explicitly depends on `core`, so the registry export step cannot race the native runtime in parallel builds.
- Root `clean` now also removes standalone `runtime` test artifacts.
- The JetBrains Makefile's `all` target now validates its source-acquisition helper files. This stage is explicitly **not** an IntelliJ/SLeeLa IDE build; no IDE artifact is claimed.
- Installer user-home resolution now handles both Linux `getent` and macOS `dscl` rather than assuming `getent` exists.

### Root build entry points

- `make all`: full root module dispatcher plus wired tests.
- `make build SLEELA_ADVANCED_BUILD=no`: exercises the user-facing build target and explicitly retains the plain build.
- `make advanced-build SLEELA_ADVANCED_BUILD=yes`: runs the OS-specific guided installer after the build products exist.

### Dedicated root qualification workflow

Workflow: [Root Build and Install Qualification](https://github.com/mearvk/SLeeLa/actions/workflows/root-build-install.yml)

It is configured to execute:

1. `make clean build SLEELA_ADVANCED_BUILD=no` on Ubuntu, macOS, and Windows 10+ / MinGW-w64.
2. Product and registry artifact presence checks on each platform.
3. Staged user-local install, executable smoke tests, and uninstall on Linux and macOS.
4. Staged system-prefix install using `DESTDIR`, executable smoke tests, and uninstall on Linux and macOS; no writes to the hosted machine's real system prefix.
5. The guided POSIX installer through the root `advanced-build` target on Linux and macOS, using a disposable user prefix.
6. The native PowerShell guided installer on Windows, using a disposable destination.
7. Full `install-all` / `uninstall-all` coverage on disposable hosted Ubuntu and macOS runners, including both user-local layouts and `/usr/local`.

The install-all test is intentionally isolated to ephemeral GitHub-hosted VMs because that target updates multiple real system/user locations. It is not run against a developer workstation.

### Current CI state

- Latest root qualification run: [43638763796](https://github.com/mearvk/SLeeLa/actions/runs/43638763796), queued when this section was written.
- Latest native portability run: [43638763611](https://github.com/mearvk/SLeeLa/actions/runs/43638763611), queued when this section was written.
- The Windows ConPTY terminal smoke test has been added ahead of the full VM regression suite to isolate the earlier timeout. It is not yet recorded as passing.
- The first draft of the root workflow failed before scheduling jobs due to a workflow-definition issue; it was replaced with separate POSIX and Windows jobs. The corrected workflow is the one linked above.

**Qualification rule:** the root build is not green until all three root build jobs pass. The installation pipeline is not green until the staged user-local and system installs, installed-product smoke tests, guided installer checks, and sandboxed install-all/uninstall-all checks pass. Pending and queued runs remain pending.


### Final dispatcher ordering refresh — 2026-11-14

The Premium VM root loop now runs both `all` and `test` for every generation `001–011`; each `test` invokes that VM's `--selfcheck`. The root `guess` and `help` output were synchronized with the expanded target order, including the new test stages.

Latest root Makefile qualification revision: `9a51ccf4018c02792ac29f6c108251c48f6f6528`.

Latest root build/install workflow run: [43638851371](https://github.com/mearvk/SLeeLa/actions/runs/43638851371) — queued at the last refresh. It includes the current dispatcher, Premium VM selfchecks, install-all home-directory resolver, and guided installer gate tests. No pass is claimed until its platform jobs and dependent installation jobs complete.

## Root build failure isolation and repair — 2026-11-14 (live refresh)

### Failed root qualification evidence

The prior Root Build and Install Qualification run **43638508681** failed on all three root build jobs. Log inspection isolated two concrete root blockers:

| Platform | Failed stage | Observed failure | Diagnosis |
|---|---|---|---|
| Windows 10+ / MSYS2 UCRT64 | java28 | java28: javac not found | The JDK installed by actions/setup-java was not on the MSYS2 POSIX shell PATH. Native Core and Nordshrift had already compiled before this stage failed. |
| Ubuntu | antivirus-test | OSError: [Errno 9] Bad file descriptor in safe_quarantine(), line 265 | The cleanup sentinel is set to -1 after ownership is transferred to os.fdopen(), but the finally block checked only for None and attempted os.close(-1). |
| macOS | antivirus-test | Same bad-file-descriptor error in safe_quarantine() | Same platform-independent descriptor ownership bug; the same test failed on both POSIX platforms. |

The failing antivirus regression was test_quarantine_collision_and_changed_file_refusal; the suite reported 36 tests, one error and one skipped optional ClamAV integration test on both Ubuntu and macOS.

### Repairs committed to master

| Repair | File | Commit | Change |
|---|---|---|---|
| Quarantine descriptor cleanup | antivirus/tools/sleelavirin_scan.py | f77602feaa2c8835e94d226d0954324d3a1fa394 | The finalizer now closes the destination descriptor only when it is non-null and non-negative, respecting the -1 ownership-transfer sentinel. |
| Windows JDK visibility | .github/workflows/root-build-install.yml | 9bc8e772577c6cc62633eaac9ac93785ec6bca63 | The Windows MSYS2 root-build step converts JAVA_HOME with cygpath, prepends the JDK bin directory to PATH, and verifies javac is resolvable before invoking the root build. |

These are targeted source/workflow repairs. They are **not yet PASS claims**; the new workflow attempts must complete.

### Current retest queue

| Workflow | Run | Commit | Status at refresh |
|---|---:|---|---|
| Root Build and Install Qualification | [43639947785](https://github.com/mearvk/SLeeLa/actions/runs/43639947785) | 9bc8e772577c6cc62633eaac9ac93785ec6bca63 | **QUEUED** — includes both repairs |
| Native C and C++ Portability | [43639947779](https://github.com/mearvk/SLeeLa/actions/runs/43639947779) | 9bc8e772577c6cc62633eaac9ac93785ec6bca63 | **QUEUED** |
| Root Build and Install Qualification | [43639945147](https://github.com/mearvk/SLeeLa/actions/runs/43639945147) | f77602feaa2c8835e94d226d0954324d3a1fa394 | **QUEUED** — source repair before the workflow PATH correction |

The older root run 43638851371 and portability run 43638881348 were also still queued at this refresh. Do not use those older queued attempts as evidence for the new fixes.

### Premium arithmetic workflow: test result versus publication result

The arithmetic workflow **43638851340** built Basic Native and Premium SLVM/1–SLVM/11, ran each generation's self-check successfully, and passed the five-sample timing/checksum matrix (eight cases per VM). Its overall GitHub Actions conclusion was **FAILURE** only because the final bot push was rejected as non-fast-forward after another workflow updated master. The arithmetic test step itself passed for all twelve targets. Preserve the artifact and test evidence; repair the workflow's concurrent publication behavior separately before treating the workflow as fully green.

### Next root-build checklist

- [ ] Wait for run 43639947785 and inspect all three OS jobs and dependent installer jobs.
- [ ] Confirm the Windows JDK preflight prints a valid javac path and that java28 compiles JNI headers/library.
- [ ] Confirm Ubuntu and macOS antivirus tests pass the quarantine collision/refusal regression.
- [ ] If either repair still fails, inspect the new job logs and fix the actual remaining failure rather than weakening/skipping the test.
- [ ] Confirm root make clean build SLEELA_ADVANCED_BUILD=no completes on Linux, macOS, and Windows.
- [ ] Confirm staged install, product launch, uninstall, guided installer, and sandboxed install-all/uninstall-all jobs complete.
- [ ] Refresh the trusted manifest only for intentional source changes; keep verification fail-closed.
- [ ] Update the root matrix only from completed run evidence, not from queued runs or target presence.

**Current root status:** **REPAIR COMMITTED — RETEST QUEUED — NOT QUALIFIED.** The complete root build and installation pipeline remains uncertified until the new three-platform root and dependent installation jobs pass.


## CI restart after cancelled qualification runs — 2026-11-14

A fresh CI cycle was requested after GitHub Actions marked the queued qualification attempts as **cancelled**. The cancellations are not build failures and provide no pass evidence.

Cancelled runs observed:
- Root Build and Install Qualification: [43639947785](https://github.com/mearvk/SLeeLa/actions/runs/43639947785) — cancelled at commit `9bc8e772577c6cc62633eaac9ac93785ec6bca63`.
- Native C and C++ Portability: [43639984320](https://github.com/mearvk/SLeeLa/actions/runs/43639984320) — cancelled at commit `daf8fac443e530ab595f88bbc37eff8f06cf114b`.
- Earlier root and portability attempts [43639945147](https://github.com/mearvk/SLeeLa/actions/runs/43639945147), [43639947779](https://github.com/mearvk/SLeeLa/actions/runs/43639947779), and [43639945068](https://github.com/mearvk/SLeeLa/actions/runs/43639945068) — cancelled.

The source repairs remain in master. A new push-triggered run is being initiated by this checklist update so the root build and portability matrices can execute again. Qualification remains **PENDING** until all required OS and dependent installation jobs complete successfully.


## CI result review and second repair cycle — 2026-11-14

### Results from the restarted qualification runs

The fresh runs on commit `b4b9d4230105fbbb6bdd9ee8993659d7a2052ab6` completed and exposed additional workflow defects:

| Workflow / platform | Result | Evidence |
|---|---|---|
| Root build — Ubuntu | **FAIL** | The shared POSIX build step called `cygpath`, which is not installed on Ubuntu. The job stopped before the root build. |
| Root build — macOS | **FAIL** | Same misplaced `cygpath` call; the job stopped before compilation. |
| Root build — Windows 10+ / MSYS2 | **FAIL** | The root build reached `java28`, where `javac` was not found in the MSYS2 environment. |
| Native portability — Ubuntu | **PASS** | Native runtime build, ISA/OSLS checks, VM regression suite, AI tests, and platform smoke targets completed successfully. |
| Native portability — macOS | **PASS** | Same native portability sequence completed successfully. |
| Native portability — Windows 10+ / MSYS2 | **FAIL / TIMEOUT** | Native compilation, ISA/OSLS checks, and earlier VM tests passed. The full regression-suite step timed out after 40 minutes, ending at the Windows ConPTY terminal smoke output and a `Terminate batch job (Y/N)?` prompt. |

Run records:
- Root qualification: [43640962056](https://github.com/mearvk/SLeeLa/actions/runs/43640962056) — failed on all three root jobs; dependent installer jobs were skipped.
- Native portability: [43640962082](https://github.com/mearvk/SLeeLa/actions/runs/43640962082) — Ubuntu/macOS passed; Windows timed out in the full VM regression suite.

### Repairs now committed to master

| Repair | File | Commit | Change |
|---|---|---|---|
| Restore POSIX root build | `.github/workflows/root-build-install.yml` | `4e6ab1156f3538fd72aabbdf19c007266ca7313f` | Removed the MSYS2-only `cygpath` / JDK PATH preflight from the shared Ubuntu/macOS build step. |
| Make Windows JDK discovery explicit | `.github/workflows/root-build-install.yml` | `4e6ab1156f3538fd72aabbdf19c007266ca7313f` | Added the JDK path conversion and `javac.exe` / `java.exe` preflight only to the Windows MSYS2 job; exports explicit `JAVAC` and `JAVA` paths before `make`. |
| Make ConPTY smoke child terminate deterministically | `impl/core/terminal_platform_smoke.c` | `f81c5165b2b95ad1d32eb070b7a3c6d757a3b1cb` | Uses `cmd.exe /D /C "echo terminal-ok & exit /b 0"` to disable command AutoRun and explicitly exit the child command. |
| Run ConPTY check on Windows | `.github/workflows/native-portability.yml` | `8802e84eb61a1a345044f62558c0805a452dbe75` | Moved the isolated ConPTY teardown smoke step out of the POSIX matrix (where its Windows condition could never be true) and into the Windows job before the full regression suite. |

### Retest requirements

- [ ] Re-run root qualification and verify Ubuntu/macOS get past setup and complete `make clean build SLEELA_ADVANCED_BUILD=no`.
- [ ] Verify Windows preflight resolves `javac.exe` and `java.exe`, then completes `java28` and the root build.
- [ ] Verify the isolated Windows ConPTY step completes within its 3-minute limit.
- [ ] Verify the complete Windows VM regression suite completes after the ConPTY repair.
- [ ] Confirm dependent staged installer and install-all/uninstall-all jobs execute and pass after the POSIX root matrix passes.

**Qualification status:** **REPAIRS COMMITTED — RETEST REQUIRED — NOT QUALIFIED.** The previous root failure is explained by a workflow-script placement error, not a source compilation defect on Ubuntu/macOS. The Windows root JDK and ConPTY fixes remain unverified until the new CI attempts finish.


## CI triage — GC limit, Windows POSIX regex, and ConPTY — 2026-11-14

### Findings from run 43647374630 (root) and run 43647408723 (native portability)

| Platform / target | Observed result | Classification | Action |
|---|---|---|---|
| Root Ubuntu runtime-test | GC security test aborted because it expected 512 MiB while the collector default is 1 GiB | **Test expectation stale; not an allocation failure** | Updated the assertion to the implemented 1 GiB default. The test checks `gc_limit()`; it does not allocate 512 MiB or 1 GiB. |
| Root macOS runtime-test | Same stale 512 MiB assertion | **Test expectation stale; not an allocation failure** | Same correction; no runtime heap policy reduction. |
| Root Windows regex C backend | Linker could not resolve POSIX regex symbols (`regcomp`, `regexec`, `regerror`, `regfree`) | **Missing Windows build dependency/link flag** | Added MSYS2 UCRT64 `mingw-w64-ucrt-x86_64-libsystre` and linked the C POSIX-regex test with `-lregex`. |
| Native Windows ConPTY smoke | Child printed `terminal-ok`, but teardown did not finish before the 3-minute timeout | **Runtime smoke/teardown hang; compilation had already succeeded** | Keep isolated as a Windows runtime qualification issue. Do not report this as a C/C++ compilation failure. |
| Native Ubuntu/macOS | All native portability jobs passed | **PASS** | Preserve the successful platform evidence. |

### Repairs pushed to master

- `2efaee0c22b1576d93f5f37074408cdf24ee3072` — update `runtime/tests/test_gc_security.c` to assert the collector's documented 1 GiB default and explicitly note that this is a configuration check, not a large allocation.
- `d26d98b048441316b8d753d0f87858333cb69f47` — add the Windows `-lregex` link dependency to the POSIX regex C target.
- `f0d5ea33ac33566066ee7be874dd77ee8e69a07a` — install the MSYS2 UCRT64 `libsystre` package in the Windows root-build environment.

### Memory policy clarification

The CI logs show the runtime memory manager reporting a 2 GiB hard limit. The GC test failure was solely an outdated expected-value assertion against a 1 GiB default in `runtime/garbage_collector.c`. It did not request a 512 MiB allocation. The test was corrected to match the current runtime contract; the runtime limit itself was not lowered or bypassed.

### Qualification status and retest

- [ ] Root Linux/macOS runtime tests pass with the corrected 1 GiB expectation.
- [ ] Windows root regex C executable links and its tests execute with the MSYS2 POSIX regex package.
- [ ] Full root builds complete on all three platforms.
- [ ] Dependent staged-install and install-all/uninstall-all jobs complete.
- [ ] Windows ConPTY teardown hang is resolved or separately documented as a non-compilation runtime limitation with an explicit local verification record.

**Current status:** two targeted CI build/test fixes committed; retest required. Ubuntu/macOS native portability remain PASS from run 43647408723. Windows native compilation completed through the terminal smoke's visible output, but ConPTY teardown remains unqualified. Do not label the overall root build/install pipeline qualified until its new matrix and dependent installation jobs pass.


## Root build qualification — repair retest cycle 3 — 2026-11-14

**Source head at audit:** `d8c38351277c14deeb1d2701ec5ad7d84e035f81` (workflow-trigger commit on `master`). The root workflow path filter now includes `FILAMENT.md`; the trigger configuration is present, but this connector does not expose the push-run listing needed to confirm the resulting hosted run ID/status.

### Verified repairs in source

| Issue from run 43647374630 | Source correction | Current source inspection |
|---|---|---|
| Ubuntu/macOS runtime test expected 512 MiB while collector default is 1 GiB | `runtime/tests/test_gc_security.c` now asserts 1 GiB and explicitly says this is a configuration-only test, not a large allocation | **PATCH PRESENT — RETEST REQUIRED** |
| Windows POSIX regex unresolved symbols | `regex/Makefile` links `-lregex` on Windows | **PATCH PRESENT — RETEST REQUIRED** |
| Windows missing regex library package | Root MSYS2 setup installs `mingw-w64-ucrt-x86_64-libsystre` | **PATCH PRESENT — RETEST REQUIRED** |
| Windows ConPTY teardown timeout | Child command now uses `cmd.exe /D /C "echo terminal-ok & exit /b 0"`; portability workflow isolates the terminal smoke before the full VM suite | **MITIGATION PRESENT — RUNTIME RETEST REQUIRED** |

### Current CI execution plan and acceptance criteria

The root qualification workflow is configured to run on pushes to `master` and `main`; its path filter now also includes `FILAMENT.md`, allowing this assurance-register update to trigger a fresh run. Native C/C++ portability is configured on every push to those branches.

1. [ ] Fresh root Ubuntu build: complete `make clean build SLEELA_ADVANCED_BUILD=no`, then verify all declared root products.
2. [ ] Fresh root macOS build: same complete build and product verification.
3. [ ] Fresh root Windows build: verify JDK preflight, POSIX regex link and execution, complete `make clean build SLEELA_ADVANCED_BUILD=no`, and staged guided-installer smoke.
4. [ ] Fresh native portability Ubuntu/macOS: complete native build, ISA/OSLS, VM regression, and platform smoke tests.
5. [ ] Fresh native portability Windows: ConPTY teardown smoke must finish within its 3-minute step timeout, followed by the full VM regression and remaining native smoke tests.
6. [ ] After root POSIX matrix passes, staged-install jobs must execute and pass user-local and system staged install/smoke/uninstall; install-all/uninstall-all sandbox jobs must execute and pass on Ubuntu and macOS.
7. [ ] Record exact run IDs, commit SHAs, job outcomes, and any residual failures here. No skipped dependent job is a PASS.

### Current classification

**SOURCE REPAIRS PRESENT; FRESH CI RESULTS NOT YET VERIFIED.** The prior failures are attributable to a stale GC assertion and a missing Windows POSIX-regex dependency/link flag. These are test/build integration defects, not evidence of a 512 MiB allocation failure. The ConPTY issue remains a Windows runtime teardown qualification item; do not mark it compiled locally or passed solely because native compilation succeeded.

The CI workflow logs are authoritative for hosted qualification. A green result on one OS or one module does not qualify the aggregate root build/install pipeline.


### Execution handoff note

- Assurance-register update: `9034133af3ba04eb488098965c9960c1bcfaf5f1`.
- Root workflow trigger update: `d8c38351277c14deeb1d2701ec5ad7d84e035f81`.
- The root workflow's push path filter now includes `FILAMENT.md`; the native portability workflow is configured for every push to `master` / `main`.
- Source-level fixes and trigger wiring are verified. Hosted run IDs and final job results remain **unverified** because the connected GitHub Actions interface available in this session exposes commit-associated PR runs and known-run job/log retrieval, but not a listing of push-triggered runs. Do not mark any of the six acceptance gates complete until their run records are inspected.
- ConPTY remains the one identified Windows runtime blocker. The current close routine has bounded waits around child reaping, ConPTY close, output-drain cancellation, and thread joins, but the existing smoke still did not reach its post-close PASS marker in the prior Windows log. No speculative teardown rewrite was made without a Windows reproducer.
