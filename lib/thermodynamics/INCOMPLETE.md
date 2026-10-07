# INCOMPLETE.md — Thermodynamics IV

This module is complete and tested, with one documented exception recorded here
honestly so the gap is visible rather than hidden.

## What IS complete and verified

- **C ABI** (`src/thermodynamics.c`, `thermo_stochastic.c`, `thermo_slots.c`)
  and **C++17 orchestration** (`src/thermodynamics.cpp`).
- **Physics:** scalar laws (sensible heat, Fourier, Newton cooling, Carnot);
  statistical mechanics (Boltzmann partition function, probabilities, mean
  energy); 3D heat-equation PDE (gradient / divergence / Laplacian + explicit
  FTCS); stochastic/Langevin step + Monte Carlo over futures; Arrhenius.
- **Enforced slot caps** (futures <= 22, positive gains <= 6, long-term
  confidences <= 2), per `MATH.KNOWNS.md`.
- **Curriculum** (`curriculum/`): the novice-to-advanced equation ladder plus
  the course sequence (`curriculum/courses/`: Thermodynamics I/II, Statistical
  Mechanics, Advanced/Chemical) as SLeeLa classes, with runnable
  `SLThermodynamicsIVDemo.sleela` and `courses/SLCoursesDemo.sleela` programs.
- **Build & tests:** `make all` compiles cleanly under
  `-Wall -Wextra -Wpedantic` and runs 55 self-test assertions, all passing.

## What is INCOMPLETE

### 1. The `.sleela` files are not compiled by the live SLeeLa compiler

**Status:** blocked (not by this module), intentionally left undone.

The `.sleela` sources in `curriculum/` and `curriculum/courses/` are:
- validated against the grammar in `SLEELA.syntax` (balanced braces/parens,
  `#sleela 1.3` header, `void main()` where runnable), and
- numerically verified: the equations they encode are mirrored in the C
  self-tests (`src/*_demo.c`), which assert them against known textbook values.

They have **not** been run through the actual Sleelvac compiler. Two obstacles:

1. **Inventory timeout.** `tools/sleela-build.py compile` runs a full
   ~10,000-file library inventory before parsing, which exceeds the sandbox
   time limit.
2. **The native compiler will not build (security gate).** Building Sleelvac
   from `impl/` is fail-closed on a SHA-256 integrity check
   (`impl` target `verify-security`, `tools/verify-before-execution.py`,
   manifest `security/sha256-manifest.json`). At the time of writing it fails
   with 6 mismatches against the trusted manifest:
   - `impl/core/sleela_core.c`
   - `impl/core/sleela_core.h`
   - `impl/frontend/compiler.cpp`
   - `impl/frontend/driver.cpp`
   - `impl/frontend/semantic.cpp`
   - `impl/nordshrift/nordshrift.cpp`

**Why it was left undone (deliberately).** The only ways to force the build
past this gate are to regenerate the manifest to match the current files,
disable/neuter the `verify-security` step, or hand-edit those six files to
restore their expected hashes. Each defeats a supply-chain integrity control
rather than resolving what it is flagging, so it was not done. The mismatch is
a pre-existing condition in `impl/` and is unrelated to this thermodynamics
module; it should be investigated by someone who can confirm the correct,
trusted contents of those core/frontend files and update the manifest through
the proper signing process.

**How to complete it, once the compiler builds cleanly:**
```sh
# from the repo root, after the impl/ SHA-256 mismatch is resolved legitimately
python3 tools/sleela-build.py build          # builds Sleelvac
# then, bypassing the slow inventory, invoke the binary directly:
impl/build/sleela compile \
  lib/thermodynamics/curriculum/SLHeatLinear.sleela -o /tmp/out
# repeat for each curriculum/ and curriculum/courses/ *.sleela file
```
If any file fails to parse under the real compiler, fix it and re-run the C
self-tests to confirm the math is unchanged.

## Impact

None on the C/C++ library, which is fully built, tested, and the authoritative
implementation. The gap is limited to not having an independent
compiler-confirmation of the SLeeLa surface files; their grammar is statically
checked and their math is executable-verified via the C self-tests.

---
_Max Rupplin - MEARVK LLC - 2026_
