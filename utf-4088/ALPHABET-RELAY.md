# UTF-4088 Documentation Relay

Purpose: join the specialized experimental UTF-4088 catalogue specification with SLeeLa's general alphabet and character-set documentation.

## Source of record

The full protocol documents are maintained in the utf-4088 directory. Corresponding synchronized mirrors are maintained in alphabet/sleela so general alphabet readers can find the same material without treating UTF-4088 as a Unicode encoding.

| Subject | Canonical UTF-4088 document | Alphabet mirror |
|---|---|---|
| Input map and pressure/depth iteration | [INPUT-MAP-AND-PRESSURE.md](INPUT-MAP-AND-PRESSURE.md) | [UTF-4088-INPUT-MAP-AND-PRESSURE.md](../alphabet/sleela/UTF-4088-INPUT-MAP-AND-PRESSURE.md) |
| National Lien alphabetic register | [NATIONAL-LIEN-REGISTER.md](NATIONAL-LIEN-REGISTER.md) | [UTF-4088-NATIONAL-LIEN-REGISTER.md](../alphabet/sleela/UTF-4088-NATIONAL-LIEN-REGISTER.md) |
| Probe ordering and nearest-match algorithm | [PROBE-ORDERING-AND-MATCHING.md](PROBE-ORDERING-AND-MATCHING.md) | [UTF-4088-PROBE-ORDERING-AND-MATCHING.md](../alphabet/sleela/UTF-4088-PROBE-ORDERING-AND-MATCHING.md) |
| Code-point and serialization contract | [CODE-POINT-AND-SERIALIZATION.md](CODE-POINT-AND-SERIALIZATION.md) | [UTF-4088-CODE-POINT-AND-SERIALIZATION.md](../alphabet/sleela/UTF-4088-CODE-POINT-AND-SERIALIZATION.md) |
| SLeeLa adapter integration | [SLeeLa-ALPHABET-INTEGRATION.md](SLeeLa-ALPHABET-INTEGRATION.md) | [UTF-4088-ALPHABET-INTEGRATION.md](../alphabet/sleela/UTF-4088-ALPHABET-INTEGRATION.md) |

## Ordering invariants

- Spatial traversal: top-left, left-to-right, top-to-bottom, then next Z-layer.
- Depth/pressure iteration: integer iteration levels 1 through 50, with 50 as the current assumed maximum. This is an abstract algorithmic scale, not physical pressure or voltage.
- Probe priority: a scheduling heuristic based on match utility, novelty/coverage, uncertainty reduction, and resource cost.
- Permanent catalogue ID, map coordinate, probing depth, registry ordinal, and alphabetic rank are independent fields.
- Final National Lien alphabetic order is generated from documented subject metadata and a versioned collation profile; it is not the order in which probes happen to run.
- Match proximity is evidence for candidate relations, not automatic proof of shared linguistic meaning.
- UTF-4088 remains an experimental catalogue/identifier namespace and must not be serialized as ordinary UTF-8 code points.

## Mirror maintenance

When changing a canonical protocol document, update its corresponding UTF-4088-prefixed mirror under alphabet/sleela in the same change. Both directories should retain these links. The relay is a documentation index; it does not assert that the full probe engine, nearest-match index, or complete verified alphabet has been implemented.
