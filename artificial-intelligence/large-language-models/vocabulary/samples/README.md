# Vocabulary source samples

These records use the same English headword, **computer**, to demonstrate three distinct dictionary-source integrations.

## Files

- `computer-sources.jsonl` — one JSON Lines record for each source.
- `computer.oewn2025` — an imported lexical record from Open English WordNet 2025, including its two noun senses and synset synonyms.
- `computer.oxford-reference` — an Oxford reference descriptor only. No Oxford definition or examples are copied into this repository.
- `computer.merriam-webster-reference` — a Merriam-Webster reference descriptor only. No Merriam-Webster definition or examples are copied into this repository.

## Status labels

| `record_status` | Meaning |
|---|---|
| `definition_imported` | The definition text is included from a source whose license permits redistribution with the recorded notices and attribution. |
| `reference_only` | Only source identity and a lookup URL are stored. Definition content must be obtained through an authorized API or other licensed access. |

A missing or empty `senses` array on a reference-only record is intentional. It does not mean that the dictionary has no definition.

## Source and rights handling

### Open English WordNet 2025

- Official project: https://github.com/globalwordnet/english-wordnet
- Lookup: https://en-word.net/view/lemma/computer
- Release downloads: https://en-word.net/downloads
- License: Creative Commons Attribution 4.0 (CC BY 4.0); the resource is derived from Princeton WordNet, so retain and review the underlying Princeton WordNet license notice as well.
- Required attribution: Open English WordNet Community and Princeton WordNet.

This sample preserves the source's two noun senses as separate records, rather than merging their meanings. The sense identifiers follow the WordNet synset convention.

### Oxford Advanced Learner's Dictionary

- Lookup: https://www.oxfordlearnersdictionaries.com/definition/english/computer
- The sample is metadata-only. Oxford dictionary definitions, examples, pronunciation and other editorial content remain proprietary. Use an authorized Oxford API/license before storing or redistributing that content.

### Merriam-Webster Dictionary

- Lookup: https://www.merriam-webster.com/dictionary/computer
- The sample is metadata-only. Use the applicable Merriam-Webster API terms and obtain commercial permission where required before storing or redistributing dictionary content.

## Data-model rule

A lexical dictionary entry is not a model tokenizer vocabulary item. A lexical record describes headwords, parts of speech, senses, definitions and semantic relations. A tokenizer vocabulary maps tokenizer-specific token strings to model-specific token IDs. Keep these stores separate and link them only through an explicit tokenization/build process.

Do not infer a tokenizer ID from a dictionary headword. Do not silently replace a source definition with an internally paraphrased definition while retaining the source's attribution. If SLeeLa later adds its own explanatory glosses, label them as SLeeLa-authored and keep them distinct from source text.

## Validation expectations

Consumers should validate each JSONL line as UTF-8 JSON, check the required source and rights metadata, and reject a reference-only record if an importer attempts to treat it as licensed definition content. The samples are fixtures for the schema and source-labeling workflow; they are not a complete dictionary.
