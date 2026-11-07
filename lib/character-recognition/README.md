# SLeeLa Character Recognition

SLeeLa-facing package for the native C11 glyph-recognition subsystem in
`artificial-intelligence/character-recognition/`. The native C API remains
authoritative for image normalization, sample storage, recognition, page
segmentation, model persistence, and transcript writing.

## Public units

- `SLPackage` — package identity and native ABI contract.
- `SLCharacterRecognition` — glyph dimensions, thresholds, and operation contract.
- `SLCharacterResult` — label and pixel-similarity result record.

## Scope

The native classifier recognizes clean, pre-segmented 28×28 glyphs using
nearest-neighbour pixel similarity. The native page segmenter is projection
based and is intended for clean, separated writing. This is not a general
OCR/layout engine: touching cursive letters, complex page layouts, deskew,
language-model correction, and calibrated probability estimates are not
provided.

A score is a pixel-similarity measure, not a probability or legal confidence
rating. Preserve the original scan and review the transcript and JSONL sidecar.
The native API does not modify the source image.

## Native bridge

A host adapter maps these classes to `character_recognition.h` and the
`slcr_*` C functions. Compile `character_recognition.c` as C11 and link the
native object. PGM loading and segmentation allocate native buffers; ownership
and matching free calls must be respected by the adapter.

Run native regression checks from the repository root:

```sh
make -C tests ai-character-recognition character-recognition-cli
```
