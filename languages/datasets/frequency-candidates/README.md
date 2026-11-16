# FRUCHST frequency candidate staging — first batch

This batch stages 1,000 unique ranked headwords for English, French, Spanish, and German from the openly licensed `wordhoard` top-2,000 preview CSVs.

| File | Language directory |
|---|---|
| `wordhoard-en.json` | `languages/english` |
| `wordhoard-fr.json` | `languages/french` |
| `wordhoard-es.json` | `languages/spanish` |
| `wordhoard-de.json` | `languages/standard-german` |

## Provenance and licensing

Upstream: https://github.com/natema/wordhoard  
Data license: CC BY-SA 4.0 (see upstream LICENSE and NOTICE.md).  
Code license: MIT; this is not the extracted data license.

The frequency layer is OpenSubtitles-2018 via hermitdave/FrequencyWords (MIT). Lexical processing uses spaCy (MIT) and Wiktionary via Kaikki/Wiktextract (CC BY-SA 4.0). Preserve upstream attribution and CC BY-SA 4.0 share-alike terms when redistributing these staged datasets.

## Selection method

For each language, the first 1,000 unique NFC-normalized lemmas are selected in upstream frequency_rank order. Duplicate lemmas across POS rows are collapsed to the first/highest-frequency occurrence. Both staged candidate rank and original source rank are retained, along with frequency count and POS.

These are staging files, **not FRUCHST binaries**. The source is subtitle-derived, not a balanced general-purpose corpus. Counts are raw corpus occurrences, not per-million rates, and should not be compared across languages.

## Remaining gate

Every entry still requires a reviewed, sense-aligned definition in English, French, and Spanish. Do not generate a production binary until all 1,000 entries have all three definitions and the definition source's license and attribution have been recorded.
