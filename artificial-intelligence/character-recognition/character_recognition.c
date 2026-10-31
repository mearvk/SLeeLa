#include "character_recognition.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct slcr_sample {
    char label[SLCR_LABEL_CAPACITY];
    uint8_t pixels[SLCR_GLYPH_PIXELS];
} slcr_sample;

struct slcr_model {
    slcr_sample *samples;
    size_t count;
    size_t capacity;
};

static int utf8_one_codepoint(const unsigned char *s, size_t *length) {
    size_t n, i;
    unsigned char c;
    if (!s || !s[0]) return 0;
    c = s[0];
    if (c < 0x80) n = 1;
    else if (c >= 0xC2 && c <= 0xDF) n = 2;
    else if (c >= 0xE0 && c <= 0xEF) n = 3;
    else if (c >= 0xF0 && c <= 0xF4) n = 4;
    else return 0;
    for (i = 1; i < n; ++i)
        if ((s[i] & 0xC0) != 0x80) return 0;
    if (n == 3 && ((c == 0xE0 && s[1] < 0xA0) ||
                   (c == 0xED && s[1] >= 0xA0))) return 0;
    if (n == 4 && ((c == 0xF0 && s[1] < 0x90) ||
                   (c == 0xF4 && s[1] >= 0x90))) return 0;
    if (s[n] != '\0') return 0;
    *length = n;
    return 1;
}

slcr_model *slcr_model_create(void) {
    return (slcr_model *)calloc(1, sizeof(slcr_model));
}

void slcr_model_destroy(slcr_model *model) {
    if (!model) return;
    free(model->samples);
    free(model);
}

int slcr_model_add(slcr_model *model, const char *label,
                   const uint8_t pixels[SLCR_GLYPH_PIXELS]) {
    size_t label_length;
    slcr_sample *grown;
    if (!model || !label || !pixels ||
        !utf8_one_codepoint((const unsigned char *)label, &label_length))
        return -1;
    if (model->count == model->capacity) {
        size_t next = model->capacity ? model->capacity * 2 : 32;
        if (next < model->capacity || next > SIZE_MAX / sizeof(slcr_sample))
            return -1;
        grown = (slcr_sample *)realloc(model->samples,
                                      next * sizeof(slcr_sample));
        if (!grown) return -1;
        model->samples = grown;
        model->capacity = next;
    }
    memcpy(model->samples[model->count].label, label, label_length + 1);
    memcpy(model->samples[model->count].pixels, pixels, SLCR_GLYPH_PIXELS);
    ++model->count;
    return 0;
}

int slcr_normalize_glyph(const uint8_t *gray, size_t width, size_t height,
                         uint8_t out[SLCR_GLYPH_PIXELS], uint8_t threshold) {
    size_t x, y, min_x, min_y, max_x, max_y, box_w, box_h;
    size_t draw_w, draw_h, offset_x, offset_y;
    if (!gray || !out || width == 0 || height == 0 ||
        width > SIZE_MAX / height) return -1;
    memset(out, 0, SLCR_GLYPH_PIXELS);
    min_x = width; min_y = height; max_x = max_y = 0;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            if (gray[y * width + x] < threshold) {
                if (x < min_x) min_x = x;
                if (y < min_y) min_y = y;
                if (x > max_x) max_x = x;
                if (y > max_y) max_y = y;
            }
        }
    }
    if (min_x == width) return -1; /* Blank crop. */
    box_w = max_x - min_x + 1;
    box_h = max_y - min_y + 1;
    if (box_w >= box_h) {
        draw_w = 20;
        draw_h = (box_h * 20 + box_w / 2) / box_w;
        if (draw_h == 0) draw_h = 1;
    } else {
        draw_h = 20;
        draw_w = (box_w * 20 + box_h / 2) / box_h;
        if (draw_w == 0) draw_w = 1;
    }
    if (draw_w > 20) draw_w = 20;
    if (draw_h > 20) draw_h = 20;
    offset_x = (SLCR_GLYPH_WIDTH - draw_w) / 2;
    offset_y = (SLCR_GLYPH_HEIGHT - draw_h) / 2;
    for (y = 0; y < draw_h; ++y) {
        size_t sy = min_y + (y * box_h) / draw_h;
        for (x = 0; x < draw_w; ++x) {
            size_t sx = min_x + (x * box_w) / draw_w;
            if (gray[sy * width + sx] < threshold)
                out[(offset_y + y) * SLCR_GLYPH_WIDTH + offset_x + x] = 255;
        }
    }
    return 0;
}

int slcr_model_recognize(const slcr_model *model,
                         const uint8_t pixels[SLCR_GLYPH_PIXELS],
                         slcr_result *result) {
    size_t i, j, best = 0;
    double best_distance = HUGE_VAL;
    const double maximum_distance =
        (double)SLCR_GLYPH_PIXELS * 255.0 * 255.0;
    if (!model || !pixels || !result || model->count == 0) return -1;
    for (i = 0; i < model->count; ++i) {
        double distance = 0.0;
        for (j = 0; j < SLCR_GLYPH_PIXELS; ++j) {
            double delta = (double)pixels[j] -
                           (double)model->samples[i].pixels[j];
            distance += delta * delta;
        }
        if (distance < best_distance) {
            best_distance = distance;
            best = i;
        }
    }
    memcpy(result->label, model->samples[best].label, SLCR_LABEL_CAPACITY);
    result->similarity = 1.0 - sqrt(best_distance / maximum_distance);
    if (result->similarity < 0.0) result->similarity = 0.0;
    return 0;
}

static int write_json_string(FILE *file, const char *value) {
    const unsigned char *p = (const unsigned char *)(value ? value : "");
    if (fputc('"', file) == EOF) return -1;
    while (*p) {
        if (*p == '"' || *p == '\\') {
            if (fputc('\\', file) == EOF || fputc(*p, file) == EOF) return -1;
        } else if (*p < 0x20) {
            if (fprintf(file, "\\u%04x", (unsigned int)*p) < 0) return -1;
        } else if (fputc(*p, file) == EOF) return -1;
        ++p;
    }
    return fputc('"', file) == EOF ? -1 : 0;
}

int slcr_write_document(const slcr_model *model,
                        const slcr_document_glyph *glyphs, size_t glyph_count,
                        const char *source_reference,
                        const char *transcript_path,
                        const char *review_jsonl_path,
                        double minimum_similarity) {
    FILE *transcript = NULL, *review = NULL;
    size_t i;
    int failed = 0;
    if (!model || (!glyphs && glyph_count) || !transcript_path ||
        !review_jsonl_path || !isfinite(minimum_similarity) ||
        minimum_similarity < 0.0 || minimum_similarity > 1.0)
        return -1;
    transcript = fopen(transcript_path, "wb");
    if (!transcript) return -1;
    review = fopen(review_jsonl_path, "wb");
    if (!review) {
        fclose(transcript);
        return -1;
    }
    if (fprintf(review, "{\"type\":\"document\",\"source\":") < 0 ||
        write_json_string(review, source_reference) != 0 ||
        fprintf(review, ",\"recognizer\":\"SLCR-nearest-glyph-v1\"}\n") < 0)
        failed = 1;
    for (i = 0; i < glyph_count && !failed; ++i) {
        slcr_result result;
        int recognized = glyphs[i].pixels &&
            slcr_model_recognize(model, glyphs[i].pixels, &result) == 0;
        const char *written = (recognized &&
            result.similarity >= minimum_similarity) ? result.label : "\xEF\xBF\xBD";
        if (glyphs[i].line_break_before && fputc('\n', transcript) == EOF)
            failed = 1;
        else if (glyphs[i].space_before && !glyphs[i].line_break_before &&
                 fputc(' ', transcript) == EOF)
            failed = 1;
        if (!failed && fputs(written, transcript) == EOF) failed = 1;
        if (failed) break;
        if (fprintf(review, "{\"type\":\"glyph\",\"index\":%zu,"
                    "\"label\":", i) < 0 ||
            write_json_string(review, recognized ? result.label : "") != 0 ||
            fprintf(review, ",\"similarity\":%.6f,\"accepted\":%s}\n",
                    recognized ? result.similarity : 0.0,
                    (recognized && result.similarity >= minimum_similarity) ?
                        "true" : "false") < 0)
            failed = 1;
    }
    if (fclose(transcript) != 0) failed = 1;
    if (fclose(review) != 0) failed = 1;
    return failed ? -1 : 0;
}
