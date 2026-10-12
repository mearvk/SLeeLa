# UTF-4088 Documentation and Implementation Relay

Status: documentation index for the experimental UTF-4088 catalogue. This file belongs in this existing `/utf-4088` directory and points to the canonical UTF-4088 files below.

UTF-4088 is a project-specific experimental catalogue/identifier namespace. It is not a standardized UTF encoding and does not make identifiers above U+10FFFF Unicode scalar values.

## Canonical protocol documents

These documents are maintained here in `/utf-4088`. The `/alphabet/sleela` versions are synchronized reader-facing mirrors; they do not replace these canonical copies.

| Subject | Canonical document | SLeeLa alphabet mirror |
|---|---|---|
| Input-map traversal and depth/pressure iteration 1–50 | [INPUT-MAP-AND-PRESSURE.md](INPUT-MAP-AND-PRESSURE.md) | [UTF-4088-INPUT-MAP-AND-PRESSURE.md](../alphabet/sleela/UTF-4088-INPUT-MAP-AND-PRESSURE.md) |
| National Lien ordered subject register | [NATIONAL-LIEN-REGISTER.md](NATIONAL-LIEN-REGISTER.md) | [UTF-4088-NATIONAL-LIEN-REGISTER.md](../alphabet/sleela/UTF-4088-NATIONAL-LIEN-REGISTER.md) |
| Candidate probe scheduling and nearest-match evidence | [PROBE-ORDERING-AND-MATCHING.md](PROBE-ORDERING-AND-MATCHING.md) | [UTF-4088-PROBE-ORDERING-AND-MATCHING.md](../alphabet/sleela/UTF-4088-PROBE-ORDERING-AND-MATCHING.md) |
| Identifier boundaries and serialization | [CODE-POINT-AND-SERIALIZATION.md](CODE-POINT-AND-SERIALIZATION.md) | [UTF-4088-CODE-POINT-AND-SERIALIZATION.md](../alphabet/sleela/UTF-4088-CODE-POINT-AND-SERIALIZATION.md) |
| SLeeLa-facing adapter | [SLeeLa-ALPHABET-INTEGRATION.md](SLeeLa-ALPHABET-INTEGRATION.md) | [UTF-4088-ALPHABET-INTEGRATION.md](../alphabet/sleela/UTF-4088-ALPHABET-INTEGRATION.md) |

## Related UTF-4088 documentation

- [README and build/test instructions](README.md)
- [Character map](CHARACTER-MAP.md)
- [Gradation matrix](GRADATION-MATRIX.md)
- [Natural die unit](NATURAL-DIE-UNIT.md)
- [Symbol weights](SYMBOL-WEIGHTS.md)
- [Implementation-level 4D geometry, bitmap topology, and coincidence/coalescence catalogue](../alphabet/utf-4088/SYMBOLS.md)
- [2D voltage field API](VOLTAGE-2D-API.md)
- [Character-set source policy](SLEELA-CHARSET.conf)

These documents describe distinct aspects of the experimental design. The voltage-field API is an explicitly named modelling/interface topic; the depth/pressure iteration values in the input-map protocol are abstract algorithmic iterations, not physical pressure, electrical voltage, or a confidence score.

## Implementation and test entry points

- [UTF-4088 public header](utf4088.hpp) and [implementation](utf4088.cpp)
- [Character-map header](character_map.hpp) and [implementation](character_map.cpp)
- [Digraph engine header](digraph_engine.hpp) and [implementation](digraph_engine.cpp)
- [Voltage-field header](voltage_field.hpp) and [implementation](voltage_field.cpp)
- [Registry generator](registry_generator.cpp)
- [SLeeLa alphabet adapter header](sleela_alphabet_adapter.hpp) and [implementation](sleela_alphabet_adapter.cpp)
- [CMake build definition](CMakeLists.txt)
- [Tests directory](tests/)
- [Registry manifest](registry_manifest.json)

The implementation and tests, not this index, determine what is executable and verified. Documentation of a proposed algorithm is not evidence that a complete probe scheduler, nearest-match index, semantic validator, or full catalogue is implemented.

## Stable ordering rules

1. Spatial traversal begins at the top-left of the current layer, proceeds right across a row, then down through rows, then advances to the next Z-layer.
2. For the baseline schedule, each map element considers depth/pressure iteration values 1 through 50 in ascending order. A versioned adaptive scheduler may reorder work, but must preserve a coverage strategy and record deferred work.
3. Probe priority, nearest-match order, permanent catalogue ID, map coordinates, registry ordinal, semantic evidence, and final alphabetic rank are separate fields.
4. The National Lien register's final alphabetical order comes from documented subject metadata and a versioned collation profile; it is not inferred from the order in which probes happened to run.
5. Similarity or distance is candidate evidence, not proof that two symbols share a linguistic meaning. Ambiguous matches must remain ambiguous until supported by additional evidence.
6. A generated glyph does not prove meaning by its geometry alone. Meanings require cited or explicitly provisional corpus, graph, historical, or application metadata.

## Identifier and capacity boundary

The project's `C<n>` value represents declared catalogue capacity. It must not be confused with the current populated registry, a verified entry count, the width of an integer, or a standards-defined code space. Values above U+10FFFF belong to the experimental catalogue/identifier namespace and must not be passed to normal Unicode UTF-8 encoding functions.

## Relay maintenance

Update the canonical document in this directory first, then update the matching `UTF-4088-...` mirror under `/alphabet/sleela` and its navigation index in the same change. Keep both `main` and `master` synchronized. This relay is a navigation/documentation contract; it does not claim unrun builds or tests passed.
