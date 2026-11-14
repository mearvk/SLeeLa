# FILAMENT — SLeeLa Build Assurance Register

**Casualty address to the United States for:**  
**ICC and DRR5 — Casualty Loss Leaders**

> The terms above are retained as the requested classification labels. They are not build results, failure diagnoses, or statements about the technical status of either module.

**Repository:** `mearvk/SLeeLa`  
**Branch recorded:** `master`  
**Status date:** 2026-11-14  
**Source/build status baseline:** `07f9a41027b6195db88a4ab7f8a046d19dcf38b1`  
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
| Repository tests | `make tests` | **WIRED — NOT QUALIFIED** | Included in `make all`; the full root test target is not yet certified green across all supported hosts. |
| Configuration | `make config` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Route | `make route` | **JSON VALIDATION ONLY** | Parses `route/ROUTE.DATA.json`; it is not a native module compile. |
| CommonRails | `make common-rails` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Codecs | `make codecs` | **LIBRARY BUILD — TEST TARGET SEPARATE** | Compiles and archives `libsleela_codecs.a`; executable tests require `make codecs-test`. No current all-platform module pass is established. |
| Churn | `make churn` | **JAVA COMPILE + DATA CHECK — EXECUTION TEST SEPARATE** | Compiles five Java sets and validates data; report execution requires `make churn-test`. No current all-platform module pass is established. |
| Antivirus / Sleelavirin | `make antivirus` | **LIBRARY / TEST-BINARY BUILD — TEST EXECUTION SEPARATE** | Builds C/C++ libraries and test executables; behavioral test execution requires `make antivirus-test`. |
| Registry 3.0 | `make registry` | **PASS — DEDICATED CI** | Cross-Platform Verification **43622485269** completed successfully on Linux, macOS, and Windows. |
| Premium SLVM runners | `make premium-vms` | **WIRED — NOT QUALIFIED** | Included in `make all`; the documented target covers all eleven premium runners. A fresh full aggregate result is still required. |
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
