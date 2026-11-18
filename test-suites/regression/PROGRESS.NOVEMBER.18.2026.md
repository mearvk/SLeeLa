# Regression and converter validation: session progress (18 November 2026)

**Repository:** `mearvk/SLeeLa`  
**Master baseline:** `c00fd2f3ed760a2a840cecd02a02daec032b6453`  
**Session status:** documentation updated; execution gates remain pending  
**Production decision:** HOLD

## Session summary

This session followed the Java signature-index and JDK 28 campaign merge. It reviewed the current regression records, converter status, Java specification refusal list, and production release qualification report, then updated the regression documentation so historical counts are not mistaken for current validation.

### Completed repository changes

- PR #375 was already merged to master before this session. It contains the signature-index, generic metadata, inherited implementation, constructor compatibility and JDK 28 campaign scaffolding.
- A production release qualification report was prepared separately in PR #376. At the time of this update PR #376 remained open and was not assumed to be part of master.
- Updated root `REGRESSION.md` with the November 18 Java converter/JDK 28 session, evidence status, refusal categories and next regression sequence.
- Updated this regression suite README to link the session status and continuation note.
- Updated `docs/reference/JAVA.CONVERSION.STATUS.md` with current master baseline, merged work, current unrun gates, historical measurement caveats and release hold.
- This progress record and the session continuation note capture what was done and how to resume.

## Evidence ledger

| Area | Latest evidence | November 18 status |
|---|---|---|
| Language-wide regression | 211 tests, 0 failures, 206 s (2026-11-17) | Historical; not re-run |
| Fast regression | 198 tests, 0 failures, 113 s (2026-11-17) | Historical; not re-run |
| ASan/UBSan native suites | 17 tests, 0 failures (2026-11-17) | Historical; not re-run |
| Mutation testing | 85 injected defects detected (2026-11-17) | Historical; not re-run |
| JDK 25 Java differential | 32/32 checks, 22 positive, 9 refusals, stale-output check (2026-11-13) | Historical; not current model validation |
| JDK 25 selected whole-tree conversion | 420/884; 426/884 with `--single-thread` (2026-11-13) | Historical; not compile or execution coverage |
| Current master runtime build | No current log | NOT RUN |
| Current C++ converter build/self-check | No current log | NOT RUN |
| Current Java positive/refusal differential suite | No current log | NOT RUN |
| SLeeLa-native per-member stub and repeat-run tests | No current log | NOT RUN |
| JDK 28 campaign artifacts | No completed artifact inspected | PENDING |
| JDK 28 functional equivalence | Stubs/envelopes remain nonfunctional contracts | PARTIAL |
| Hosted status for PR #375 merge commit | No workflow runs/statuses exposed at inspection | UNVERIFIED |

No historical result is promoted to a November 18 pass.

## Open refusal and behavior categories

The Java specification still lists generic target inference, wildcard capture, intersection LUB, generic constructors, unknown functional-interface targets, nested capture chains, local classes, qualified inner allocation, switch type/record patterns, sealed exhaustiveness, static initializer blocks, unsupported inherited Object methods, boxed null and wrapper arrays, multithreaded synchronization/volatile semantics, and native methods without a verified ABI binding.

The C++ converter refuses a whole file if a required construct cannot be lowered. The SLeeLa-native converter preserves supported siblings and emits fail-loud per-member stubs for unsupported members. Keep refusal counts, stub counts and implemented behavior as separate metrics.

## Next steps

1. Check out exact `master` and record `git rev-parse HEAD`; verify it equals the intended release candidate.
2. Regenerate the checkout-local SHA-256 manifest and important-file manifest; run integrity verification.
3. Build the SLeeLa runtime and C++ converter; retain full stdout/stderr and exit status.
4. Run converter self-check and generated signature/result-type row checks.
5. Run every positive differential fixture and every refusal fixture. Save the exact refusal diagnostic and prove no stale output is published.
6. Run the SLeeLa-native per-member stub-policy test and transactional repeat-run/provenance tests.
7. Execute representative converted Java programs and compare Java reference output with SLeeLa output byte-for-byte.
8. Run the JDK 28 reflection, signature-index, cross-version API, conformance and functional-equivalence campaign. Archive all artifacts.
9. Run the general regression suite, sanitizers and native portability matrix. Report Linux, Windows 10+ and macOS separately.
10. Update counts only from raw current-run artifacts. Do not mark production ready while any required gate is pending or any placeholder is counted as behavior.

## Session boundary

Repository documentation and pull-request records were updated using the GitHub repository interface. This session did not have a local repository checkout or shell execution path. No compiler build, regression command, converter run or representative program execution was performed in this session.
