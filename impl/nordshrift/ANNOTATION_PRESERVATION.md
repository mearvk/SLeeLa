# Nordshrift Annotation Preservation

Nordshrift consumes the same Program AST used by the SLeeLa compiler. Document
annotations are therefore preserved as part of the language model rather than
being treated as comments by the front end.

## Targets

- **Sleela target:** emits first-class annotation lines so a round-trip can
  reconstruct the Program annotation collection.
- **Java target:** emits annotation metadata as source comments because the
  generated Java program does not claim the SLeeLa annotation grammar.
- **C target:** emits annotation metadata as C comments for traceability.

The authoritative semantic interpretation remains in the SLeeLa front end and
runtime. Nordshrift does not become a second annotation routing engine.

## Round-trip invariant

For a Sleela → Nordshrift(Sleela) → Sleela round trip, the annotation names and
values must remain unchanged. Semantic validation occurs again after reparsing.

Max Rupplin — MEARVK LLC — 2026
