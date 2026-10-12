# National Lien Alphabetic Register for UTF-4088

Status: proposed experimental catalogue policy, version 1.

## Purpose

The National Lien Alphabetic Register is the ordered subject index for the UTF-4088 catalogue. It supports print ordering, search, citation, and cross-reference while keeping catalogue identity separate from spatial input-map traversal.

## Independent fields

Do not use one number to represent all forms of ordering:

- **Permanent catalogue ID** identifies a record and does not change when the register is resorted.
- **Alphabetic rank** is the generated position in the subject index and may change when new subject terms are added.
- **Input-map coordinate** is the record's optional `(x,y,z)` location.
- **Pressure level** is the protocol iteration value from 1 through 50.
- **Registry ordinal** is the record's position in a particular versioned registry manifest.

Recommended printable ID format: `UTF4088-000001`. Zero-padding is for presentation only; the underlying ID is an integer or canonical string.

## Subject hierarchy and sorting

Each ranked entry should provide a primary subject and may provide secondary subjects, aliases, language tags, a display label, a source reference, and a verification status. The sorting contract must define case folding, Unicode normalization version, accent handling, punctuation, whitespace, numerals, and multi-script collation. Until a complete collation implementation is selected, the register generator must name and version its sorting profile.

A glyph shape or generated identifier alone does not prove a word, subject, or historical meaning. Mark entries as `verified`, `provisional`, or `experimental` and include provenance before describing a meaning as established.

## Suggested CSV fields

`catalogue_id,registry_ordinal,alphabetic_rank,subject_primary,subject_secondary,display_label,aliases,language_tag,x,y,z,pressure_min,pressure_max,status,source_ref`

A CSV header is a schema, not a claim that every catalogue slot has already been populated or semantically verified.

## Validation

The register validator should reject duplicate permanent IDs, duplicate ranks within the same register version, malformed coordinates, invalid pressure bounds, missing required subject/status fields, and references to nonexistent registry records. It should report gaps separately from errors because a catalogue may intentionally have unpopulated capacity.

## Relationship to input map

The spatial traversal and pressure rules are defined in [Input Map and Pressure Iteration Protocol](INPUT-MAP-AND-PRESSURE.md). Traversal order is top-left to right, then down through rows, then to the next z-layer. For each element, the canonical protocol processes pressure levels 1–50 in ascending order before advancing to the next element.

## Scope and caveats

The current UTF-4088 implementation documents a generated front-end registry of 16,606 records, while catalogue capacity is a separate declaration. This register policy does not automatically create 16,606 verified subject definitions. Populate it from actual registry records, preserve provenance, and mark unknown meanings as unknown rather than inventing them.


## Probe-driven candidate discovery

Candidate discovery and probe scheduling are defined separately in [PROBE-ORDERING-AND-MATCHING.md](PROBE-ORDERING-AND-MATCHING.md). Probe priority may optimize discovery logistics, but it must not alter permanent catalogue IDs or replace the separately versioned alphabetic collation. Approximate glyph proximity is evidence for a candidate relationship, not proof of shared meaning.
