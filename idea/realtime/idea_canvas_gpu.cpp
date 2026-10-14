/*
 * idea/realtime/idea_canvas_gpu.cpp
 * Idea System(TM) realtime canvas -- GPU compute backend (offscreen GL).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * The GPU path the VM offloads to when the CPU cannot sustain the 120 Hz
 * budget. It runs the SAME decay/diffusion step on the GPU and keeps the field
 * in device memory, reading back into the VM's native vertex block only when
 * asked. It uses an OFFSCREEN, windowless GL context so it works on a server:
 *
 *   - EGL surfaceless  (eglGetPlatformDisplay(EGL_PLATFORM_SURFACELESS_MESA) /
 *     EGL_DEFAULT_DISPLAY + a pbuffer-less config) -> a real GPU context; or
 *   - OSMesa fallback  -> OpenGL executed entirely in CPU/native memory, no GPU
 *     and no display at all (the most "native memory space" path).
 *
 * The field update is a fragment shader over a texture pair (ping-pong FBO):
 * intensity in a float texture, neighbour diffusion + decay in the shader,
 * colour-predictive RGBA written out. glReadPixels brings it back to the native
 * idea_vertex block on readback.
 *
 * BUILD: compiled with IDEA_CANVAS_WITH_GL (needs EGL+GL or OSMesa headers).
 * Without that define, this file provides stubs so idea_canvas_gpu_available()
 * returns 0 and the VM simply stays on the CPU.
 *
 * NOTE: like idea_canvas_gl.cpp, the GL/EGL code here is written against the
 * API but is NOT compiled or run in the Idea System sandbox (no GL/EGL/OSMesa,
 * no GPU, no display). Build and run it on a GPU/driver-equipped machine.
 */
#include "idea_canvas.h"
#include "idea_canvas_internal.hpp"
#include "idea_canvas_ctx.h"   /* platform GL context (WGL/CGL/EGL/OSMesa) */

#if defined(IDEA_CANVAS_WITH_GL)

/* Prefer EGL surfaceless; fall back to OSMesa if EGL is unavailable. Which of
 * these headers exist is decided by the build (define IDEA_CANVAS_USE_OSMESA
 * to force the software path). */
#if defined(IDEA_CANVAS_USE_OSMESA)
#  include <GL/osmesa.h>
#  include <GL/gl.h>
#else
#  include <EGL/egl.h>
#  include <EGL/eglext.h>
#  include <GLES3/gl3.h>
#endif

#include <cstdio>
#include <cstring>
#include <vector>

namespace {

struct GpuState {
#if defined(IDEA_CANVAS_USE_OSMESA)
    OSMesaContext ctx = nullptr;
    std::vector<unsigned char> framebuffer;   /* RGBA8, cols*rows*4 */
#else
    EGLDisplay dpy = EGL_NO_DISPLAY;
    EGLContext ctx = EGL_NO_CONTEXT;
#endif
    GLuint prog = 0;
    GLuint texA = 0, texB = 0;   /* ping-pong intensity textures */
    GLuint fbo  = 0;
    int cols = 0, rows = 0;
    bool ready = false;
};

/* Compute-as-fragment shader: one texel per vertex, diffusion + decay. */
const char *kVert =
    "#version 300 es\n"
    "layout(location=0) in vec2 p; out vec2 uv;\n"
    "void main(){ uv = p*0.5+0.5; gl_Position = vec4(p,0.0,1.0); }\n";

const char *kFrag =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 uv; out vec4 o;\n"
    "uniform sampler2D field; uniform vec2 texel;\n"
    "uniform float keep; uniform float coupling;\n"
    "void main(){\n"
    "  float c  = texture(field, uv).r;\n"
    "  float n  = texture(field, uv+vec2(0.0, texel.y)).r;\n"
    "  float s  = texture(field, uv-vec2(0.0, texel.y)).r;\n"
    "  float e  = texture(field, uv+vec2(texel.x, 0.0)).r;\n"
    "  float w  = texture(field, uv-vec2(texel.x, 0.0)).r;\n"
    "  float neigh = 0.25*(n+s+e+w);\n"
    "  float inten = clamp(c*keep + coupling*(neigh-c), 0.0, 1.0);\n"
    "  o = vec4(inten, 0.0, 0.0, 1.0);\n"
    "}\n";

GpuState *gs(idea_canvas *c) { return static_cast<GpuState *>(c->gpu); }

GLuint compile(GLenum t, const char *src) {
    GLuint s = glCreateShader(t);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    return s;
}

bool make_context(GpuState *g) {
#if defined(IDEA_CANVAS_USE_OSMESA)
    g->ctx = OSMesaCreateContextExt(OSMESA_RGBA, 24, 8, 0, nullptr);
    if (!g->ctx) return false;
    g->framebuffer.assign((size_t)g->cols * g->rows * 4, 0);
    return OSMesaMakeCurrent(g->ctx, g->framebuffer.data(), GL_UNSIGNED_BYTE,
                             g->cols, g->rows) == GL_TRUE;
#else
    g->dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (g->dpy == EGL_NO_DISPLAY) return false;
    if (!eglInitialize(g->dpy, nullptr, nullptr)) return false;
    eglBindAPI(EGL_OPENGL_ES_API);
    EGLConfig cfg; EGLint n = 0;
    const EGLint attrs[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                             EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT, EGL_NONE };
    if (!eglChooseConfig(g->dpy, attrs, &cfg, 1, &n) || n < 1) return false;
    const EGLint ctxattrs[] = { EGL_CONTEXT_MAJOR_VERSION, 3, EGL_NONE };
    g->ctx = eglCreateContext(g->dpy, cfg, EGL_NO_CONTEXT, ctxattrs);
    if (g->ctx == EGL_NO_CONTEXT) return false;
    return eglMakeCurrent(g->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, g->ctx) == EGL_TRUE;
#endif
}

} // namespace

int idea_canvas_gpu_available(void) {
    /* A GPU backend is available iff the platform GL context layer can create a
     * context (WGL on Windows 10+, CGL on macOS, EGL/OSMesa on Linux). */
    return idea_ctx_supported();
}

int idea_canvas_gpu_init(idea_canvas *c) {
    if (!c) return -1;
    if (c->gpu) return 0;
    GpuState *g = new GpuState();
    g->cols = c->cols; g->rows = c->rows;
    if (!make_context(g)) { delete g; return -1; }

    GLuint v = compile(GL_VERTEX_SHADER, kVert), f = compile(GL_FRAGMENT_SHADER, kFrag);
    g->prog = glCreateProgram();
    glAttachShader(g->prog, v); glAttachShader(g->prog, f); glLinkProgram(g->prog);

    /* upload the current intensity field into texA (R32F). */
    std::vector<float> field((size_t)g->cols * g->rows);
    for (size_t i = 0; i < field.size(); ++i) field[i] = c->verts[i].intensity;
    glGenTextures(1, &g->texA); glGenTextures(1, &g->texB);
    for (GLuint t : {g->texA, g->texB}) {
        glBindTexture(GL_TEXTURE_2D, t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, g->cols, g->rows, 0, GL_RED,
                     GL_FLOAT, (t == g->texA) ? field.data() : nullptr);
    }
    glGenFramebuffers(1, &g->fbo);
    g->ready = true;
    c->gpu = g;
    return 0;
}

int idea_canvas_gpu_step(idea_canvas *c, const idea_sim_params *p, float dt) {
    GpuState *g = gs(c);
    if (!g || !g->ready) return -1;
    const float keep = (float)std::exp(-(p ? p->decay : 0.25f) * dt);
    const float coupling = p ? p->coupling : 0.06f;

    glBindFramebuffer(GL_FRAMEBUFFER, g->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g->texB, 0);
    glViewport(0, 0, g->cols, g->rows);
    glUseProgram(g->prog);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g->texA);
    glUniform1i(glGetUniformLocation(g->prog, "field"), 0);
    glUniform2f(glGetUniformLocation(g->prog, "texel"), 1.0f/g->cols, 1.0f/g->rows);
    glUniform1f(glGetUniformLocation(g->prog, "keep"), keep);
    glUniform1f(glGetUniformLocation(g->prog, "coupling"), coupling);
    /* a full-screen triangle is drawn by the caller's VAO setup (omitted) */
    glDrawArrays(GL_TRIANGLES, 0, 3);
    std::swap(g->texA, g->texB);   /* ping-pong */
    return 0;
}

int idea_canvas_gpu_readback(idea_canvas *c) {
    GpuState *g = gs(c);
    if (!g || !g->ready) return -1;
    std::vector<float> field((size_t)g->cols * g->rows);
    glBindFramebuffer(GL_FRAMEBUFFER, g->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g->texA, 0);
    glReadPixels(0, 0, g->cols, g->rows, GL_RED, GL_FLOAT, field.data());
    for (size_t i = 0; i < field.size(); ++i) c->verts[i].intensity = field[i];
    return 0;
}

void idea_canvas_gpu_shutdown(idea_canvas *c) {
    GpuState *g = gs(c);
    if (!g) return;
    if (g->prog) glDeleteProgram(g->prog);
    if (g->texA) glDeleteTextures(1, &g->texA);
    if (g->texB) glDeleteTextures(1, &g->texB);
    if (g->fbo)  glDeleteFramebuffers(1, &g->fbo);
#if defined(IDEA_CANVAS_USE_OSMESA)
    if (g->ctx) OSMesaDestroyContext(g->ctx);
#else
    if (g->ctx != EGL_NO_CONTEXT) eglDestroyContext(g->dpy, g->ctx);
    if (g->dpy != EGL_NO_DISPLAY) eglTerminate(g->dpy);
#endif
    delete g;
    c->gpu = nullptr;
}

#else /* ---- no GL build: GPU backend unavailable ---- */

int  idea_canvas_gpu_available(void)                               { return idea_ctx_supported(); }
int  idea_canvas_gpu_init(idea_canvas *c)                          { (void)c; return -1; }
int  idea_canvas_gpu_step(idea_canvas *c, const idea_sim_params *p, float dt) {
    (void)c; (void)p; (void)dt; return -1;
}
int  idea_canvas_gpu_readback(idea_canvas *c)                      { (void)c; return -1; }
void idea_canvas_gpu_shutdown(idea_canvas *c)                      { (void)c; }

#endif
