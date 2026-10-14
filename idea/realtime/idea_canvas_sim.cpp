/*
 * idea/realtime/idea_canvas_sim.cpp
 * Idea System(TM) realtime canvas -- physics / compute core.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * The graphics-independent half of the module.  It owns the contiguous vertex
 * block and advances it with an OpenMP + SIMD decay/flow step.  Everything
 * here builds and is verified with any C++20 + OpenMP compiler -- no GL.
 *
 * The GL render layer (idea_canvas_gl.cpp) and this file share the single
 * `struct idea_canvas` definition below.
 */
#include "idea_canvas.h"
#include "idea_canvas_internal.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <new>
#include <vector>

#if defined(_OPENMP)
#  include <omp.h>
#endif

/* ---- lifecycle -------------------------------------------------------- */

idea_canvas *idea_canvas_create(int cols, int rows) {
    if (cols <= 0 || rows <= 0) return nullptr;
    if ((long long)cols * rows > (long long)(1 << 26)) return nullptr; /* sane cap */

    idea_canvas *c = new (std::nothrow) idea_canvas();
    if (!c) return nullptr;
    c->cols = cols;
    c->rows = rows;
    try {
        c->verts.assign((size_t)cols * rows, idea_vertex{});
    } catch (...) {
        delete c;
        return nullptr;
    }

    /* Lay the grid out across the unit square with a flat initial field. */
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            idea_vertex &v = c->verts[(size_t)row * cols + col];
            v.x = (cols > 1) ? (float)col / (float)(cols - 1) : 0.5f;
            v.y = (rows > 1) ? (float)row / (float)(rows - 1) : 0.5f;
            v.r = v.g = v.b = 0.05f;
            v.a = 1.0f;
            v.intensity = 0.0f;
            v.velocity  = 0.0f;
        }
    }
    return c;
}

void idea_canvas_destroy(idea_canvas *c) {
    if (!c) return;
    idea_canvas_gl_teardown(c);   /* no-op when GL absent */
    delete c;
}

int    idea_canvas_cols(const idea_canvas *c)  { return c ? c->cols : 0; }
int    idea_canvas_rows(const idea_canvas *c)  { return c ? c->rows : 0; }
size_t idea_canvas_count(const idea_canvas *c) { return c ? c->verts.size() : 0; }

idea_vertex *idea_canvas_vertices(idea_canvas *c) {
    return c ? c->verts.data() : nullptr;
}
const idea_vertex *idea_canvas_vertices_const(const idea_canvas *c) {
    return c ? c->verts.data() : nullptr;
}

/* ---- parameters ------------------------------------------------------- */

idea_sim_params idea_sim_defaults(void) {
    idea_sim_params p;
    p.decay     = 0.25f;   /* slow global decay so the field lingers           */
    p.flow_gain = 0.9f;
    p.hue_base  = 205.0f;  /* water */
    p.coupling  = 0.06f;   /* small per-step diffusion -> a slow spreading ring */
    return p;
}

idea_canvas_status idea_canvas_excite(idea_canvas *c, int col, int row,
                                      float amount) {
    if (!c) return IDEA_CANVAS_INVALID;
    if (col < 0 || col >= c->cols || row < 0 || row >= c->rows)
        return IDEA_CANVAS_INVALID;
    idea_vertex &v = c->verts[(size_t)row * c->cols + col];
    v.intensity += amount;
    if (v.intensity > 1.0f) v.intensity = 1.0f;
    return IDEA_CANVAS_OK;
}

double idea_canvas_timestep(void) { return 1.0 / IDEA_CANVAS_SIM_HZ; }

/* ---- the step: OpenMP + SIMD decay / flow / colour -------------------- */

/* HSV(h deg, s, v) -> straight-alpha RGB in [0,1]. Branch-light for SIMD. */
static inline void hsv_to_rgb(float h, float s, float v,
                              float &r, float &g, float &b) {
    float c = v * s;
    float hp = h / 60.0f;
    float x = c * (1.0f - std::fabs(std::fmod(hp, 2.0f) - 1.0f));
    float m = v - c;
    float rr = 0, gg = 0, bb = 0;
    int seg = (int)hp % 6; if (seg < 0) seg += 6;
    switch (seg) {
        case 0: rr = c; gg = x; bb = 0; break;
        case 1: rr = x; gg = c; bb = 0; break;
        case 2: rr = 0; gg = c; bb = x; break;
        case 3: rr = 0; gg = x; bb = c; break;
        case 4: rr = x; gg = 0; bb = c; break;
        default: rr = c; gg = 0; bb = x; break;
    }
    r = rr + m; g = gg + m; b = bb + m;
}

idea_canvas_status idea_canvas_step(idea_canvas *c, const idea_sim_params *p,
                                    float dt) {
    if (!c || dt <= 0.0f) return IDEA_CANVAS_INVALID;
    idea_sim_params pr = p ? *p : idea_sim_defaults();

    const int cols = c->cols, rows = c->rows;
    idea_vertex *V = c->verts.data();
    const float keep = std::exp(-pr.decay * dt);  /* multiplicative decay    */

    /* Read neighbour intensities from a snapshot so the step is order-free. */
    std::vector<float> prev(c->verts.size());
    #pragma omp parallel for schedule(static)
    for (long i = 0; i < (long)c->verts.size(); ++i) prev[i] = V[i].intensity;

    /* Main update: parallel over rows, vectorised across each row. */
    #pragma omp parallel for schedule(static)
    for (int row = 0; row < rows; ++row) {
        const size_t base = (size_t)row * cols;
        #pragma omp simd
        for (int col = 0; col < cols; ++col) {
            const size_t i = base + col;

            /* neighbour coupling (Laplacian-ish diffusion of intensity) */
            float acc = 0.0f; int n = 0;
            if (col > 0)        { acc += prev[i - 1];    ++n; }
            if (col < cols - 1) { acc += prev[i + 1];    ++n; }
            if (row > 0)        { acc += prev[i - cols]; ++n; }
            if (row < rows - 1) { acc += prev[i + cols]; ++n; }
            float neigh = (n > 0) ? (acc / (float)n) : prev[i];

            /* diffusion: coupling is a stable per-step coefficient in [0,0.5];
             * it is NOT scaled by the step rate (that made it over-diffuse). */
            float inten = prev[i] * keep
                        + pr.coupling * (neigh - prev[i]);
            if (inten < 0.0f) inten = 0.0f;
            if (inten > 1.0f) inten = 1.0f;

            /* velocity tracks the rate of change (drives colour prediction) */
            float vel = (inten - prev[i]) * IDEA_CANVAS_SIM_HZ;
            V[i].intensity = inten;
            V[i].velocity  = vel;

            /* colour-predictive: hue leads the flow, value follows energy */
            float hue = pr.hue_base + pr.flow_gain * vel * 40.0f;
            if (hue < 0.0f)   hue += 360.0f;
            if (hue >= 360.0f) hue = std::fmod(hue, 360.0f);
            float r, g, b;
            hsv_to_rgb(hue, 0.85f, 0.08f + 0.92f * inten, r, g, b);
            V[i].r = r; V[i].g = g; V[i].b = b; V[i].a = 1.0f;
        }
    }
    return IDEA_CANVAS_OK;
}

int idea_canvas_advance(idea_canvas *c, const idea_sim_params *p,
                        double frame_dt_seconds, double *accumulator) {
    const double step = idea_canvas_timestep();
    int steps = 0;
    if (!c || !accumulator) return 0;
    if (frame_dt_seconds < 0.0) frame_dt_seconds = 0.0;
    if (frame_dt_seconds > 0.25) frame_dt_seconds = 0.25; /* clamp a stall */
    *accumulator += frame_dt_seconds;
    while (*accumulator >= step) {
        idea_canvas_step(c, p, (float)step);
        *accumulator -= step;
        ++steps;
    }
    return steps;
}

const char *idea_canvas_status_name(idea_canvas_status s) {
    switch (s) {
        case IDEA_CANVAS_OK:          return "OK";
        case IDEA_CANVAS_INVALID:     return "INVALID";
        case IDEA_CANVAS_OOM:         return "OOM";
        case IDEA_CANVAS_NO_RENDERER: return "NO_RENDERER";
        case IDEA_CANVAS_GL_ERROR:    return "GL_ERROR";
        default:                      return "UNKNOWN";
    }
}
