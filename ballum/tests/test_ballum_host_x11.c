/*
 * Integration test for the private-display program source.
 *
 * Needs Xvfb, libXtst.so.6 and an X client (xterm by default, or the program in
 * BALLUM_TEST_X11_PROGRAM). When any of them is missing the source must fail
 * closed WITHOUT leaving files behind, and this test reports SKIPPED and exits
 * 0. When they are present it runs the real thing: launch, capture, 1:1 input.
 */
#define _GNU_SOURCE 1
#include "sleela_ballum/ballum_host.h"
#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define W 640u
#define H 400u

static unsigned count_private_dirs(void) {
    unsigned n = 0;
    const char *bases[2] = { getenv("XDG_RUNTIME_DIR"), "/tmp" };
    for (int b = 0; b < 2; ++b) {
        if (!bases[b] || !*bases[b]) continue;
        DIR *d = opendir(bases[b]);
        if (!d) continue;
        struct dirent *e;
        while ((e = readdir(d))) if (strncmp(e->d_name, "ballum-", 7) == 0) ++n;
        closedir(d);
    }
    return n;
}

static void nap(unsigned ms) {
    struct timespec ts = { (time_t)(ms / 1000u), (long)(ms % 1000u) * 1000000L };
    nanosleep(&ts, NULL);
}

static size_t distinct_colors(const ballum_color_t *p, size_t n) {
    size_t seen = 0; uint32_t first[8];
    for (size_t i = 0; i < n && seen < 8; ++i) {
        uint32_t v = ((uint32_t)p[i].r << 16) | ((uint32_t)p[i].g << 8) | p[i].b;
        size_t j = 0;
        while (j < seen && first[j] != v) ++j;
        if (j == seen) first[seen++] = v;
    }
    return seen;
}

int main(void) {
    const char *prog = getenv("BALLUM_TEST_X11_PROGRAM");
    if (!prog || !*prog) prog = "xterm";
    const char *argv_xterm[] = { prog, "-fa", "Monospace", "-bg", "white", "-fg", "black", NULL };
    const char *argv_other[] = { prog, NULL };
    ballum_program_t p;
    memset(&p, 0, sizeof(p));
    p.argv = getenv("BALLUM_TEST_X11_PROGRAM") ? argv_other : argv_xterm;
    p.sandbox = 1; p.log_fd = -1;

    unsigned dirs_before = count_private_dirs();
    ballum_host_source_t src; void *ctx = NULL;
    ballum_status_t st = ballum_host_source_x11_create(&p, &src, &ctx);
    if (st == BALLUM_STATUS_UNSUPPORTED && !ctx) { puts("test_ballum_host_x11: SKIPPED (built without X11)"); return 0; }
    assert(st == BALLUM_STATUS_OK && ctx && src.start && src.poll_frame && src.key && src.stop);

    st = src.start(ctx, W, H);
    if (st != BALLUM_STATUS_OK) {
        /* missing Xvfb / libXtst / program: fail closed, leave nothing behind */
        assert(st == BALLUM_STATUS_UNSUPPORTED || st == BALLUM_STATUS_BACKEND_UNAVAILABLE ||
               st == BALLUM_STATUS_PERMISSION_DENIED);
        assert(!src.alive(ctx));
        src.stop(ctx);
        ballum_host_source_x11_destroy(ctx);
        assert(count_private_dirs() == dirs_before);
        printf("test_ballum_host_x11: SKIPPED (source failed closed with status %s)\n", ballum_status_name(st));
        return 0;
    }

    /* the program draws something */
    const ballum_color_t *px = NULL; int changed = 0; int got = 0;
    for (int i = 0; i < 400 && !got; ++i) {
        assert(src.poll_frame(ctx, &px, &changed) == BALLUM_STATUS_OK);
        if (px && changed) got = 1; else nap(25);
    }
    assert(got && "the program never produced a frame");
    assert(src.alive(ctx));
    assert(distinct_colors(px, (size_t)W * H) >= 2);

    /* 1:1 input: type into the program and see the frame change */
    ballum_color_t *before = malloc((size_t)W * H * sizeof(ballum_color_t));
    memcpy(before, px, (size_t)W * H * sizeof(ballum_color_t));
    assert(src.pointer(ctx, 100, 100) == BALLUM_STATUS_OK);
    const uint32_t keys[] = { 'e', 'c', 'h', 'o', ' ', 'h', 'i' };
    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
        assert(src.key(ctx, keys[i], 0, 1) == BALLUM_STATUS_OK);
        assert(src.key(ctx, keys[i], 0, 0) == BALLUM_STATUS_OK);
    }
    int moved = 0;
    for (int i = 0; i < 200 && !moved; ++i) {
        nap(25);
        assert(src.poll_frame(ctx, &px, &changed) == BALLUM_STATUS_OK);
        if (changed && memcmp(before, px, (size_t)W * H * sizeof(ballum_color_t)) != 0) moved = 1;
    }
    assert(moved && "typed input did not reach the program");
    free(before);

    src.stop(ctx);
    assert(!src.alive(ctx));
    src.stop(ctx);                                   /* idempotent */
    ballum_host_source_x11_destroy(ctx);
    assert(count_private_dirs() == dirs_before);     /* auth file and directory removed */
    puts("test_ballum_host_x11: ok (real program captured and driven)");
    return 0;
}
