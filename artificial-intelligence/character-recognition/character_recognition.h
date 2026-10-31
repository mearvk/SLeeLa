#ifndef SLEEELA_CHARACTER_RECOGNITION_H
#define SLEEELA_CHARACTER_RECOGNITION_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLCR_GLYPH_WIDTH 28u
#define SLCR_GLYPH_HEIGHT 28u
#define SLCR_GLYPH_PIXELS (SLCR_GLYPH_WIDTH * SLCR_GLYPH_HEIGHT)
#define SLCR_LABEL_CAPACITY 8u

typedef struct slcr_model slcr_model;

typedef struct slcr_result {
    char label[SLCR_LABEL_CAPACITY];
    double similarity; /* Pixel similarity, not a calibrated probability. */
} slcr_result;

typedef struct slcr_document_glyph {
    const uint8_t *pixels; /* Normalized 28x28, foreground=255, background=0. */
    int line_break_before;
    int space_before;
} slcr_document_glyph;

/* Creates/destroys an in-memory nearest-neighbour glyph model. */
slcr_model *slcr_model_create(void);
void slcr_model_destroy(slcr_model *model);

/* Add one labelled 28x28 training glyph. Label must be one UTF-8 code point. */
int slcr_model_add(slcr_model *model, const char *utf8_label,
                   const uint8_t pixels[SLCR_GLYPH_PIXELS]);

/* Normalize a grayscale glyph crop into a centered 28x28 binary glyph. */
int slcr_normalize_glyph(const uint8_t *gray, size_t width, size_t height,
                         uint8_t out[SLCR_GLYPH_PIXELS], uint8_t threshold);

/* Predict the nearest training glyph. Returns 0 on success, -1 on error. */
int slcr_model_recognize(const slcr_model *model,
                         const uint8_t pixels[SLCR_GLYPH_PIXELS],
                         slcr_result *result);

/*
 * Write a UTF-8 transcript and JSONL review sidecar.
 * The source image is referenced for provenance and is never modified.
 * A glyph below minimum_similarity is written as U+FFFD in the transcript.
 */
int slcr_write_document(const slcr_model *model,
                        const slcr_document_glyph *glyphs, size_t glyph_count,
                        const char *source_reference,
                        const char *transcript_path,
                        const char *review_jsonl_path,
                        double minimum_similarity);

#ifdef __cplusplus
}
#endif
#endif
