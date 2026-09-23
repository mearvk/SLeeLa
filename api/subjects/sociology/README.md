# Sociology Subject Library

Sociology is a SLeeLa Subject Library for quantitative description of populations, groups, distributions, rates, transitions, association measures, inequality and group separation.

It is intentionally descriptive. A statistic does not by itself establish causation, mechanism, intent, or normative value.

## Native mathematics

The C ABI provides proportion, rate, arithmetic mean, sample variance, weighted mean, growth rate, risk ratio, odds ratio, Gini coefficient, transition probability, and a two-group dissimilarity index.

The C++ facade exposes the same operations as sleela::sociology::Model. The native frontend contains a bounded first vocabulary for proportion, rate, growth and transition calculations.

## Evidence discipline

Sociological observations retain population, time, variable, value, unit, source and provenance. Observed, specified, derived, modeled, inferred and assumed quantities remain distinct.

The library does not encode a causal conclusion merely because a relation or statistical measure is computed. Sampling frame, missingness, weighting, uncertainty and study design belong in the model/procedure/evidence layer.

## Source-addition rule

A new sociology operation should enter executable source only after its definition, inputs, units or dimensions, domain restrictions, numerical behavior, interpretation limits and focused tests are documented.
