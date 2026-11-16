# FRUCHST definition candidate pipeline

`fruchst_definition_queue.py` creates versioned **review queues**, not production definitions.
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
- Every matching headword/POS gloss is retained independently for each
  Wiktionary edition, including senses without a Wikidata QID.
- A cross-edition alignment candidate is additionally proposed only when all
  three editions expose the same **sense-level** Wikidata QID and normalized
  part of speech.
- Gloss arrays and source line/sense evidence are retained; nothing is silently
  selected when multiple senses exist.
- QID/POS agreement is a candidate signal, not proof of equivalence. Every
  aligned candidate remains `review_status: required`; no production
  definition is approved by this tool.
- Missing edition glosses go to manual lookup. Missing QIDs do not cause glosses
  to be discarded; those remain independent edition candidates for human
  sense alignment.

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

## Current source dump locations

The tool reads either plain JSONL or `.jsonl.gz` directly. Current Kaikki raw-data
downloads (check the catalog for newer revisions before each production run):

- English edition: https://kaikki.org/dictionary/raw-wiktextract-data.jsonl.gz
  (about 2.9 GB compressed; English glosses)
- French edition: https://kaikki.org/frwiktionary/raw-wiktextract-data.jsonl.gz
  (about 702.5 MB compressed; French glosses; dump revision 2026-10-01)
- Spanish edition: https://kaikki.org/eswiktionary/raw-wiktextract-data.jsonl.gz
  (about 98.8 MB compressed; Spanish glosses; dump revision 2026-11-01)

The corresponding uncompressed extracts are very large (English about 24.8 GB,
French about 6.4 GB, Spanish about 1.1 GB). Keep source dumps outside the
repository; never commit them. The verified catalog revisions used by the staging workflow are English
2026-10-01, French 2026-10-01, and Spanish 2026-11-01. Refresh these revision
labels when Kaikki publishes newer dumps. Generated review queues record the
exact download URL, dump revision, retrieval timestamp, compressed-file size,
and SHA-256 for each edition. The SHA-256 identifies the actual bytes used.

Source catalog: https://kaikki.org/dictionary/rawdata.html
