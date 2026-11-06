# Tokenizers

A model's tokenizer is part of its model contract. The current `LanguageModelVocabulary` API is an exact-token vocabulary primitive with whitespace splitting; it is not a production subword tokenizer.

Store tokenizer metadata and assets alongside each model, including algorithm and version, normalization rules, special-token definitions, vocabulary IDs, merge/model files, and any byte-fallback rules. A training backend must reject or explicitly migrate mismatched tokenizer/vocabulary versions rather than silently accepting incompatible token IDs.

Future tokenizer implementations should expose a common text-to-token-ID interface and preserve the existing vocabulary API for simple/custom token sets.

## Base (seed) vocabulary — the premises a model indexes into

Before any corpus tokens are added, a model needs a stable, known set of token
IDs to build its neural premises (embedding rows, special-token positions) on.
That seed set is declared here and loaded natively.

| File | Role |
|---|---|
| `base-vocabulary.json` | The seed token-ID table: reserved special tokens at fixed low IDs, then digits, punctuation/symbols, closed-class function words, and a compact open-class core. Original SLeeLa authorship, CC0. |
| `base-vocabulary.schema.json` | JSON Schema (draft-07) the base set must satisfy. |
| `../large_language_models_base_vocabulary.h/.c` | C11 loader (`sllmbv_*`, ABI v1) that seeds the set into an `sllmv_vocabulary`, verifying each token lands at its declared ID (fail closed). |
| `../large_language_models_base_vocabulary.hpp/.cpp` | C++17 RAII facade (`sleela::ai::seed_default_base_vocabulary`). |

### The ID contract

- **Special tokens occupy the lowest, reserved IDs** and precede every ordinary
  token: `<pad>`=0, `<unk>`=1, `<bos>`=2, `<eos>`=3, `<sep>`=4, `<mask>`=5, plus
  role markers `<sys>`/`<user>`/`<asst>`/`<tool>`. `<pad>` is pinned to 0 so a
  zero-filled batch is safe padding.
- IDs are a **gap-free `0..N-1` sequence**; each entry's `id` equals its array
  position. The native store assigns stable insertion-order IDs, so loading the
  base set first guarantees the declared IDs.
- The loader **fails closed**: it refuses a non-empty target vocabulary and
  errors if any token does not land at its declared ID.

```c
sllmv_vocabulary *v; sllmv_vocabulary_create(&v);
sllmbv_specials sp;
sllmbv_load_default(v, &sp);              /* seeds sleela-base-en-1.0 */
uint32_t pad = sllmbv_special_id(&sp, SLLMBV_ROLE_PAD);   /* 0 */
```

## Building a vocabulary from the base set + lexical corpus

`tools/build_vocabulary.py` is the sanctioned bridge from lexical records to
token IDs. It lays the base set down first (preserving reserved IDs), then
appends corpus-derived token **strings** with the next free IDs.

```sh
python3 tools/build_vocabulary.py \
  --base tokenizers/base-vocabulary.json \
  --corpus vocabulary/corpus/core-en.jsonl \
  --out built-vocab.txt --report build-report.json --strict
```

Rules the builder enforces (see `../vocabulary/` for the lexical side):

- **A token ID is never taken from a dictionary headword.** Headwords and their
  synonym/antonym surface forms are run through the tokenizer algorithm
  (whitespace: NFC + lowercase + split); only the resulting strings get IDs.
- **`reference_only` records contribute nothing** — their (absent) definition
  text never crosses the bridge. `--strict` makes the builder fail if such a
  record carries content.
- Only token strings cross the bridge; definitions stay in the lexical store.

## The gate

`tools/validate_vocabulary.py` validates the base set (schema + ID contract) and
the lexical corpus (schema + licensing discipline), failing closed with exact
locations. It is wired into CI as `.github/workflows/llm-vocabulary.yml` and
uses `jsonschema` when installed (and a built-in subset check otherwise).
