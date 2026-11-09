# Advantage Library Verification Report

**Recorded:** 2026-11-09  
**Verification:** Advantage Library Verification (ALV), run #395  
**Status:** PASS  
**Repository:** `mearvk/SLeeLa`  
**Branch:** `ci/math-synthesis-smoke-benchmark`  
**Pull request:** [#350 — Add native math synthesis CI and speed report](https://github.com/mearvk/SLeeLa/pull/350)  
**Verified commit:** `53555964abba674a337c2f2f0e9b1e514b74a13f`

## Executive result

ALV run #395 completed successfully. The source-authoritative runtime build, integrity verification, Advantage documentation checks, generated API documentation, all six runnable demonstrations, report publication, and artifact upload passed.

- **Workflow run ID:** `42680582114`
- **Workflow run:** [Advantage Library Verification #395](https://github.com/mearvk/SLeeLa/actions/runs/42680582114)
- **Conclusion:** `success`
- **Integrity manifest:** PASS — 12,499 items verified
- **Runtime build:** PASS
- **Documentation checks:** PASS
- **Generated Advantage API documentation:** PASS
- **Runnable demonstrations:** 6/6 PASS
- **Artifact:** `advantage-verification-42680582114`
- **Artifact ID:** `13701708704` (592,384 bytes; not expired at time of verification)

## Demonstration results

| Demonstration | Result |
|---|---|
| `lib/advantage/advantage-demo.sleela` | PASS |
| `lib/advantage/math/trade-math-demo.sleela` | PASS |
| `lib/advantage/physics/physics-demo.sleela` | PASS |
| `lib/advantage/integration/integration-demo.sleela` | PASS |
| `lib/advantage/grading/grading-demo.sleela` | PASS |
| `lib/advantage/output/output-demo.sleela` | PASS |

Each demonstration produced the required `ALL CHECKS PASSED` sentinel.

## Verification coverage

The successful workflow included:

1. Checkout and installation of native compiler dependencies.
2. Generation and review of a source-derived SHA-256 integrity manifest candidate.
3. Confirmation that the approved manifest matched the candidate.
4. Build of the source-authoritative SLeeLa runtime.
5. Verification of Advantage documentary artifacts.
6. Generation and verification of Advantage API documentation.
7. Execution of all six Advantage demonstrations.
8. Publication of the verification report.
9. Upload of verification logs and report artifacts.

The manual documentation audit also confirmed that `PREONEM.md`, `DEFINITIONS.md`, `MATH.md`, `PHYSICS.md`, and `EXAMPLES.md` are present with the required `artifact;` marker; the required PREONEM references are present; the README links to `EXAMPLES.md`; and the examples guide identifies all six runnable demonstration paths.

## Repairs completed on the verification branch

The following commits addressed issues found during the initial verification cycle:

- `65ee1ef474c0fb877a04947bbe0e19c1ef5168c6` — include `<stddef.h>` for `NULL` in the C loader rejection test.
- `2ce1bb5770eaf3816a266e071706b3e57f8f7ec7` — use the typed loader fixture in the C++ loader test.
- `afc3028e8909d38080540e071c304c310aba7977` — correct ALV report shell quoting and retain the candidate manifest artifact.
- `3933cca807a8b5d5b4ee41397901f62cfac95e09` — add the candidate-manifest commit and clean-verification workflow step.
- `003383be61b22c01d6e632ef6a55c0b5256f2f21` — correct candidate comparison using `cmp -s`.
- `5c535ca065266e6f25c87508f548c64d2149a433` — check out the actual PR head branch before committing a refreshed manifest from a pull-request workflow.
- `ea220cf30c82134eea75ced6fe79ab4e52b1d1b1` — refresh the source-derived SHA-256 integrity baseline (12,499 items).
- `53555964abba674a337c2f2f0e9b1e514b74a13f` — link the runnable examples guide from `lib/advantage/README.md`.

## Initial failure and resolution

The earlier ALV run #388 (`42663149625`) stopped because the checked-in integrity manifest was stale: 14 source hashes differed and 13 entries were missing. The demonstrations were not reached in that run; they were unverified, not failed.

The manifest-candidate comparison and branch checkout/push handling also required correction. Those workflow issues were repaired, the approved baseline was refreshed, and the clean run #395 then passed all verification stages.

## Repository state and disposition

- PR #350 remains **OPEN** against `master`.
- The verified head is `53555964abba674a337c2f2f0e9b1e514b74a13f`.
- No merge to `master` and no push to `master` was performed as part of this verification.
- The ALV work covered by this report is complete and passing.

**Final disposition: ADVANTAGE LIBRARY VERIFICATION PASSED.**
