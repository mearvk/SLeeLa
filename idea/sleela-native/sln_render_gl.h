/*
 * idea/sleela-native/sln_render_gl.h
 * Sleela-Native -- original OpenGL renderer (lit model + moving light + vignette).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 * Clean-room original. SPDX-License-Identifier: MIT (see PROVENANCE.md)
 */
#ifndef SLN_RENDER_GL_H
#define SLN_RENDER_GL_H

#include "sln_scene.h"

namespace sln {

/* Owns the GL objects for drawing a Scene: the lit-model program, the vertex
 * buffers, an offscreen colour target, and the vignette fullscreen program.
 * A GL context must already be current (see idea/realtime/idea_canvas_ctx.h).
 */
class GlRenderer {
public:
    GlRenderer() = default;
    ~GlRenderer();

    /* Compile shaders, upload the scene's model, allocate the offscreen
     * target. Returns 0 on success, <0 on error (also if GL could not load). */
    int  init(const Scene &scene, int fb_width, int fb_height);

    /* Draw one frame of the (already-advanced) scene. */
    void render(const Scene &scene);

    void resize(int fb_width, int fb_height);
    void shutdown();

private:
    struct Impl;
    Impl *impl_ = nullptr;
};

} // namespace sln

#endif /* SLN_RENDER_GL_H */
