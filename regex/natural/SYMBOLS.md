# SLeeLa Regex Natural Form — Symbols

Version: 1.1.0-dev

Natural Form is the portable, user-facing regex language for SLeeLa. This core vocabulary is finite and normative.

## Atoms

| Form | Meaning |
|---|---|
| `any` / `·` | one character |
| `digit` / `#` | one decimal digit |
| `letter` / `@` | one letter |
| `space` / `_` | one whitespace character |
| quoted text | literal text |
| `[abc]` | one character from a set |
| `[^abc]` | one character outside a set |

## Quantity

| Form | Meaning |
|---|---|
| `one` / `=` | exactly once |
| `optional` / `?` | zero or one |
| `some` / `+` | one or more |
| `many` / `*` | zero or more |
| `{n}` | exactly n |
| `{n,m}` | n through m |

## Grouping and logic

| Form | Meaning |
|---|---|
| `(...)` | capture group |
| `<name: ...>` | named capture |
| `(?:...)` | non-capturing group |
| `|` / `or` | alternative |

## Anchors

| Form | Meaning |
|---|---|
| `begin` / `^` | beginning of input |
| `end` / `$` | end of input |

Unknown words MUST be rejected. Engine-specific constructs are extensions, not hidden additions to the core language.
