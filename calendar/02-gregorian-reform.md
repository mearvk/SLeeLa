# 02 — The Gregorian reform (the principal exception)

*Promulgated by Pope Gregory XIII in the papal bull* Inter gravissimas*, 24
February 1582. This is the one **large** structural exception to the Julian
baseline — it gets its own document because everything in 03–05 is the staggered
adoption of* this *reform.*

## The two corrections

The Gregorian reform keeps the entire Julian frame (365-day year, 12 months,
leap day in February) and changes exactly two things.

### 1. A refined leap rule

```
A Gregorian year is a leap year  ⇔
    (year mod 4 == 0)  AND  NOT (year mod 100 == 0  AND  year mod 400 != 0)

equivalently: divisible by 4, except centuries, except centuries divisible by 400.
```

- Years divisible by 100 but **not** by 400 (1700, 1800, 1900, 2100) are made
  **common** years, dropping 3 leap days every 400 years.
- Average year length = **365.2425 days**, versus Julian's 365.25 and the true
  tropical year's ≈365.2422. The residual error is only about **1 day in 3,030
  years** — two orders of magnitude better than Julian.

### 2. A one-time re-anchoring

To undo the ~10 days of Julian drift that had already accumulated since the
Council of Nicaea (325 CE), **ten days were deleted once**: in the adopting
countries, **Thursday 4 October 1582 was followed immediately by Friday 15
October 1582**. The weekday cycle was never broken — only the date labels
jumped.

This restored the vernal equinox to around 21 March, the date assumed by the
Easter computation.

## Why it is "the exception," singular

Measured against the Julian baseline (document 01), the Gregorian reform is the
*only* structural change in this entire history. Everything that follows is not a
new idea but the **same reform arriving at different times**:

- Where a country adopted **determines the size of its day-skip**, because the
  skip equals the Julian drift accumulated up to that date:
  - **10 days** in 1582 (Portugal, Spain, Papal States, Poland-Lithuania),
  - **11 days** by 1752 (British Empire and American colonies),
  - **12 days** in the 1800s,
  - **13 days** by 1918 (Russia) and 1923 (Greece).
- This is why the national threads (03–05) are *adoptions*, not *reforms*: they
  share one rule and differ only in date and day-count.

## Old Style / New Style (O.S. / N.S.)

During the long adoption gap, the same instant had two date labels — **Old
Style** (Julian) and **New Style** (Gregorian). Historians annotate dates in the
transition window accordingly (e.g. documents 04 and 06 note O.S./N.S. explicitly
for Russia and the American colonies). The model in `src/` can convert between
the two by applying the correct day offset for a given year.

## Concepts that carry forward

- **The 4/100/400 leap rule** — now the global civil standard and the rule
  implemented in `SLCalendarGregorian` in `src/`.
- **A bounded, known residual error** (≈1 day / 3,030 years) — small enough that
  civil timekeeping now worries about leap *seconds* (UTC), not leap *days*.
- **The O.S./N.S. distinction** — the formal bridge between the Julian baseline
  and every adopting polity.
