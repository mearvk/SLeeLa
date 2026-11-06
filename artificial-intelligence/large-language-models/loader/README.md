# Loader-handler idiom

A small, careful way for users to bring **their own classes/data** — words,
maths, objects — and load them into trainable features against a known
vocabulary or a real neural net. The user writes one tiny **handler**; the
framework owns the careful machinery.

## The idiom, in one sentence

> Implement `encode()` for YOUR type; the framework calls it once per item with
> framework-owned, correctly-sized feature and target slices, and feeds each
> encoded example to a sink (e.g. a real `slnn_*` trainer).

```c
sllme_item_result my_encode(void *user_data, const void *item,
                            double *features, size_t fw,
                            double *target,  size_t tw) {
    const MyThing *x = item;           /* your type */
    features[0] = x->a; features[1] = x->b;   /* write exactly fw features */
    target[0]   = x->label;                   /* write exactly tw targets  */
    return SLLME_ITEM_OK;              /* or SLLME_ITEM_SKIP / SLLME_ITEM_ERROR */
}
```

That is the whole contract. The user never touches a network, a buffer, or an
allocator.

## Why it is subtle, performant, and clear

- **Subtle** — one vtable (`encode`, optional `describe`, two widths). Nothing
  else to implement; no inheritance, no framework types leak into the handler.
- **Performant** — a single caller-owned `sllme_scratch` is sized once and
  **reused for every item**: zero per-item heap allocation in the hot loop. The
  handler writes straight into framework slices; the framework zero-fills them
  first so sparse encodings (one-hot) set only nonzero slots.
- **Clear** — fixed feature/target widths, validated once by `sllme_validate`
  before the loop; every item is encoded identically; a misfit **fails closed**
  (`SLLME_WIDTH_MISMATCH`) instead of silently truncating.

## Careful by construction

The loader never fabricates data. A handler that cannot encode an item returns
`SLLME_ITEM_SKIP` (counted and omitted, honestly) or `SLLME_ITEM_ERROR` (the
load stops). A word not in the known vocabulary is the handler's decision — map
it to `<unk>` or skip it — the framework does not guess. Counts are reported
(`seen` / `loaded` / `skipped`) so a load is auditable.

## Training against known weights / vocabulary

`example_loader_slnn.{h,c}` provides a ready-made sink that feeds every encoded
example straight into `slnn_network_train_sample` — a **real** in-process weight
update. Point a word handler at a known vocabulary (`sllmv_*` / the seed base
set) and the loader turns "load these words" into genuine training against the
network's weights.

## Worked handlers (see `example_loader_demo.c`)

| Handler | Encodes | Target |
|---|---|---|
| word | a word -> one-hot over a known-vocabulary id window | next word's one-hot (unknown words skipped) |
| math | `(a, b)` normalized | `a + b` normalized (regression) |
| object | a flat record of named numeric fields (schema-driven) | a label (classification) |

The demo loads all three and trains a real `slnn_*` net on each; it also shows
the fail-closed paths.

## Files

| File | Role |
|---|---|
| `example_loader.h/.c` | C11 loader-handler core (`sllme_*`, ABI v1): handler vtable, reused scratch, array loader, reports |
| `example_loader_slnn.h/.c` | A sink that trains a real `slnn_*` network from encoded examples |
| `example_loader_demo.c` | Words (vs a known vocabulary), maths, objects -> real net training; fail-closed checks |

## Build and run

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic \
   example_loader.c example_loader_slnn.c \
   ../large_language_models_vocabulary.c ../large_language_models_base_vocabulary.c \
   ../../neural-nets/neural_nets.c example_loader_demo.c -o example_loader_demo -lm
./example_loader_demo
```

## What is NOT bundled

No weights, no datasets. The user supplies the items, the handler, and (for word
handlers) a known vocabulary. SLeeLa ships the loader machinery and the real
training sink.
