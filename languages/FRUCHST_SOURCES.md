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
