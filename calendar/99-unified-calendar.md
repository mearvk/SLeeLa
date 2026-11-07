# 99 — The Unified Calendar

*The agreed result. Having considered the Greek lunisolar reckoning, the Julian
baseline, the single Gregorian exception, and the China / Russia /
Portugal-Turkey adoption threads, and having anchored them to the United States'
System of Integration (document 06), we arrive at the Unified Calendar.*

## The claim

The Unified Calendar is **not a newly invented calendar**. It is the recognition
that the world has already *converged* on one, and the statement of the rules
that make that convergence precise and interoperable. The agreement is:

> **The Unified Calendar is the proleptic Gregorian civil calendar, referenced
> to UTC, expressed in ISO 8601, with every historical and cultural calendar
> admitted as an overlay that maps to it.**

This is "unified" in two senses at once:

1. **Historically unified** — it is the single endpoint that Portugal (1582),
   the United States (1752), China (1912/1949), Russia (1918), and Turkey
   (1917/1926) all reached from different starting calendars (documents 03–06).
2. **Structurally unified** — it is *one* arithmetic rule plus *one* physical
   reference plus *one* interchange form, into which all the other systems map.

## The three unified rules

### Rule 1 — The civil reckoning (from the Gregorian reform, document 02)

```
Leap year  ⇔  (y mod 4 == 0) and not (y mod 100 == 0 and y mod 400 != 0)
Average year = 365.2425 days; residual error ≈ 1 day / 3,030 years.
```

Applied **proleptically** — projected backward before 1582 — so that a single
rule covers all of recorded history once a frame is declared. The model in `src/`
also carries the **Julian** rule (`y mod 4 == 0`) so it can speak the baseline
and perform O.S./N.S. conversion (documents 01, 04, 06).

### Rule 2 — The physical reference (from the U.S. System of Integration, document 06)

```
Civil instants are referenced to UTC.
Reconciliation with Earth's rotation is by leap seconds, not leap days.
```

This is the modern form of the Greek reconciliation-with-the-Sun (document 00):
the correction has moved leap *months* → leap *days* → leap *seconds*, each step
smaller and more precise.

### Rule 3 — The interchange form (from the U.S. System of Integration, document 06)

```
Dates:      YYYY-MM-DD                     (ISO 8601)
Instants:   YYYY-MM-DDThh:mm:ss±hh:mm       (ISO 8601 / RFC 3339)
Default:    proleptic Gregorian, UTC frame
```

So that a date is unambiguous across place, system, and century.

## The overlay principle (from China, Russia, Turkey — documents 03–05)

Unification does **not** abolish any calendar. The recurring settlement in every
adoption thread was *two layers*: a **civil** Gregorian calendar over a
**retained** cultural/liturgical one. The Unified Calendar makes that general:

| Overlay calendar | Kind | Maps to the Unified Calendar by… |
|------------------|------|----------------------------------|
| Chinese traditional (农历) | lunisolar | computing new-moon months + 24 solar terms, labeling the Gregorian date |
| Orthodox liturgical | Julian | applying the O.S./N.S. day offset for the year |
| Islamic (Hijri) | lunar | counting lunar months from the Hijra epoch, mapping to the Gregorian date |
| Greek festival / Olympiad | lunisolar | Metonic 19-year cycle, mapped to a proleptic Gregorian date |

Each overlay **keeps its own meaning** (festivals, feasts, eras) while resolving
to one civil instant. This is the heart of the agreement: *one civil calendar,
many preserved cultural calendars, all mutually convertible.*

## Why the United States is the frame (tie back to document 06)

A unified calendar needs a place where it is actually *operative* — enacted in
law, held to a physical standard, and emitted in a machine-readable form. The
U.S. System of Integration (document 06) supplies exactly that stack:
Gregorian calendar (Layer 1) · time-zone and DST law (Layer 2) · NIST/USNO and
UTC (Layer 3) · ISO 8601/RFC 3339 (Layer 4). The Unified Calendar is that stack
stated as a general agreement rather than one nation's arrangement — the United
States is the worked example that proves it is real.

## The agreement, in one paragraph

From the Greek insight that a civil year must be reconciled with the Sun, through
Caesar's mechanical leap rule, through Gregory's single refinement, through the
staggered adoption by Portugal, the American colonies, China, Russia, and Turkey,
the world has converged on one civil calendar. Stated precisely — proleptic
Gregorian arithmetic, UTC physical reference, ISO 8601 interchange, with every
historical and cultural calendar admitted as a convertible overlay, and realized
concretely through the United States' System of Integration — this is the
**Unified Calendar**. The SLeeLa sources in `src/` implement it.

## Implementation map

| Concept here | SLeeLa class in `src/` |
|--------------|------------------------|
| Julian baseline (doc 01) | `SLCalendarJulian` |
| Gregorian reform (doc 02) | `SLCalendarGregorian` |
| O.S./N.S. conversion (docs 02, 04, 06) | `SLCalendarConverter` |
| U.S. System of Integration (doc 06) | `SLCalendarIntegrationUS` |
| The Unified Calendar (this doc) | `SLUnifiedCalendar` |
