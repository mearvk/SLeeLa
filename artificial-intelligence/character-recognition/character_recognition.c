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
    memset(model->samples[model->count].label, 0, SLCR_LABEL_CAPACITY);
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
    memset(result->label, 0, sizeof(result->label));
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

#define SLCR_MODEL_MAGIC "SLCRM01"
#define SLCR_MODEL_MAX_SAMPLES 1000000u

int slcr_model_save(const slcr_model *model, const char *path) {
    FILE *f;
    uint64_t count;
    size_t i;
    if (!model || !path || model->count > SLCR_MODEL_MAX_SAMPLES) return -1;
    f = fopen(path, "wb");
    if (!f) return -1;
    count = (uint64_t)model->count;
    if (fwrite(SLCR_MODEL_MAGIC, 1, 7, f) != 7 ||
        fwrite(&count, sizeof(count), 1, f) != 1) {
        fclose(f); return -1;
    }
    for (i = 0; i < model->count; ++i) {
        if (fwrite(model->samples[i].label, 1, SLCR_LABEL_CAPACITY, f) != SLCR_LABEL_CAPACITY ||
            fwrite(model->samples[i].pixels, 1, SLCR_GLYPH_PIXELS, f) != SLCR_GLYPH_PIXELS) {
            fclose(f); return -1;
        }
    }
    return fclose(f) == 0 ? 0 : -1;
}

slcr_model *slcr_model_load(const char *path) {
    FILE *f;
    char magic[7];
    uint64_t count;
    slcr_model *model;
    size_t i;
    if (!path || !(f = fopen(path, "rb"))) return NULL;
    if (fread(magic, 1, 7, f) != 7 || memcmp(magic, SLCR_MODEL_MAGIC, 7) != 0 ||
        fread(&count, sizeof(count), 1, f) != 1 || count > SLCR_MODEL_MAX_SAMPLES) {
        fclose(f); return NULL;
    }
    model = slcr_model_create();
    if (!model) { fclose(f); return NULL; }
    for (i = 0; i < (size_t)count; ++i) {
        char label[SLCR_LABEL_CAPACITY];
        uint8_t pixels[SLCR_GLYPH_PIXELS];
        size_t label_length;
        if (fread(label, 1, sizeof(label), f) != sizeof(label) ||
            fread(pixels, 1, sizeof(pixels), f) != sizeof(pixels) ||
            !utf8_one_codepoint((const unsigned char *)label, &label_length) ||
            slcr_model_add(model, label, pixels) != 0) {
            slcr_model_destroy(model); fclose(f); return NULL;
        }
    }
    if (fgetc(f) != EOF || ferror(f)) {
        slcr_model_destroy(model); fclose(f); return NULL;
    }
    fclose(f);
    return model;
}

void slcr_free_segments(slcr_box *boxes) { free(boxes); }

int slcr_segment_page(const uint8_t *binary, size_t width, size_t height,
                      size_t minimum_ink_pixels, slcr_box **boxes,
                      size_t *box_count) {
    size_t y, x, count = 0, capacity = 0;
    size_t *rows;
    slcr_box *items = NULL;
    if (!binary || !width || !height || width > SIZE_MAX / height ||
        !boxes || !box_count) return -1;
    *boxes = NULL; *box_count = 0;
    rows = (size_t *)calloc(height, sizeof(size_t));
    if (!rows) return -1;
    for (y = 0; y < height; ++y)
        for (x = 0; x < width; ++x)
            if (binary[y * width + x]) ++rows[y];
    y = 0;
    while (y < height) {
        size_t top, bottom, *cols, line_ink = 0;
        while (y < height && rows[y] == 0) ++y;
        if (y == height) break;
        top = y;
        while (y < height && rows[y] != 0) { line_ink += rows[y]; ++y; }
        bottom = y;
        if (line_ink < minimum_ink_pixels) continue;
        cols = (size_t *)calloc(width, sizeof(size_t));
        if (!cols) { free(rows); free(items); return -1; }
        for (size_t yy = top; yy < bottom; ++yy)
            for (x = 0; x < width; ++x)
                if (binary[yy * width + x]) ++cols[x];
        x = 0;
        while (x < width) {
            size_t left, right, ink = 0;
            while (x < width && cols[x] == 0) ++x;
            if (x == width) break;
            left = x;
            while (x < width && cols[x] != 0) { ink += cols[x]; ++x; }
            right = x;
            if (ink < minimum_ink_pixels) continue;
            if (count == capacity) {
                size_t next = capacity ? capacity * 2 : 32;
                slcr_box *grown;
                if (next < capacity || next > SIZE_MAX / sizeof(slcr_box)) {
                    free(cols); free(rows); free(items); return -1;
                }
                grown = (slcr_box *)realloc(items, next * sizeof(slcr_box));
                if (!grown) { free(cols); free(rows); free(items); return -1; }
                items = grown; capacity = next;
            }
            items[count].x = left; items[count].y = top;
            items[count].width = right - left; items[count].height = bottom - top;
            ++count;
        }
        free(cols);
    }
    free(rows);
    *boxes = items; *box_count = count;
    return 0;
}

int slcr_write_corrected_document(const slcr_model *model,
                        const slcr_document_glyph *glyphs, size_t glyph_count,
                        const char *corrections_tsv,
                        const char *transcript_path,
                        double minimum_similarity) {
    FILE *cf = NULL, *out = NULL;
    char line[128];
    char **labels = NULL;
    size_t i;
    int failed = 0;
    if (!model || (!glyphs && glyph_count) || !transcript_path ||
        !isfinite(minimum_similarity) || minimum_similarity < 0.0 ||
        minimum_similarity > 1.0) return -1;
    labels = (char **)calloc(glyph_count ? glyph_count : 1, sizeof(char *));
    if (!labels) return -1;
    if (corrections_tsv && (cf = fopen(corrections_tsv, "rb")) == NULL) {
        free(labels); return -1;
    }
    while (cf && fgets(line, sizeof(line), cf)) {
        char *tab = strchr(line, '\t');
        char *end;
        unsigned long index;
        size_t n;
        if (!tab) { failed = 1; break; }
        *tab++ = '\0';
        end = strchr(tab, '\r'); if (end) *end = '\0';
        end = strchr(tab, '\n'); if (end) *end = '\0';
        index = strtoul(line, &end, 10);
        if (*line == '\0' || *end || index >= glyph_count ||
            !utf8_one_codepoint((const unsigned char *)tab, &n)) {
            failed = 1; break;
        }
        labels[index] = (char *)malloc(n + 1);
        if (!labels[index]) { failed = 1; break; }
        memcpy(labels[index], tab, n + 1);
    }
    if (cf && ferror(cf)) failed = 1;
    if (cf) fclose(cf);
    out = failed ? NULL : fopen(transcript_path, "wb");
    if (!out) failed = 1;
    for (i = 0; i < glyph_count && !failed; ++i) {
        slcr_result result;
        const char *label = NULL;
        if (glyphs[i].line_break_before && fputc('\n', out) == EOF) { failed = 1; break; }
        if (glyphs[i].space_before && !glyphs[i].line_break_before &&
            fputc(' ', out) == EOF) { failed = 1; break; }
        if (labels[i]) label = labels[i];
        else if (glyphs[i].pixels &&
                 slcr_model_recognize(model, glyphs[i].pixels, &result) == 0 &&
                 result.similarity >= minimum_similarity) label = result.label;
        else label = "\xEF\xBF\xBD";
        if (fputs(label, out) == EOF) failed = 1;
    }
    if (out && fclose(out) != 0) failed = 1;
    for (i = 0; i < glyph_count; ++i) free(labels[i]);
    free(labels);
    return failed ? -1 : 0;
}
