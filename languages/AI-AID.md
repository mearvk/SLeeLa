# AI-AID: model of the `languages/` folder

For Kiro and any other AI assistant. Read this before editing or summarizing the folder. Each subfolder has a `DEGREE.MD` stating how complete and valid it is.

## Purpose

`languages/` holds three independent things. Do not merge them.

| Part | Files | Model |
|---|---|---|
| Localization packs | `*.lang` (12: en es fr de pt it ja zh hi ko th ar), `KEYS.md`, `settings.conf` | `key = value` UTF-8 text. English is the reference key set. Missing keys fall back to English. Read by `lib/languages` (`SLLanguage`, `SLLanguageCatalog`). Documented in `README.md`. |
| Language research profiles | 80 folders named by language (`english`, `mandarin-chinese`, ... `maithili`) | Top 80 languages by total speakers (2025 ranking). Each holds `README.md` (research template), `alphabet.json`, `ALPHABET.xml`. Statistics are per country and year, never traits of speakers. |
| FRUCHST dictionaries | `FRUCHST_FORMAT.md`, `FRUCHST_SOURCES.md`, `FRUCHST_STATUS.md`, `FRUCHST_TARGETS.json`, `datasets/`, `tools/` | One binary `fruchst.<code>.bin` per profile: 1,000 frequency-ranked native headwords, each with aligned English, French and Spanish definitions. |

## Ground rules

1. A profile folder is not "done" until its corpus, definition sources and licenses are reviewed and its binary validates (`tools/fruchst.py`).
2. Never invent ranks, frequencies, definitions, statistics or sources. Unknown means "to be researched" or "none".
3. Never present frequency candidates as approved definitions. Every candidate is `review_status: required`; approved count is 0.
4. Keep the three parts independent. Alphabet files and `.lang` packs do not depend on FRUCHST.
5. Social indicators (education, fertility, savings, well-being) belong to geographies, not languages. Do not compute a combined "aptitude" score.
6. Respect upstream licenses recorded in `FRUCHST_TARGETS.json` (`license_review`). Do not copy third-party text that is not listed there.
7. Source of truth for numbers: `FRUCHST_STATUS.md` and `FRUCHST_TARGETS.json`. `DEGREE.MD` files are derived snapshots (2026-11-16). On conflict, the sources win and the `DEGREE.MD` should be refreshed.

## Pipeline stages (used by every `DEGREE.MD`)

| Tier | Meaning |
|---|---|
| C0 | Scaffold only. No licensed frequency source found. |
| C1 | 1,000-word list staged. Definitions negligible (under 50 words with all three). |
| C2 | Word list plus partial EN/FR/ES candidate definitions. |
| C3 | Near-complete candidates, still unreviewed. |
| C4 | Reviewed and approved definitions. (none yet) |
| C5 | Binary built and validated. (none yet) |

Degree percent = 10 (scaffold) + 20 (word list) + 30 x (all-three definitions / 1,000). Review and binary make up the other 40 points, currently 0 everywhere. No folder can exceed 60% until review starts.

## Overall state (2026-11-16)

- 51 of 80 profiles have a 1,000-word list; 29 have none (no licensed source).
- 0 approved definitions, 0 binaries built.
- Profile `README.md` research fields are untouched templates in all 80 folders.
- Alphabet inventories are `provisional`.

## Where to look

- Format: `FRUCHST_FORMAT.md`. Targets: `FRUCHST_TARGETS.json`. Status per profile: `FRUCHST_STATUS.md`.
- Pipeline scripts and tests: `tools/` (see `tools/DEGREE.MD`). Data: `datasets/` (see `datasets/DEGREE.MD`).
