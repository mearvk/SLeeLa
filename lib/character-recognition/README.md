# SLeeLa Character Recognition

SLeeLa-facing package for the native C11 glyph-recognition subsystem in
`artificial-intelligence/character-recognition/`. The native C API remains
authoritative for normalization, sample storage, recognition, segmentation,
model persistence, and transcript writing. The SLeeLa classes do not claim
that those operations run in the VM without an installed host adapter.

## Public units

- `SLPackage` — package identity and native ABI contract.
- `SLCharacterRecognition` — glyph dimensions, thresholds, and operation contract.
- `SLCharacterResult` — label and pixel-similarity result record.

## Native bridge contract and limits

The adapter maps `SLCharacterRecognition` to `character_recognition.h` and
the `slcr_*` C functions. Glyph buffers contain exactly 784 bytes in
row-major 28×28 order. Grayscale input dimensions must match the input buffer;
normalization emits the native binary convention (foreground 255, background
0). Native labels are one UTF-8 code point and fit in
`SLCR_LABEL_CAPACITY` bytes including the terminator. The adapter must
preserve the native model/buffer ownership rules and pair segment allocations
with `slcr_free_segments`.

The native classifier recognizes clean, pre-segmented 28×28 glyphs using
nearest-neighbour pixel similarity. The page segmenter is projection based and
is intended for clean, separated writing. This is not a general OCR/layout
engine: touching cursive letters, complex page layouts, deskew, language-model
correction, and calibrated probability estimates are not provided.

A similarity score is not a probability or legal confidence rating. Preserve
the original scan and review the transcript and JSONL sidecar. The native API
does not modify the source image.

Compile `character_recognition.c` as C11 and link the native object. PGM
loading and segmentation allocate native buffers; ownership and matching free
calls must be respected by the adapter.

Run native regression checks from the repository root:

```sh
make -C tests ai-character-recognition character-recognition-cli
```
