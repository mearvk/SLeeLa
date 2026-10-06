# SLeeLa Economics

## Purpose

The native economics layer provides quantitative economic relationships for analysis, simulation, accounting models, markets, and macroeconomic systems. The **133+ → 181+** range is a project design marker for increasing mathematical and systems complexity, not a psychometric claim.

## Bridging the paralocations of logic

Economic reasoning crosses household, firm, market, financial, and macroeconomic representations. It also crosses accounting identities, behavioral assumptions, statistical estimates, and dynamic models. Sleela should keep these layers distinct:

**observation → measurement → accounting identity → behavioral model → equilibrium/system → estimate → simulation → interpretation**

The key logical bridge is that an identity can be true by definition while a behavioral equation remains an empirical hypothesis. The API should preserve that distinction.

## Core Constants

The `economics` module should expose documented constants and conventions rather than scattering magic numbers through programs:

- `EPSILON` — numerical comparison tolerance, configurable by context.
- `YEAR_DAYS` — documented calendar convention for annualized calculations.
- `BASIS_POINTS = 0.0001` — one basis point as a decimal rate.
- `GROWTH_EPSILON` — guard threshold for near-zero denominators.
- `LOG_BASE_E` — natural-log convention.
- `PI` may be imported from `math`; economics should not duplicate mathematical constants unnecessarily.

Economic quantities should carry units/conventions where practical: currency, currency/year, percentage, rate, quantity, index, real versus nominal, and seasonally adjusted versus unadjusted.

## Interest and Compounding

```text
FV = PV*(1+r)^n
PV = FV/(1+r)^n
FV = PV*e^(r*t)                 continuous compounding
EAR = (1 + r/m)^m - 1
```

API:

```sleela
import economics;

double future = economics.future_value(pv, rate, periods);
double present = economics.present_value(fv, rate, periods);
double ear = economics.effective_annual_rate(rate, compounds);
```

## Annuities and Cash Flows

For an ordinary annuity:

```text
PV = PMT * (1 - (1+r)^(-n)) / r
FV = PMT * ((1+r)^n - 1) / r
```

The API should also support irregular cash-flow schedules through an explicit cash-flow object rather than forcing users into a regular-period approximation.

## Growth and Inflation

```text
GDP_t = GDP_0*(1+g)^t
real_value = nominal_value / price_index
real_rate ≈ nominal_rate - inflation_rate
```

For exact Fisher-style conversion:

```text
1 + nominal_rate = (1 + real_rate)*(1 + inflation_rate)
```

The API should distinguish nominal and real quantities.

## Elasticity and Marginal Analysis

Point elasticity:

```text
E = (dQ/dP)*(P/Q)
```

Arc elasticity:

```text
E_arc = (ΔQ/average(Q)) / (ΔP/average(P))
```

Marginal concepts:

```text
MC = dTC/dQ
MR = dTR/dQ
profit = TR - TC
```

These functions should accept curve/model objects where possible rather than pretending every derivative is a scalar lookup.

## Supply, Demand, and Equilibrium

A simple linear representation:

```text
Q_d = a - bP
Q_s = c + dP
P* = (a-c)/(b+d)
Q* = a - bP*
```

The important API distinction is between a **model** and a **result**. A demand curve object should retain its coefficients and assumptions so an equilibrium result can be traced back to the model that produced it.

## Consumer and Producer Surplus

For suitable continuous curves:

```text
CS = ∫(WTP(Q) - P*) dQ
PS = ∫(P* - MC(Q)) dQ
```

Numerical integration should use the native mathematical integration facilities rather than requiring application code to reproduce quadrature algorithms.

## Present-Value Valuation and IRR

Net present value:

```text
NPV = Σ[t=0..n] CF_t/(1+r)^t
```

Internal rate of return solves:

```text
0 = Σ[t=0..n] CF_t/(1+IRR)^t
```

`economics.npv(...)` and `economics.irr(...)` should report convergence status and assumptions rather than returning an unexplained scalar when numerical solving fails.

## Probability and Statistics

Economic analysis depends heavily on probability and statistics. The economics module should therefore interoperate with the mathematical distribution and statistics objects defined by `math`:

```text
E[X] = Σ x*p(x)
Var(X) = E[(X-E[X])^2]
Cov(X,Y) = E[(X-E[X])(Y-E[Y])]
```

Regression, estimation, confidence intervals, and forecasting belong to the statistical layer but should be callable from economic model objects.

## Macroeconomic Identities

National accounts provide identities such as:

```text
Y = C + I + G + NX
NX = X - M
```

Money/price relationships can be represented as models, with assumptions explicitly stored:

```text
M*V = P*Y
```

The API must distinguish an accounting identity from an empirical or theoretical model.

## Optimization and Equilibrium

The advanced layer should support:

- constrained optimization;
- utility maximization;
- cost minimization;
- profit maximization;
- equilibrium systems;
- dynamic optimization;
- stochastic processes;
- numerical root finding;
- sensitivity and comparative statics.

## 181+ Systems Target

At the advanced level, economics becomes a systems discipline: multiple agents, constraints, expectations, time, uncertainty, feedback, and institutional rules interact. Sleela should represent these as explicit objects and transformations rather than flattening them into opaque formulas.

The intended bridge is:

**parameter → model → constraint → objective → equilibrium → trajectory → statistic → review**

This makes economic conclusions inspectable and allows a Review object to retain the model, data provenance, assumptions, and limitations behind a result.

## Native API

```sleela
import economics;

 double npv = economics.npv(cashflows, discount_rate);
 double growth = economics.compound_growth(initial, rate, periods);
 double real = economics.real_value(nominal, price_index);
```

Economic functions remain module-qualified. Mathematical primitives such as `sqrt`, `log`, and `exp` belong to `math`.

## Status

This document defines the economics API contract and formula scope. Runtime implementation should explicitly distinguish implemented functions from planned functions.
