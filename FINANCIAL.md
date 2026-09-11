# SLeeLa Financial Mathematics Library

## Purpose

SLeeLa now has a dedicated `financial` native integration for financial mathematics, elementary algebra, valuation identities, and explanatory structure.

The external reference point is **QuantLib**, a mature free/open-source C++ quantitative-finance framework covering modeling, pricing, trading, and risk management. Its current project describes a clean C++ object model and a modified-BSD license. SLeeLa uses QuantLib as a conceptual benchmark only; no QuantLib source is copied into this repository. urlQuantLibhttps://www.quantlib.org/

For educational explanations of time value of money, SLeeLa follows the standard concepts of present value, future value, discount/growth rates, periods, and cash-flow timelines. OpenStax's *Principles of Finance* treats time value of money as a foundational finance concept. Its textbook content has its own licensing and attribution requirements, so this repository uses original explanatory language rather than reproducing its prose. urlOpenStax Principles of Financehttps://openstax.org/books/principles-finance/pages/index

## Source dependence

The dependency relationship is deliberately layered:

```text
Quantitative-finance concepts
        ↓
SLeeLa original formulas and algebraic kernels
        ↓
impl/finance/financial.cpp
        ↓
Sleela frontend lowering
        ↓
Sleela Core
```

The library does **not** require the external QuantLib binary to compile or run. This keeps `.sleela` artifacts self-contained after compilation.

## Import

```sleela
import math;
import financial;
```

## Executable API

### Time value of money

```text
financial.future_value(principal, rate, periods)
financial.present_value(future, rate, periods)
financial.annuity_present(payment, rate, periods)
financial.annuity_future(payment, rate, periods)
financial.npv4(rate, cashflow1, cashflow2, cashflow3, cashflow4)
```

The core relations are:

- `FV = PV(1+r)^n`
- `PV = FV/(1+r)^n`
- ordinary annuity PV = `P(1-(1+r)^-n)/r`
- ordinary annuity FV = `P((1+r)^n-1)/r`

### Valuation and capital

```text
financial.bond_price(face, coupon_rate, yield, periods)
financial.capm(risk_free, beta, market_return)
financial.wacc(equity_weight, debt_weight, cost_equity, cost_debt, tax_rate)
```

### Algebra

```text
financial.ratio(numerator, denominator)
financial.determinant2(a,b,c,d)
financial.solve2x2_x(a,b,c,d,e,f)
financial.solve2x2_y(a,b,c,d,e,f)
financial.quadratic_discriminant(a,b,c)
financial.quadratic_root_plus(a,b,c)
financial.quadratic_root_minus(a,b,c)
```

For the linear system

```text
ax + by = e
cx + dy = f
```

`det = ad-bc`, and the two solution functions use Cramer's rule when the determinant is nonzero.

## Explanation model

Every financial subject can be explained in a conserved order:

**Financial Subject → Quantity and Unit → Algebraic Relation → Financial Formula → Transformation → Result → Comparative Norm → Interpretation**

The purpose is to prevent a formula from becoming detached from the subject it describes.

## Prior Subject / IQ Conservator

Nordshrift may attach a semantic conservation record to a financial subject:

```text
PriorSubject
    ↓
CurrentSubject
    ↓
ReferenceSubject
    ↓
ComparativeNorm
```

The conservator records preserve:

- subject identity;
- quantity identity;
- unit identity;
- formula identity;
- assumptions;
- comparative ratios;
- algebraic invariants;
- result trace;
- explanation trace.

In this repository **IQ** denotes system insight/quality. It is not a human intelligence score.

## Source and artifact equivalence

The financial module is lowered during both source execution and artifact compilation:

```text
.sleela source
    ↓
Sleelvac financial lowering
    ↓
Sleela Core bytecode
    ↓
.sleela persistent artifact
    ↓
Sleela runtime
```

The runtime does not need to call the financial frontend after artifact generation.

## Nordshrift

Nordshrift has:

```text
impl/nordshrift/FINANCIAL.model
impl/nordshrift/IQ_CONSERVATORS.FINANCIAL.model
impl/nordshrift/SST.FINANCIAL.md
```

These define the financial integration components, semantic ordering, explanation order, relative meaning, and conservation records.

## Domain contract

These are mathematical models. They require explicit assumptions about periods, rates, timing, units, taxation, compounding, and cash-flow conventions. A numerical output is not automatically an empirical forecast or investment recommendation.

Functions that encounter a singular algebraic system, zero denominator, invalid discount factor, or invalid square-root domain should eventually expose a formal domain/error object rather than silently turning every failure into zero. The current dependency-light kernels retain conservative zero-return behavior in several low-level reference functions; this is documented as an area for the next numerical-contract pass.
