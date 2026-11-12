# Assurance

Part of [Trading Actions on Verb](./README.md).

Six independent layers:

1. **Self-checks.** Every use case prints `ok` or `FAIL` per check and ends with
   `checks passed=N failed=0`.
2. **Independent oracle.** Expected values for use cases 1 to 13, 16 to 20 and 23 to 26 were computed
   by a separate Python model of the same rules and compared with what SLeeLa
   printed; one wrong expectation of mine (use case 11) was found and fixed that way.
3. **Differential testing.** Use case 14 runs 1,200 random verbs (four fixed
   seeds, one buy-heavy so the 16-lot capacity is reached) against a shadow
   model that stores one entry per share and shares no code with the lot-based
   book, and the outcome tallies were also reproduced in Python. Use case 15
   checks every Euclid routine against a slower method over all small cases.
4. **Mutation checks.** The short-side stress cases were run against three
   deliberately broken copies of `SLShortBook` (borrow off by one day, sale
   proceeds off by one cent with a flipped profit sign, a cover allowed one share
   too many); each was caught (8, 8 and 13 failed checks) and the original
   passes.
5. **Mutation checks on the margin class.** Off-by-one in the ceiling and in
   the first call day were caught (2 and 4 to 5 failed checks). A binary search
   whose midpoint rounds up never terminates; that mutation was caught as a hang,
   so the gate runs every case under `timeout` (default 300 seconds) and fails
   instead of waiting forever.
6. **Mutation checks and a worst-case corner sweep on the batch class.** An
   unsafe division in `idealBatchReturnBp` (found during a general code-quality
   review, not by a use case) was confirmed by reverting the fix and watching
   the regression test fail with the exact overflow symptom; a 500-sample
   random sweep across the full valid input space, independently checked
   against a Python model, found no other overflow.
7. **Expected-output gate.** `test-suites/test-trading-actions.sh` runs all
   twenty-seven, compares each with its `.expected.txt`, and fails on any `FAIL` line.

```
bash test-suites/test-trading-actions.sh
```

Set `UPDATE=1` to rewrite the expected files after reviewing a deliberate change.

Limits of the assurance: the stress cases do not generate code 8 (size bounds) or
code 9, which use cases 4 and 18 cover by hand; the margin model ignores fees and
price paths and is an illustration, not a broker or regulator rule; and the VM comparison behaviour above 2^53 was
measured on the committed `working-binaries/sleela`, not proven from its source.

## Review log (second look)

| finding | severity | fix |
|---------|----------|-----|
| `isqrt` assumed a 32-bit word and returned 46340 for large inputs | wrong answer | bounds rebuilt for the measured 64-bit arithmetic |
| VM comparisons are inexact above 2^53; `isqrt` returned 3037000499 for 3037000499^2 - 1 | wrong answer | every compared value bounded by `maxExact`; out-of-range refused; steering note added |
| no size bounds on quantity, price, fee, day or running totals | silent overflow risk | new code 8 and bounds |
| time queries accepted a day before the book day | negative ages | -1 answer |
| `canFill` scanned up to c/a times | cost grows with order size | O(log) closed form; scan kept as the reference |
| use case 4 compared a hash of the book | collision could hide a change | five values compared separately |
| `cap` was a mutable field beside fixed 16-slot arrays | out-of-bounds risk | a `maxLots()` getter was added |
| `realizedNet` meaning was implicit | misreading | documented as a cash view |
| README said "no sum can pass 2,000,000,000" | false claim | corrected |

## Review log (general code-quality pass)

A later pass checked the actual code (not just that outputs matched their
saved expectations) across every class built in this folder and its
courseware. Findings:

| finding | severity | fix |
|---------|----------|-----|
| `idealBatchProfit`'s rejection sentinel (`-(maxCapital + 1)`) could equal a real, legitimate large-batch loss, so the real loss would be misread as a refusal | **wrong answer, silently** | explicit `idealBatchInRange()` check, called before reading a profit, never inferred from its magnitude |
| `idealBatchReturnBp` formed `(-profit) * 10000` directly; at the corrected bound this still overflowed the exact-comparison limit, returning a wrong basis-point value (`-776` instead of `-10000` in the worst-case test) | **wrong answer, silently** | `maxTrades`, `maxCapital` and `maxBp` re-derived together against the single tightest product, `maxTrades * maxCapital * 10000 <= 2^53 - 1`, documented at the bound declarations |
| the explicit per-trade batch path (`startExplicitBatch`) had no bound at all on its capital argument, so the same overflow was reachable there too, just unnoticed because no test used a large enough capital | **latent wrong answer** | added `maxExplicitCapital`, derived for its own 64-trade cap; `startExplicitBatch` now returns `false` and resets state on an out-of-range input |
| `reason()` in `SLBatchReturn` documented five status codes (0-5) that no method in the class actually returned, and was never called from anywhere | dead/misleading code | removed; replaced with `addTradeReason()`, documenting only `addTrade`'s three real codes |
| `maxLots()` existed as a getter in `SLLongBook` and `SLShortBook` (added in the review above) but was never actually called; the `16`-lot cap was duplicated as a raw literal in 2-3 places per class across four classes, and `SLDailyCapBook` (courseware) did not even have the getter | duplication / inconsistency | `maxLots()` is now the single source of truth, called from every cap check and `invariantsHold()`, in all four long-only/short-only book classes |
| a session learning claimed an apostrophe inside an angle-bracket comment is a lexical error on this toolchain | **stale documentation, not a code bug** | re-tested directly against the committed `working-binaries/sleela` (1.3.2-pro): apostrophes in comments, including the exact example from the learning, check and run cleanly; the claim does not reproduce on the current build and should not be treated as current |

The batch-return bugs were caught by writing new boundary-case regression
tests (the class's own worst-case corner: maximum trades at maximum capital)
rather than by the use case's original hand-picked examples, which never
exercised a value large enough to overflow. A 500-sample random sweep across
the full valid input space, independently checked against a Python model,
found no further overflow after the fix. Both fixes were confirmed genuine
by reverting them and watching the regression tests fail with the original
symptom.
