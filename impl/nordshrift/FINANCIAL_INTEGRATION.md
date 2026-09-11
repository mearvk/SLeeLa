# Nordshrift Financial Integration

Nordshrift treats the SLeeLa financial library as a semantic integration component rather than as a collection of opaque function names.

## Subject order

```text
Financial Subject
 -> Quantity and Unit
 -> Algebraic Relation
 -> Financial Formula
 -> Transformation
 -> Result
 -> Comparative Norm
 -> Interpretation
```

## IQ Conservator

The `IQ Conservator` preserves relative meaning when a prior subject is transformed or compared with a current subject. It conserves identity, quantities, units, formulas, assumptions, ratios, algebraic invariants, result traces, and explanation traces.

IQ is an information/insight-quality conservation concept, not a psychometric score.

## Targets

The same semantic subject may be represented as:

- executable Sleela source;
- persistent runnable `.sleela` Core artifact;
- Nordshrift-emitted Sleela;
- Java output;
- C output.

The compiler must not imply that a generated target is experimentally or financially authoritative merely because it preserves syntax or algebra.

## Norms

Financial norms are ordered relationships rather than isolated numbers. Examples include:

- time-value ratios;
- valuation ratios;
- capital-cost relationships;
- leverage relationships;
- return relationships;
- algebraic invariants.

A downstream explanation should preserve the distinction between measured input, calculated result, model assumption, and interpretation.
