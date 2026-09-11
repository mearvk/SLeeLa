# FINANCIAL.md

## Purpose

`FINANCIAL.md` is the primary reference for SLeeLa's executable Financial domain. It provides a dependency-light quantitative-finance core with explicit algebra, assumptions, and explanation traces.

The design uses **QuantLib** as a conceptual benchmark for the breadth of professional quantitative-finance software. No QuantLib source is copied into SLeeLa; the formulas and lowering layer are original project implementation.

## Executable API

```text
financial.future_value(p,r,n)
financial.present_value(f,r,n)
financial.annuity_present(p,r,n)
financial.annuity_future(p,r,n)
financial.npv4(r,c1,c2,c3,c4)
financial.bond_price(face,coupon,yield,n)
financial.capm(rf,beta,rm)
financial.wacc(ew,dw,ce,cd,tax)
financial.determinant2(a,b,c,d)
financial.solve2x2_x(a,b,c,d,e,f)
financial.solve2x2_y(a,b,c,d,e,f)
financial.quadratic_discriminant(a,b,c)
financial.quadratic_root_plus(a,b,c)
financial.quadratic_root_minus(a,b,c)
financial.ratio(numerator,denominator)
```

## Formula Reference

```text
FV = P(1+r)^n
PV = F/(1+r)^n
AnnuityPV = P(1-(1+r)^(-n))/r
AnnuityFV = P((1+r)^n-1)/r
NPV4 = c1/(1+r) + c2/(1+r)^2 + c3/(1+r)^3 + c4/(1+r)^4
BondPrice = face*coupon*((1-(1+yield)^(-n))/yield) + face/(1+yield)^n
CAPM = rf + beta*(rm-rf)
WACC = ew*ce + dw*cd*(1-tax)
Determinant2 = a*d - b*c
```

The two-by-two solver uses Cramer's rule. Quadratic roots use the standard discriminant and square-root transformation.

## Domain Conditions

Financial formulas depend on timing, rate convention, compounding, units, tax treatment, and domain restrictions. The implementation should not silently turn an invalid domain into a valid economic conclusion.

Important current limitations include explicit handling for zero denominators, singular matrices, invalid quadratic discriminants, zero yields, and other boundary cases. These are identified as future numerical-contract improvements rather than hidden assumptions.

`npv4` represents four future cash-flow terms and does not include an initial cash flow. `bond_price` assumes `coupon` is a coupon rate applied to face value.

## Explanation Contract

The financial explanation order is:

**Financial Subject → Quantity and Unit → Algebraic Relation → Financial Formula → Transformation → Result → Comparative Norm → Interpretation**.

The system should preserve the difference between a calculated value and an empirical forecast, valuation opinion, or investment recommendation.

## IQ Conservator

The financial integration includes a semantic-preservation model for relative meaning:

```text
PriorSubject
  → CurrentSubject
  → ReferenceSubject
  → ComparativeNorm
```

The conservator records:

- `PriorSubject`
- `SubjectIdentity`
- `QuantityIdentity`
- `UnitIdentity`
- `FormulaIdentity`
- `AssumptionIdentity`
- `ComparativeRatio`
- `AlgebraicInvariant`
- `ResultTrace`
- `ExplanationTrace`

Here **IQ** means system insight/quality: preservation of inspectable relationships and explanation. It is not a psychometric score.

## Nordshrift Integration

Nordshrift represents the financial domain through components including:

- `FinancialSubject`
- `Algebra`
- `TimeValueOfMoney`
- `Valuation`
- `FixedIncome`
- `CapitalPricing`
- `FinancialRatio`
- `ExplanationChain`
- `IQConservator`

The intended path is:

```text
.sst financial model
  ↓
Nordshrift
  ↓
financial integration component
  ↓
Sleela source import
  ↓
Sleelvac financial lowering
  ↓
Sleela Core bytecode
  ↓
runnable .sleela artifact
```

## Source Example

```sleela
#sleela 1.0
import math;
import financial;

class FinancialExample {
    static double future() {
        return financial.future_value(1000.0, 0.05, 10.0);
    }

    static double determinant() {
        return financial.determinant2(4.0, 7.0, 2.0, 6.0);
    }
}
```

The current financial lowerer uses the executable Math helpers, so source programs should import `math` together with `financial` until automatic dependency insertion is implemented.

## Source and Artifact Equivalence

Financial source can be compiled by Sleelvac into a persistent `.sleela` artifact. The runtime loads the compiled Core representation directly. The artifact therefore preserves the executable result of the financial lowering without requiring a second source-compilation pass at runtime.

## Expansion Roadmap

The financial domain is intentionally a foundation rather than a claim to replace a complete professional library. Future additions include:

- IRR and arbitrary cash-flow arrays;
- duration, modified duration, and convexity;
- yield curves and discount factors;
- day-count and compounding conventions;
- Black-Scholes and Bachelier models;
- normal distributions and error functions;
- portfolio variance/covariance;
- Monte Carlo methods;
- stochastic processes and SDE support;
- richer vector and matrix types;
- fixed-income schedules;
- numerical risk measures.

## Status

The current scalar financial library, frontend lowering, example source, persistent artifact path, and Nordshrift declarative integration are implemented. Full local compilation and CI validation should be treated separately from the existence of these source integrations.
