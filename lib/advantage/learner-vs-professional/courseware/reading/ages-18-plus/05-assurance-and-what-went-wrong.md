# Assurance and What Went Wrong

The library is checked in six independent ways (details in `ASSURANCE.md`):

1. **Self-checks**: every use case prints `ok` or `FAIL` per check and ends
   `failed=0`.
2. **Independent oracle**: expected numbers are computed by a separate Python
   model.
3. **Differential testing**: random verbs run against a one-entry-per-share
   shadow model that shares no code with the lot-based book.
4. **Mutation checks**: deliberately broken copies must be caught.
5. **Margin mutation checks**: off-by-one errors were caught; a never-ending
   search was caught as a hang, so the runner has a timeout.
6. **Expected-output gate**: every case is compared with its saved output.

## What the second review found

| finding | effect | fix |
|---------|--------|-----|
| a square root assumed a 32-bit word | wrong answers for large inputs | bounds redone for the real 64-bit arithmetic |
| comparisons inexact above 2^53 - 1 (measured) | a silent wrong answer near the top of the range | every compared value bounded; out-of-range input refused |
| no size bounds on quantity, price, fee, day, totals | silent overflow risk | status 8 and explicit bounds |
| time queries before the book day | negative ages | answer -1 |
| exact fill scanned c / a times | cost grew with order size | O(log) closed form, scan kept as the reference |
| a hash used to compare book states | a collision could hide a change | five values compared separately |
| a README claim that no sum could pass 2 000 000 000 | false statement | corrected |

## What this shows

A confident first version contained real errors. They were found by checking the
same facts several independent ways, measuring the runtime instead of assuming it,
and writing down both the findings and the limits of the checks. The limits are
stated too: the stress tests do not generate every status code, and the runtime
comparison behaviour was measured on one binary, not proven from its source.

## Questions

1. Why is a shadow model stronger than a second reading of the same code?
2. Why keep the slow reference implementation after the fast one is written?

**Answers:** 1. It is independent, so a shared mistake is unlikely. 2. It is the
oracle the fast version is tested against, now and after every future change.
