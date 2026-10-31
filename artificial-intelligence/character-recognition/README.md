# Character Recognition

A small, dependency-light C11 foundation for recognizing **clean, carefully written,
pre-segmented European characters** and producing a reviewable document transcript.

## What it does

- Normalizes one grayscale glyph crop into a centered 28×28 binary image.
- Builds an in-memory nearest-neighbour sample bank from labelled glyphs.
- Accepts one UTF-8 code point per label, including European characters such as
  `é`, `è`, `ü`, `ñ`, `ç`, `ø`, `å`, `æ`, and `ß`.
- Recognizes a glyph against the registered examples.
- Writes a UTF-8 transcript and a JSONL sidecar containing the source reference,
  per-glyph result, pixel-similarity score, and acceptance flag.
- Replaces below-threshold readings with U+FFFD (`�`) so uncertain characters
  are visible for human review.

## Scope and important limits

This is a **character classifier**, not a complete page-scanning OCR system.
The caller must segment a page into individual glyph crops and supply their
reading order, spaces, and line breaks. The first version is designed for
separated, clean glyphs. Connected cursive writing, overlapping strokes,
automatic word/line segmentation, page deskew, and language-model correction
are not implemented. A connected handwritten word must not be passed as one
glyph and expected to decode correctly.

The nearest-neighbour score is a normalized pixel-similarity measure, **not a
statistical probability or legal confidence rating**. The threshold must be
calibrated against representative handwriting samples. Register multiple
examples per writer and character; a single alphabet specimen is not adequate
for reliable archival transcription.

## Document preservation

The writer creates two separate outputs:

1. A plain UTF-8 transcript for search and reading.
2. A JSONL review sidecar that records the source reference and each character's
   recognition result.

It never opens or modifies the source scan. Preserve the original scan as the
authoritative source; compare and human-review the transcript before relying
on it in legal work. This prototype does not provide a digital signature,
chain-of-custody guarantee, or evidentiary certification.

## C interface

Include `character_recognition.h` and compile
`character_recognition.c` as C11. Create a model, add labelled normalized
28×28 glyphs with `slcr_model_add`, normalize incoming grayscale crops with
`slcr_normalize_glyph`, and classify them with `slcr_model_recognize`.

For a document, construct an ordered `slcr_document_glyph` array. Set
`space_before` or `line_break_before` from the page segmentation stage, then
call `slcr_write_document` with distinct transcript and review-sidecar paths.

The model is currently in memory only; applications must load their approved,
labelled training samples and register them at startup. A portable, versioned
model-file format and page segmentation are future work.

## Regression check

From the repository root:

```sh
make -C tests ai-character-recognition
```
