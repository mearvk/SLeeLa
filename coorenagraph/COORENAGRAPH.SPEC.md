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

## 3. Coordinates

Coordinates are application data. A coordinate record must declare its coordinate system or units when those values have geographic, physical, temporal, or otherwise externally meaningful interpretation.

No geographic meaning should be inferred merely because a field is named `x`, `y`, or `z`.

## 4. Integrity

Implementations should reject:

- empty node identifiers;
- edges referring to nonexistent nodes when a closed graph is required;
- malformed coordinate values;
- duplicate identifiers where uniqueness is required;
- records exceeding configured size limits.

## 5. Transport

COORENAGRAPH is transport-neutral. HTTP integration belongs in an adapter layer.

An HTTP adapter must identify:

- graph format/version;
- content length;
- character encoding where applicable;
- integrity/authentication requirements;
- negotiated SLeeLa HTTP generation.

## 6. Security

Graph data may contain sensitive application information. Implementations should avoid logging complete graph payloads by default.

Authentication, authorization, encryption, and access policy belong at the appropriate application and transport boundaries.

## 7. Versioning

The graph format version must be explicit. A newer reader may reject an unsupported version rather than guessing its meaning.

## 8. Non-goals

COORENAGRAPH is not inherently:

- a map service;
- a GPS service;
- a surveillance system;
- a routing authority;
- a geographic database;
- an Internet standard.

Those capabilities require separate, explicit modules and specifications.
