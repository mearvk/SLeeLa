# SLeeLa Design Activity and Scientific Variance

## Purpose

SLeeLa treats **Design Goals as an executable activity**, not only as prose documentation. Each principal executable can expose a normalized engineering profile and compare that profile with a declared scientific activity domain.

This is an engineering conformance mechanism. It does not claim that a numerical result proves scientific validity or the quality of a scientific result.

Scientific measurement practice treats reproducibility and uncertainty as explicit properties of scientific work; NIST describes reproducibility in terms of substantially reproducible analysis and recommends quantitative treatment of uncertainty where applicable.

## Six dimensions

1. Correctness
2. Reproducibility
3. Observability
4. Safety
5. Resource discipline
6. Interoperability

Raw activity measurements are 0–100 and are normalized to 0–1.

## Variance

For a normalized executable vector `x` and a documented science reference vector `p`, SLeeLa records mean squared component variance, RMS deviation, and L2 distance.

The variance is a measurable dispersion from the selected reference profile. It is not a ranking of scientific disciplines and is not itself a measurement uncertainty estimate.

## Scientific domains

The initial reference set covers the SLeeLa subject model:

- General
- Mathematics
- Physics
- Chemistry
- Economics
- Inference
- Finance
- Data Analytics

Profiles are versioned in `impl/core/sleela_design_activity.c` so changes remain explicit and reviewable.

## Executable rule

A SLeeLa executable participating in Design Activity should be able to answer:

- What executable am I?
- What version of the design-activity schema do I implement?
- What scientific activity/domain am I serving?
- What are my six measured design dimensions?
- What normalized vector results?
- What reference profile was used?
- What variance resulted?

## C/C++ boundary

`impl/core/sleela_design_activity.h/.c` is a small C ABI module. `sleela` and `nordshrift` link the same implementation, preventing duplicate C++ definitions.

The module rejects non-finite values and out-of-range measurements and performs no dynamic allocation.

## Command forms

```text
sleela design-activity <science> <correctness> <reproducibility> <observability> <safety> <resource> <interoperability>
nordshrift design-activity <science> <correctness> <reproducibility> <observability> <safety> <resource> <interoperability>
```

Example:

```text
sleela design-activity physics 98 95 100 95 85 85
```

The command produces a machine-readable JSON activity record containing the schema version, executable, science domain, variance, RMS deviation, and L2 distance.

## Relationship to existing science support

The repository already contains native Math, Physics, Economics, Inference, Chemistry, and Finance subject implementations and a Data Analytics API. The Design Activity layer provides the common engineering measurement envelope around those activities.

## Norming versus scientific uncertainty

**Normed** here means normalized to a documented numerical scale. It does not mean that SLeeLa declares one discipline superior to another.

**Variance** here means mean squared deviation from a selected software-design reference vector. It does not replace scientific uncertainty analysis of an actual physical or experimental measurement. NIST's measurement guidance distinguishes measurement uncertainty from ordinary software conformance metrics.

## Adequacy of the native boundary

The C/C++ architecture already provides the important foundations for this activity: a stable C ABI at the VM core, bounded VM-owned handles, managed memory, platform abstraction layers, native subject modules, SHA-256 execution verification, and explicit security boundaries. The new Design Activity module is deliberately independent of the scientific calculation implementations, so it can measure the executable envelope without changing the scientific result itself.

## Status

- Schema version: 1
- C ABI: implemented
- C++ consumers: `sleela`, `nordshrift`
- Scientific reference domains: 8
- Engineering dimensions: 6
- Dynamic allocation in metric calculation: none
