# Vocabulary assets

The public tokenizer-vocabulary implementation remains in the parent directory as `large_language_models_vocabulary.h/.c` and its C++17 wrapper. This folder holds model-specific vocabulary exports, lexical dictionary resources, and source metadata. These are related resources but are not interchangeable.

## Separation of concerns

- **Tokenizer vocabulary:** exact token strings, stable model-specific token IDs, tokenizer algorithm/version, special tokens, normalization behavior, and model compatibility.
- **Lexical dictionary:** headwords, parts of speech, sense identifiers, definitions, examples, synonyms, antonyms, semantic relations, provenance, and license metadata.

Never assign tokenizer IDs directly from dictionary headwords. A model's tokenizer may split a headword into multiple subword or byte-level tokens.

## Samples

See [samples/](samples/README.md) for three carefully labelled source records for the English headword `computer`:

- Open English WordNet 2025 — imported senses under CC BY 4.0 with Princeton WordNet attribution/license notice retained.
- Oxford Advanced Learner's Dictionary — metadata-only reference record; proprietary definition content is not copied.
- Merriam-Webster Dictionary — metadata-only reference record; proprietary definition content is not copied.

The JSONL fixture is `samples/computer-sources.jsonl`. The `record_status` field distinguishes imported content from reference-only entries. Empty senses on a reference-only record are intentional.

## Import and licensing requirements

Before importing a lexical dataset, pin its release/version, record the official source URL, preserve all required attribution and license notices, and validate the data format. Do not scrape or redistribute proprietary dictionary content. Oxford and Merriam-Webster content must be accessed under the applicable API or content license.

The Open English WordNet project publishes versioned downloadable releases at https://en-word.net/downloads and maintains its source at https://github.com/globalwordnet/english-wordnet. Review both the CC BY 4.0 terms and the underlying Princeton WordNet license notice before distributing derived data.

The samples are schema fixtures, not a complete dictionary or tokenizer model.
