/*
 * idea/realtime/idea_canvas_gl_stub.cpp
 * Idea System(TM) realtime canvas -- render layer stub (no GL).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Compiled when the module is built WITHOUT IDEA_CANVAS_WITH_GL.  It satisfies
 * the render entry points of the public ABI so a VM can always link the module;
 * every render call reports IDEA_CANVAS_NO_RENDERER, and the physics core
 * (idea_canvas_sim.cpp) remains fully usable headless.
 */
#include "idea_canvas.h"
#include "idea_canvas_internal.hpp"

void idea_canvas_gl_teardown(idea_canvas *) { /* nothing to tear down */ }

idea_canvas_status idea_canvas_open_window(idea_canvas *, int, int, const char *) {
    return IDEA_CANVAS_NO_RENDERER;
}

int idea_canvas_should_close(const idea_canvas *) { return 1; }

idea_canvas_status idea_canvas_present(idea_canvas *) {
    return IDEA_CANVAS_NO_RENDERER;
}

idea_canvas_status idea_canvas_run(idea_canvas *, const idea_sim_params *,
                                   int, int, const char *) {
    return IDEA_CANVAS_NO_RENDERER;
}

int idea_canvas_has_renderer(void) { return 0; }
