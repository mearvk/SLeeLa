# Nordshrift Financial Integration

Nordshrift treats the SLeeLa financial library as an integration component rather than as an opaque external dependency.

## Integration path

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

The same financial operation therefore has a source form and a persistent artifact form.

## Financial components

- FutureValue
- PresentValue
- AnnuityPresent
- AnnuityFuture
- NetPresentValue
- BondPrice
- CAPM
- WACC
- Determinant2x2
- LinearSolve2x2
- QuadraticEquation
- Ratio

The executable subset is intentionally dependency-light and uses the existing Sleela math kernels. The standalone `impl/finance/financial.cpp` provides a conventional C++ reference implementation for the same core identities.

## Mathematical basis

The architecture follows the broad scope of quantitative-finance software represented by QuantLib, which describes itself as a free/open-source C++ framework for quantitative finance, modeling, trading, and risk management. QuantLib is modified-BSD licensed. SLeeLa does **not** vendor or copy QuantLib source; this integration implements an original dependency-light subset. urlQuantLib projecthttps://www.quantlib.org/

Time-value-of-money explanations follow standard finance mathematics: present and future value, discount/growth rates, periods, and cash-flow timelines. OpenStax identifies time value of money as a foundational finance concept. SLeeLa's documentation uses original explanations and formulas rather than copying textbook prose. urlOpenStax Principles of Financehttps://openstax.org/books/principles-finance/pages/index

## IQ Conservators

Nordshrift may preserve financial meaning across transformations with the conservator chain:

**PriorSubject → CurrentSubject → ReferenceSubject → ComparativeNorm**

The conservators preserve:

- subject identity;
- quantity and unit identity;
- formula identity;
- assumptions;
- comparative ratios;
- algebraic invariants;
- result traces;
- explanation traces.

“IQ” is used here in the repository's existing system-insight/quality sense. It is not a psychometric measurement.

## Explanation order

Financial descriptions should preserve this order:

**Financial Subject → Quantity and Unit → Algebraic Relation → Financial Formula → Transformation → Result → Comparative Norm → Interpretation**

This lets Nordshrift retain the meaning of a financial subject even when its implementation target changes between Sleela, Java, and C.

## Domain contract

Financial functions are computational models. Rates require a stated period convention; currency values require currency identity; discounting requires a timing convention; and capital-pricing models require their assumptions. A computed result is not automatically a forecast, investment recommendation, or empirical fact.
