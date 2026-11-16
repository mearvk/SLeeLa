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

## unilex batch (20 languages)

`unilex-<tag>.json` files stage 1,000 ranked headwords for: Nigerian Pidgin (`pcm`), Marathi (`mr`), Hausa (`ha`), Swahili (`sw`), Javanese (`jv`), Gujarati (`gu`), Amharic (`am`), Kannada (`kn`), Yoruba (`yo`), Burmese (`my`), Odia (`or`), Eastern Punjabi (`pa-Guru`), Sundanese (`su`), Dari (`prs`, from unilex `fa-AF`), Northern Uzbek (`uz`), Somali (`so`), Cebuano (`ceb`), Magahi (`mag`), Khmer (`km`), and Maithili (`mai`).

Upstream: https://github.com/unicode-org/unilex at pinned revision `b3a9ae27a0c29eac217bb15a75c6f8d8efb15d17`. Each staged file records the source path, its git blob SHA-1 (checked against a pin in the stager), SHA-256, corpus size, and the variety mapping used.
Data license: Unicode Data Files license (SPDX `Unicode-DFS-2016`, per file header) under the repository's Unicode License v3. Both are permissive and require the Unicode copyright and permission notice to be kept.
Frequency source: Google Corpus Crawler web and scripture crawls. Values are scaled by unilex to occurrences per billion tokens, so `frequency_count` in these files is that scaled value, not a raw count.

Selection: NFC normalization, removal of edge punctuation and letterless tokens, merging of case variants (counts summed, most frequent surface form kept, all variants listed in `source_forms`), and ranking by frequency descending with a code-point tie-break. unilex files are alphabetical, so `source_rank` equals the re-ranked position.

Limitations: the corpora vary widely in size (about 108k tokens for Kannada up to about 60M for Punjabi). Several (Kannada, Odia, Uzbek) are dominated by Bible text. Proper nouns are not filtered. Uzbek is Cyrillic-script although the current official orthography is Latin.

Not staged from unilex:
- Igbo (`ig`, 693 usable forms) and Nigerian Fulfulde (`fuv`, 749): fewer than 1,000 forms; not padded.
- Northern and Southern Pashto (`pbu`, `pst`): unilex has only macrolanguage `ps` with no documented variety, so neither profile was assigned it.

Regenerate with a local sparse checkout at the pinned revision:

```sh
git clone --filter=blob:none --sparse https://github.com/unicode-org/unilex.git
git -C unilex checkout b3a9ae27a0c29eac217bb15a75c6f8d8efb15d17
git -C unilex sparse-checkout set --no-cone /data/frequency/
python3 languages/tools/stage_unilex.py --unilex-dir unilex --overwrite
```
