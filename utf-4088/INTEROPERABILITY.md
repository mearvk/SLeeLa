# SLeeLa document interoperability

Status: experimental integration contract, version 1.

## Principle

UTF-4088 identifiers are application-level symbol references. They are not
Unicode scalar values and must never be emitted as if they were UTF-8/UTF-16
characters. SLeeLa documents should preserve ordinary Unicode text as their
human-readable fallback and store UTF-4088 references in explicit metadata.

## Suggested JSON representation

```json
{
  "format": "SLeeLa-Document",
  "version": 1,
  "text": "A standard is a declared criterion.",
  "symbol_references": [
    {
      "scheme": "UTF-4088",
      "scheme_version": 1,
      "identifier": "0x00200000",
      "concept_key": "standard",
      "status": "provisional"
    }
  ]
}
```

Identifiers are strings in interchange JSON to avoid precision loss in clients
whose number type cannot represent every 64-bit integer exactly. The
`concept_key` is optional annotation, not an identity substitute. Preserve
unknown schemes and identifiers without reinterpretation.

## Binary and text safety

- Keep the Unicode text payload intact and normalized only under the document's
  separately declared Unicode normalization policy.
- Store experimental identifiers in a dedicated metadata field.
- Do not cast an identifier to `char32_t`, encode it as UTF-8, or use it as a
  font glyph index.
- A renderer must resolve the scheme/version and registry entry before drawing.
- Unknown or unvalidated symbols should display a visible fallback and retain
  the original identifier for round trips.
- Never execute content because it contains a symbol reference.

## Versioning

The identifier namespace, input packing, and reserved registry interval are
versioned. A change to any of them requires a new scheme version and a migration
note. Document format versioning remains independent of UTF-4088 versioning.
