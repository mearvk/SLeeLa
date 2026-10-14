/*
 * idea/realtime/idea_canvas_gl.cpp
 * Idea System(TM) realtime canvas -- OpenGL render layer (GLFW + modern GL).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Compiled only when the module is built WITH IDEA_CANVAS_WITH_GL (needs GLFW
 * and an OpenGL 4.4+ loader at build time).  It renders the physics core's
 * contiguous vertex block with:
 *
 *   - a PERSISTENT-MAPPED, COHERENT, TRIPLE-BUFFERED vertex buffer
 *     (glBufferStorage + GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT), so the
 *     exact memory block the OpenMP step produced is handed to the GPU with no
 *     glBufferData re-upload each frame;
 *   - a glFenceSync per buffer region so the CPU never writes a region the GPU
 *     is still reading (the correctness the raw "map it straight to GL" idea
 *     needs);
 *   - a tiny point/quad shader with additive blending for a glowing field;
 *   - a loop that advances the sim at a 120 Hz fixed step and presents at 60 Hz.
 *
 * NOTE: this file is written against the API but is NOT compiled or verified in
 * the Idea System build environment (no GL/GLFW/display there).  Build and run
 * it on a machine with a GPU + display using the CMake option below.
 *
 * This file intentionally includes GL headers only; include <glad/glad.h> (or
 * your loader) before <GLFW/glfw3.h>.  Adjust the include to your loader.
 */
#include "idea_canvas.h"
#include "idea_canvas_internal.hpp"

#if defined(IDEA_CANVAS_WITH_GL)

#include <glad/glad.h>      /* or <GL/glew.h>; must be included before GLFW   */
#include <GLFW/glfw3.h>

#include <chrono>
#include <cstdio>
#include <cstring>

namespace {

constexpr int kBuffers = 3;   /* triple-buffering */

struct GlState {
    GLFWwindow *win   = nullptr;
    GLuint      vao   = 0;
    GLuint      vbo   = 0;
    GLuint      prog  = 0;
    idea_vertex *mapped = nullptr;      /* persistent-mapped base pointer     */
    GLsync      fences[kBuffers] = {};
    size_t      count = 0;              /* vertices per region                */
    size_t      regionBytes = 0;
    int         ring = 0;               /* which of the kBuffers regions next */
    double      acc = 0.0;              /* fixed-step accumulator             */
};

const char *kVert = R"(#version 440 core
layout(location=0) in vec2  aPos;
layout(location=1) in vec4  aColor;
layout(location=2) in float aIntensity;
out vec4 vColor;
void main() {
    vec2 ndc = aPos * 2.0 - 1.0;      // [0,1] -> [-1,1]
    ndc.y = -ndc.y;                   // top-left origin
    gl_Position = vec4(ndc, 0.0, 1.0);
    gl_PointSize = 2.0 + 6.0 * aIntensity;
    vColor = aColor;
}
)";

const char *kFrag = R"(#version 440 core
in  vec4 vColor;
out vec4 oColor;
void main() {
    vec2 d = gl_PointCoord - vec2(0.5);
    float r = dot(d, d);
    float fall = exp(-r * 8.0);       // soft round point
    oColor = vec4(vColor.rgb, vColor.a * fall);
}
)";

GLuint compile(GLenum type, const char *src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) { char log[1024]; glGetShaderInfoLog(s, sizeof log, nullptr, log);
               std::fprintf(stderr, "idea-canvas shader: %s\n", log); }
    return s;
}

GLuint linkProgram() {
    GLuint v = compile(GL_VERTEX_SHADER, kVert);
    GLuint f = compile(GL_FRAGMENT_SHADER, kFrag);
    GLuint p = glCreateProgram();
    glAttachShader(p, v); glAttachShader(p, f); glLinkProgram(p);
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

GlState *state(idea_canvas *c) { return static_cast<GlState *>(c->gl); }

} /* namespace */

void idea_canvas_gl_teardown(idea_canvas *c) {
    if (!c || !c->gl) return;
    GlState *s = state(c);
    for (int i = 0; i < kBuffers; ++i) if (s->fences[i]) glDeleteSync(s->fences[i]);
    if (s->vbo) { glUnmapNamedBuffer(s->vbo); glDeleteBuffers(1, &s->vbo); }
    if (s->vao) glDeleteVertexArrays(1, &s->vao);
    if (s->prog) glDeleteProgram(s->prog);
    if (s->win) glfwDestroyWindow(s->win);
    glfwTerminate();
    delete s;
    c->gl = nullptr;
}

idea_canvas_status idea_canvas_open_window(idea_canvas *c, int width, int height,
                                           const char *title) {
    if (!c) return IDEA_CANVAS_INVALID;
    if (c->gl) return IDEA_CANVAS_OK;

    if (!glfwInit()) return IDEA_CANVAS_GL_ERROR;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GlState *s = new GlState();
    s->count = c->verts.size();
    s->win = glfwCreateWindow(width, height, title ? title : "Idea System(TM)",
                              nullptr, nullptr);
    if (!s->win) { delete s; glfwTerminate(); return IDEA_CANVAS_GL_ERROR; }
    glfwMakeContextCurrent(s->win);
    glfwSwapInterval(1);                       /* vsync -> ~present rate       */

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        delete s; glfwTerminate(); return IDEA_CANVAS_GL_ERROR;
    }

    s->prog = linkProgram();

    /* Persistent-mapped, coherent, triple-sized buffer. */
    s->regionBytes = s->count * sizeof(idea_vertex);
    const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    glCreateBuffers(1, &s->vbo);
    glNamedBufferStorage(s->vbo, (GLsizeiptr)(s->regionBytes * kBuffers), nullptr, flags);
    s->mapped = static_cast<idea_vertex *>(
        glMapNamedBufferRange(s->vbo, 0, (GLsizeiptr)(s->regionBytes * kBuffers), flags));
    if (!s->mapped) { delete s; glfwTerminate(); return IDEA_CANVAS_GL_ERROR; }

    glCreateVertexArrays(1, &s->vao);
    glBindVertexArray(s->vao);
    glBindBuffer(GL_ARRAY_BUFFER, s->vbo);
    const GLsizei stride = sizeof(idea_vertex);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(idea_vertex, x));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(idea_vertex, r));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(idea_vertex, intensity));

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);         /* additive glow                */

    c->gl = s;
    return IDEA_CANVAS_OK;
}

int idea_canvas_should_close(const idea_canvas *c) {
    if (!c || !c->gl) return 1;
    return glfwWindowShouldClose(static_cast<GlState *>(c->gl)->win) ? 1 : 0;
}

idea_canvas_status idea_canvas_present(idea_canvas *c) {
    if (!c || !c->gl) return IDEA_CANVAS_NO_RENDERER;
    GlState *s = state(c);
    const int r = s->ring;

    /* Wait for the GPU to finish with this region, then write it. */
    if (s->fences[r]) {
        glClientWaitSync(s->fences[r], GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
        glDeleteSync(s->fences[r]);
        s->fences[r] = nullptr;
    }
    idea_vertex *dst = s->mapped + (size_t)r * s->count;
    std::memcpy(dst, c->verts.data(), s->regionBytes);  /* coherent map */

    int w, h; glfwGetFramebufferSize(s->win, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.051f, 0.055f, 0.071f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(s->prog);
    glBindVertexArray(s->vao);
    glDrawArrays(GL_POINTS, (GLint)(r * s->count), (GLsizei)s->count);

    s->fences[r] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    s->ring = (r + 1) % kBuffers;

    glfwSwapBuffers(s->win);
    glfwPollEvents();
    return IDEA_CANVAS_OK;
}

idea_canvas_status idea_canvas_run(idea_canvas *c, const idea_sim_params *p,
                                   int width, int height, const char *title) {
    idea_canvas_status st = idea_canvas_open_window(c, width, height, title);
    if (st != IDEA_CANVAS_OK) return st;

    using clock = std::chrono::steady_clock;
    auto last = clock::now();
    while (!idea_canvas_should_close(c)) {
        auto now = clock::now();
        double dt = std::chrono::duration<double>(now - last).count();
        last = now;
        /* advance the live simulation at a deterministic 120 Hz fixed step */
        idea_canvas_advance(c, p, dt, &state(c)->acc);
        /* present at the display/vsync rate (~60 Hz) */
        idea_canvas_present(c);
    }
    return IDEA_CANVAS_OK;
}

int idea_canvas_has_renderer(void) { return 1; }

#endif /* IDEA_CANVAS_WITH_GL */
