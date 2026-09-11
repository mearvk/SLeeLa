# ECONOMICS.md

## Purpose

`ECONOMICS.md` is the primary reference for the executable Economics domain in SLeeLa. It treats economic relationships as explicit algebraic models with stated assumptions rather than as automatic forecasts or empirical conclusions.

## Executable API

```text
 economics.future_value(principal,rate,periods)
 economics.present_value(future,rate,periods)
 economics.annuity_present(payment,rate,periods)
 economics.annuity_future(payment,rate,periods)
 economics.elasticity(pct_quantity,pct_price)
 economics.real_rate(nominal,inflation)
 economics.fisher_nominal(real,inflation)
 economics.doubling_time(rate)
 economics.continuous_value(principal,rate,time)
 economics.profit(revenue,cost)
 economics.margin(profit,revenue)
 economics.gdp_identity(C,I,G)
```

## Formula Reference

```text
FV = P(1+r)^n
PV = F/(1+r)^n
AnnuityPV = payment * (1 - 1/(1+r)^n) / r
AnnuityFV = payment * ((1+r)^n - 1) / r
Elasticity = pct_quantity / pct_price
real_rate = (1+nominal)/(1+inflation) - 1
nominal = (1+real)*(1+inflation) - 1
doubling_time = ln(2) / ln(1+r)
continuous_value = P*exp(r*t)
profit = revenue - cost
margin = profit / revenue
GDP_identity = C + I + G
```

The current GDP identity is intentionally the implemented `C + I + G` subset; a future expansion can include net exports explicitly.

## Modeling Contract

Economic quantities should retain their period, rate convention, unit, and assumptions. A numerical model can describe a specified scenario without becoming a prediction of the real economy.

The preferred explanation order is:

**Economic Subject → Quantity and Unit → Algebraic Relation → Economic Formula → Transformation → Result → Comparative Norm → Interpretation**.

## Comparative Meaning

Economic comparisons should preserve the identity of the reference subject and distinguish current values from prior values, reference values, and comparative norms. Ratios are useful only when numerator, denominator, timing, and units are compatible.

## Cross-Domain Role

Economics builds on the executable Math domain for powers, logarithms, and exponential transformations. This allows economic formulas to execute through the same Sleela Core mechanism used by other native domains.

## Status

The current scalar Economics API is integrated with Sleelvac and persistent `.sleela` artifact generation. Expanded time-series structures, econometric objects, statistical inference, uncertainty propagation, and richer macro/microeconomic models remain future extensions.
