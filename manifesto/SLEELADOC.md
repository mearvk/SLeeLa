# SleelaDoc — the SLeeLa documentation brand

**SleelaDoc** is SLeeLa's in-source documentation convention: structured
doc-comments that make a `.sleela` **Wrapper™** self-describing. It is to a
Sleela source file what Javadoc is to Java — but it documents *intent* (the
Sleela mandate is "express intent, not mechanism"), not just signatures.

SleelaDoc is a **comment convention**, so it is fully compatible with the
compiler as it exists today: every SleelaDoc block is an ordinary `/** ... */`
or `///` comment and is ignored by the lexer. A future `sleeladoc` extractor can
parse these blocks out of any Wrapper™ without running it.

## Block forms

| Form | Meaning |
|------|---------|
| `/** ... */`         | A **SleelaDoc block** — attaches to the class, struct, field, or method that immediately follows it. |
| `///`                | A **SleelaDoc line** — a single-line doc comment. |
| `/*! page N: Title */`| A **page banner** — marks a numbered page of a long-form document (a manifesto, spec, or essay carried in source). |

Ordinary `//` and `/* */` comments remain non-doc implementation notes.

## Tags

A SleelaDoc block is free prose plus optional `@tags`. The brand's tag set:

| Tag | Purpose |
|-----|---------|
| `@title`     | The name of the work or unit. |
| `@intent`    | *What* this expresses (never *how*). |
| `@axiom`     | A stated first principle taken as given. |
| `@thesis`    | A claim the unit argues or demonstrates. |
| `@invariant` | A condition that must always hold. |
| `@proof`     | The reasoning or runnable demonstration that backs a thesis. |
| `@congruence`| A structural sameness linking this unit to another (see `SHEET.sheet`). |
| `@evidence`  | Status of a statement: OBSERVED / DERIVED / MODELED / ASSUMED. |
| `@to`        | The intended reader. |
| `@see`       | A cross-reference. |

## Example

```java
/**
 * @title    Reason
 * @intent   Model the smallest unit of thought that can be checked.
 * @axiom    A claim that cannot be checked is not yet knowledge.
 * @thesis   Structured doubt increases the value of a belief.
 * @proof    reason.check() runs the demonstration and prints the result.
 * @evidence DERIVED
 */
class Reason {
    /// The confidence, in [0,100], after checking.
    int confidence;
}
```

## The manifesto

[`manifesto.sleela`](manifesto.sleela) is the reference long-form SleelaDoc
document: a runnable ~25-page **"Manifesto to Thinking Kind."** Every page is a
`/*! page N */` banner over real, compiling SLeeLa; the classes/structs/methods
*enact* the argument, and `main()` performs it end to end. It is both the
manifesto and its own proof-of-work.
