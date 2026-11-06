# Debugger examples — what to learn here

These examples teach the **thinking** behind debugging, expressed as small
deterministic programs. They do not attach to a live process; instead they model
the techniques a debugger helps you apply, so you can see the logic clearly and
run it anywhere.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-debugging-checklist.sleela`](BASIC-debugging-checklist.sleela) | A repeatable checklist: reproduce → capture → inspect → change one thing → rerun. |
| Moderate | [`MODERATE-bisect-regression.sleela`](MODERATE-bisect-regression.sleela) | Binary-search the history to find the first bad revision in ~log₂(n) tests. |
| Advanced | [`ADVANCED-invariant-harness.sleela`](ADVANCED-invariant-harness.sleela) | A mini assertion harness that checks invariants and reports the first failure. |

## What we want you to take away

1. **Change one thing at a time.** The single most effective debugging rule, and
   step 4 of the checklist. If you change two things and the bug disappears, you
   have learned nothing about which one mattered.
2. **Bisection beats scanning.** The moderate example finds the bug-introducing
   revision among 100 in 7 comparisons, not 100. The only requirement is a
   reliable yes/no "is this version bad?" test — exactly what `git bisect run`
   automates. Note the loop invariant kept in the comments: `lo` is always good,
   `hi` is always bad.
3. **Make failure specific.** The advanced harness does not just say "something
   broke" — it names the first failing check (`triangular(3) == 7`). Good tests
   localize a bug to one expectation, which is where a debugger then earns its
   keep. (That last check is wrong on purpose, to show a reported failure.)

## Practical notes for SLeeLa 1.7

- **No `break`.** The examples end a `while` early by advancing the index to its
  limit; the bisect loop instead shrinks its range until `hi - lo == 1`.
- **String state sentinels.** The harness tracks "no failure yet" with the
  literal `"none"` and compares with `==`/`!=` (the only String operators at this
  level).

## Run them

```sh
sleela run lib/examples/debugger/MODERATE-bisect-regression.sleela
sleela run lib/examples/debugger/ADVANCED-invariant-harness.sleela
```

Output is deterministic: bisect reports first-bad-revision 42 in 7 steps; the
harness reports 4 passed, 1 failed, naming the intentional failure.
