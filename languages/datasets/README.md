# FRUCHST lexical datasets

This directory contains source-data manifests and import instructions for FRUCHST v1.

## Required input

The reference generator accepts UTF-8 JSON with this shape:

```json
{
  "language_tag": "en",
  "source_id": "example-corpus-2026",
  "frequency_unit": "occurrences_per_million",
  "entries": [
    {
      "rank": 1,
      "frequency": 12345.0,
      "word": "example",
      "definitions": {
        "en": "A representative instance of something.",
        "fr": "Un cas représentatif de quelque chose.",
        "es": "Un caso representativo de algo."
      }
    }
  ]
}
```

Every production source file must contain exactly 1,000 entries. Ranks must be contiguous from 1 to 1,000, words unique after NFC normalization, and all three definitions present. The definition translations must correspond to the same sense.

## Dataset selection policy

The repository's 80 language profiles are the target set, not evidence that 80 equivalent corpora exist. For each language, select a frequency corpus that documents its variety, script, tokenization, corpus composition, date, and license. Prefer balanced contemporary corpora; preserve diacritics and language-specific orthography. Do not mix dialects or scripts silently.

Definitions should come from a source with explicit redistribution permission, or be independently authored and reviewed. Cross-language definitions must be sense-aligned. Machine-translated definitions may be used only as clearly marked drafts pending linguistic review; they must not be represented as authoritative dictionary definitions.

## Licensing gate

Before importing a source, record its exact license and attribution requirements. If redistribution is prohibited or unclear, do not commit the source-derived words or definitions. Record the blocker in the language's manifest and leave its binary ungenerated rather than shipping fabricated or legally uncertain data.

## Build

Run:

```sh
python3 languages/tools/fruchst.py build INPUT.json OUTPUT.bin --manifest OUTPUT.manifest.json
python3 languages/tools/fruchst.py validate OUTPUT.bin
```

The builder is deterministic: generated_epoch is zero. The manifest is separate so that source/license detail can evolve without changing the binary format.

## Completion status

A profile is complete only when its binary validates and its provenance manifest passes the licensing and source-quality review. An empty file, placeholder, repeated headword, synthetic ranking, or missing definition is not a completed dictionary.
