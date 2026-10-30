# Module Data

This directory owns the data contracts used by Philosophy modules.

## Responsibilities

- Define stable event and observation shapes.
- Preserve integer versus floating-point value types and their units.
- Retain timestamps, optional event-ending times, half-life settings, and related-event IDs.
- Keep source provenance distinct: self-report, direct observation, publication/media, assessor-authored statement, or system import.
- Record consent-to-share explicitly; private records must not enter a conversation view or export.
- Keep human observations separate from animal-model or laboratory evidence.

## Current implementation

The initial event record and AVL event-tree API live in:

- `../../event_tree.h`
- `../../event_tree.c`

Future schemas should remain versioned and backwards-compatible where possible. Persistence, encryption, identity verification, and access control belong to a separately reviewed storage layer; this directory does not make an in-memory record a secure medical record.
