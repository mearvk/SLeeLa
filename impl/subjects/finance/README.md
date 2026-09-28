# SLeeLa Financial Integration

The financial library provides executable financial mathematics and algebraic explanation primitives for Sleela. It is conceptually informed by mature quantitative-finance practice, including the QuantLib tradition, but contains original SLeeLa source rather than copied QuantLib implementation.

## Integration contract

```text
Sleela source
  -> import financial
  -> financial lowering
  -> math/algebra lowering
  -> Sleela Core bytecode
  -> persistent .sleela artifact
  -> Sleela Runtime
```

Nordshrift may preserve the same semantic subject when translating the program to Sleela, Java, or C.

## Subject conservation

Financial transformations preserve, where available:

- subject identity
- quantity and unit
- algebraic relation
- financial formula
- assumptions
- comparative ratios
- invariants
- result trace
- explanation trace

This is represented in Nordshrift by the **IQ Conservator** integration model. IQ here means information/insight quality preservation and is not a psychometric score.
