/*
 * ballum/include/sleela_ballum/ballum_image.h
 * Ballum image handler -- load a JPEG/encoded image into an RGBA pixel buffer a
 * session composites onto the display. Decoding is done with the bundled
 * nanojpeg (MIT); the handler owns the decoded buffer and offers the geometry
 * a splash/effect needs (scaled, centered placement on a target display).
 *
 * Fail closed: a load that cannot decode leaves the image empty (ready()==0);
 * sampling an empty image returns transparent, never garbage.
 */
#ifndef SLEELA_BALLUM_IMAGE_H
#define SLEELA_BALLUM_IMAGE_H

#include <stddef.h>
#include <stdint.h>
#include "ballum_manager.h"   /* ballum_color_t */

#ifdef __cplusplus
extern "C" {
#endif

/* A decoded image: tightly-packed RGBA, top-left origin, straight alpha. */
typedef struct {
    int             width;
    int             height;
    ballum_color_t *pixels;   /* width*height, owned; NULL when not loaded */
} ballum_image_t;

/* Initialize to empty (no allocation). */
void ballum_image_init(ballum_image_t *img);

/* Decode an encoded image (JPEG) in memory into `img` as RGBA. Returns
 * BALLUM_STATUS_OK on success; on any decode failure `img` is left empty and a
 * non-OK status is returned. Frees any previous contents first. */
ballum_status_t ballum_image_load_memory(ballum_image_t *img,
                                         const void *data, size_t size);

/* Decode an encoded image from a file path. Same contract as load_memory. */
ballum_status_t ballum_image_load_file(ballum_image_t *img, const char *path);

/* 1 if the image holds decoded pixels, else 0. */
int ballum_image_ready(const ballum_image_t *img);

/* Sample the image at (x,y). Out-of-range or empty -> fully transparent black.
 * Straight alpha. */
ballum_color_t ballum_image_at(const ballum_image_t *img, int x, int y);

/* Sample the image as if scaled to fit (dst_w x dst_h) preserving aspect and
 * centered, at destination pixel (dx,dy). Nearest-neighbor. Pixels outside the
 * fitted image are transparent. This is what a full-screen splash uses to place
 * a logo centered on the display without distortion. */
ballum_color_t ballum_image_sample_fit(const ballum_image_t *img,
                                       int dst_w, int dst_h, int dx, int dy);

/* Release the decoded buffer and reset to empty. */
void ballum_image_free(ballum_image_t *img);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BALLUM_IMAGE_H */
