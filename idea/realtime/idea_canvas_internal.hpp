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

    /* GL render state is owned by the GL layer; kept as an opaque pointer so
     * the physics core needs no GL headers.  Null when GL is absent/unopened. */
    void *gl = nullptr;
};

/* Implemented in idea_canvas_gl.cpp when built with IDEA_CANVAS_WITH_GL;
 * a no-op stub (idea_canvas_gl_stub.cpp) otherwise.  Called from destroy(). */
void idea_canvas_gl_teardown(idea_canvas *c);

#endif /* IDEA_CANVAS_INTERNAL_HPP */
