<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# Sleela Data Structures — C / C++ / Sleela

This directory establishes a small, explicit data-structure layer corresponding to the Java-like model used by the historical Java implementation and the current Sleela C/C++ execution core.

The same semantic structures are represented three ways:

| Structure | C | C++ | Sleela |
|---|---|---|---|
| Value | `SLDSValue` | `Value` | `DSValue` |
| Dynamic sequence | `SLDSVector` | `Vector<T>` | `DSList` |
| Stack | `SLDSStack` | `Stack<T>` | `DSStack` |
| Queue | `SLDSQueue` | `Queue<T>` | `DSQueue` |
| String map | `SLDSMap` | `Map` | `DSMap` |
| Object/field record | `SLDSObject` | `ObjectRecord` | `DSObject` |

The C and C++ versions are executable reference implementations. The Sleela version is the language-level semantic counterpart, expressed using the currently supported Java-like class/field/method surface.

## Design correspondence

The model intentionally separates:

1. **Value** — a tagged runtime value.
2. **Sequence** — ordered storage with size/capacity semantics.
3. **Stack** — LIFO storage.
4. **Queue** — FIFO storage.
5. **Map** — key/value association.
6. **Object** — named fields grouped into an object record.

This gives Nordshrift and the Sleela front end stable concepts to target without requiring the language surface to expose the implementation mechanism.

## Current-core relationship

The current core already contains a tagged `SLValue`, constant pool, globals, operand stack, and call frames. These files do not replace those structures. They make their data-structure semantics explicit and provide a reusable library boundary for future lowering.

The C ABI remains the execution boundary described by `impl/core/sleela_core.h`.

## Files

- `data_structures.h/.c` — C11 reference implementation.
- `data_structures.hpp/.cpp` — C++17 reference implementation.
- `data_structures.sleela` — Sleela semantic model.
- `DATA_STRUCTURES.md` — correspondence and invariants.

## Arrays

The Sleela surface now has a first-class **array** type: a dynamic, growable,
zero-indexed sequence written `T[]`, created with `new T[n]`, read/written with
`a[i]`, and operated on with the `arrayNew` / `arrayLength` / `arrayGet` /
`arraySet` / `arrayPush` built-ins. See [`STRUCTS.md`](../STRUCTS.md#arrays) for
the full surface and the `SL_ARRAY` / `OP_ARR*` implementation. Earlier
collection structures (list/stack/queue/map) remain modelled as the semantic
`struct`-based contracts in `data_structures.sleela`; arrays are the primitive
sequence those higher structures can be expressed over.