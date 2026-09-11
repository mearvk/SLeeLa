# CHEMISTRY.md

## Purpose

`CHEMISTRY.md` is the primary reference for the executable Chemistry integration in SLeeLa. The library is designed to represent chemical subjects, relationships, ratios, and inferred properties while maintaining a strict distinction between a computational model and experimentally verified observation.

## Integration Model

The chemistry path is:

```text
chemical subject
  → composition / properties
  → ratio and relationship model
  → bond / valence concepts
  → symmetry / structural relations
  → numerical inference
  → stochastic evaluation
  → result and uncertainty
```

The frontend adapter lowers chemistry calls into executable Sleela Core operations and uses the Math domain for numerical transformations.

## Subject Representation

A chemistry subject may be represented through:

- identity;
- formula or composition;
- elemental constituents;
- valence and bond relationships;
- structural or symmetry information;
- physical-chemical properties;
- ratios and comparative measures;
- signals and relational evidence;
- conferrers or supporting relationships;
- stochastic evaluation from `0–24` where a bounded ranking model is appropriate.

The `0–24` evaluation is a computational scoring/ranking mechanism. It is not a laboratory confidence measurement, scientific certainty scale, or psychometric score.

## Normed Explanation Order

Chemistry explanations should follow a stable title order:

1. **Subject Identity**
2. **Formula / Composition**
3. **Valence and Bond Norm**
4. **Symmetry / Structural Norm**
5. **Physical-Chemical Properties**
6. **Comparative Similarity**
7. **Astronomical / Elemental Origin**
8. **Inference and Uncertainty**

This order keeps the represented subject grounded before interpretation.

## Scientific Grounding

The astronomical component is intended in the physical-science sense: elemental origin, nucleosynthesis, stellar and planetary context, and other relevant physical models. It does not turn a symbolic or inferred property into a direct astronomical observation.

Chemical nomenclature, element identity, and property references should remain compatible with authoritative scientific conventions. Experimental or newly proposed subjects should be marked as modeled, specified, or inferred when their properties have not been independently verified.

## Ratio and Causal Ordering

The preferred causal ordering is:

**subject → quantities → units → assumptions → ratios → relationships → equation → transformation → result → interpretation**.

Ratios should preserve their numerator, denominator, unit compatibility, and comparison basis. Bonds and valence relationships should be represented as domain constraints rather than treated as arbitrary labels.

## Uncertainty

The chemistry layer should distinguish:

- `observed` — directly supplied observational evidence;
- `specified` — an explicit model input;
- `derived` — calculated from supplied information;
- `modeled` — generated under an explicit model;
- `inferred` — obtained through a comparative or probabilistic relationship.

This epistemic distinction is part of the integration contract.

## Cross-Domain Role

Chemistry depends on executable Math for ratios, powers, roots, and related numerical operations. It is integrated into source compilation and persistent `.sleela` artifact generation through Sleelvac.

## Status

The current integration provides a dependency-light computational chemistry model and frontend lowering. A future expansion can add a structured element registry, molecular/formula parsing, property datasets, explicit valence and bond inference, richer symmetry objects, dimensional typing, uncertainty propagation, and deeper astronomy/nucleosynthesis grounding.
