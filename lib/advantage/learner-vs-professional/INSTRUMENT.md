# INSTRUMENT: The Advantage™ Courseware Data Index

A single, neatly organized index of every dataset, worked example, exercise
set, and self-checking entry used across the Advantage™ courseware — the
"instrument" a teacher, learner, or reviewer can use to find exactly what
numeric material exists, where it lives, and how it was verified.

> **A note on "training data."** This document is an index of **teaching
> material** — worked numeric examples, exercises with answer keys, and
> self-checking `.sleela` entries with their expected outputs — not machine
> learning training data. Nothing in this courseware trains a model; every
> number below was computed by hand-written code and independently checked,
> usually against a separate Python model, as described in each topic's own
> `ASSURANCE.md`/`README.md`. This reading is stated explicitly so the term
> is never ambiguous to a future reader.

All figures quoted below were read directly from committed, already-passing
`.expected.txt` files or `.sleela` source at the time of writing, not
retyped from memory. Illustrative only: no real market, price, or order is
used anywhere in this courseware, and nothing here is investment, tax, or
legal advice.

## Contents

1. [Overview: how the courseware is organized](#1-overview-how-the-courseware-is-organized)
2. [The Device Instructor series](#2-the-device-instructor-series-32-lessons)
3. [Children's units](#3-childrens-units-5-units)
4. [Adults' units](#4-adults-units-5-units)
5. [Age-sorted reading material](#5-age-sorted-reading-material-24-readers)
6. [Long-Medium-Averages](#6-long-medium-averages-20-entries)
7. [Long-Term-Potencies](#7-long-term-potencies-6-entries)
8. [Glossary and defined-vs-undefined terms](#8-glossary-and-defined-vs-undefined-terms)
9. [Verification methodology, repository-wide](#9-verification-methodology-repository-wide)
10. [Quick-reference counts](#10-quick-reference-counts)

## 1. Overview: how the courseware is organized

```
lib/advantage/learner-vs-professional/
├── series/                     the Device Instructor series (32 lessons)
└── courseware/
    ├── children/                5 units, ages-appropriate logic/process lessons
    ├── adults/                  5 units, formal logic/process lessons
    ├── reading/                 24 readers, sorted ages 5-7 through 18+
    ├── long-medium-averages/    20 entries, the daily-action-cap teaching rule
    ├── long-term-potencies/     6 entries, defined term/age potency ratios
    ├── ORGANIZED.md             glossary: defined vs. undefined/declined terms
    ├── LEARNING-ENVIRONMENT.md  the registry/field-of-observation model
    └── REGISTRY-OBSERVATION-EXAMPLES.md
```

Each runnable topic (series, long-medium-averages, long-term-potencies) has
its own test gate under `test-suites/` and its own `README.md` with the full
entry table; this document summarizes and cross-links them rather than
duplicating their detail.

## 2. The Device Instructor series (32 lessons)

Location: [`series/`](./series/README.md). Gate:
`test-suites/test-device-instructor.sh` (32 lessons, confirmed against the
gate's own count check).

Lessons 1–6 build the registry/agreement model on
[`SLNounRegistry`](./series/SLNounRegistry.sleela); lessons 7–32 are 26
trading decisions on [`SLDecisionDesk`](./series/SLDecisionDesk.sleela), each
comparing a student's hand total against a Purchase Master's onion-computed
total.

| lesson | subject | sample verified data point |
|---|---|---|
| 1 | registry intake | tally: `accepted=5 refused=3 registered=5` |
| 2 | student input gate | 6 submissions, 3 good and 3 refused for three distinct reasons (empty claim, unregistered evidence, wrong step order); final tally `good=3 refused=3 last accepted step=3` |
| 3 | system readiness | 5 checks (memory, storage, display, input, clock) go from "5 check(s) to fix" to "the instructor is ready," distinguishing NOT OBSERVED, BELOW MINIMUM, and IMPLAUSIBLE readings |
| 4 | agreement | two independently-built registries compared entry by entry; first pass finds 2 differences ("re-observe," "master only"), second pass after re-observing ends `AGREE, work accepted` |
| 5 | nested carriers | a carrier chain up to 3 levels deep |
| 6 | purchase onion | supplier A total 11,650¢ vs. supplier B 12,400¢; a missed discount flips the student's total to 12,450¢ (off by exactly 800¢); a later tie at 11,650¢/11,650¢ demonstrates the tie norm (the earlier-listed supplier wins) |
| 7–22 | sixteen trading decisions | lumber, cement, copper, steel, coffee, cotton, solar, lithium, semiconductors, server racks, smartphones, televisions, wheat, fuel, freight, a tie capstone |
| 23–32 | ten more trading decisions | rice, sugar, aluminum, zinc, nickel, rubber, glass, paper, resin, an eight-way capstone |

Full table of every decision's goods, options, and chosen supplier:
[`series/TRADING-DECISIONS.md`](./series/TRADING-DECISIONS.md). User-facing
troubleshooting: [`series/MANUAL.md`](./series/MANUAL.md).

## 3. Children's units (5 units)

Location: [`courseware/children/`](./courseware/children/README.md).
Play-based logic lessons, each with worked examples, questions, and an
answer key, paired with specific series lessons.

| unit | topic | paired with |
|---|---|---|
| 1 | true or false | lessons 1, 2 |
| 2 | and, or, not | lesson 4 |
| 3 | if this, then that | lesson 1 |
| 4 | steps in order | lesson 2 |
| 5 | peeling the onion | lessons 5, 6 |

## 4. Adults' units (5 units)

Location: [`courseware/adults/`](./courseware/adults/README.md). Formal
logic and process lessons, each with exercises and a worked answer key.

| unit | topic | paired with |
|---|---|---|
| 1 | propositional logic | lessons 1, 2, 4 |
| 2 | predicates and inference | lessons 1, 4, 5 |
| 3 | processes and algorithms | lessons 1, 2, 5 |
| 4 | complexity basics | lessons 1, 4, 5 |
| 5 | applied onion decisions | lesson 6, plus lessons 7–32 for further practice |

## 5. Age-sorted reading material (24 readers)

Location: [`courseware/reading/`](./courseware/reading/README.md), sorted
from youngest to oldest. Count per band verified directly against the
folder contents:

| band | readers | sample subject |
|---|---|---|
| ages 5–7 | 4 | the toy shelf, true/false, fair swaps, step order |
| ages 8–10 | 4 | where things live, and/or/not, peeling the onion, how old is it |
| ages 11–13 | 4 | buy/hold/sell, averages with a remainder, Euclid's gcd loop, honest answers |
| ages 14–17 | 5 | long vs. short risk, Euclid/Bezout, time/fees/term, how correctness is checked, numbers that lie above 2^53 - 1 |
| ages 18+ | 7 | system overview, position books, margin arithmetic, correctness and cost, assurance review log, single-trade sensitivity, batch vs. per-trade rate |

The ages-18+ band is the direct adult-level companion to
`trading-actions-on-verb` and `long-medium-averages`; reader 7 is the most
recently added, tying a hypothetical "20% per 2,000 trades" claim to the
real, checked `SLBatchReturn` arithmetic.

## 6. Long-Medium-Averages (20 entries)

Location: [`courseware/long-medium-averages/`](./courseware/long-medium-averages/README.md).
Gate: `test-suites/test-long-medium-averages.sh` (20 entries, confirmed
against the gate's count check).

Classes: [`SLDailyCapBook`](./courseware/long-medium-averages/SLDailyCapBook.sleela)
(long side) and [`SLDailyCapShortBook`](./courseware/long-medium-averages/SLDailyCapShortBook.sleela)
(short side), each `SLLongBook`/`SLShortBook` plus a fixed 2,000-actions-per-day
teaching rule (status code 10 on the 2,001st action of a day).

| entries | subject | a verified data point |
|---|---|---|
| 1–16 | long-side goods, one per entry: copper coins, silver tokens, gold chips, brass rounds, bronze medals, nickel blanks, tin pucks, zinc slugs, steel washers, platinum foils, titanium tabs, iron tags, quartz chips, obsidian flakes, granite counters, slate markers | entry 1 (copper coins): realized gross 60,000¢, net of fees 59,980¢ |
| 17 | the daily cap boundary itself | the 2,000th action is accepted, the 2,001st is refused and changes nothing, the count resets on a new day |
| 18–20 | short-side goods: coins, tokens, chips | the short-side mirror of entries 1–16, with borrow cost added |

## 7. Long-Term-Potencies (6 entries)

Location: [`courseware/long-term-potencies/`](./courseware/long-term-potencies/README.md).
Gate: `test-suites/test-long-term-potencies.sh` (6 entries, confirmed
against the gate's count check).

Class: [`SLTermPotency`](./courseware/long-term-potencies/SLTermPotency.sleela),
which **defines** two basis-point ratios (10,000 bp = 100%) rather than using
the word "potency" loosely:

| metric | formula |
|---|---|
| term potency | (units held/short that are long term) / (units held/short) |
| age potency | weighted-average age / oldest lot's age |

| entries | subject | a verified data point |
|---|---|---|
| 1 | a long book aging past the threshold | at day 365 a lot is exactly 365 days old and **not** long term (the rule is strictly more than); at day 500, with a 60-unit younger lot added, term potency = 4,000 bp (40 of 100 units); at day 900, term potency = 10,000 bp, age potency = 7,333 bp |
| 2 | a FIFO sell and potency | selling the entire oldest lot drops term potency on the remainder to 0 bp |
| 3 | honest empty/invalid answers | an empty book answers -2 (term) / -1 (age); a day before the book's last action answers -1 for both; the book's own current day is always valid, even at age 0 |
| 4 | ten randomly generated portfolios | each cross-checked against an independent Python model |
| 5 | the short-side mirror | identical readings to entry 1, proving the two overloads agree |
| 6 | short-side honesty and cover | the short-side counterpart of entries 2 and 3 |

`SLShortBook.isLongTerm`/`qtyLongTerm` did not exist before this topic
needed them; they were added mirroring `SLLongBook` exactly, and entry 5's
cross-check was the condition for trusting them.

## 8. Glossary and defined-vs-undefined terms

The authoritative glossary for this courseware's trading vocabulary is
[`courseware/ORGANIZED.md`](./courseware/ORGANIZED.md); it is not duplicated
here, this section only flags what it covers. Two metrics used above —
**term potency** and **age potency** — are defined in
[`SLTermPotency.sleela`](./courseware/long-term-potencies/SLTermPotency.sleela)'s
own header comment, not in `ORGANIZED.md` (which predates that topic); both
places agree on being precise about what a term means before using it.

- **Defined in `ORGANIZED.md`, used in running, checked code:** long, short,
  lot, FIFO, verb, refusal, invariant, borrow cost, the tie norm, the term
  boundary, margin/equity/excess/call, differential testing, the
  exact-comparison bound, the daily-trades cap (14 terms as of this writing).
- **Defined in `SLTermPotency.sleela`, not in `ORGANIZED.md`:** term potency,
  age potency (see §7 above).
- **Named but never defined, and not used in any number here:** "Flat 2,"
  "'2'-ness," "medium-concerns" as a formal metric, any "military/aluminum/
  grey Quality of the Instrument" tier scheme.
- **A claim recorded as a claim, not as a verified fact:** "day traders make
  about 20% profit per 2,000 trades," attributed to its source, with the
  real arithmetic that distinguishes a batch-total reading (1 bp per trade,
  plausible) from a per-trade-repeated reading (400x capital, not
  plausible) — see [`SLBatchReturn`](../trading-actions-on-verb/SLBatchReturn.sleela)
  and reader 7 of the ages-18+ band.
- **One declined request,** recorded plainly rather than hidden: framing
  sexual content as part of a trading example.

## 9. Verification methodology, repository-wide

Every runnable topic indexed above uses the same discipline, described in
full in each topic's own `ASSURANCE.md`/`README.md`:

1. **Self-checks.** Each entry prints `ok`/`FAIL` per check and ends
   `checks passed=N failed=0`.
2. **Independent oracle.** Expected values were computed by a separate
   Python model of the same rules, not copied from the SLeeLa output.
3. **Differential testing** (where applicable). Random actions are run
   against an independent shadow implementation sharing no code with the
   class under test.
4. **Mutation testing.** Deliberately broken copies of the code are
   confirmed to fail their gate before the real fix is trusted.
5. **Expected-output gate.** Each topic's `test-suites/test-*.sh` script
   runs every entry and fails the build on any mismatch.

A later general code-quality review of `trading-actions-on-verb` (not
driven by any single use case) found and fixed two real overflow bugs in
`SLBatchReturn`, confirmed genuine by reverting each fix and watching the
new regression test fail with the original symptom; see
[`ASSURANCE.md`'s second review log](../trading-actions-on-verb/ASSURANCE.md#review-log-general-code-quality-pass).

## 10. Quick-reference counts

| topic | entries/lessons | gate |
|---|---:|---|
| Device Instructor series | 32 | `test-suites/test-device-instructor.sh` |
| Children's units | 5 | (documentation only; no `.sleela` gate) |
| Adults' units | 5 | (documentation only; no `.sleela` gate) |
| Age-sorted reading | 24 | (documentation only; no `.sleela` gate) |
| Long-Medium-Averages | 20 | `test-suites/test-long-medium-averages.sh` |
| Long-Term-Potencies | 6 | `test-suites/test-long-term-potencies.sh` |
| **Total runnable entries** | **58** | three gates |
| **Total written units/readers** | **34** | documentation |

(The companion `trading-actions-on-verb` library, which several of the
entries above read from, has its own 27 use cases and its own index in
[`advantage/PROFITABILITY.SYSTEMS.INDEX.md`](../../../advantage/PROFITABILITY.SYSTEMS.INDEX.md);
it is referenced here, not re-indexed, to avoid duplicating that document.)
