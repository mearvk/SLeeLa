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


## First-batch frequency candidates

The English, French, Spanish, and German 1,000-word candidate lists are staged in `frequency-candidates/`. They are sourced from a pinned wordhoard sample revision and carry source-rank, raw occurrence count, POS, and attribution metadata. Regenerate one with:

```sh
python3 languages/tools/stage_wordhoard.py en languages/datasets/frequency-candidates/wordhoard-en.json
python3 languages/tools/stage_wordhoard.py fr languages/datasets/frequency-candidates/wordhoard-fr.json
python3 languages/tools/stage_wordhoard.py es languages/datasets/frequency-candidates/wordhoard-es.json
python3 languages/tools/stage_wordhoard.py de languages/datasets/frequency-candidates/wordhoard-de.json
```

These files are not complete dictionaries: all entries still require reviewed, aligned English, French, and Spanish definitions.


## Additional frequency-candidate source

A second staging path uses the pinned \`hermitdave/FrequencyWords\` 2018 files, derived from OpenSubtitles 2018. Its content license is CC BY-SA 4.0; the upstream code license is MIT. These source files provide surface-form counts rather than lemmas or POS labels. The stager records the original source line rank and count, filters punctuation-only tokens, preserves NFC spelling, and does not invent missing data.

Run the supported-language batch with:

\`\`\`sh
python3 languages/tools/stage_frequencywords.py
\`\`\`

This batch currently maps 25 target profiles to explicit source variants. It deliberately does not substitute dialects (for example, Modern Standard Arabic for Egyptian or Levantine Arabic, or Mandarin for Yue/Wu). The remaining profiles need separate, variety-appropriate source review. Candidate files are not completed dictionaries: all three English/French/Spanish definitions and the associated human review remain pending.


## English round (`definition-english/`)

`tools/fruchst_english_round.py` runs in the source-definition staging workflow after the close-variant pass. It reads the review queues and the close-variant files and makes up to three passes over the English Wiktionary extract (`raw-wiktextract-data.jsonl.gz`). It writes one `<stem>.english.json` per profile. It is add-only: every review-queue gloss stays exactly as staged, and every review sense appears in the English file.

Each headword gets a ranked list of English definitions with source evidence, in five tiers:

1. `own_sense`: the headword's own English sense. Senses corroborated by a translation table rank first, which means an English entry that lists this headword in its translations also appears in the gloss (this picks "from" over "favor" for unvocalized Arabic من). Obsolete, archaic, rare and similar senses rank last. Otherwise Wiktionary order is kept.
2. `lemma_of_form`: a form-of, alt-of or synonym-of sense ("plural of mtu", "genitive of on") resolved to the lemma's own definitions, following up to two links. Korean hanja-reading senses whose gloss is only a Han character are resolved to that character's definition.
3. `form_of_gloss`: a pointer gloss whose lemma was not found, kept verbatim.
4. `close_variant`: the close-variant pass, with its form-of senses resolved the same way.
5. `translation_equivalent`: English entries whose translation tables list the headword, matched exactly or with optional marks removed (Arabic vowel points, for example). Mandarin rows (`cmn`) count for `zh-Hans`, and Uzbek Cyrillic headwords are also matched in Latin script.

Each entry also records usage examples that have English translations, romanizations, the English words from translation tables, and `gloss_notes` for wiki-markup remnants, hanja-reading glosses and unresolved pointers, each with a cleaned display text. `lead_candidate` is the first ranked definition. It is a reviewer's starting point, not a selection. Every row stays `review_required`, and `production_definitions_approved` stays 0.
