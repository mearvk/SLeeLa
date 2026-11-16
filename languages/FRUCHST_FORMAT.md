# FRUCHST Binary Dictionary Format — Version 1

**Status:** Initial stable format specification  
**Encoding:** UTF-8, Unicode NFC  
**Scope:** Ranked lexical entries with aligned English, French, and Spanish definitions

## 1. Purpose

FRUCHST stores a ranked word list for one language. Each entry contains the native headword and three definitions that describe the same lexical sense in English, French, and Spanish. A dictionary is language-specific; definitions are not substitutes for a separate translation lexicon.

## 2. File naming

Each language profile stores one file named `fruchst.<iso-639-1>.bin` where a two-letter ISO 639-1 code exists. Profiles without an ISO 639-1 code use a documented ISO 639-3 code. The profile's `language.json` is authoritative for the exact BCP 47 locale and ISO identifiers.

## 3. Binary layout

All integers are unsigned big-endian. Floating-point values are IEEE-754 binary64 big-endian. All strings are UTF-8 and MUST be Unicode NFC. Lengths count bytes, not Unicode code points.

### 3.1 Fixed header (72 bytes)

| Offset | Size | Field | Meaning |
|---:|---:|---|---|
| 0 | 8 | magic | ASCII `FRUCHST\0` |
| 8 | 2 | major | 1 |
| 10 | 2 | minor | 0 |
| 12 | 2 | header_size | 72 |
| 14 | 2 | flags | Bit 0: frequency values present; other bits reserved and zero |
| 16 | 4 | entry_count | Must be exactly 1000 for production dictionaries |
| 20 | 2 | language_tag_len | Byte length of BCP 47 language tag |
| 22 | 2 | source_id_len | Byte length of source dataset identifier |
| 24 | 8 | generated_epoch | UTC Unix seconds; zero for reproducible builds |
| 32 | 8 | payload_len | Bytes following the fixed header |
| 40 | 32 | payload_sha256 | SHA-256 of all bytes following the fixed header |

### 3.2 Variable payload

The payload begins with the language tag and source identifier, each encoded as its declared number of UTF-8 bytes. These are followed by exactly `entry_count` records.

Each record is:

| Field | Type | Meaning |
|---|---|---|
| rank | uint16 | One-based frequency rank; contiguous from 1 |
| frequency | float64 | Non-negative source frequency; zero if unavailable |
| headword_len | uint32 | UTF-8 byte length |
| headword | bytes | Native-language headword |
| definition_en_len | uint32 | UTF-8 byte length |
| definition_en | bytes | English definition |
| definition_fr_len | uint32 | UTF-8 byte length |
| definition_fr | bytes | French definition |
| definition_es_len | uint32 | UTF-8 byte length |
| definition_es | bytes | Spanish definition |

A zero-length headword or definition is invalid. Production records must have unique normalized headwords. Definitions must express the same selected sense in all three languages.

## 4. Ranking and frequency

Ranks are contiguous and authoritative. The input data must already be ordered by the chosen corpus-frequency methodology. Frequency values must use a consistent unit within a dictionary (for example, occurrences per million tokens); the unit and corpus methodology belong in the adjacent provenance manifest. Do not compare raw frequencies across languages unless their corpus methodology is demonstrably comparable.

## 5. Provenance and licensing

The binary embeds a source identifier. The adjacent `fruchst.<code>.manifest.json` records:
- source name, version, URL, retrieval date, and corpus description;
- source license and attribution requirements;
- frequency unit and tokenization methodology;
- definition source, license, attribution, and sense-selection method;
- generator version and SHA-256 of the binary.

Do not bundle data unless its license permits redistribution. A source URL alone is not proof of permission. Do not claim a word list is corpus-ranked when its ordering is editorial or synthetic.

## 6. Validation requirements

A production dictionary MUST:
1. have the exact magic, supported version, and 72-byte header;
2. declare exactly 1,000 entries;
3. have a matching payload length and SHA-256;
4. use a valid BCP 47 language tag and NFC UTF-8 strings;
5. have ranks 1 through 1000 without gaps or duplicates;
6. have unique non-empty headwords;
7. have all three non-empty definitions for every entry;
8. have non-negative finite frequencies;
9. have a matching provenance manifest and redistributable source license.

## 7. Compatibility

Readers MUST reject unsupported major versions and unknown required flags. Minor-version additions may use only reserved fields or explicitly documented optional extensions. Existing alphabet XML and JSON files are independent and are not modified by FRUCHST.

## 8. Reference implementation

The reference writer and validator are in `languages/tools/fruchst.py`. The input JSON schema and dataset acquisition rules are documented in `languages/datasets/README.md`.
