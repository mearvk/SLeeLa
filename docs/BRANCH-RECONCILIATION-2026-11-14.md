# SLeeLa branch reconciliation

Audit date: 2026-11-14 (GitHub timestamps may display as 2026-11-15 UTC)  
Repository: `mearvk/SLeeLa`  
Integration destination: `master`

## Policy

- `master` is the sole integration destination and the repository default branch.
- Feature, fix, work, test, and documentation branches are temporary development references. Their changes must return through pull requests targeting `master`.
- Do not merge a branch solely because GitHub reports it as mergeable.
- Do not force-push, delete, or bulk-merge historical branches as part of routine cleanup.
- Preserve dependency order for stacked pull requests.
- Retire a branch only after confirming its unique commits are integrated or intentionally abandoned.

## Repository-level findings

- The default branch is `master`.
- `main` is divergent: 22 commits ahead and 749 behind `master` at the time of comparison. Do not merge `main` wholesale.
- `integrate/merge-unmerged-branches` is divergent: 122 commits ahead and 3,176 behind `master`, with 57 changed files in the compare response. Reconcile its unique changes individually; do not merge wholesale.
- The branch search returned 76 refs in its first page and no additional refs on the next page.

## Open pull requests reviewed

| PR | Topic | Base | Head | Comparison / disposition |
|---|---|---|---|---|
| #364 | Java-to-SLeeLa converter mapping | master | work/java-converter-mapping-pass-01 | 26 commits ahead at initial comparison; draft; CI pending/cancelled; do not merge yet |
| #356 | VM-owned Native IPC worker handles | native-ipc-bytecode-isa-2026-11-09 | native-ipc-vm-handle-table-2026-11-09 | Stacked PR; do not retarget blindly |
| #354 | Native IPC stdio stream adapter | master | native-ipc-stream-adapter-2026-11-09 | 9 commits ahead, divergent; draft; inspect conflict and tests |
| #350 | Native math synthesis CI | master | ci/math-synthesis-smoke-benchmark | 16 commits ahead, divergent; inspect CI behavior and test evidence |
| #348 | Native linear algebra | master | feat/native-linear-algebra-foundation | 140 commits ahead, divergent; draft; review scope and dependencies |
| #347 | Native math VM dispatch | work/native-math-kernel-sleelactl | work/vm-native-math-dispatch | Stacked on #346; preserve dependency order |
| #346 | Native math kernel | master | work/native-math-kernel-sleelactl | 10 commits ahead, divergent; inspect conflict and tests |
| #345 | VM exception source regressions | master | work/vm-exception-source-regressions | 4 commits ahead, divergent; inspect conflict and tests |
| #340 | Java UTF-16 String facade search | master | work/java-string-api-completion | 4 commits ahead, divergent; inspect conflict and tests |
| #303 | Recursion/syntax documentation | master | docs/recursion-contract-audit-2026-10 | 11 commits ahead, divergent; inspect docs against current contracts |
| #278 | Historical branch integration | master | integrate/merge-unmerged-branches | 122 commits ahead, divergent; selective recovery only |

## Converter PR #364 qualification gate

At the time of this audit, PR #364 head was
`cfb2c1b141007a25d62455a3e8a5d3b9838abde0`, targeting master at
`0bc4f270ae3bb50a11b4c19322e072da423d834d`.

The associated check-runs response contained 18 checks: 16 queued and 2
cancelled, with no completed successful qualification result. The patch itself
records remaining gaps in transactional output publication, cryptographic
source/model provenance, exact JDK 28 source identity, and parity of the
SLeeLa-native converter's idempotence behavior. New fixtures were explicitly
not yet verified in the patch documentation.

Therefore #364 remains a draft and must not be merged until the code is
reviewed, both converters pass their relevant tests, and required CI checks
complete successfully.

## Branches with previously merged PRs

The recent PR history records successful merges to master for build portability
(#363), native method AOT gating/emission/metadata (#359, #358, #357), Native
IPC bytecode foundation (#355), Windows clean-install layout (#349), VM
exception foundation (#344, #341), Java UTF-16 integration (#339, #335, #333),
native callback guard (#332), Native Drive (#330), UTF-4088 tooling (#322),
library inventory (#315), Slecompiler API paths (#314), Java launcher fixes
(#311, #309, #308, #306), runtime VM hardening (#304), UI themes (#302), and
browser tabs/events (#300).

These merged PRs are evidence of integration history, not by themselves proof
that every similarly named branch is safe to delete.

## Required completion sequence

1. Recompare every non-master branch with the current master tip; record ahead,
   behind, divergence, unique files, and any associated PR.
2. For each open PR, inspect the complete patch, base/head relationship,
   dependencies, and CI results.
3. Integrate independent, reviewed changes through PRs targeting master.
4. Integrate stacked work only after its prerequisite has landed.
5. For stale branches, verify their unique commits are already in master before
   proposing deletion; preserve branches with unresolved unique work.
6. Verify the repository default branch remains master and ensure all new PRs
   target master.
7. Re-run root build, platform builds, converter differential tests, and
   affected module tests after integration. Record exact commits and outcomes.

This document is an audit snapshot, not a claim that all 76 branches have been
individually source-reviewed or that all open PRs are ready to merge.
