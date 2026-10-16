/*
 * idea/realtime/idea_canvas_ctx_egl.cpp
 * Idea System(TM) realtime canvas -- Linux/Unix GL context (EGL, OSMesa fallback).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Standards used: EGL (the Khronos standard for GL context/surface management),
 * surfaceless where available for headless GPU use; OSMesa (software OpenGL in
 * native CPU memory) as the no-GPU/no-display fallback. GLX could be slotted in
 * for X11 windowed use behind the same interface.
 *
 * This is the one platform file the Idea System sandbox can at least COMPILE
 * (as the no-GL stub below) -- it still has no EGL/OSMesa/GPU to run against.
 * Build with IDEA_CANVAS_WITH_GL (+ EGL or OSMESA) on a Linux host to enable it.
 */
#include "idea_canvas_ctx.h"

#if defined(IDEA_CANVAS_WITH_GL) && (defined(__linux__) || defined(__unix__))

#if defined(IDEA_CANVAS_USE_OSMESA)
#  include <GL/osmesa.h>
#  include <GL/gl.h>
#  include <vector>
struct idea_ctx {
    OSMesaContext ctx = nullptr;
    std::vector<unsigned char> fb;
    int w = 0, h = 0;
};
idea_ctx *idea_ctx_create(int width, int height, int windowed, const char *title) {
    (void)windowed; (void)title;
    idea_ctx *c = new idea_ctx();
    c->w = width; c->h = height; c->fb.assign((size_t)width*height*4, 0);
    c->ctx = OSMesaCreateContextExt(OSMESA_RGBA, 24, 8, 0, nullptr);
    if (!c->ctx || OSMesaMakeCurrent(c->ctx, c->fb.data(), 0x1401 /*GL_UNSIGNED_BYTE*/,
                                     width, height) == 0) { delete c; return nullptr; }
    return c;
}
int  idea_ctx_make_current(idea_ctx *c) {
    return (c && OSMesaMakeCurrent(c->ctx, c->fb.data(), 0x1401, c->w, c->h)) ? 0 : -1;
}
void idea_ctx_present(idea_ctx *) {}
int  idea_ctx_pump(idea_ctx *) { return 1; }
void idea_ctx_destroy(idea_ctx *c) { if (c){ if(c->ctx) OSMesaDestroyContext(c->ctx); delete c; } }
idea_ctx_kind idea_ctx_kind_of(void) { return IDEA_CTX_OSMESA; }
int idea_ctx_supported(void) { return 1; }

#else  /* EGL */
#  include <EGL/egl.h>
#  include <EGL/eglext.h>
#  include <GLES3/gl3.h>
struct idea_ctx {
    EGLDisplay dpy = EGL_NO_DISPLAY;
    EGLContext ctx = EGL_NO_CONTEXT;
};
idea_ctx *idea_ctx_create(int width, int height, int windowed, const char *title) {
    (void)width; (void)height; (void)windowed; (void)title;
    idea_ctx *c = new idea_ctx();
    c->dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (c->dpy == EGL_NO_DISPLAY || !eglInitialize(c->dpy, nullptr, nullptr)) { delete c; return nullptr; }
    eglBindAPI(EGL_OPENGL_ES_API);
    EGLConfig cfg; EGLint n = 0;
    const EGLint a[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                         EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT, EGL_NONE };
    if (!eglChooseConfig(c->dpy, a, &cfg, 1, &n) || n < 1) { delete c; return nullptr; }
    const EGLint ca[] = { EGL_CONTEXT_MAJOR_VERSION, 3, EGL_NONE };
    c->ctx = eglCreateContext(c->dpy, cfg, EGL_NO_CONTEXT, ca);
    if (c->ctx == EGL_NO_CONTEXT ||
        !eglMakeCurrent(c->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, c->ctx)) { delete c; return nullptr; }
    return c;
}
int  idea_ctx_make_current(idea_ctx *c) {
    return (c && eglMakeCurrent(c->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, c->ctx)) ? 0 : -1;
}
void idea_ctx_present(idea_ctx *) {}
int  idea_ctx_pump(idea_ctx *) { return 1; }
void idea_ctx_destroy(idea_ctx *c) {
    if (!c) return;
    if (c->ctx != EGL_NO_CONTEXT) eglDestroyContext(c->dpy, c->ctx);
    if (c->dpy != EGL_NO_DISPLAY) eglTerminate(c->dpy);
    delete c;
}
idea_ctx_kind idea_ctx_kind_of(void) { return IDEA_CTX_EGL; }
int idea_ctx_supported(void) { return 1; }
#endif

#else  /* ---- no-GL build (default sandbox): a compilable stub ---- */

struct idea_ctx { int unused; };
idea_ctx *idea_ctx_create(int, int, int, const char *) { return 0; }
int  idea_ctx_make_current(idea_ctx *) { return -1; }
void idea_ctx_present(idea_ctx *) {}
int  idea_ctx_pump(idea_ctx *) { return 0; }
void idea_ctx_destroy(idea_ctx *) {}
idea_ctx_kind idea_ctx_kind_of(void) { return IDEA_CTX_NONE; }
int idea_ctx_supported(void) { return 0; }

#endif
