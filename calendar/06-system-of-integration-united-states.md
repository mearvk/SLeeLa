# 06 — The System of Integration, respective to the United States

*The operative unit. All of the considered calendar concepts — Greek lunisolar,
the Julian baseline, the Gregorian reform, and the China/Russia/Portugal-Turkey
adoption threads — integrate, in practice, through the civil-time system of the
United States. This document is the concrete realization of the Unified Calendar
(document 99), anchored to a single reference frame: U.S. law and standards.*

The United States is used here as the **integrating reference frame** — the one
place to see the whole inheritance resolved into a single governing civil-time
system. It is a "system of integration" in that it stacks four layers, each
absorbing the concepts from the history, into one coherent whole.

## Layer 1 — The calendar layer (inherited)

The U.S. civil calendar **is the Gregorian calendar** (document 02), and it
arrived by inheritance, not by independent reform:

- The American colonies were part of the **British Empire**, which adopted
  Gregorian via the **Calendar (New Style) Act 1750**.
- The changeover: **Wednesday 2 September 1752 was followed by Thursday 14
  September 1752** — **11 days deleted** (the accumulated Julian drift by 1752;
  see document 02). The British Empire also moved New Year's Day from 25 March to
  **1 January**.
- Consequence for American records: colonial dates before 1752 carry **Old
  Style / New Style** ambiguity. George Washington's birth is the textbook case —
  **11 February 1731 (O.S.)** = **22 February 1732 (N.S.)**, the date now
  observed. This is exactly the O.S./N.S. conversion the model in `src/` performs.

So Layer 1 absorbs the entire Greek → Julian → Gregorian line (documents 00–02)
as the United States' own calendrical foundation.

## Layer 2 — The standardization layer (U.S. civil law)

On top of the calendar, the U.S. built a national **time standardization** stack —
the machinery that makes a calendar date resolve to an agreed instant across a
continent:

- **Standard railway time (1883).** U.S. and Canadian railroads divided the
  continent into standard time zones to replace a chaos of local solar times.
- **Standard Time Act of 1918** ("Calder Act"). Gave U.S. standard time zones the
  force of federal law and first introduced **daylight saving time** nationally.
- **Uniform Time Act of 1966.** Standardized the start/end of DST across states
  and set the framework still in force; states may exempt themselves (e.g.
  Arizona, Hawaii).
- **Energy Policy Act of 2005.** Set the current DST window (second Sunday in
  March to first Sunday in November), effective 2007.

Layer 2 integrates the *when* of the civil calendar with geography and law.

## Layer 3 — The authority / measurement layer (U.S. standards bodies)

A date-and-time needs a physical reference. In the U.S. system that authority is
split between two institutions:

- **NIST** (National Institute of Standards and Technology) — maintains the U.S.
  civil time standard, **UTC(NIST)**, disciplined to atomic clocks, and
  disseminates it (radio station WWV/WWVB, the Internet Time Service / NTP).
- **USNO** (United States Naval Observatory) — maintains **UTC(USNO)** for the
  Department of Defense and GPS, and the astronomical reference (Earth-orientation
  data).
- **UTC and leap seconds.** Both anchor to **Coordinated Universal Time (UTC)**,
  which keeps atomic time within 0.9 s of Earth-rotation time by inserting
  **leap seconds**. This is the modern descendant of the ancient
  reconciliation-with-the-Sun problem (document 00): the correction has simply
  moved from leap *months* (Greek) to leap *days* (Julian/Gregorian) to leap
  *seconds* (UTC).

Layer 3 integrates the civil calendar with physics.

## Layer 4 — The interchange layer (machine-readable integration)

Finally, the U.S. system expresses dates for computation and international
exchange in formats that make every prior layer unambiguous:

- **ISO 8601** — `YYYY-MM-DD` dates and `YYYY-MM-DDThh:mm:ssZ` timestamps, with
  explicit UTC offsets. Adopted across U.S. government, finance, and software.
- **RFC 3339** — the Internet profile of ISO 8601 used by virtually all U.S.
  networked systems (logs, APIs, databases).
- **The proleptic Gregorian calendar** — ISO 8601 reckons all dates, even those
  before 1752/1582, on the Gregorian rule projected backward, which is why a
  single unambiguous date string can span the O.S./N.S. divide once a frame is
  declared.

Layer 4 is where the China, Russia, and Portugal/Turkey threads (documents 03–05)
finally meet the U.S. frame: whatever cultural or liturgical calendar a date was
born in, it is **integrated** by expressing it as a Gregorian/UTC ISO 8601
instant. The two-layer settlement those countries reached (civil Gregorian over a
retained cultural calendar) is honored — the cultural label is preserved — but
interchange happens in the common civil form.

## The integration, summarized

```
             GREEK lunisolar reconciliation  (leap months)
                          │
             JULIAN baseline  (leap day every 4 yrs)            ← document 01
                          │
             GREGORIAN reform 1582  (4/100/400 rule, −10 days)  ← document 02
                          │
   ┌──────────┬───────────┼────────────┬─────────────┐
 Portugal   China       Russia      Turkey      …rest of world   ← documents 03–05
  1582       1912/49     1918        1917/26
   │          │           │            │
   └──────────┴─────┐ ┌───┴────────────┘
                    ▼ ▼
        UNITED STATES SYSTEM OF INTEGRATION                      ← THIS document
        ├─ Layer 1  Gregorian calendar (inherited 1752, O.S./N.S.)
        ├─ Layer 2  Time-zone & DST law (1883 · 1918 · 1966 · 2005)
        ├─ Layer 3  NIST / USNO · UTC · leap seconds
        └─ Layer 4  ISO 8601 / RFC 3339 · proleptic Gregorian
                    ▼
        UNIFIED CALENDAR  (document 99) — realized, not merely proposed
```

The U.S. System of Integration is what makes the Unified Calendar an *operative*
thing rather than an abstraction: it is the concrete, legally-grounded,
physically-referenced, machine-readable stack in which all the considered
calendar concepts actually coexist and interoperate. The SLeeLa model in `src/`
implements this stack as `SLCalendarIntegrationUS`, composing the Julian and
Gregorian reckoners, the O.S./N.S. converter, and the ISO 8601 / UTC interchange
surface.
