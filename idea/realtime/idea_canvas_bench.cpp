/*
 * idea/realtime/idea_canvas_bench.cpp
 * Idea System(TM) realtime canvas -- physics-core demo + benchmark.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Exercises the graphics-independent physics core: builds a grid, excites a
 * few cells, advances many 120 Hz steps under OpenMP, and reports throughput
 * and a small ASCII intensity map (so the result is legible headless).  This
 * is the part that is BUILT AND VERIFIED in the Idea System environment.
 */
#include "idea_canvas.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>

#if defined(_OPENMP)
#  include <omp.h>
#endif

int main(int argc, char **argv) {
    int cols = argc > 1 ? atoi(argv[1]) : 256;
    int rows = argc > 2 ? atoi(argv[2]) : 256;
    int steps = argc > 3 ? atoi(argv[3]) : 1200;   /* 10 s at 120 Hz */

    idea_canvas *c = idea_canvas_create(cols, rows);
    if (!c) { std::fprintf(stderr, "create failed\n"); return 1; }

    idea_sim_params p = idea_sim_defaults();

#if defined(_OPENMP)
    std::printf("OpenMP: %d threads\n", omp_get_max_threads());
#else
    std::printf("OpenMP: not enabled (serial)\n");
#endif
    std::printf("grid: %dx%d (%zu vertices)  steps: %d @ %.0f Hz\n",
                cols, rows, idea_canvas_count(c), steps, IDEA_CANVAS_SIM_HZ);

    /* excite a few "importance" points */
    idea_canvas_excite(c, cols / 2, rows / 2, 1.0f);
    idea_canvas_excite(c, cols / 4, rows / 4, 0.8f);
    idea_canvas_excite(c, 3 * cols / 4, 3 * rows / 4, 0.8f);

    const float dt = (float)idea_canvas_timestep();
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < steps; ++i) {
        if (i % 240 == 0) idea_canvas_excite(c, cols / 2, rows / 2, 0.6f);
        idea_canvas_step(c, &p, dt);
    }
    auto t1 = std::chrono::steady_clock::now();
    double secs = std::chrono::duration<double>(t1 - t0).count();

    double vps = (double)idea_canvas_count(c) * steps / secs;
    std::printf("ran %d steps in %.3f s  ->  %.1f steps/s  (%.2f M vertex-updates/s)\n",
                steps, secs, steps / secs, vps / 1e6);
    std::printf("headroom vs 120 Hz: %.1fx\n", (steps / secs) / IDEA_CANVAS_SIM_HZ);

    /* peak / mean intensity so the field is checkable beyond the ASCII ramp */
    {
        const idea_vertex *VV = idea_canvas_vertices_const(c);
        float peak = 0.0f; double sum = 0.0;
        for (size_t i = 0; i < idea_canvas_count(c); ++i) {
            if (VV[i].intensity > peak) peak = VV[i].intensity;
            sum += VV[i].intensity;
        }
        std::printf("field: peak=%.4f  mean=%.5f\n", peak, sum / idea_canvas_count(c));
    }

    /* tiny ASCII intensity map (downsampled) */
    const char *ramp = " .:-=+*#%@";
    const idea_vertex *V = idea_canvas_vertices_const(c);
    int sw = 40, sh = 16;
    /* normalise to the current peak so a decayed-but-alive field is legible,
     * and downsample by MAX over each block so bright cells are not missed. */
    float peakv = 1e-6f;
    for (size_t i = 0; i < idea_canvas_count(c); ++i)
        if (V[i].intensity > peakv) peakv = V[i].intensity;
    std::printf("intensity field (normalised to peak=%.3f):\n", peakv);
    for (int sy = 0; sy < sh; ++sy) {
        for (int sx = 0; sx < sw; ++sx) {
            int c0 = (int)((double)sx / sw * cols), c1 = (int)((double)(sx + 1) / sw * cols);
            int r0 = (int)((double)sy / sh * rows), r1 = (int)((double)(sy + 1) / sh * rows);
            if (c1 <= c0) c1 = c0 + 1;
            if (r1 <= r0) r1 = r0 + 1;
            float m = 0.0f;
            for (int row = r0; row < r1 && row < rows; ++row)
                for (int col = c0; col < c1 && col < cols; ++col) {
                    float v = V[(size_t)row * cols + col].intensity;
                    if (v > m) m = v;
                }
            int idx = (int)((m / peakv) * 9.0f + 0.5f);
            if (idx > 9) idx = 9; if (idx < 0) idx = 0;
            std::putchar(ramp[idx]);
        }
        std::putchar('\n');
    }

    idea_canvas_destroy(c);
    return 0;
}
