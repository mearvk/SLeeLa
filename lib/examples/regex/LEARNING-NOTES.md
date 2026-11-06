# Regular-expression examples — what to learn here

These examples teach **pattern matching from first principles**. SLeeLa 1.7 has
no regex intrinsic and its `String` type supports only `+`, `==`, and `!=` — no
`length()`, no `charAt()`. So instead of pretending, these examples represent
text and patterns as **arrays of character codes (ints)** and implement matching
honestly. You learn how matching actually works, not just how to call a library.

Read them in order.

## The cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-literal-match.sleela`](BASIC-literal-match.sleela) | Exact whole-string compare over code arrays. |
| Moderate | [`MODERATE-wildcard-match.sleela`](MODERATE-wildcard-match.sleela) | Glob matching with `?` (any one char) and `*` (any run) using two-pointer backtracking. |
| Advanced | [`ADVANCED-anchored-classes.sleela`](ADVANCED-anchored-classes.sleela) | Character classes + one-or-more, anchored: validate digits (`^[0-9]+$`) and identifiers (`^[A-Za-z_]\w*$`). |
| Draft | [`DRAFT-literal-find.sleela`](DRAFT-literal-find.sleela) | A *proposed* readable regex vocabulary. **It does not pass `check`** — it is intentionally a sketch of an API, not an implemented one. |

## What we want you to take away

1. **A matcher is a loop over characters.** Every regex engine, underneath, walks
   the input and the pattern together. Seeing this on plain int arrays
   demystifies the whole topic.
2. **`*` needs backtracking.** The moderate matcher remembers where the last `*`
   was and how much text it had consumed, so on a later mismatch it can let the
   `*` swallow one more character and try again. That remember-and-retry move is
   the heart of glob (and simple regex) matching. Here `-1` encodes `?` and `-2`
   encodes `*` inside a pattern array.
3. **Character classes are just range tests.** The advanced example builds
   `isDigit`, `isAlpha`, `isUnder` from ASCII code ranges, then composes them —
   exactly what `[0-9]` and `[A-Za-z_]` mean. "Anchored" means the *whole* input
   must match, which is why each validator loops to the end and fails on any bad
   character.
4. **Drafts are honest about being drafts.** The `DRAFT-` file shows a nicer API
   we might want, but it is labeled clearly and is not claimed to compile. Do not
   copy a draft into real code expecting it to run.

## Practical notes for SLeeLa 1.7

- **Why int arrays, not String?** Without `length()`/`charAt()`, you cannot index
  into a `String`. Modeling text as `int[]` of character codes is the honest way
  to do character-level work at this language level. A production matcher would
  use the text library's string type via the native bridge.
- **ASCII codes** used here: digits `48..57`, `A..Z` `65..90`, `a..z` `97..122`,
  underscore `95`, `.` `46`.

## Run them

```sh
sleela run lib/examples/regex/MODERATE-wildcard-match.sleela
sleela run lib/examples/regex/ADVANCED-anchored-classes.sleela
```

Output is deterministic: `report.txt` matches `*.txt` and `re?ort*` but not
`*.csv`; `2048` is all digits; `sleela_v2` is a valid identifier but `2bad` is
not. (`DRAFT-literal-find.sleela` is expected to fail `check` by design.)
