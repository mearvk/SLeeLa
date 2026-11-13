# Device Instructor Series

Thirty-two runnable SLeeLa lessons for people who want to see that the **Device
Instructor** is ready to receive good input about the **System** and about
**Students**. Each lesson is one class with a `main()`, in its own folder, with
an `.expected.txt` file holding its exact output. Lessons 1, 2 and 4 to 32 build
on the shared [`SLNounRegistry`](./SLNounRegistry.sleela); lessons 7 to 32 also use
the [`SLDecisionDesk`](./SLDecisionDesk.sleela).

| # | lesson | the Instructor shows that it... |
|---|--------|---------------------------------|
| 1 | [`01-registry-intake`](./01-registry-intake/DiRegistryIntake.sleela) | refuses input until a field of observation is declared, keeps one carrier per noun, accepts a carrier only if it is the field or a registered noun, and answers "not observed" for anything unregistered |
| 2 | [`02-student-input-gate`](./02-student-input-gate/DiStudentInput.sleela) | accepts only a claim with registered evidence in sequitur order, and says exactly why it refuses anything else |
| 3 | [`03-system-readiness`](./03-system-readiness/DiSystemReadiness.sleela) | checks system readings for presence, plausibility and sufficiency before declaring itself ready |
| 4 | [`04-agreement`](./04-agreement/DiAgreement.sleela) | compares the student and the Purchase Master registries entry by entry and points at what to re-observe |
| 5 | [`05-nested-carriers`](./05-nested-carriers/DiNestedCarriers.sleela) | handles a carrier that is itself carried: prints the chain back to the field, counts what a carrier holds, and refuses a second place, an unregistered carrier or a self-carrier |
| 6 | [`06-purchase-onion`](./06-purchase-onion/DiPurchaseOnion.sleela) | takes registry to onion: the student totals registered quotes by hand, the Purchase Master runs the Sequitur onion, and the two are compared until they agree |

Lessons 7 to 32 are twenty-six trading decisions, all in the same shape as
lesson 6. See [`TRADING-DECISIONS.md`](./TRADING-DECISIONS.md) for the full table
(goods, options, student slip, master pick, ties).

| # | goods | options | slip |
|---|-------|:------:|------|
| 7 | Lumber | 2 | missed discount |
| 8 | Cement | 2 | fee counted twice |
| 9 | Copper | 2 | discount sign flipped |
| 10 | Steel | 3 | option not observed |
| 11 | Coffee | 2 | quote in dollars, not cents |
| 12 | Cotton | 3 | no slip (clean) |
| 13 | Solar panels | 2 | missed discount |
| 14 | Lithium | 3 | fee counted twice |
| 15 | Semiconductors | 2 | discount sign flipped |
| 16 | Server racks | 4 | option not observed |
| 17 | Smartphones | 3 | quote in dollars, not cents |
| 18 | Televisions | 4 | missed discount |
| 19 | Wheat | 3 | no slip (clean) |
| 20 | Fuel | 3 | fee counted twice |
| 21 | Freight | 3 | discount sign flipped |
| 22 | Tie capstone | 4 | option not observed |
| 23 | Rice | 5 | discount filed under the next option |
| 24 | Sugar | 5 | missed discount |
| 25 | Aluminum | 6 | fee counted twice |
| 26 | Zinc | 6 | discount filed under the next option |
| 27 | Nickel | 5 | discount sign flipped |
| 28 | Rubber | 7 | option not observed |
| 29 | Glass | 6 | quote in dollars, not cents |
| 30 | Paper | 8 | no slip (clean) |
| 31 | Plastic resin | 8 | discount filed under the next option |
| 32 | Eight-way capstone | 8 | missed discount |

Courseware that uses these lessons:
[children, unit 1](../courseware/children/unit-01-true-or-false.md) (lessons 1
and 2) and
[adults, unit 1](../courseware/adults/unit-01-propositional-logic.md) (lessons
1, 2, 4 and 5).

## Running

From the repository root, with the verified-build manifest in place:

```
working-binaries/sleela run lib/advantage/learner-vs-professional/series/01-registry-intake/DiRegistryIntake.sleela
```

Do the same for the others. A lesson finds `SLNounRegistry` (and, for lesson
6, the `lib/advantage` onion classes) in the folders above it. Lessons are
independent; the suggested order is 1 to 32. See [`MANUAL.md`](./MANUAL.md) for
user concerns and troubleshooting.

## Assurance

`test-suites/test-device-instructor.sh` runs every lesson and compares its
output with the lesson `.expected.txt`, line for line. It also checks that
every lesson has a `main()`, the Metadata block and an expected file.

```
bash test-suites/test-device-instructor.sh
```

If a lesson is changed on purpose, review the new output and regenerate its
expected file with `UPDATE=1 bash test-suites/test-device-instructor.sh`.

## What each transcript proves (key lines)

1. Lesson 1: the first `register` is refused with "declare the field of
   observation first"; the second `ball` is refused as a duplicate; `kite` in
   the unregistered `green box` is refused; `kite` is "not observed".
   Tally: `accepted=5 refused=3 registered=5`.
2. Lesson 2: three of six submissions are refused, one each for an empty claim,
   unregistered evidence and wrong step order. Tally:
   `good=3 refused=3 last accepted step=3`.
3. Lesson 3: verdicts go from "NOT ready, 5 check(s) to fix" to "NOT ready, 3"
   to "the instructor is ready to receive input".
4. Lesson 4: first comparison has 2 differences; after the student
   re-observes, the verdict is "AGREE, work accepted".
5. Lesson 5: `order 2 -> order list -> customer-17 -> customer file` at depth
   3; three refusals. Tally: `accepted=7 refused=3 registered=7`.
6. Lesson 6: the master picks supplier A at 11650 cents; the student first
   misses an 800-cent discount and picks supplier B (12450 vs 12400), is told
   to re-observe supplier A, then agrees. On an exact tie the norm picks
   supplier A (see `.kiro/steering/advantage-tie-norm.md`).

7. Lessons 7 to 32: each prints the master totals and pick, the student first
   pass with its slip, every differing entry, the verdict, and (unless the
   first pass agrees) the re-observation ending in "AGREE, work accepted".

## Conventions

- Every class carries the Metadata Class metapattern (baseline identity).
- A reading of `-1`, or a noun the registry has not seen, is "not observed". It
  is never guessed and never counted as ready.
- Money is integer cents.
