#include "character_recognition.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *program) {
    fprintf(stderr,
        "Usage: %s MODEL.slcrm INPUT.pgm OUTPUT.txt REVIEW.jsonl "
        "[pixel-threshold] [minimum-similarity]\n"
        "\n"
        "Recognizes clean, separated glyphs in a PGM page using a saved SLCR model.\n"
        "pixel-threshold: 0..255 (default 180; darker pixels become ink)\n"
        "minimum-similarity: 0..1 (default 0.70)\n"
        "\n"
        "The source PGM is read-only. Keep the original scan for comparison.\n",
        program);
}

static int parse_unsigned(const char *text, unsigned long *value) {
    char *end = NULL;
    unsigned long parsed;
    errno = 0;
    parsed = strtoul(text, &end, 10);
    if (errno || !text[0] || !end || *end) return -1;
    *value = parsed;
    return 0;
}

static int parse_similarity(const char *text, double *value) {
    char *end = NULL;
    double parsed;
    errno = 0;
    parsed = strtod(text, &end);
    if (errno || !text[0] || !end || *end || !isfinite(parsed) ||
        parsed < 0.0 || parsed > 1.0) return -1;
    *value = parsed;
    return 0;
}

int main(int argc, char **argv) {
    slcr_model *model = NULL;
    uint8_t *gray = NULL, *binary = NULL, *normalized = NULL;
    size_t width = 0, height = 0, pixel_count, box_count = 0, i;
    slcr_box *boxes = NULL;
    slcr_document_glyph *glyphs = NULL;
    unsigned long threshold = 180;
    double minimum_similarity = 0.70;
    size_t minimum_ink = 1;
    int status = EXIT_FAILURE;

    if (argc == 2 && (strcmp(argv[1], "--help") == 0 ||
                      strcmp(argv[1], "-h") == 0)) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }
    if (argc < 5 || argc > 7) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (argc >= 6 && (parse_unsigned(argv[5], &threshold) != 0 ||
                      threshold > 255)) {
        fprintf(stderr, "Invalid pixel threshold: expected 0..255.\n");
        return EXIT_FAILURE;
    }
    if (argc == 7 && parse_similarity(argv[6], &minimum_similarity) != 0) {
        fprintf(stderr, "Invalid minimum similarity: expected 0..1.\n");
        return EXIT_FAILURE;
    }

    model = slcr_model_load(argv[1]);
    if (!model) {
        fprintf(stderr, "Could not load recognition model: %s\n", argv[1]);
        goto cleanup;
    }
    if (slcr_load_pgm(argv[2], &gray, &width, &height) != 0 ||
        width > SIZE_MAX / height) {
        fprintf(stderr, "Could not read an 8-bit P2/P5 PGM page: %s\n", argv[2]);
        goto cleanup;
    }
    pixel_count = width * height;
    binary = (uint8_t *)malloc(pixel_count);
    if (!binary) {
        fprintf(stderr, "Out of memory while preparing page.\n");
        goto cleanup;
    }
    for (i = 0; i < pixel_count; ++i)
        binary[i] = gray[i] < (uint8_t)threshold ? 255 : 0;

    if (slcr_segment_page(binary, width, height, minimum_ink,
                          &boxes, &box_count) != 0 || box_count == 0) {
        fprintf(stderr, "No glyph regions found; check scan polarity and threshold.\n");
        goto cleanup;
    }
    if (box_count > SIZE_MAX / sizeof(*glyphs) ||
        box_count > SIZE_MAX / SLCR_GLYPH_PIXELS) {
        fprintf(stderr, "Page contains too many glyph regions.\n");
        goto cleanup;
    }
    glyphs = (slcr_document_glyph *)calloc(box_count, sizeof(*glyphs));
    normalized = (uint8_t *)calloc(box_count, SLCR_GLYPH_PIXELS);
    if (!glyphs || !normalized) {
        fprintf(stderr, "Out of memory while preparing glyphs.\n");
        goto cleanup;
    }

    for (i = 0; i < box_count; ++i) {
        size_t x, y, crop_size;
        uint8_t *crop;
        const slcr_box *box = &boxes[i];
        if (box->width > SIZE_MAX / box->height) {
            fprintf(stderr, "Invalid glyph crop dimensions.\n");
            goto cleanup;
        }
        crop_size = box->width * box->height;
        crop = (uint8_t *)malloc(crop_size);
        if (!crop) {
            fprintf(stderr, "Out of memory while cropping glyph.\n");
            goto cleanup;
        }
        for (y = 0; y < box->height; ++y)
            for (x = 0; x < box->width; ++x)
                crop[y * box->width + x] =
                    gray[(box->y + y) * width + box->x + x];
        if (slcr_normalize_glyph(crop, box->width, box->height,
                normalized + i * SLCR_GLYPH_PIXELS, (uint8_t)threshold) == 0)
            glyphs[i].pixels = normalized + i * SLCR_GLYPH_PIXELS;
        else
            glyphs[i].pixels = NULL;
        free(crop);

        glyphs[i].line_break_before =
            i > 0 && box->y != boxes[i - 1].y;
        if (i > 0 && !glyphs[i].line_break_before) {
            size_t previous_right = boxes[i - 1].x + boxes[i - 1].width;
            size_t gap = box->x > previous_right ? box->x - previous_right : 0;
            size_t space_threshold = box->height / 3;
            if (space_threshold < 3) space_threshold = 3;
            glyphs[i].space_before = gap >= space_threshold;
        }
    }

    if (slcr_write_document(model, glyphs, box_count, argv[2],
                            argv[3], argv[4], minimum_similarity) != 0) {
        fprintf(stderr, "Could not write transcript and review sidecar.\n");
        goto cleanup;
    }
    printf("Recognized %zu glyph regions.\nTranscript: %s\nReview: %s\n",
           box_count, argv[3], argv[4]);
    status = EXIT_SUCCESS;

cleanup:
    free(glyphs);
    free(normalized);
    free(boxes);
    free(binary);
    free(gray);
    slcr_model_destroy(model);
    return status;
}
