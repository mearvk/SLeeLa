# FRUCHST multilingual definition candidate pipeline

`fruchst_definition_batch.py` creates a **review queue**, not production definitions. The batch processes all 31 language sets with staged frequency candidates:

- Four `wordhoard-*.json` sets: English, French, Spanish, and German.
- Twenty-seven `frequencywords-*.json` sets: Simplified Chinese, Hindi, Spanish, Arabic, French, Bengali, Portuguese, Russian, Indonesian, Urdu, German, Japanese, Vietnamese, Telugu, Turkish, Tagalog, Tamil, Persian, Korean, Thai, Italian, Polish, Ukrainian, Malayalam, Malay, Dutch, Romanian, Kazakh, Sinhala, and Afrikaans.

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

Output filenames follow the candidate source, for example `wordhoard-en.review.json` and `frequencywords-hi.review.json`.

## Alignment and review safeguards

- Exact NFC headword matching is required; no fuzzy spelling substitution.
- The source entry Wiktionary `lang_code` must match the explicit mapping in `WIKTIONARY_LANG_CODES`.
- Candidate POS is used as a filter when supplied. FrequencyWords candidates do not include POS, so **all matching POS senses are retained** for review rather than being discarded.
- Every matching headword/POS gloss is retained independently for each Wiktionary edition, including senses without a Wikidata QID.
- A strong cross-edition alignment candidate is proposed when all three editions expose the same sense-level Wikidata QID and normalized POS.
- A partial-QID review candidate is also proposed when the same QID and POS occur in two editions. The third edition contributes every same-POS sense as an explicitly marked fallback alternative; this is a review pool, not an asserted sense match.
- A QID present in only one edition does not create an alignment candidate. POS-only gloss pools remain separate and unaligned.
- QID/POS agreement is a candidate signal, not proof of equivalence. All source senses and source-line evidence remain available; nothing is silently selected.
- Missing edition glosses are marked for manual lookup. Missing QIDs do not cause glosses to be discarded.
- The `zh-Hans` frequency source is the `zh_cn` corpus, while Wiktionary uses the broader `zh` code. The queue records that mapping, and every Chinese entry requires human script/variety verification.
- Every definition candidate remains `review_status: required`; partial-QID candidates also carry `selection_status: none`, their supporting editions, and per-edition fallback evidence.
- `qid_pos_candidate_triples` counts only full 3-of-3 QID matches. `qid_pos_review_candidates` separately counts the broader 2-of-3-or-3-of-3 review candidates. These metrics must not be conflated.
- `production_definitions_approved` stays zero until a reviewer records explicit selection and approval.

## Source and license

The definition sources are Kaikki/Wiktextract extractions of the English, French, and Spanish Wiktionary editions. Wiktionary content is dual-licensed under CC BY-SA 4.0 and the GNU Free Documentation License (GFDL). Preserve attribution and comply with the applicable license terms; verify the source edition notices before redistributing derived definitions.

The frequency-candidate files retain their independent provenance and license metadata:
- `wordhoard` first-batch data: CC BY-SA 4.0.
- FrequencyWords/OpenSubtitles-2018: CC BY-SA 4.0 for content; the FrequencyWords repository code is MIT. The code license does not relicense the frequency data.

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
