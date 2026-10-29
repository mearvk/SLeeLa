# GC-Hint Semantics and Safety Boundaries

## Observable behavior

A hint requests collection work; it must not change the program's correct observable result. Values that remain reachable after the statement must remain usable. The stress fixtures provide regression witnesses for repeated hints while structs and arrays are created.

## Aggressiveness

The documented band is 0 through 100 inclusive. The fixtures describe 0 as a no-op and 100 as a full-collection request; intermediate values are runtime guidance, not a portable byte-count or latency guarantee. Bare `gc` requests a modest default as described in the fixture README.

## Liveness and reclamation

- Reachability, not the hint target's assigned value alone, determines whether an object can be reclaimed.
- A collection hint must preserve live object graphs.
- A successful hint does not guarantee that every unreachable allocation is immediately returned to the operating system.
- VM-local handle stores and collector metadata are implementation details; regression tests may assert that repeated full hints avoid exhausting bounded stores.

## Error handling

Invalid aggressiveness and unsupported syntax versions must be explicit semantic errors. The compiler must not silently reinterpret an invalid hint as an ordinary assignment. Contextual spellings must still be usable as ordinary identifiers in other syntactic contexts.

## Distinguish related layers

- **Language hint:** the source construct `target = gc N;` / `target = mem N;`.
- **Native collector:** the object graph collector in the runtime, with its own allocation/root/collection API.
- **Native memory manager:** tracked allocation, limits, structures and telemetry in `api/native/memory/`.

These layers can cooperate but their contracts are not interchangeable.
