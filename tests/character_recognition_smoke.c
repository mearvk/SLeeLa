#include "../artificial-intelligence/character-recognition/character_recognition.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void make_glyph(uint8_t pixels[SLCR_GLYPH_PIXELS], int variant) {
    size_t x, y;
    memset(pixels, 0, SLCR_GLYPH_PIXELS);
    for (y = 4; y < 24; ++y) {
        for (x = 5; x < 23; ++x) {
            if (variant == 0) {
                if (x == 14 - (int)(y - 4) / 2 ||
                    x == 14 + (int)(y - 4) / 2 ||
                    (y == 15 && x >= 9 && x <= 19))
                    pixels[y * SLCR_GLYPH_WIDTH + x] = 255;
            } else if (x == 8 || x == 19 || y == 4 || y == 23) {
                pixels[y * SLCR_GLYPH_WIDTH + x] = 255;
            }
        }
    }
}

int main(void) {
    slcr_model *model = slcr_model_create();
    uint8_t a[SLCR_GLYPH_PIXELS], e[SLCR_GLYPH_PIXELS];
    uint8_t gray[49], normalized[SLCR_GLYPH_PIXELS];
    slcr_result result;
    slcr_document_glyph glyphs[2];
    FILE *file;
    slcr_model *loaded = NULL;
    slcr_box *boxes = NULL;
    size_t box_count = 0;
    uint8_t page[20 * 16];
    uint8_t *pgm = NULL;
    size_t pgm_width = 0, pgm_height = 0;
    char line[256];
    int ok = 1;

    if (!model) return 1;
    make_glyph(a, 0);
    make_glyph(e, 1);
    if (slcr_model_add(model, "A", a) != 0 ||
        slcr_model_add(model, "é", e) != 0 ||
        slcr_model_add(model, "AB", a) == 0) {
        fprintf(stderr, "character recognition: sample registration failed\n");
        slcr_model_destroy(model);
        return 1;
    }
    if (slcr_model_recognize(model, a, &result) != 0 ||
        strcmp(result.label, "A") != 0 || result.similarity < 0.999999) {
        fprintf(stderr, "character recognition: exact glyph lookup failed\n");
        ok = 0;
    }
    memset(gray, 255, sizeof(gray));
    gray[3 * 7 + 3] = 0;
    gray[4 * 7 + 3] = 0;
    gray[5 * 7 + 3] = 0;
    if (slcr_normalize_glyph(gray, 7, 7, normalized, 128) != 0) {
        fprintf(stderr, "character recognition: glyph normalization failed\n");
        ok = 0;
    }
    if (slcr_model_save(model, "character-recognition-smoke.model") != 0 ||
        !(loaded = slcr_model_load("character-recognition-smoke.model"))) {
        fprintf(stderr, "character recognition: model persistence failed\n");
        ok = 0;
    } else {
        if (slcr_model_recognize(loaded, e, &result) != 0 || strcmp(result.label, "é") != 0) ok = 0;
        slcr_model_destroy(loaded);
    }
    file = fopen("character-recognition-smoke.pgm", "wb");
    if (file) {
        fputs("P2\n# tiny grayscale page\n3 2\n255\n0 255 0\n255 0 255\n", file);
        fclose(file);
    }
    if (!file || slcr_load_pgm("character-recognition-smoke.pgm", &pgm,
                               &pgm_width, &pgm_height) != 0 ||
        pgm_width != 3 || pgm_height != 2 || pgm[0] != 0 || pgm[1] != 255) {
        fprintf(stderr, "character recognition: PGM page loading failed\n");
        ok = 0;
    }
    free(pgm);
    remove("character-recognition-smoke.pgm");
    memset(page, 0, sizeof(page));
    for (size_t y = 2; y < 8; ++y) {
        for (size_t x = 2; x < 5; ++x) page[y * 20 + x] = 255;
        for (size_t x = 10; x < 14; ++x) page[y * 20 + x] = 255;
    }
    if (slcr_segment_page(page, 20, 16, 1, &boxes, &box_count) != 0 || box_count != 2) {
        fprintf(stderr, "character recognition: page segmentation failed\n");
        ok = 0;
    }
    slcr_free_segments(boxes);
    glyphs[0].pixels = a;
    glyphs[0].line_break_before = 0;
    glyphs[0].space_before = 0;
    glyphs[1].pixels = e;
    glyphs[1].line_break_before = 0;
    glyphs[1].space_before = 1;
    if (ok && slcr_write_document(model, glyphs, 2, "original-scan.pgm",
                                  "character-recognition-smoke.txt",
                                  "character-recognition-smoke.jsonl",
                                  0.90) != 0) {
        fprintf(stderr, "character recognition: document output failed\n");
        ok = 0;
    }
    if (ok) {
        file = fopen("character-recognition-smoke.txt", "rb");
        if (!file || !fgets(line, sizeof(line), file) ||
            strcmp(line, "A é") != 0) {
            fprintf(stderr, "character recognition: transcript content mismatch\n");
            ok = 0;
        }
        if (file) fclose(file);
    }
    if (ok) {
        file = fopen("character-recognition-smoke.jsonl", "rb");
        if (!file || !fgets(line, sizeof(line), file) ||
            !strstr(line, "original-scan.pgm")) {
            fprintf(stderr, "character recognition: provenance sidecar missing\n");
            ok = 0;
        }
        if (file) fclose(file);
    }
    file = fopen("character-recognition-smoke.tsv", "wb");
    if (file) { fputs("0\tZ\n", file); fclose(file); }
    if (ok && slcr_write_corrected_document(model, glyphs, 2,
            "character-recognition-smoke.tsv", "character-recognition-corrected.txt", 0.90) != 0) ok = 0;
    if (ok) {
        file = fopen("character-recognition-corrected.txt", "rb");
        if (!file || !fgets(line, sizeof(line), file) || strcmp(line, "Z é") != 0) ok = 0;
        if (file) fclose(file);
    }
    remove("character-recognition-smoke.model");
    remove("character-recognition-smoke.tsv");
    remove("character-recognition-corrected.txt");
    remove("character-recognition-smoke.txt");
    remove("character-recognition-smoke.jsonl");
    slcr_model_destroy(model);
    if (!ok) return 1;
    puts("character recognition: PASS");
    return 0;
}
