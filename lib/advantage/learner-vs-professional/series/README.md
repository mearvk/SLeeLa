# Device Instructor Series

Four runnable SLeeLa lessons for people who want to see that the **Device
Instructor** is ready to receive good input about the **System** and about
**Students**. Each lesson is one class with a `main()`, in its own folder, and
prints a short transcript you can check against this page.

| # | lesson | the Instructor shows that it... |
|---|--------|---------------------------------|
| 1 | [`01-registry-intake`](./01-registry-intake/DiRegistryIntake.sleela) | refuses input until a field of observation is declared, keeps one carrier per noun, and answers "not observed" for anything unregistered |
| 2 | [`02-student-input-gate`](./02-student-input-gate/DiStudentInput.sleela) | accepts only a claim with registered evidence in sequitur order, and says exactly why it refuses anything else |
| 3 | [`03-system-readiness`](./03-system-readiness/DiSystemReadiness.sleela) | checks system readings for presence, plausibility and sufficiency before declaring itself ready |
| 4 | [`04-agreement`](./04-agreement/DiAgreement.sleela) | compares the student and the Purchase Master entry by entry and points at what to re-observe |

## Running

From the repository root, with the verified-build manifest in place:

```
working-binaries/sleela run lib/advantage/learner-vs-professional/series/01-registry-intake/DiRegistryIntake.sleela
```

Do the same for the other three. Lessons are independent; the suggested order
is 1 to 4. See [`MANUAL.md`](./MANUAL.md) for user concerns and troubleshooting.

## Expected transcripts (key lines)

1. Lesson 1: the first `register` is refused with "declare the field of
   observation first"; the second `ball` is refused as a duplicate; `kite` is
   "not observed". Tally: `accepted=3 refused=2 registered=3`.
2. Lesson 2: three of six submissions are refused, one each for an empty claim,
   unregistered evidence and wrong step order. Tally:
   `good=3 refused=3 last accepted step=3`.
3. Lesson 3: verdicts go from "NOT ready, 5 check(s) to fix" to "NOT ready, 3"
   to "the instructor is ready to receive input".
4. Lesson 4: first comparison has 2 differences; after the student
   re-observes, the verdict is "AGREE, work accepted".

## Conventions

- Every class carries the Metadata Class metapattern (baseline identity).
- A reading of `-1` means "not observed". It is never guessed or counted as ready.
