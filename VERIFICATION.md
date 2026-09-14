# VERIFICATION

A point-in-time verification record for this repository: who authored it, what
it is built from, the repository identity, how complete it is, the work
invested, and its connections to other systems.

**Generated:** 2026-09-14 · **Branch of record:** `master` (default) / `main`

---

## Author

- **Attention / owner:** Max Rupplin — MEARVK LLC — 2026.
- **Authorship standard:** new code authored for this repository is the work of
  the owner; vendored third-party trees retain their upstream authorship and
  license.
- **This record + recent integration work** (HTTP 3.0 relocation, subject-library
  unification, Java 28 SecureJDK memory link) was produced by the **Kiro** AI
  software agent working with the owner, committed under the owner's account.

## Source

- **Primary language(s):** C/C++ execution core and front end; Java connectors
  (`connector/`, `gui/`, `rmi/`); Python data tooling (`tools/`).
- **Own source:** the Sleela compiler/runtime (`impl/`), Nordshrift `.sst`
  driver, subject libraries under `impl/subjects/` (math, physics, economics,
  chemistry, finance), HTTP 3.0 layer (`http-3.0/` + `http/spec/`), and the
  Java 28 SecureJDK memory integration (`java28/`).
- **Vendored source:** `bash/` is a full GNU Bash tree (upstream
  `git.savannah.gnu.org/git/bash.git`); it is reference/vendored code, not
  project-authored. New bash-related work belongs in `terminal/` (see the
  steering rule in `.kiro/steering/`).

## Repo

- **Repository:** `mearvk/SLeeLa` (GitHub).
- **Default branch:** `master`; a parallel `main` branch is kept in sync.
- **Scale:** ~2,000+ tracked files across the compiler, specs, subject
  libraries, HTTP layer, Java integration, and vendored bash.
- **CI:** GitHub Actions under `.github/workflows/` (HTTP 3.0 crypto build/self-test
  plus data-pipeline jobs).

## Completeness

| Area | State |
|---|---|
| Sleela core + front end (`impl/`) | Builds (`build/sleela`); examples run. |
| Nordshrift `.sst` driver | Builds (`build/nordshrift`); example sheets check/build. |
| Subject libraries (`impl/subjects/`) | Unified layout; math/physics/economics/chemistry/finance build and run; outputs verified. |
| HTTP 3.0 (`http-3.0/`) | Crypto self-test **PASS**; spec set under `http/spec/`. |
| Java 28 memory integration (`java28/`) | Port + JNI channels build and demo **PASS** (identical output). |
| Verified caveat | `impl` `test-network` (TCP loopback smoke) fails in the restricted sandbox — pre-existing/environmental, not a code defect. |

Overall: the actively-developed components **build and pass their checks**;
this is a research repository under continuing development.

## Time Spent

Honest qualitative accounting (wall-clock not precisely measured):

- HTTP 3.0 source relocation + CI/README fixups — small.
- Fixing pre-existing `impl` build breakage to reach a green baseline — moderate.
- Unifying the five subject libraries into `impl/subjects/` (with a shared native
  dispatcher and per-subject split) — moderate–large.
- Designing + building the Java 28 SecureJDK memory link (spec, Java memory
  server, port + JNI channels, demos, build) — large.
- Wiki authoring and this verification record — small.

## Outward Connections

- **Submodule:** `terminal` → GNU Bash (`https://git.savannah.gnu.org/git/bash.git`)
  — declared in `.gitmodules` (currently an uninitialized pointer).
- **Sibling project:** `mearvk/Ubuntu.Determinant.Beta.Restricted` — referenced
  from `README.md`; shares the **SecureJDK 28** vocabulary. This repo implements
  the Sleela-side Java 28 SecureJDK memory link; that repo is the systems/OS side.
- **External toolchains:** OpenSSL (HTTP 3.0 crypto), a JDK (`javac`/JNI for
  `java28/`), standard C/C++ toolchain.
- **Runtime interfaces:** HTTP 3.0 (network transport), Sleela RMI, and the
  Java 28 memory link over a TCP **port** and in-process **JNI** channel.

---

*This VERIFICATION.md is a descriptive project record. It does not assert any
external legal, certification, or institutional status; such status must be
established from the appropriate primary authority. Per project discipline, the
existence of a record is not itself proof of a claim.*
