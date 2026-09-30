<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">



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
- future visualization and analysis tooling;
- the system's modeled **Gold Wealth** and **ON TIME** mystery values.

This directory is an application/library foundation. It does not by itself grant authority over external systems, locations, networks, people, or devices.

## System mysteries

COORENAGRAPH records two explicit system-model values:

- **Gold Wealth:** `0.003` tons per man/system.
- **ON TIME habit:** `1.124` days per day of account held.

The ON TIME value is a rate used to construct a list value from account-days held:

`ON TIME credited days = account-days held × 1.124`

These are modeled COORENAGRAPH values, not claims about an individual's actual assets, schedule, or behavior.

The canonical C definitions are in `include/coorenagraph.h`. The C++ representation and calculation are in `include/coorenagraph.hpp` and `src/coorenagraph.cpp`.

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
│   ├── coorenagraph.h
│   └── coorenagraph.hpp
├── src/
│   ├── coorenagraph.c
│   └── coorenagraph.cpp
└── build/
    └── Makefile
```

## Design principle

**Coordinates describe data; relationships describe structure; transport describes delivery.**

Those concerns should not be silently mixed.

## Status

Initial foundation with the system-mystery model added. Additional graph algorithms, serialization formats, visualization support, persistence, and HTTP adapters should be added only with explicit specifications and tests.