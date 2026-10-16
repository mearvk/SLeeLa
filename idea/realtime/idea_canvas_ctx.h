/*
 * idea/realtime/idea_canvas_ctx.h
 * Idea System(TM) realtime canvas -- platform GL context interface.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * One small, standards-based interface over OpenGL context creation on the
 * three target platforms, selected at build time by OS:
 *
 *   Windows 10+ : Win32 + WGL        (idea_canvas_ctx_wgl.cpp)
 *   macOS       : Cocoa + CGL        (idea_canvas_ctx_cgl.cpp)
 *   Linux/Unix  : EGL (surfaceless) / GLX, OSMesa fallback (idea_canvas_ctx_egl.cpp)
 *
 * All three expose a modern OpenGL core-profile context. We use OPEN STANDARDS
 * only -- OpenGL plus each OS's standard WGL/CGL/EGL binding -- so nothing is
 * proprietary (no D3D/Metal) and nothing is invented beyond the minimal
 * per-OS window/context glue, for which no cross-platform standard exists.
 *
 * The context can be:
 *   - OFFSCREEN (no window): for the GPU compute backend (FBO + glReadPixels);
 *   - WINDOWED: for on-screen presentation via idea_canvas_run/present.
 *
 * This header is internal to the module (not part of the public C ABI).
 */
#ifndef IDEA_CANVAS_CTX_H
#define IDEA_CANVAS_CTX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Which standard backend was compiled in. */
typedef enum idea_ctx_kind {
    IDEA_CTX_NONE   = 0,
    IDEA_CTX_WGL    = 1,   /* Windows 10+ */
    IDEA_CTX_CGL    = 2,   /* macOS       */
    IDEA_CTX_EGL    = 3,   /* Linux EGL/GLX */
    IDEA_CTX_OSMESA = 4    /* software GL in native memory (any OS, fallback) */
} idea_ctx_kind;

/* Opaque per-platform context handle. */
typedef struct idea_ctx idea_ctx;

/*
 * Create an OpenGL core-profile context.
 *   width,height : surface size (also the offscreen framebuffer size).
 *   windowed     : 1 to open an on-screen window, 0 for an offscreen context.
 *   title        : window title (ignored when offscreen).
 * Returns NULL if no context could be created (no driver/display/GPU).
 * The returned context is made current on the calling thread.
 */
idea_ctx *idea_ctx_create(int width, int height, int windowed, const char *title);

/* Make the context current on the calling thread (0 ok, <0 error). */
int  idea_ctx_make_current(idea_ctx *c);

/* Swap buffers / present (windowed); a no-op for offscreen contexts. */
void idea_ctx_present(idea_ctx *c);

/* Pump window events; returns 1 while the window should stay open, 0 to close.
 * Always returns 1 for offscreen contexts. */
int  idea_ctx_pump(idea_ctx *c);

/* Destroy the context and any window. */
void idea_ctx_destroy(idea_ctx *c);

/* Which standard backend this build provides (compile-time). */
idea_ctx_kind idea_ctx_kind_of(void);

/* 1 if this build can create a GL context at all on this platform. */
int idea_ctx_supported(void);

#ifdef __cplusplus
}
#endif

#endif /* IDEA_CANVAS_CTX_H */
