/*
 * ballum/src/ballum_image.c
 * Image handler implementation (see ballum_image.h). Decodes JPEG via the
 * bundled nanojpeg and exposes scaled/centered sampling for a splash.
 */
#include "sleela_ballum/ballum_image.h"
#include "nanojpeg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ballum_image_init(ballum_image_t *img) {
    if (!img) return;
    img->width = 0;
    img->height = 0;
    img->pixels = NULL;
}

void ballum_image_free(ballum_image_t *img) {
    if (!img) return;
    free(img->pixels);
    img->pixels = NULL;
    img->width = 0;
    img->height = 0;
}

int ballum_image_ready(const ballum_image_t *img) {
    return (img && img->pixels && img->width > 0 && img->height > 0) ? 1 : 0;
}

ballum_status_t ballum_image_load_memory(ballum_image_t *img,
                                         const void *data, size_t size) {
    if (!img) return BALLUM_STATUS_INVALID_ARGUMENT;
    ballum_image_free(img);
    if (!data || size == 0) return BALLUM_STATUS_INVALID_ARGUMENT;

    njInit();
    if (njDecode(data, (int)size) != NJ_OK) {
        njDone();
        return BALLUM_STATUS_INVALID_ARGUMENT;
    }
    int w = njGetWidth();
    int h = njGetHeight();
    int color = njIsColor();            /* 1 = RGB, 0 = grayscale */
    const unsigned char *src = njGetImage();
    if (w <= 0 || h <= 0 || !src) { njDone(); return BALLUM_STATUS_BACKEND_ERROR; }

    ballum_color_t *buf = (ballum_color_t *)malloc((size_t)w * (size_t)h * sizeof(ballum_color_t));
    if (!buf) { njDone(); return BALLUM_STATUS_BACKEND_ERROR; }

    /* nanojpeg emits 3 bytes/pixel for color, 1 byte/pixel for grayscale. */
    const int bpp = color ? 3 : 1;
    for (int i = 0; i < w * h; ++i) {
        if (color) {
            buf[i].r = src[i * bpp + 0];
            buf[i].g = src[i * bpp + 1];
            buf[i].b = src[i * bpp + 2];
        } else {
            unsigned char g = src[i];
            buf[i].r = g; buf[i].g = g; buf[i].b = g;
        }
        buf[i].a = 255;
    }
    njDone();

    img->width = w;
    img->height = h;
    img->pixels = buf;
    return BALLUM_STATUS_OK;
}

ballum_status_t ballum_image_load_file(ballum_image_t *img, const char *path) {
    if (!img || !path) return BALLUM_STATUS_INVALID_ARGUMENT;
    FILE *f = fopen(path, "rb");
    if (!f) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return BALLUM_STATUS_BACKEND_ERROR; }
    long n = ftell(f);
    if (n <= 0) { fclose(f); return BALLUM_STATUS_BACKEND_ERROR; }
    rewind(f);
    void *data = malloc((size_t)n);
    if (!data) { fclose(f); return BALLUM_STATUS_BACKEND_ERROR; }
    size_t got = fread(data, 1, (size_t)n, f);
    fclose(f);
    if (got != (size_t)n) { free(data); return BALLUM_STATUS_BACKEND_ERROR; }
    ballum_status_t st = ballum_image_load_memory(img, data, (size_t)n);
    free(data);
    return st;
}

static ballum_color_t transparent(void) {
    ballum_color_t c; c.r = 0; c.g = 0; c.b = 0; c.a = 0; return c;
}

ballum_color_t ballum_image_at(const ballum_image_t *img, int x, int y) {
    if (!ballum_image_ready(img) || x < 0 || y < 0 || x >= img->width || y >= img->height)
        return transparent();
    return img->pixels[(size_t)y * (size_t)img->width + (size_t)x];
}

ballum_color_t ballum_image_sample_fit(const ballum_image_t *img,
                                       int dst_w, int dst_h, int dx, int dy) {
    if (!ballum_image_ready(img) || dst_w <= 0 || dst_h <= 0) return transparent();

    /* Fit preserving aspect: scale so the image fits inside dst, then center. */
    /* fitted size = image * min(dst_w/iw, dst_h/ih), computed in integers. */
    long iw = img->width, ih = img->height;
    long fit_w, fit_h;
    /* compare iw/ih vs dst_w/dst_h via cross-multiply to pick the limiting axis */
    if (iw * (long)dst_h >= ih * (long)dst_w) {
        fit_w = dst_w;
        fit_h = ih * (long)dst_w / iw;
    } else {
        fit_h = dst_h;
        fit_w = iw * (long)dst_h / ih;
    }
    if (fit_w <= 0 || fit_h <= 0) return transparent();

    long off_x = ((long)dst_w - fit_w) / 2;
    long off_y = ((long)dst_h - fit_h) / 2;
    long lx = dx - off_x;
    long ly = dy - off_y;
    if (lx < 0 || ly < 0 || lx >= fit_w || ly >= fit_h) return transparent();

    /* nearest-neighbor map back into source */
    int sx = (int)(lx * iw / fit_w);
    int sy = (int)(ly * ih / fit_h);
    return ballum_image_at(img, sx, sy);
}
