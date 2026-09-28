# Natural Form Grammar

Version: 1.1.0-dev

The grammar is intentionally finite. Implementations MUST reject unknown core words rather than guessing their meaning.

```text
pattern      := sequence (OR sequence)*
sequence     := term*
term         := atom quantifier?
atom         := literal | any | digit | letter | space | charset | group | named | anchor
quantifier   := one | optional | some | many | exact | range
exact        := "{" integer "}"
range        := "{" integer "," integer "}"
group        := "(" pattern ")" | "(?:" pattern ")"
named        := "<" name ":" pattern ">"
OR           := "|" | "or"
anchor       := begin | end | "^" | "$"
```

Whitespace outside quoted literals is insignificant in expanded notation. Implementations should preserve source offsets for diagnostics.
