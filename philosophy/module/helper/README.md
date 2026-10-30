# Module Helpers

This directory is reserved for small, deterministic helpers shared by Philosophy modules.

## Planned responsibilities

- Validate numeric values, timestamps, units, and event identifiers.
- Calculate event relevance from an explicit half-life.
- Check that longitudinal comparisons use compatible keys, units, and domains.
- Normalize display formatting without changing source values.
- Resolve related-event links as navigation/reflection links, never as independent corroboration.
- Apply consent and domain filters before creating a Screen Map.

## Engineering rules

Helpers should be side-effect-light, testable, and explicit about errors. They must not guess missing values, infer causal relationships, assign intelligence or maturity scores, or classify a person's beliefs as a psychiatric condition.

The existing implementations remain in `../../event_tree.c` and `../../screen_map.c` until a deliberate refactor moves code and updates all build references together.
