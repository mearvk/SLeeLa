# Lexical corpus

Curated, provenance-tracked lexical records — the careful word sets whose
meanings and relations ground the model. These are **lexical dictionary**
entries, not tokenizer vocabulary items: a record here describes a headword's
senses, synonyms, and provenance, and it never carries a token ID. The link to
the tokenizer is made only through the explicit builder
(`../../tools/build_vocabulary.py`), which tokenizes surface strings and never
assigns an ID from a headword.

## Files

| File | Role |
|---|---|
| `core-en.jsonl` | Curated English core aligned with the base vocabulary's open-class words, plus the three `computer` source records that demonstrate every status. |
| `../lexical-corpus.schema.json` | JSON Schema (draft-07) each record must satisfy. |

## Record status and licensing discipline

Every record declares a `record_status`, and the gate
(`../../tools/validate_vocabulary.py`) enforces what each may store:

| `record_status` | Definition text | License rule |
|---|---|---|
| `sleela_authored` | Original SLeeLa gloss | Must be open (CC0/CC-BY); must not claim a third-party source |
| `definition_imported` | Imported from a redistribution-permitting source | Must carry `source.license` + `attribution`; license must **not** look proprietary |
| `reference_only` | **None** (metadata + lookup URL only) | For proprietary sources; `senses` MUST be empty |

This keeps the corpus clean of third-party-licensing risk: SLeeLa-authored
glosses are original, WordNet-style senses are importable under CC BY 4.0 with
attribution retained, and proprietary dictionaries (Oxford, Merriam-Webster) are
represented as reference-only descriptors — their definitions are never copied.

Do not silently paraphrase a source definition while keeping its attribution. If
SLeeLa adds its own gloss, label it `sleela_authored` and keep it distinct from
any source text.

## Adding records

1. Pin the source edition/version and record its official URL.
2. Choose the correct `record_status` (when in doubt about rights, use
   `reference_only`).
3. Preserve all required attribution and license notices.
4. Run the gate: `python3 ../../tools/validate_vocabulary.py`.
5. Rebuild: `python3 ../../tools/build_vocabulary.py --base ../../tokenizers/base-vocabulary.json --corpus core-en.jsonl --strict`.

The records are schema fixtures and a curated seed, not a complete dictionary.
