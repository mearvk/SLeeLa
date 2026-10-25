<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Core Library

**Status: the base contracts of the language.** This directory is the SLeeLa
`core` family — the small, foundational classes the language rests on: the
scalar value types (`SLString`, `SLInteger`, `SLBoolean`, `SLDouble`, …), the
base object contracts (`SLObject`, `SLCloneable`, `SLDisposable`,
`SLComparable`, …), the collection and concurrency primitives, and the service
and lifecycle contracts. See `SERVICE-STANDARD.md` for the service surface and
`CORE.BASE.md` (referenced from `SLString`) for the bootstrap-level rules.

Every class here carries the **Metadata Class metapattern** (the descriptive
`meta*` record with get/set accessors), with values faithful to the subject's
real origin — unknown values are `"none"`/`"unknown"`, never fabricated.

## Authorship and the Victory Model

Two companion classes express the *standing* of a work and its *wins*. Both are
author-marked (syntax 2.4; see `SLEELA.syntax` §18) and read as a pair.

### `SLAuthorship` — the authorship *Method : Class*

The standing of a work's author, in three honest registers:

- **Continuance** — the work is carried forward; triumph here is not conquest
  but continuance (`carryForward(work)`).
- **Dignity** — a modest, *protected* standing; an empty claim honestly becomes
  `"unverified"` rather than an inflated boast (`dignified(claim)`).
- **Free guidance** — direction offered, never imposed (`guide(toward)`,
  `imposes() == false`, `offersFreely() == true`).

On distinction, the class asserts **no measured intelligence score** for any
person. Where a figure appears it is an **illustrative historiometric estimate
with its provenance** (e.g. the Cox 1926 estimate for Leonardo da Vinci, with an
explicit caveat that it is retrospective, not a measurement, and not a claim
about this work's author). A social-theory note is carried as an *attributed,
contested* position; `ranksPersons() == false`.

### `SLVictory` — the Victory Model

A **win** is a **check that resolves to an exact item**. Resolution is **exact
equality** — the one exact test SLeeLa's String surface provides — against a
declared item, measured by a named **calculus** (a citable reference standard;
the default is the U.S. standard, citing NIST), in the direction
**Humanities → Science**. "Means are goals and directions without worship
elsewhere": a victory stands on its own check, names no idol, and ranks no
person.

```
SLVictory v = new SLVictory();
v.declare("continuance");          // the one exact item the victory is defined against
v.resolves("continuance");         // true  -- the dependable boolean win-test
v.check("continuance");            // records a win and returns a self-standing token
v.check("something-else");         // pending -- does not resolve, not counted
```

| Method | Meaning |
|--------|---------|
| `declare(item)` | name the one exact item the victory is defined against |
| `resolves(candidate)` | boolean win-test: `candidate == item` (no recording) |
| `check(candidate)` | resolve **and** record; returns a win or pending token |
| `declareAndCheck(target)` | the single-step "means *is* the goal" win |
| `direction()` | the orientation advanced (`Humanities -> Science`) |
| `theCalculus()` | the named reference standard (default cites NIST) |

Both classes keep the same promises as the rest of `core`: honest values, no
fabricated claims, and no ranking of persons.

## Validating core classes

A `core` class checked alone reports *no `main` method found* — that is
expected. Validate through a sibling demo with a `main()` (the compiler
auto-links sibling `.sleela` files in a directory). The source-built compiler
(`impl/build/sleela`) is required for `#sleela 2.x` sources. After editing any
`lib/**` file, regenerate `security/sha256-manifest.json` and keep
`lib/LIBRARY.SYMBOLS.md` in lockstep via `tools/generate-library-symbols.py`
(the inventory test enforces this).
