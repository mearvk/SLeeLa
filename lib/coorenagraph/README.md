<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="Debian logo">

<p align="center">
  <img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa logo" width="360">
</p>

# Coorenagraph — SLeeLa Moral-Grounding Contract

**Location:** `lib/coorenagraph/`  
**Primary source:** [`Coorenagraph.sleela`](Coorenagraph.sleela)

Coorenagraph is a small SLeeLa library model for representing two moral arguments alongside a grounding mechanism, then producing a structured truth result. Its current source defines a deliberately simple, invariant truth contract and includes smoke-style tests for Vile, Lively, and mixed argument inputs.

> **Important scope note:** The implementation currently returns `true` unconditionally. It is a formal contract/prototype, not a validated moral-reasoning engine, ethical authority, or factual judgment about any real person. A supplied grounding string is carried into the result; the source does not independently verify that grounding.

## Contents

- [Purpose and model](#purpose-and-model)
- [Source types](#source-types)
- [Evaluation flow](#evaluation-flow)
- [Current behavior and limitations](#current-behavior-and-limitations)
- [Tests](#tests)
- [Using the module](#using-the-module)
- [Engineering roadmap](#engineering-roadmap)
- [Repository links](#repository-links)

## Purpose and model

The module provides a minimal typed shape for an evaluation:

1. Accept a first `MoralArgument`.
2. Accept a second `MoralArgument`.
3. Accept a `GroundingMechanism`.
4. Return a `CoorenagraphTruth` result, with a Boolean truth field and a grounding string.

The source also names two example person-state labels, `Vile` and `Lively`, and includes a mixed-input test. These labels are test data in the current implementation; they should not be interpreted as classifications of real people.

## Source types

| Type | Fields | Role |
|---|---|---|
| `MoralArgument` | `String value` | Holds one argument label or value. |
| `GroundingMechanism` | `String value` | Holds the grounding description passed to the model. |
| `CoorenagraphTruth` | `boolean truth`, `String grounding` | Represents the produced truth flag and its grounding description. |
| `Coorenagraph` | Two arguments and one grounding mechanism | Stores inputs and exposes the evaluation methods. |
| `CoorenagraphTest` | Test methods | Exercises Vile, Lively, and mixed argument cases. |

### Main operations

- `Coorenagraph(first, second, grounding)` initializes the three input fields.
- `produceTruth()` constructs a result with `truth = true` and copies the current grounding value.
- `isTruth()` returns `true`.
- `isVileOrLively(personState)` checks whether the string equals `"Vile"` or `"Lively"`.
- `truthIsInvariant(personState)` returns `true` for every input path in the current implementation.
- `evaluate(first, second, grounding)` stores the provided inputs and returns `true`.

## Evaluation flow

```text
MoralArgument #1 ─┐
                  ├──> Coorenagraph ───> CoorenagraphTruth
MoralArgument #2 ─┤                         truth: true
                  │                         grounding: supplied value
GroundingMechanism┘
```

The diagram describes the current data flow, not a substantive proof or independent validation of the arguments.

## Current behavior and limitations

The current source is intentionally straightforward, but its behavior should be understood precisely:

- **Unconditional result:** `evaluate()`, `isTruth()`, and `truthIsInvariant()` return `true`; the argument values do not affect the decision.
- **Grounding is passed through, not verified:** `produceTruth()` copies the grounding string. It does not establish its provenance, validity, or adequacy.
- **No input validation:** Empty strings, missing semantic content, and unknown labels are not rejected by the current methods.
- **No contradiction analysis:** The two arguments are stored but are not compared for consistency, conflict, evidence, or logical entailment.
- **No real-person inference:** `Vile` and `Lively` are illustrative input strings, not a defensible rating system or a claim about an individual.
- **Tests are limited:** The included test methods check that the unconditional Boolean result is returned for three example combinations. They do not prove correctness of an ethical framework.

Accordingly, treat this module as a **data-model and behavior-contract prototype** until meaningful evaluation rules and stronger tests are implemented.

## Tests

The source includes these methods in `CoorenagraphTest`:

| Test | Inputs | Current expected result |
|---|---|---|
| `testVile()` | `Vile`, `Vile` | `true` |
| `testLively()` | `Lively`, `Lively` | `true` |
| `testMixedMoralArguments()` | `Vile`, `Lively` | `true` |

These are methods defined in the source, not evidence that a test runner has executed them. Run them through the repository's supported SLeeLa compiler/test workflow once the relevant test harness is available. Do not report them as passing unless they have actually been run.

## Using the module

The canonical source is:

- [`lib/coorenagraph/Coorenagraph.sleela`](Coorenagraph.sleela)

The source defines the model types and test methods in one file. Consult the repository's current [language syntax](../../SLEELA.syntax), [native implementation and build guide](../../impl/README.md), and [testing policy](../../markdown/TESTING.md) for supported syntax and execution commands. Build and test commands can vary with the active compiler/runtime version; no module-specific command is asserted here without a verified target.

## Engineering roadmap

For a more robust and scientifically reviewable implementation, prioritize:

1. **Define semantics:** Specify what “moral argument,” “grounding,” and “truth” mean in this module, including the intended domain and explicit non-goals.
2. **Make decisions evidence-sensitive:** Replace unconditional `true` returns with a documented rule that consumes both arguments and grounding data.
3. **Validate inputs:** Define behavior for empty, malformed, unknown, contradictory, or absent values; return explicit errors or a typed evaluation status where appropriate.
4. **Preserve provenance:** Represent grounding source, evidence, assumptions, and uncertainty rather than storing only an unstructured string.
5. **Separate output states:** Distinguish at least supported, refuted, indeterminate, and invalid input if the domain requires those outcomes; do not conflate “not disproved” with “true.”
6. **Expand tests:** Add positive, negative, boundary, contradiction, invalid-input, and property/invariance tests. Ensure tests fail when an input that should matter is changed.
7. **Document limitations:** Keep value judgments, factual claims, and formal properties separate; avoid presenting output as an objective verdict on a person.

These are recommendations for future work, not claims that the features already exist.

## Repository links

- [Coorenagraph source](https://github.com/mearvk/SLeeLa/blob/master/lib/coorenagraph/Coorenagraph.sleela)
- [SLeeLa repository](https://github.com/mearvk/SLeeLa)
- [SLeeLa documentation index](../../docs/README.md)
- [SLeeLa testing policy](../../markdown/TESTING.md)

---

SLeeLa is developed by **MEARVK LLC**.
