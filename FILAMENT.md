# FILAMENT — SLeeLa Build Assurance Register

**Casualty address to the United States for:**  
**ICC and DRR5 — Casualty Loss Leaders**

> The terms above are retained as the requested classification labels. They are not build results, failure diagnoses, or statements about the technical status of either module.

**Repository:** `mearvk/SLeeLa`  
**Branch recorded:** `master`  
**Status date:** 2026-11-14  
**Latest inspected commit:** `07f9a41027b6195db88a4ab7f8a046d19dcf38b1`

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
| Decompiler | `make decompiler` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| VM | `make vm` | **PASS — CONFORMANCE WORKFLOW** | Complete SLeeLa VM and Language Conformance **43619118411** and VM Arithmetic Table Completion **43619118403** passed on commit `d5034d8f`. This is dedicated conformance evidence, not a fresh aggregate build. |
| Scripting | `make scripting` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| JetBrains integration | `make jetbrains` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Installer checks | `make install-check` | **WIRED — NOT QUALIFIED** | Included in `make all`; the complete install qualification still needs fresh Linux, macOS, and Windows evidence. |
| Tutorial validation | `make tutorial-check` | **WIRED — NOT QUALIFIED** | Included in `make all`; no fresh aggregate result identified. |
| Repository tests | `make tests` | **WIRED — NOT QUALIFIED** | Included in `make all`; the full root test target is not yet certified green across all supported hosts. |
| Configuration | `make config` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Route | `make route` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| CommonRails | `make common-rails` | **WIRED — NOT QUALIFIED** | Included in `make all`; no dedicated current passing result identified. |
| Codecs | `make codecs` | **WIRED — NOT QUALIFIED** | Included in `make all`; Windows executable suffix handling was repaired, but a fresh all-platform module pass is not established here. |
| Churn | `make churn` | **WIRED — NOT QUALIFIED** | Included in `make all`; Python launcher handling was repaired, but a fresh all-platform module pass is not established here. |
| Antivirus / Sleelavirin | `make antivirus` | **WIRED — NOT QUALIFIED** | Included in `make all`; portable build and test entry points exist, but no current dedicated passing run was identified. |
| Registry 3.0 | `make registry` | **PASS — DEDICATED CI** | Cross-Platform Verification **43622485269** completed successfully on Linux, macOS, and Windows. |
| Premium SLVM runners | `make premium-vms` | **WIRED — NOT QUALIFIED** | Included in `make all`; the documented target covers all eleven premium runners. A fresh full aggregate result is still required. |
| Ballum native display library | `make ballum` | **PARTIAL / HOST-DEPENDENT** | Root target attempts the native build when host display dependencies are present and otherwise falls back to portable tests. The openSUSE package list now includes `libXext-devel`; the latest Native C/C++ Portability run is still in progress. No openSUSE package-install/build run is claimed. |

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
