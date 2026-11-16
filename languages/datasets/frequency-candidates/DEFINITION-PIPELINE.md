# FRUCHST multilingual definition candidate pipeline

`fruchst_definition_batch.py` creates a **review queue**, not production definitions. The batch processes all 51 language sets with staged frequency candidates:

- Four `wordhoard-*.json` sets: English, French, Spanish, and German.
- Twenty-seven `frequencywords-*.json` sets: Simplified Chinese, Hindi, Spanish, Arabic, French, Bengali, Portuguese, Russian, Indonesian, Urdu, German, Japanese, Vietnamese, Telugu, Turkish, Tagalog, Tamil, Persian, Korean, Thai, Italian, Polish, Ukrainian, Malayalam, Malay, Dutch, Romanian, Kazakh, Sinhala, and Afrikaans.
- Twenty `unilex-*.json` sets: Nigerian Pidgin, Marathi, Hausa, Swahili, Javanese, Gujarati, Amharic, Kannada, Yoruba, Burmese, Odia, Eastern Punjabi (Gurmukhi), Sundanese, Dari, Uzbek, Somali, Cebuano, Magahi, Khmer, and Maithili.

Each candidate headword is looked up in three Wiktionary editions (English, French, and Spanish). The tool streams each source dump once and retains exact NFC headword matches for the explicitly mapped target-language code.

## Batch execution

The workflow downloads the three source archives outside the repository and runs:

```sh
python3 languages/tools/fruchst_definition_batch.py \
  --candidates languages/datasets/frequency-candidates \
  --en /data/kaikki/enwiktionary.jsonl.gz \
  --fr /data/kaikki/frwiktionary.jsonl.gz \
  --es /data/kaikki/eswiktionary.jsonl.gz \
  --output languages/datasets/definition-review
```

Output filenames follow the candidate source, for example `wordhoard-en.review.json`, `frequencywords-hi.review.json`, and `unilex-sw.review.json`.

## Alignment and review safeguards

- Exact NFC headword matching is required; no fuzzy spelling substitution.
- The source entry Wiktionary `lang_code` must match the explicit mapping in `WIKTIONARY_LANG_CODES`.
- Candidate POS is used as a filter when supplied. FrequencyWords candidates do not include POS, so **all matching POS senses are retained** for review rather than being discarded.
- Every matching headword/POS gloss is retained independently for each Wiktionary edition, including senses without a Wikidata QID.
- A cross-edition alignment candidate is proposed only when all three editions expose the same sense-level Wikidata QID and normalized POS.
- QID/POS agreement is a candidate signal, not proof of equivalence. All source senses and source-line evidence remain available; nothing is silently selected.
- Missing edition glosses are marked for manual lookup. Missing QIDs do not cause glosses to be discarded.
- The `zh-Hans` frequency source is the `zh_cn` corpus, while Wiktionary uses the broader `zh` code. The queue records that mapping, and every Chinese entry requires human script/variety verification.
- A language tag may map to more than one Wiktionary code. Dari (`prs`) is matched against both Dari (`prs`) and Persian (`fa`) entries, because Wiktionary files most Dari vocabulary under Persian. Every sense candidate records `source_lang_code`, and a Persian-sourced gloss must be confirmed for Dari usage before approval.
- Eastern Punjabi (`pa-Guru`) maps to Wiktionary `pa`, which covers both scripts; only Gurmukhi headwords can match. Uzbek candidates are Cyrillic while Wiktionary Uzbek lemmas are mostly Latin, so expect few Uzbek matches until a reviewed transliteration step exists.
- Every definition candidate remains `review_status: required`. `production_definitions_approved` stays zero until a reviewer records explicit selection and approval.

## French and Spanish equivalents through English

French and Spanish Wiktionary have entries for few of the target-language headwords, so a second step, `fruchst_english_pivot.py`, finds French and Spanish equivalents through English. It runs in the same workflow after the review queues are built and writes `languages/datasets/definition-pivot/<source>-<tag>.pivot.json`.

1. The English word(s) are taken from each English Wiktionary gloss of the headword. Form-of senses (plural of, locative of, and similar) and wikitext glosses are skipped; parentheticals are removed; the gloss is split on `;` and `,`; a leading to/a/an/the is removed; pieces of one to three words are kept. For the English profile, the headword is the English word.
2. Each English word is looked up in English Wiktionary, and its French and Spanish translation-table entries are kept with their translation sense label and tags.
3. As secondary evidence, the French and Spanish Wiktionary glosses of the same English word are kept.
4. Items whose English POS matches the source sense POS are preferred. If none match, all items are kept with `pos_match: false`.

Each output file stores every English word once under `english_lookup`. `translation_senses` are English Wiktionary translation boxes (one per sense label) with the French and Spanish words side by side; `glosses` are the French and Spanish Wiktionary definitions of the English word. Each headword lists its `english_pivots`, and each pivot carries `pos_match` per edition (`true`, `false`, or `null` when nothing was found). Files are written as compact JSON.

The pivot can cross senses (for example, an English gloss "bank" may lead to river-bank and money-bank translations), so every equivalent records the English word, both POS values, and source lines. Nothing is selected; every row stays `review_status: required`.

## Close-variant English definitions

`fruchst_close_variants.py` handles headwords with no English Wiktionary sense. It looks for a close variant in English Wiktionary, in the same Wiktionary language, and records that variant's English glosses as a close definition in `languages/datasets/definition-variants/<source>-<tag>.variants.json`. Methods, closest first:

| Method | Meaning |
|---|---|
| `case_variant` | Same word, different letter case. |
| `orthographic_variant` | Same word after removing optional marks: Latin/Cyrillic/Greek diacritics (e.g. Russian ё/е), Arabic short vowels and tatweel, and unifying Arabic-script letter variants (ي/ى/ی, ك/ک; for Arabic only, also أ/إ/آ/ٱ to ا). ة and ه stay distinct because they separate words. Abugida vowel signs are never removed. |
| `inflected_form` | The word appears in another entry's inflection table (`forms`), exactly or after the same mark removal (`via`). The close definition is that lemma's gloss. For Chinese, only forms tagged exactly `Simplified-Chinese` are used; dialectal and alternative characters are skipped. |
| `transliteration` | Uzbek only: official Cyrillic-to-Latin conversion. |
| `clitic_stripped` | Arabic only: a leading و/ف/ب/ل/ك and/or ال removed. |

Each match records the variant, method, POS, source line, the listed form and its tags, and the variant's English senses (with `form_of` targets when present). Up to 20 matches are kept per headword. A close definition is not the headword's own definition, so every row stays `review_status: required`.

## Source and license

The definition sources are Kaikki/Wiktextract extractions of the English, French, and Spanish Wiktionary editions. Wiktionary content is dual-licensed under CC BY-SA 4.0 and the GNU Free Documentation License (GFDL). Preserve attribution and comply with the applicable license terms; verify the source edition notices before redistributing derived definitions.

The frequency-candidate files retain their independent provenance and license metadata:
- `wordhoard` first-batch data: CC BY-SA 4.0.
- FrequencyWords/OpenSubtitles-2018: CC BY-SA 4.0 for content; the FrequencyWords repository code is MIT. The code license does not relicense the frequency data.
- unilex (Unicode, Google Corpus Crawler): Unicode-DFS-2016 / Unicode License v3.

No FRUCHST binary should be built until a human reviewer has selected one sense per headword, confirmed that the English, French, and Spanish glosses express the same sense, checked spelling/grammar and language variety, and recorded source/license metadata. The binary builder approval and provenance checks are necessary but not sufficient for semantic accuracy.

## Source dump catalog

The current workflow URLs and revisions are recorded in `SOURCE_CATALOG` in `languages/tools/fruchst_definition_batch.py`. Check the Kaikki catalog before production runs because Wiktionary extracts are updated regularly:

- English edition: https://kaikki.org/dictionary/raw-wiktextract-data.jsonl.gz
- French edition: https://kaikki.org/frwiktionary/raw-wiktextract-data.jsonl.gz
- Spanish edition: https://kaikki.org/eswiktionary/raw-wiktextract-data.jsonl.gz

Compressed extracts are large. Keep them outside the repository and never commit raw dumps. Generated queues record each downloaded file SHA-256, compressed size, retrieval timestamp, URL, and dump revision.

References:
- Kaikki raw-data catalog: https://kaikki.org/dictionary/rawdata.html
- Wiktextract extracted-entry format: https://github.com/tatuylonen/wiktextract#format-of-the-extracted-word-entries
- FRUCHST frequency candidate provenance: `README.md` in this directory.
