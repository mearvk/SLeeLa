# Wiggle™ Native Structural Models

Wiggle can infer a bounded structural profile from XML before a Bodi change is considered. The inference is structural and evidentiary: it does not claim application semantics, causation, or authorization to execute a change.

## Ten Native Models

| Model | Native interpretation |
|---|---|
| `TREE` | Parent/child hierarchy and nesting |
| `SEQUENCE` | Ordered elements and structural progression |
| `COLLECTION` | Repeated sibling elements forming a set/list |
| `RECORD` | Named fields or attributes describing an object |
| `REFERENCE` | IDs, keys, URIs, names, or external links |
| `STATE` | Status, phase, mode, or lifecycle information |
| `COMMAND` | Operation-like structure with target and datum |
| `DESCRIPTION` | Metadata/schema-like structure describing another structure |
| `RELATION` | Structure connecting identifiable objects |
| `STREAM` | Ordered recurring events or messages |

These are hypotheses about observable structure. They are not ten claims about what every XML vocabulary means.

## Inference Flow

```text
XML → secure parse → structural walk → native-model scores
   → ranked structural profile → bounded Wiggle searches
   → satisfaction/science note → validated candidate
   → optional Bodi middle verb → BodiWitness
```

`WiggleInference` intentionally stops before the change layer. It cannot silently turn a structural match into `install`, `connect`, `enable`, or another Bodi operation.

## Native Grammar

```text
TREE        root → parent → child
SEQUENCE    A → B → C
COLLECTION  A → {B, B, B}
RECORD      A → {field₁, field₂, field₃}
REFERENCE   A → identifier → B
STATE       object → state → transition
COMMAND     operation → target → datum
DESCRIPTION descriptor → describes → structure
RELATION    A → relation → B
STREAM      event₁ → event₂ → event₃ → ...
```

## Confidence

`WiggleStructuralProfile` exposes a confidence score from `0.0` through `0.99` for each native model and provides a ranked model list. These scores are heuristic evidence, not probabilities unless a separately calibrated statistical model is supplied.

The profile also records element count, attribute count, maximum depth, repeated element kinds, reference candidates, and bounded search count.

## Search Envelope

A normal concern can use approximately four to five searches: `trace`/`inspect`, `seek`/`scan`, `ascend`/`descend`, `correlate`/`tune`, and `confirm`/`question`. A simpler concern may legitimately be `1 → 1`; a larger topology may be `4–5 → 1 → 1 → 4–5 → satisfied output`. Search count is bookkeeping, not proof.

## Structural Safety

The parser uses namespace-aware DOM parsing with secure-processing and external-entity/DTD protections where supported by the JAXP implementation. XML input remains untrusted data.

```text
FOUND       ≠ VALIDATED
VALIDATED   ≠ CAUSAL
CAUSAL      ≠ EXECUTED
EXECUTED    ≠ OBSERVED SUCCESS
COUNTED     ≠ PROVEN
GAIN        ≠ PHYSICAL ENERGY
```

Terms such as `electron_gain` and `transferon_gain` remain software metaphors unless independent physical measurement establishes otherwise.

## Example

Given repeated `<service>` elements with `name`, `id`, and `state` attributes, Wiggle may produce strong `TREE`, `COLLECTION`, `RECORD`, `REFERENCE`, and `STATE` hypotheses. That profile can guide further XML-directed searches, but it does not by itself establish that a service should be started or connected.

> **Wiggle to discover; witness to establish; change to act; contain to preserve; propagate only when the boundary permits; and never let a counter claim more than its evidence.**
