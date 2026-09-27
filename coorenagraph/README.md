# COORENAGRAPH

**SLeeLa coordination and graph foundation**

The `/coorenagraph` directory is reserved for SLeeLa's coordination-graph work: a common place to describe nodes, edges, coordinates, relationships, and the exchange of graph information between SLeeLa components.

The name **COORENAGRAPH** is intentionally retained as the project directory name.

## Initial scope

- coordinate and node identity;
- graph nodes and directed/undirected edges;
- relationship metadata;
- versioned graph records;
- serialization boundaries;
- validation and integrity checks;
- interoperability with SLeeLa HTTP modules;
- future visualization and analysis tooling.

This directory is an application/library foundation. It does not by itself grant authority over external systems, locations, networks, people, or devices.

## Relationship to HTTP

HTTP 1.0 through 9.0 may transport COORENAGRAPH records when an application explicitly integrates the two systems.

The graph layer should remain independent of a particular HTTP generation so that:

1. a graph can be created locally;
2. it can be validated before transmission;
3. it can be serialized for a selected protocol;
4. a receiver can validate the graph independently of transport.

## Planned layout

```text
coorenagraph/
├── README.md
├── COORENAGRAPH.SPEC.md
├── include/
│   └── coorenagraph.h
├── src/
│   └── coorenagraph.c
└── build/
    └── Makefile
```

## Design principle

**Coordinates describe data; relationships describe structure; transport describes delivery.**

Those concerns should not be silently mixed.

## Status

Initial foundation. Additional graph algorithms, serialization formats, visualization support, persistence, and HTTP adapters should be added only with explicit specifications and tests.
