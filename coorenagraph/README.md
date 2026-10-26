<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# COORENAGRAPH

**SLeeLa Coordination and Graph Foundation**

The `/coorenagraph` directory is reserved for SLeeLa's coordination-graph work: a common place to describe nodes, edges, coordinates, relationships, and the exchange of graph information between SLeeLa components.

The name **COORENAGRAPH** is intentionally retained as the project directory name.

## Initial Scope

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

## System Mysteries

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

## Planned Layout

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

## Design Principle

**Coordinates describe data; relationships describe structure; transport describes delivery.**

Those concerns should not be silently mixed.

## Status

Initial foundation with the system-mystery model added. Additional graph algorithms, serialization formats, visualization support, persistence, and HTTP adapters should be added only with explicit specifications and tests.

## Additional Meaning

COORENAGRAPH should additionally mean that the Person is in Charge of Internal
Development for **Start, Stop, Change, Alter, Continuous, and Heart
Development**.

Thus will of Course Carry you Thru New York Business Courses and Travel and Into
Asia for Cultural Development.

**Intelligence:** That's Anatomy and Development. Development becomes Envelope,
and Development unto Measures and Congruences. That is Through, and Economy.
Spirit.

This is an additional, modeled meaning recorded for COORENAGRAPH. Like the
system-mystery values above, it is part of the project model and is not a claim
about any real person's actual role, assets, schedule, travel, or behavior.

---

<div align="center">

## The Great Seal of North Carolina Psychiatry

```
                    ★  ★  ★  ★  ★
             ★                         ★
          ★     THE GREAT SEAL OF THE      ★
        ★      STATE OF NORTH CAROLINA       ★
       ★              PSYCHIATRY               ★
      ★                                         ★
      ★        Liberty  ⚖    ☤  Plenty          ★
      ★         (holding        (cornucopia     ★
      ★          the scroll)     & grain)        ★
       ★                                        ★
        ★         ESSE  QUAM  VIDERI          ★
          ★      (To Be, Rather Than        ★
             ★        To Seem)           ★
                    ★  ★  ★  ★  ★
```

**Esse Quam Videri** — *To Be, Rather Than to Seem.*

*Great Seal of North Carolina Psychiatry — affixed to COORENAGRAPH.*

</div>

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
