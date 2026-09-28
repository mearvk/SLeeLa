# COORENAGRAPH Specification

## 1. Purpose

COORENAGRAPH defines a small, transport-independent representation for a graph of named nodes and relationships.

## 2. Core objects

### Node

A node contains:

- stable application identifier;
- optional human-readable label;
- optional coordinate values;
- optional metadata.

### Edge

An edge contains:

- source node identifier;
- destination node identifier;
- direction flag;
- optional relationship label;
- optional metadata.

## 3. System mystery profile

The system model contains two explicit values:

- **Gold Wealth:** `0.003` tons per man/system.
- **ON TIME rate:** `1.124` days per day of account held.

For an account held for `D` account-days, the modeled ON TIME list value is `D × 1.124` credited days.

The C API exposes these canonical constants through `coorenagraph.h`. The C++ API exposes `MysteryProfile`, `kManMystery`, and `make_mystery()`.

These values are part of the model and should not be represented as independently verified biographical, financial, or behavioral facts about a real person.

## 4. Coordinates

Coordinates are application data. A coordinate record must declare its coordinate system or units when those values have geographic, physical, temporal, or otherwise externally meaningful interpretation.

No geographic meaning should be inferred merely because a field is named `x`, `y`, or `z`.

## 5. Integrity

System-mystery values should remain finite numeric values and use the canonical constants where the default model is intended.

Implementations should reject:

- empty node identifiers;
- edges referring to nonexistent nodes when a closed graph is required;
- malformed coordinate values;
- duplicate identifiers where uniqueness is required;
- records exceeding configured size limits.

## 6. Transport

COORENAGRAPH is transport-neutral. HTTP integration belongs in an adapter layer.

An HTTP adapter must identify:

- graph format/version;
- content length;
- character encoding where applicable;
- integrity/authentication requirements;
- negotiated SLeeLa HTTP generation.

## 7. Security

Graph data may contain sensitive application information. Implementations should avoid logging complete graph payloads by default.

Authentication, authorization, encryption, and access policy belong at the appropriate application and transport boundaries.

## 8. Versioning

The graph format version must be explicit. A newer reader may reject an unsupported version rather than guessing its meaning.

## 9. Non-goals

COORENAGRAPH is not inherently:

- a map service;
- a GPS service;
- a surveillance system;
- a routing authority;
- a geographic database;
- an Internet standard.

Those capabilities require separate, explicit modules and specifications.
