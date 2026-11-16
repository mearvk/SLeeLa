# FRUCHST definition candidate pipeline

`fruchst_definition_queue.py` creates a **review queue**, not production definitions.
It reads one 1,000-word frequency candidate file and three Kaikki/Wiktextract
JSONL extracts: English, French, and Spanish Wiktionary editions.

Example (Spanish headwords from the first batch):

```sh
python3 languages/tools/fruchst_definition_queue.py \
  languages/datasets/frequency-candidates/wordhoard-es.json es \
  --en /data/kaikki/enwiktionary.jsonl \
  --fr /data/kaikki/frwiktionary.jsonl \
  --es /data/kaikki/eswiktionary.jsonl \
  --output /tmp/fruchst-review/es.json
```

Repeat for `en`, `fr`, and `de`, changing the candidate file, target
Wiktionary language code, and output path. Each extract must be a UTF-8 JSONL
file from the named Wiktionary edition. The input can be the uncompressed
Kaikki/Wiktextract dump; decompress it before invoking this script.

## Alignment safeguards

- Exact NFC headword matching is required; no fuzzy spelling substitution.
- The entry's `lang_code` must match the target language (not the Wiktionary
  edition's language).
- A triple is proposed only when all three editions expose the same
  **sense-level** Wikidata QID and normalized part of speech.
- Gloss arrays and source line/sense evidence are retained; nothing is silently
  selected when multiple senses exist.
- QID/POS agreement is a candidate signal, not proof of equivalence. Every
  triple remains `review_status: required`; no production definition is
  approved by this tool.
- Missing QIDs, missing editions, and unmatched senses go to manual lookup.

## Source and license

Kaikki publishes machine-readable Wiktionary extractions. Wiktionary content
is dual-licensed under CC BY-SA 4.0 and the GNU Free Documentation License
(GFDL). Preserve attribution and comply with the applicable license terms;
verify the source edition's notices before redistributing derived definitions.
The frequency-candidate files retain their separate wordhoard/OpenSubtitles
attribution and CC BY-SA 4.0 dataset notice. Software license does not replace
the data licenses.

No FRUCHST binary should be built until a human reviewer has selected one
sense per headword, confirmed that the three glosses express that same sense,
checked spelling/grammar, and recorded source/license metadata. The current
binary builder's all-definitions-present check is necessary but not sufficient
for semantic approval.

See the Wiktextract format documentation:
https://github.com/tatuylonen/wiktextract#format-of-the-extracted-word-entries
and Kaikki data catalog:
https://kaikki.org/
