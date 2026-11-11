# How We Know It Is Right

Claiming code is correct is easy. Showing it takes several independent checks.

## Invariants

An **invariant** is a fact that must be true after every action. For a position
book: every lot has a positive quantity and price; shares held = shares bought
- shares sold; cost held = cost bought - cost sold. If one is broken, the book is
wrong.

## Refusals must change nothing

When a verb is refused (for example selling more than you hold), the book must be
exactly as before. Test it by recording shares, cost, fees and day before and
after.

## A shadow model

Write a second, different version: store **one entry per share** and sell by
removing the first entries. It shares no code with the lot-based version. Drive
both with the same random actions. After every step they must agree. This is
**differential testing**.

## A third opinion

Compute the expected numbers in a different language (Python) and compare with
what the program prints.

## Mutation checks

Break the code on purpose: change a "+ 1", flip a sign, allow one share too many.
A good test suite **fails**. If it still passes, the tests are not strong enough.
One mutation made a search loop that never ends, so the test runner needs a
**timeout**.

## Questions

1. Why is a shadow model more convincing than re-reading the code?
2. What does it mean if a deliberately broken program still passes the tests?

**Answers:** 1. It is independent, so the same mistake is unlikely to appear in
both. 2. The tests cannot see that kind of bug, so they need to be improved.
