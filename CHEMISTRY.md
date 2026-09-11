# Sleela Chemistry

## Purpose

Sleela Chemistry provides a machine-oriented chemistry foundation for elemental identity, chemical relationships, experimental observations, structural hypotheses, ratios, similarity norms, signals, bonds, valence constraints, causal ordering, and bounded inference.

The library distinguishes **observed**, **derived**, and **inferred** properties. A presumed property is never promoted to experimental fact merely because the model assigns it a high score.

## Scientific grounding

The elemental registry is organized around the internationally recognized periodic-table framework. IUPAC maintains the periodic table, standard atomic-weight work, nomenclature, terminology, and procedures relevant to recognizing and naming new elements. The project should use IUPAC standards as the nomenclature and identity layer.

For future measured-property ingestion, the intended reference hierarchy includes authoritative chemistry resources such as IUPAC, NIST Chemistry WebBook, and PubChem. External data should retain provenance, units, source date, and uncertainty rather than being copied into an undifferentiated fact pool.

## Native import

```sleela
import math;
import chemistry;
```

The chemistry frontend is lowered before Sleela Core bytecode generation. The first executable chemistry calls are evidence-oriented numerical operations:

```text
chemistry.ratio(numerator,denominator)
chemistry.similarity(a,b)
chemistry.stochastic(seed,weight)
chemistry.inference_level(observation,bond,valence,similarity,signal,symmetry)
chemistry.confidence(observation,bond,valence,similarity,signal,symmetry)
chemistry.uncertainty(observation,bond,valence,similarity,signal,symmetry)
```

`chemistry.inference_level(...)` produces a bounded 0..24 model-support score. It is not a probability of truth and is not a substitute for laboratory validation.

## Normed Presumed Chemistry (NPC)

The NPC model is designed for a chemical subject that is experimental, newly proposed, incompletely characterized, or otherwise outside the established reference corpus.

The intended chain is:

**subject → observation → measurement → structure → bond → valence → geometry → electronic state → reaction relationship → compound-family similarity → thermodynamic consequence → astronomical context**

This is an evidence-ordering contract. A later inference must not silently be treated as though it were an earlier observation.

## Ratio norms

The fundamental comparison object is a relation rather than an isolated bond:

- mass ratio
- charge ratio
- valence ratio
- bond compatibility ratio
- electronegativity ratio
- geometry ratio
- signal ratio
- symmetry ratio
- compound-family similarity ratio

A ratio may be undefined. Division by zero therefore produces an explicit zero/uncertain computational result rather than a fabricated relationship.

Similarity means relational compatibility; it does not mean chemical identity.

## Bonds and valences

Bonds are evidence-bearing structural hypotheses. Valence and charge-balance constraints act as stronger structural filters than superficial similarity. The model therefore gives valence and direct observation greater weight than family similarity or symmetry.

The current executable evidence weighting is:

| Evidence | Weight |
|---|---:|
| Observation | 0.24 |
| Bond | 0.20 |
| Valence | 0.22 |
| Similarity | 0.13 |
| Signal | 0.12 |
| Symmetry | 0.09 |

These are project-model weights, not universal chemical laws. They are intentionally inspectable and can be revised as the chemistry object model develops.

## Signals and conferrers

A **signal** is an experimental or observational feature supporting a candidate interpretation. A **conferrer** is an independent relation that supports the same interpretation, such as periodic position, valence consistency, structural analogy, or convergent spectral evidence.

A candidate should gain support from independent convergent evidence. One weak signal should not be allowed to manufacture a complete molecular structure.

## Stochastic evaluation

The executable chemistry layer contains a small deterministic seeded perturbation for ranking competing hypotheses. The perturbation is deliberately bounded and deterministic for reproducibility.

It may help order candidates when several models are close. It must never manufacture an observation, erase a contradiction, or turn an inferred property into a measured property.

## 0..24 inference norms

| Level | Norm |
|---:|---|
| 0 | Raw subject; insufficient evidence |
| 1–4 | Direct observations dominate |
| 5–8 | Elementary structural inference |
| 9–12 | Valence and bonding inference |
| 13–16 | Similarity and compound-family inference |
| 17–20 | Multi-property convergent inference |
| 21–23 | Strong convergent model support |
| 24 | Maximum model-supported inference; never experimental proof |

The 0..24 scale is a project semantic scale. It is not an IQ scale, probability scale, educational credential, or scientific certainty scale.

## Normed titles

A future `chemistry.describe(subject)` layer should generate 2–8 paragraphs according to available evidence, with titles ordered approximately as:

1. **Chemical Subject**
2. **Identity and Composition**
3. **Structural Presumption**
4. **Bond and Valence Norms**
5. **Signals and Similarity**
6. **Presumed Properties**
7. **Inference Status**
8. **Astronomical Norm**

The astronomy section is intended to connect the subject to its elements' periodic and astrophysical context where scientifically supported: elemental origin, stellar nucleosynthesis, planetary chemistry, or cosmic abundance. It must not imply that an arbitrary laboratory compound has an independently established astronomical origin.

## Data provenance

The first-party project registry contains the 118 element identities. Future measured property tables should carry:

```text
subject
formula
property
value
unit
source
source_date
uncertainty
measurement_or_derivation
```

The project should prefer authoritative, versioned sources. IUPAC's periodic-table and nomenclature resources provide the identity and terminology layer; NIST provides critically evaluated chemical-property and spectroscopic data; PubChem provides a large programmatic chemical-information corpus.

## Architecture

```text
chemical subject
      ↓
observations / experimental signals
      ↓
normalization
      ↓
bonds + valences + structure candidates
      ↓
ratios + similarity norms
      ↓
causal-symmetry ordering
      ↓
stochastic candidate ranking
      ↓
0..24 model-support score
      ↓
bounded presumed properties
      ↓
2–8 paragraph normed description
      ↓
periodic / astronomical context
```

The compiled execution boundary remains:

**Sleela source → chemistry lowering → native lowering → Sleela Core bytecode → runnable `.sleela` artifact**

The runtime therefore receives executable Core instructions rather than requiring the chemistry frontend to be present at runtime.

## Current source tree

```text
impl/chemistry/chemistry.h
impl/chemistry/chemistry.cpp
impl/chemistry/periodic_table.model
impl/chemistry/presumed_chemistry.model
impl/frontend/chemistry_api.h
impl/frontend/chemistry_api.cpp
impl/examples/chemistry_presumed.sleela
```

The chemistry C++ library contains the reusable norm/scoring model. The frontend layer lowers executable chemistry calls into ordinary Core methods. The declarative model files preserve the conceptual chemistry contract independently of the compiler implementation.
