# 01 — The Julian calendar (the baseline)

*Instituted by Julius Caesar, effective 1 January 45 BCE (reform enacted 46
BCE). This is the baseline against which every later exception in this
collection is measured.*

## What changed

On the advice of the Alexandrian astronomer Sosigenes, Caesar abandoned the
abusable lunisolar scheme (document 00) for a **purely solar** calendar with a
**fixed, mechanical leap rule**:

- A common year of **365 days** in 12 months of the lengths we still use.
- One **leap day every fourth year**, giving an average year of **365.25 days**.
- No more discretionary intercalation by priests — the rule is arithmetic and
  cannot be gamed.

To reset the seasons before the new system began, **46 BCE was stretched to 445
days** (the "year of confusion"), after which the regular pattern took over.

## The leap rule

```
A Julian year is a leap year  ⇔  year mod 4 == 0.
Average length = 365.25 days.
```

Simple and durable — but **too long**. The true mean tropical year is about
365.2422 days, so the Julian year overshoots by roughly **0.0078 days per year**,
or about **1 day every 128 years**. Over centuries the calendar date of the
vernal equinox slips *earlier*. By the 16th century the equinox had drifted
about **10 days** from where it sat at the Council of Nicaea (325 CE) — the error
that the Gregorian reform (document 02) exists to correct.

## Why it is the baseline

Every item in this collection is expressed as a **deviation from Julian**:

- The **Gregorian reform** (02) keeps the Julian frame and changes only the leap
  rule and the starting offset — it is a *correction to Julian*, not a new
  calendar.
- The **national adoption dates** (03–05) are the moments each polity switched
  *off* Julian and *onto* Gregorian; the size of the day-skip at each switch is
  a direct function of how long Julian error had accumulated by that date (10
  days in 1582, 11 by 1752, 13 by 1918).
- The **proleptic Julian calendar** (projecting the rule backward before 45 BCE)
  is the conventional frame historians use to place ancient dates, which is why
  the model in `src/` can reckon in Julian as well as Gregorian.

## Concepts that carry forward

- **A fixed, non-discretionary leap rule.** The core idea — the calendar is an
  arithmetic function of the year number, not a political decision.
- **The 365/366-day solar year and the 12-month structure.** Unchanged by every
  later reform.
- **An accumulating, measurable error.** Because the error is exactly
  quantifiable (≈1 day / 128 years), the later reforms can correct it precisely
  rather than by guesswork.
