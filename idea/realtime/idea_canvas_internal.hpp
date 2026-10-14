/*
 * idea/realtime/idea_canvas_internal.hpp
 * Idea System(TM) realtime canvas -- shared internal definition.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * The single definition of `struct idea_canvas`, shared by the physics core
 * (idea_canvas_sim.cpp) and the optional GL render layer (idea_canvas_gl.cpp).
 * Not part of the public ABI; do not install.
 */
#ifndef IDEA_CANVAS_INTERNAL_HPP
#define IDEA_CANVAS_INTERNAL_HPP

#include "idea_canvas.h"

#include <vector>

/* Opaque to the ABI; concrete here. */
struct idea_canvas {
    int cols = 0;
    int rows = 0;
    std::vector<idea_vertex> verts;   /* the contiguous vertex block */

    /* Stored simulation parameters, so a VM can set them once and then call
     * step()/advance() without passing a struct across the bridge. */
    idea_sim_params params{};
    bool            params_set = false;

    /* Backend selection + auto GPU-offload state (owned by the core). */
    idea_backend_policy policy = IDEA_POLICY_AUTO;
    idea_backend        backend = IDEA_BACKEND_CPU;  /* currently simulating on */
    double  measured_hz = 0.0;      /* rolling sustained steps/sec             */
    double  ema_step_s  = 0.0;      /* EMA of one step's wall time (seconds)   */
    long    samples     = 0;        /* steps measured so far                   */
    int     under_budget_streak = 0;/* consecutive advances under 120 Hz (hyst)*/

    /* GL render state is owned by the GL layer; kept as an opaque pointer so
     * the physics core needs no GL headers.  Null when GL is absent/unopened. */
    void *gl  = nullptr;
    /* GPU compute state, owned by idea_canvas_gpu.cpp (opaque here). */
    void *gpu = nullptr;
};

/* Implemented in idea_canvas_gl.cpp when built with IDEA_CANVAS_WITH_GL;
 * a no-op stub (idea_canvas_gl_stub.cpp) otherwise.  Called from destroy(). */
void idea_canvas_gl_teardown(idea_canvas *c);

#endif /* IDEA_CANVAS_INTERNAL_HPP */
