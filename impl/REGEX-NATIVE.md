# SLeeLa Regex Native Foundation

**Regex API:** 1.0.0 draft  
**Native layer:** C11

## Implemented now

The native foundation provides Level 1 literal `contains`, `starts`, `ends`, and `exact`, plus Level 2 ASCII `digit`, `word`, `space`, and `any` with `+`, `*`, `{n}`, and `{n,m}`.

The result includes match status and byte offsets. Levels 3 and 4 currently return an explicit unsupported status until their structural, capture, resource-limit, and Unicode semantics are finalized.

## Unicode boundary

The first native Level 2 predicates are intentionally ASCII-oriented. They do not claim full Unicode character-property semantics. Level 4 reserves explicit Unicode properties and advanced expression behavior for the completed contract.

## Safety

The foundation uses a small forward scanner for its executable subset. It has no network, file, process, or code-execution side effects.

## Smoke test

`impl/tests/core/regex_smoke.c` exercises Level 1 and Level 2 and verifies that Level 3 is reported as unsupported rather than silently misinterpreted.
