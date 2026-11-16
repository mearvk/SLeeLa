# FRUCHST frequency and definition candidate pipeline

`fruchst_definition_batch.py` creates **review queues**, not approved definitions or production binaries. It reads staged word lists from three source families:

- Four `wordhoard-*.json` lists: English, French, Spanish, and German.
- Twenty-seven `frequencywords-*.json` lists from FrequencyWords/OpenSubtitles 2018.
- Up to twenty-eight `wikipedia-*.json` lists from Wikimedia Wikipedia article dumps, clearly marked as encyclopedia-domain candidates rather than conversational frequencies.

For each word-list entry, the definition tool extracts exact NFC headword gloss candidates from English, French, and Spanish Wiktionary editions. It streams each dump once and retains source line, sense index, POS, tags, QIDs, and dump checksums.

## Review and alignment policy

- Exact NFC headword matching is required; no fuzzy spelling substitution.
- Every target language has an explicit mapping to the language code used by Wiktionary. Script/dialect mappings such as `zh-Hans` → `zh`, `pnb-Arab` → `pnb`, `pa-Guru` → `pa`, and `bho` → `bho` are disclosed in the output and require native-speaker review.
- Candidate POS filters are applied when supplied. When a frequency source has no POS, all matching POS senses are retained.
- Glosses without Wikidata QIDs are preserved. Shared sense-level QIDs create optional cross-edition alignment candidates only.
- QID/POS agreement is a candidate signal, not proof of semantic equivalence. Every source sense remains reviewable; nothing is silently selected.
- Missing edition glosses require manual lookup. Each candidate remains `review_status: required`.
- `production_definitions_approved` remains zero until a human reviewer explicitly selects and approves a consistent EN/FR/ES sense.

## Source-specific limitations

FrequencyWords/OpenSubtitles is subtitle-domain data, not a balanced general-purpose corpus. Its lists contain surface forms rather than lemmas, and ranks should not be compared between languages.

Wikipedia candidates count lexical surface forms in current-revision main-namespace article text after conservative wikitext cleanup. They are encyclopedia-domain, not conversational or spoken-language frequencies. Topics, named entities, editorial style, bot-generated articles, and text cleanup can skew ranks. The tokenizer intentionally excludes languages needing script-specific segmentation (for example Chinese, Burmese, and Khmer) rather than pretending whitespace tokenization is valid.

## Licensing and attribution

The wordhoard and FrequencyWords datasets retain their own attribution and CC BY-SA 4.0 source notices. FrequencyWords' MIT code license does not relicense its frequency content.

Wiktionary content is dual-licensed under CC BY-SA 4.0 and GFDL. Wikipedia text is generally available under CC BY-SA 4.0 and GFDL, but imported pages can have additional notices. Preserve attribution, indicate changes, follow ShareAlike requirements, and review per-page notices before redistribution. Each generated candidate file records source URL, revision/checksum, retrieval time, and source license notice. The registry deliberately marks these source reviews as pending.

No FRUCHST binary should be built until a human reviewer selects one sense per headword, verifies that the English, French, and Spanish glosses express the same meaning, checks spelling and language variety, and records the required source attribution/license information.

## References

- FrequencyWords: https://github.com/hermitdave/FrequencyWords
- Kaikki raw-data catalog: https://kaikki.org/dictionary/rawdata.html
- Wiktextract format: https://github.com/tatuylonen/wiktextract#format-of-the-extracted-word-entries
- Wikimedia dumps: https://dumps.wikimedia.org/
- Wikimedia data dump format: https://meta.wikimedia.org/wiki/Data_dumps/Dump_format
- Wikimedia Terms of Use: https://foundation.wikimedia.org/wiki/Policy:Terms_of_Use
- Wikimedia reuse guidance: https://foundation.wikimedia.org/wiki/Legal:Wikimedia_Developer_App_Guidelines
