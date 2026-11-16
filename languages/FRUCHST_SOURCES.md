# FRUCHST data sources and licensing policy

FRUCHST separates the binary format and generator code from lexical datasets. Each dictionary release must carry its own provenance manifest. The format's three definition languages (English, French, Spanish) do not imply that a source corpus provides all three glosses for every headword.

## Frequency candidates

- **wordfreq** is a candidate for languages it actually supports. Its code is Apache-2.0, while its distributed frequency data is under CC BY-SA 4.0 and carries additional source attribution obligations. Its upstream notice identifies Robyn Speer and source collections including Google Books Ngrams, Leeds Internet Corpus, Wikipedia, ParaCrawl, OpenSubtitles and SUBTLEX. The repository must preserve the data license and all applicable attributions; do not treat the code license as the data license.
- **Language-specific national or academic corpora** may be preferable for varieties not represented by wordfreq. Record the exact corpus edition, language variety, tokenization, frequency unit, access date, and redistribution permission.
- For Chinese varieties and Arabic varieties, do not substitute Mandarin or Modern Standard Arabic rankings for Yue, Wu, Egyptian, Levantine, or other profiles without explicitly changing the profile's scope.

## Definition candidates

- **Wiktionary / Wiktextract / Kaikki** provide broad multilingual lexical material. Wiktionary content is dual-licensed CC BY-SA 4.0 and GFDL; extracted/modified datasets retain applicable attribution and share-alike obligations. Verify the specific dump, extraction, and output terms before redistribution.
- **DBnary** extracts lexical data from 28 Wiktionary language editions and is distributed under CC BY-SA 4.0. Its language-edition coverage is not equivalent to coverage of all 80 target language varieties.
- A definition record must be sense-aligned across English, French, and Spanish. If only an English gloss or translation equivalents are available, the record is incomplete until French and Spanish definitions are sourced or independently authored and reviewed.

## Release gate

A language is marked complete only after:
1. a documented frequency source supports the target variety and the 1,000-word ordering;
2. all 1,000 entries have three sense-aligned definitions;
3. the source licenses permit the intended redistribution;
4. attribution and share-alike obligations are recorded;
5. the generated binary passes the FRUCHST validator and its SHA-256 is recorded.

A missing source or unclear license is a tracked blocker, not a reason to synthesize frequency ranks or invent definitions.


## First frequency-candidate batch (English, French, Spanish, German)

The initial candidate lists are staged under `datasets/frequency-candidates/` from wordhoard's top-2,000 samples at pinned revision `1bc5730e8d6e682c416c03680b7cb8c6c7ca8cd0`. The source dataset is CC BY-SA 4.0; the upstream code is MIT. Its frequency backbone is OpenSubtitles-2018 via FrequencyWords. This is a subtitle-domain ranking, not a balanced general-language corpus. The staged lists preserve raw occurrence counts and POS, collapse duplicate lemmas, and are not yet FRUCHST dictionaries. English/French/Spanish definitions remain outstanding for all four language lists. See `datasets/frequency-candidates/README.md` and `tools/stage_wordhoard.py`.

## unilex frequency-candidate batch (20 languages)

After the wordhoard batch above and the 27-language FrequencyWords batch (`tools/stage_frequencywords.py`, `frequencywords-*.json`), twenty further profiles are staged from Unicode **unilex** (`unicode-org/unilex`, revision `b3a9ae27a0c29eac217bb15a75c6f8d8efb15d17`) by `tools/stage_unilex.py`. unilex data is under the permissive Unicode Data Files license (Unicode-DFS-2016) and its frequencies come from Google Corpus Crawler web and scripture crawls, scaled to per-billion-token values. Variety mappings are explicit in the stager; `fa-AF` is used for Dari (`prs`) and Gurmukhi `pa` for Eastern Punjabi. Details and limitations are in `datasets/frequency-candidates/README.md`.

Remaining blockers after this batch (29 profiles, no frequency candidates):
- **No licensed source reachable:** Egyptian, Levantine, Sudanese, Algerian, Moroccan, Saidi, Najdi and Mesopotamian Arabic; Yue, Wu, Jin, Min Nan, Hakka, Xiang and Gan Chinese; Western Punjabi (Shahmukhi), Bhojpuri, Sindhi, Nepali, Assamese, Saraiki, Lingala, Zulu, Xhosa, West Central Oromo. Neither FrequencyWords, wordfreq nor unilex publishes these codes. Leipzig Corpora Collection, OSCAR/CulturaX, and Wikipedia dumps are candidates but need per-corpus license review.
- **Too small in unilex:** Igbo (693 forms), Nigerian Fulfulde (749 forms).
- **Macrolanguage only:** Northern and Southern Pashto (unilex `ps` does not identify the variety).
