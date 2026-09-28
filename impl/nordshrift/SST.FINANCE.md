# `.sst` Finance Support — NS-SST-FIN-0001

Nordshrift `.sst` sheets have a first-class `finance` declaration block, parallel
to the `network` block (NS-SST-NET-0001). The finance declaration is parsed by
the same indentation-aware NS-SST parser as every other section, so finance
metadata is available to the transpiler without changing the source/target
grammar. It complements the financial integration described in
[`SST.FINANCIAL.md`](SST.FINANCIAL.md).

A `finance:` block is a compile-time declaration of the financial object series
a sheet uses, together with the conventions those objects require. It is not a
program: executable financial behavior is lowered by the Sleela front end to the
financial kernels (see `impl/frontend/financial_api.cpp` and
`impl/finance/financial.cpp`).

## Finance object series

The supported `FinanceObject` enum is closed to these components (the same set
as `SST.FINANCIAL.md`):

- `FutureValue`
- `PresentValue`
- `AnnuityPresent`
- `AnnuityFuture`
- `NetPresentValue`
- `BondPrice`
- `CAPM`
- `WACC`
- `Determinant2x2`
- `LinearSolve2x2`
- `QuadraticEquation`
- `Ratio`

Unknown object names are rejected with `NSS-E-FIN-001`.

## Period convention

Rates require a stated period convention before a quantity can be interpreted.
Supported values are:

- `annual`
- `semi-annual`
- `quarterly`
- `monthly`
- `continuous`

Unknown period conventions are rejected with `NSS-E-FIN-002`.

## Discounting

Time-value-of-money components apply discounting either discretely or
continuously:

- `discrete`
- `continuous`

Unknown discounting modes are rejected with `NSS-E-FIN-003`.

## Currency identity

Currency values require a currency identity. The `currency` directive records
it (e.g. `USD`); it is a free scalar and is not validated against a closed set.

## SST syntax

A complete finance declaration may be written directly in an `.sst` sheet:

```sst
finance:
  objects: [ FutureValue, PresentValue, AnnuityPresent, AnnuityFuture,
             NetPresentValue, BondPrice, CAPM, WACC,
             Determinant2x2, LinearSolve2x2, QuadraticEquation, Ratio ]
  currency:    USD
  period:      annual
  discounting: discrete
```

The singular forms are also supported for repeated declarations, and
`component`/`components` are accepted as aliases for `object`/`objects`:

```sst
finance:
  object: PresentValue
  object: NetPresentValue
  currency: EUR
  period: quarterly
  discounting: discrete
```

Duplicate object values are coalesced.

## Object structure

Every finance object is also carried on the repo-root `SHEET.sheet` catalog
(section `finance`, role `finance`), so a declared finance object has a home in
the shared Nordshrift object-compatibility / relevance structure alongside the
system and network objects. Each converts to a **model** relevance for every
target — it is realized as a modeled financial shape rather than a single target
keyword.

## Domain contract

Finance objects are computational models. A rate requires a stated period
convention; a currency value requires currency identity; discounting requires a
timing convention; and capital-pricing models require their assumptions. A
computed result is not automatically a forecast, an investment recommendation,
or an empirical fact. See the domain contract in
[`SST.FINANCIAL.md`](SST.FINANCIAL.md).

## Diagnostics summary

| Code | Meaning |
|---|---|
| `NSS-E-FIN-001` | unknown finance object |
| `NSS-E-FIN-002` | unknown finance period convention |
| `NSS-E-FIN-003` | unknown finance discounting |
