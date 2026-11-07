# Calendar

A history-grounded calendar system for SLeeLa: the Western calendar as agreed by
the Greeks and carried forward to the present, the ordered set of *exceptions*
to the Julian baseline, the national/regional adoption threads (China, Russia,
Portugal/Turkey), a **Unified Calendar** synthesis, and the concrete **System of
Integration respective to the United States** that realizes it.

The prose lives in the Markdown documents below. The executable model lives in
the SLeeLa source under [`src/`](src/).

## Reading order

| # | Document | Covers |
|---|----------|--------|
| — | [`README.md`](README.md) | This index + the ordered exception list |
| 00 | [`00-greek-and-pre-julian.md`](00-greek-and-pre-julian.md) | Greek lunisolar calendars and the pre-Julian Roman calendar |
| 01 | [`01-julian-baseline.md`](01-julian-baseline.md) | The Julian calendar (46 BCE) — the baseline all later changes are measured against |
| 02 | [`02-gregorian-reform.md`](02-gregorian-reform.md) | The Gregorian reform of 1582 — the principal exception to Julian |
| 03 | [`03-adoption-china.md`](03-adoption-china.md) | China: its calendar concepts and unification to the Gregorian civil calendar |
| 04 | [`04-adoption-russia.md`](04-adoption-russia.md) | Russia: its concepts and the 1918 switch |
| 05 | [`05-adoption-portugal-turkey.md`](05-adoption-portugal-turkey.md) | Portugal (early, 1582) and Turkey/Ottoman (late, 1917/1926) |
| 06 | [`06-system-of-integration-united-states.md`](06-system-of-integration-united-states.md) | The U.S.-referenced System of Integration — the operative unit |
| 07 | [`07-greece-bc-to-1600.md`](07-greece-bc-to-1600.md) | Greece BC→1600: dated events, concessions, and 1–10 weighings |
| 08 | [`08-roman-calendar-1244bc-to-present.md`](08-roman-calendar-1244bc-to-present.md) | Rome 1244 BC→present: the spine line, events, concessions, weighings |
| 99 | [`99-unified-calendar.md`](99-unified-calendar.md) | The Unified Calendar synthesis |

## The ordered exceptions to the Julian baseline

"Roughly order those exceptions" — the deviations from the Julian calendar,
earliest to latest, folded into this single list where small and broken out into
their own documents where large. The one *large* exception (the Gregorian
reform) has its own document (02); everything downstream is adoption of that
exception on different dates, which is the subject of documents 03–05.

0. **The Greek span (BC → 1600).** Before and across the baseline, Greece
   supplies the Metonic arithmetic (432 BCE) and the computus behind Easter,
   concedes its civil calendar to Rome's Julian in 45 BCE, and then *refuses* the
   1582 Gregorian reform — remaining Julian through 1600. The dated events and
   their 1–10 weighings are document 07.
0b. **The Roman line (`[2541 cbc a]` ≈ 1244 BC → present).** The spine of this
   whole set: established at the New Capelet origin epoch `[2541 cbc a]` (weight
   2), then from a legendary 10-month year to Numa's 12 months, through the
   Julian baseline and the Gregorian correction, to today's proleptic Gregorian /
   UTC / ISO 8601 civil calendar. The dated events, concessions, and 1–10
   weighings are document 08.
1. **Pre-Julian drift (before 46 BCE).** The Roman republican calendar fell out
   of step with the seasons; intercalation was manual and politically abused.
   This is the condition the Julian calendar was created to fix — see 00/01.
2. **The Gregorian reform (1582).** The single structural exception: a refined
   leap rule (century years not divisible by 400 are common years) plus a
   one-time 10-day deletion to re-anchor the vernal equinox. This is document 02
   and is the parent of every item below. *Large → own document.*
3. **Portugal, Spain, Papal States, Poland-Lithuania (Oct 1582).** Early
   adopters; Thursday 4 Oct 1582 was followed by Friday 15 Oct 1582. *Small → in
   document 05 (Portugal) and noted here.*
4. **British Empire incl. American colonies (1752).** Adopted via the Calendar
   (New Style) Act 1750; Wednesday 2 Sep 1752 was followed by Thursday 14 Sep
   1752 (11 days dropped). *The U.S. inheritance — document 06.*
5. **Japan (1873), then the wider world.** Civil adoption of Gregorian for
   administrative and international interchange.
6. **China (1912 Republic; unified nationwide 1949).** Gregorian becomes the
   civil calendar while the traditional lunisolar calendar remains cultural.
   *Document 03.*
7. **Russia / Soviet Russia (1918).** The last major European holdout; 31 Jan
   1918 was followed by 14 Feb 1918 (13 days dropped). *Document 04.*
8. **Ottoman Empire / Turkey (1917 fiscal; 1926 full civil).** Late adoption
   completing the convergence. *Document 05.*

## The through-line

Greek lunisolar reckoning → Julian solar baseline (46 BCE) → **one** structural
correction (Gregorian, 1582) → staggered worldwide adoption of that one
correction → de-facto global convergence on the Gregorian civil calendar,
machine-readable via ISO 8601 / UTC. The United States is the concrete frame in
which all of this is integrated (document 06), and the Unified Calendar
(document 99) is the agreed result.
