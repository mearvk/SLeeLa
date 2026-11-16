# FRUCHST status: words and English/French/Spanish definitions

Snapshot as of 2026-11-16. Counts come from the committed data in `datasets/frequency-candidates/`, `datasets/definition-review/` and `datasets/definition-pivot/`. No definition is approved yet, so no `fruchst.<code>.bin` has been built.

## Summary

| Measure | Count |
|---|---|
| Profiles with a 1,000-word list | 51 of 80 (51,000 of 80,000 words) |
| Words with an English definition | 28,111 |
| Words with a French definition or equivalent | 25,010 |
| Words with a Spanish definition or equivalent | 22,485 |
| Words with all three (direct Wiktionary only) | 4,462 |
| Words with all three (direct or through English) | 22,344 |
| Of those, part of speech matched | 21,357 |
| Approved definitions | 0 |
| Profiles with all 1,000 words fully defined | 0 |

A word counts as having all three when it has an English Wiktionary definition and a French and a Spanish definition or equivalent. French and Spanish come either directly from French/Spanish Wiktionary or through English: the English word from the definition is looked up in English Wiktionary translation tables and in French/Spanish Wiktionary (`tools/fruchst_english_pivot.py`, see `datasets/frequency-candidates/DEFINITION-PIPELINE.md`). "Part of speech matched" means the English word was found with the same part of speech as the original sense.

## Per profile

| Rank | Profile | Tag | Words | EN | FR | ES | All 3 | All 3, POS matched | Word list / blocker |
|---|---|---|---|---|---|---|---|---|---|
| 1 | english | `en` | 1,000 | 1000 | 970 | 962 | 960 | 884 | wordhoard-en.json |
| 2 | mandarin-chinese | `zh-Hans` | 1,000 | 508 | 737 | 492 | 492 | 278 | frequencywords-zh-Hans.json |
| 3 | hindi | `hi` | 1,000 | 911 | 578 | 535 | 533 | 518 | frequencywords-hi.json |
| 4 | spanish | `es` | 1,000 | 935 | 924 | 931 | 920 | 915 | wordhoard-es.json |
| 5 | standard-arabic | `ar` | 1,000 | 431 | 374 | 371 | 366 | 357 | frequencywords-ar.json |
| 6 | french | `fr` | 1,000 | 904 | 910 | 880 | 878 | 876 | wordhoard-fr.json |
| 7 | bengali | `bn` | 1,000 | 540 | 443 | 443 | 441 | 398 | frequencywords-bn.json |
| 8 | portuguese | `pt` | 1,000 | 973 | 950 | 669 | 668 | 663 | frequencywords-pt.json |
| 9 | russian | `ru` | 1,000 | 983 | 698 | 475 | 473 | 453 | frequencywords-ru.json |
| 10 | indonesian | `id` | 1,000 | 820 | 802 | 719 | 717 | 697 | frequencywords-id.json |
| 11 | urdu | `ur` | 1,000 | 597 | 494 | 494 | 493 | 478 | frequencywords-ur.json |
| 12 | standard-german | `de` | 1,000 | 716 | 723 | 680 | 677 | 670 | wordhoard-de.json |
| 13 | japanese | `ja` | 1,000 | 896 | 901 | 836 | 834 | 772 | frequencywords-ja.json |
| 14 | nigerian-pidgin | `pcm` | 1,000 | 41 | 35 | 33 | 33 | 33 | unilex-pcm.json |
| 15 | egyptian-arabic | `arz` | — | — | — | — | — | — | No licensed frequency source found |
| 16 | marathi | `mr` | 1,000 | 403 | 358 | 357 | 356 | 337 | unilex-mr.json |
| 17 | vietnamese | `vi` | 1,000 | 977 | 940 | 887 | 887 | 874 | frequencywords-vi.json |
| 18 | telugu | `te` | 1,000 | 399 | 254 | 261 | 254 | 243 | frequencywords-te.json |
| 19 | hausa | `ha` | 1,000 | 208 | 343 | 192 | 182 | 172 | unilex-ha.json |
| 20 | turkish | `tr` | 1,000 | 739 | 531 | 494 | 493 | 455 | frequencywords-tr.json |
| 21 | western-punjabi | `pnb-Arab` | — | — | — | — | — | — | No licensed frequency source found |
| 22 | swahili | `sw` | 1,000 | 846 | 594 | 538 | 535 | 521 | unilex-sw.json |
| 23 | tagalog | `tl` | 1,000 | 626 | 533 | 525 | 524 | 511 | frequencywords-tl.json |
| 24 | tamil | `ta` | 1,000 | 481 | 411 | 389 | 389 | 372 | frequencywords-ta.json |
| 25 | yue-chinese | `yue` | — | — | — | — | — | — | No licensed frequency source found |
| 26 | wu-chinese | `wuu` | — | — | — | — | — | — | No licensed frequency source found |
| 27 | iranian-persian | `fa` | 1,000 | 514 | 456 | 445 | 444 | 436 | frequencywords-fa.json |
| 28 | korean | `ko` | 1,000 | 423 | 379 | 349 | 349 | 323 | frequencywords-ko.json |
| 29 | thai | `th` | 1,000 | 257 | 232 | 225 | 225 | 213 | frequencywords-th.json |
| 30 | javanese | `jv` | 1,000 | 312 | 94 | 97 | 94 | 90 | unilex-jv.json |
| 31 | italian | `it` | 1,000 | 960 | 920 | 655 | 649 | 645 | frequencywords-it.json |
| 32 | gujarati | `gu` | 1,000 | 312 | 279 | 276 | 276 | 258 | unilex-gu.json |
| 33 | levantine-arabic | `apc` | — | — | — | — | — | — | No licensed frequency source found |
| 34 | amharic | `am` | 1,000 | 265 | 258 | 259 | 253 | 247 | unilex-am.json |
| 35 | kannada | `kn` | 1,000 | 146 | 137 | 137 | 137 | 133 | unilex-kn.json |
| 36 | bhojpuri | `bho` | — | — | — | — | — | — | No licensed frequency source found |
| 37 | sudanese-arabic | `apd` | — | — | — | — | — | — | No licensed frequency source found |
| 38 | yoruba | `yo` | 1,000 | 356 | 335 | 339 | 324 | 315 | unilex-yo.json |
| 39 | jinyu-chinese | `cjy` | — | — | — | — | — | — | No licensed frequency source found |
| 40 | min-nan-chinese | `nan` | — | — | — | — | — | — | No licensed frequency source found |
| 41 | polish | `pl` | 1,000 | 981 | 586 | 445 | 441 | 411 | frequencywords-pl.json |
| 42 | burmese | `my` | 1,000 | 800 | 759 | 752 | 752 | 730 | unilex-my.json |
| 43 | algerian-arabic | `arq` | — | — | — | — | — | — | No licensed frequency source found |
| 44 | hakka-chinese | `hak` | — | — | — | — | — | — | No licensed frequency source found |
| 45 | lingala | `ln` | — | — | — | — | — | — | No licensed frequency source found |
| 46 | moroccan-arabic | `ary` | — | — | — | — | — | — | No licensed frequency source found |
| 47 | odia | `or` | 1,000 | 209 | 204 | 206 | 204 | 188 | unilex-or.json |
| 48 | ukrainian | `uk` | 1,000 | 642 | 523 | 328 | 327 | 312 | frequencywords-uk.json |
| 49 | xiang-chinese | `hsn` | — | — | — | — | — | — | No licensed frequency source found |
| 50 | malayalam | `ml` | 1,000 | 351 | 304 | 301 | 301 | 269 | frequencywords-ml.json |
| 51 | sindhi | `sd` | — | — | — | — | — | — | No licensed frequency source found |
| 52 | eastern-punjabi | `pa-Guru` | 1,000 | 473 | 360 | 362 | 357 | 328 | unilex-pa-Guru.json |
| 53 | sunda | `su` | 1,000 | 231 | 217 | 216 | 214 | 208 | unilex-su.json |
| 54 | igbo | `ig` | — | — | — | — | — | — | Too few forms (unilex: 693) |
| 55 | dari | `prs` | 1,000 | 758 | 687 | 670 | 670 | 659 | unilex-prs.json |
| 56 | nepali | `ne` | — | — | — | — | — | — | No licensed frequency source found |
| 57 | malay | `ms` | 1,000 | 778 | 743 | 715 | 713 | 690 | frequencywords-ms.json |
| 58 | northern-uzbek | `uz` | 1,000 | 39 | 2 | 3 | 2 | 2 | unilex-uz.json |
| 59 | zulu | `zu` | — | — | — | — | — | — | No licensed frequency source found |
| 60 | saidi-arabic | `aec` | — | — | — | — | — | — | No licensed frequency source found |
| 61 | northern-pashto | `pbu` | — | — | — | — | — | — | Only macrolanguage Pashto (`ps`) available |
| 62 | west-central-oromo | `gaz` | — | — | — | — | — | — | No licensed frequency source found |
| 63 | saraiki | `skr` | — | — | — | — | — | — | No licensed frequency source found |
| 64 | dutch | `nl` | 1,000 | 978 | 891 | 726 | 726 | 709 | frequencywords-nl.json |
| 65 | somali | `so` | 1,000 | 119 | 112 | 115 | 106 | 104 | unilex-so.json |
| 66 | romanian | `ro` | 1,000 | 814 | 603 | 501 | 486 | 472 | frequencywords-ro.json |
| 67 | assamese | `as` | — | — | — | — | — | — | No licensed frequency source found |
| 68 | gan-chinese | `gan` | — | — | — | — | — | — | No licensed frequency source found |
| 69 | southern-pashto | `pst` | — | — | — | — | — | — | Only macrolanguage Pashto (`ps`) available |
| 70 | cebuano | `ceb` | 1,000 | 400 | 371 | 360 | 360 | 349 | unilex-ceb.json |
| 71 | kazakh | `kk` | 1,000 | 373 | 374 | 315 | 315 | 297 | frequencywords-kk.json |
| 72 | magahi | `mag` | 1,000 | 4 | 4 | 4 | 4 | 4 | unilex-mag.json |
| 73 | najdi-arabic | `ars` | — | — | — | — | — | — | No licensed frequency source found |
| 74 | sinhala | `si` | 1,000 | 128 | 100 | 100 | 99 | 97 | frequencywords-si.json |
| 75 | mesopotamian-arabic | `acm` | — | — | — | — | — | — | No licensed frequency source found |
| 76 | xhosa | `xh` | — | — | — | — | — | — | No licensed frequency source found |
| 77 | khmer | `km` | 1,000 | 900 | 861 | 849 | 847 | 831 | unilex-km.json |
| 78 | afrikaans | `af` | 1,000 | 626 | 698 | 555 | 547 | 544 | frequencywords-af.json |
| 79 | nigerian-fulfulde | `fuv` | — | — | — | — | — | — | Too few forms (unilex: 749) |
| 80 | maithili | `mai` | 1,000 | 28 | 18 | 17 | 17 | 16 | unilex-mai.json |

## Blockers

- **No word list (29 profiles).** No licensed frequency source reachable from the build environment covers these varieties; FrequencyWords, wordfreq and unilex were checked. Igbo and Nigerian Fulfulde have fewer than 1,000 usable forms in unilex, and unilex Pashto (`ps`) does not identify Northern or Southern Pashto.
- **Definitions.** Coverage is limited mainly by English Wiktionary: the English pivot only works for words that already have an English definition. Uzbek candidates are Cyrillic while Wiktionary Uzbek is mostly Latin.
- **Review.** Every candidate is `review_status: required`. A reviewer must select one sense per headword with sense-aligned English, French and Spanish definitions before a binary can be built.
