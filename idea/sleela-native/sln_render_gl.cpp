/*
 * idea/sleela-native/sln_render_gl.cpp
 * Sleela-Native -- OpenGL renderer: lit physical model, moving light, vignette.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 * Clean-room original, including all GLSL. SPDX-License-Identifier: MIT.
 *
 * NOT compiled/verified in the headless sandbox (no GL). Written to the
 * OpenGL 3.3 core profile against our own loader (sln_gl_loader).
 */
#include "sln_render_gl.h"
#include "sln_gl_loader.h"

#include <cstdio>
#include <vector>

/* GL enum constants we use (from the spec; stable values). */
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER   0x8B31
#define GL_COMPILE_STATUS  0x8B81
#define GL_ARRAY_BUFFER    0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW     0x88E4
#define GL_FLOAT           0x1406
#define GL_FALSE           0
#define GL_TRIANGLES       0x0004
#define GL_UNSIGNED_INT    0x1405
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_DEPTH_TEST      0x0B71
#define GL_TEXTURE_2D      0x0DE1
#define GL_TEXTURE0        0x84C0
#define GL_FRAMEBUFFER     0x8D40
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_RGBA            0x1908
#define GL_UNSIGNED_BYTE   0x1401
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_LINEAR          0x2601

namespace sln {

/* ---- original GLSL -------------------------------------------------- */

static const char *kModelVert =
    "#version 330 core\n"
    "layout(location=0) in vec3 aPos;\n"
    "layout(location=1) in vec3 aNormal;\n"
    "layout(location=2) in vec3 aColor;\n"
    "uniform mat4 uModel; uniform mat4 uView; uniform mat4 uProj;\n"
    "out vec3 vN; out vec3 vWorld; out vec3 vColor;\n"
    "void main(){\n"
    "  vec4 w = uModel * vec4(aPos,1.0);\n"
    "  vWorld = w.xyz;\n"
    "  vN = mat3(uModel) * aNormal;\n"
    "  vColor = aColor;\n"
    "  gl_Position = uProj * uView * w;\n"
    "}\n";

static const char *kModelFrag =
    "#version 330 core\n"
    "in vec3 vN; in vec3 vWorld; in vec3 vColor;\n"
    "uniform vec3 uLightPos; uniform vec3 uLightColor; uniform float uLightI;\n"
    "uniform vec3 uEye;\n"
    "out vec4 o;\n"
    "void main(){\n"
    "  vec3 N = normalize(vN);\n"
    "  vec3 L = normalize(uLightPos - vWorld);\n"
    "  vec3 V = normalize(uEye - vWorld);\n"
    "  vec3 R = reflect(-L, N);\n"
    "  float diff = max(dot(N,L), 0.0);\n"
    "  float spec = pow(max(dot(R,V),0.0), 24.0) * 0.4;\n"
    "  float dist = length(uLightPos - vWorld);\n"
    "  float atten = uLightI / (1.0 + 0.08*dist*dist);\n"
    "  vec3 ambient = 0.12 * vColor;\n"
    "  vec3 lit = (vColor*diff + spec) * uLightColor * atten + ambient;\n"
    "  o = vec4(lit, 1.0);\n"
    "}\n";

/* Fullscreen vignette pass: samples the scene texture, darkens the edges. */
static const char *kPostVert =
    "#version 330 core\n"
    "out vec2 uv;\n"
    "void main(){\n"
    "  vec2 p = vec2((gl_VertexID<<1)&2, gl_VertexID&2);\n"
    "  uv = p; gl_Position = vec4(p*2.0-1.0, 0.0, 1.0);\n"
    "}\n";

static const char *kPostFrag =
    "#version 330 core\n"
    "in vec2 uv; out vec4 o;\n"
    "uniform sampler2D uScene;\n"
    "uniform float uRadius; uniform float uSoftness; uniform float uStrength;\n"
    "void main(){\n"
    "  vec3 c = texture(uScene, uv).rgb;\n"
    "  vec2 d = uv - vec2(0.5);\n"
    "  float r = length(d) * 1.41421356;\n"            /* 0 center .. 1 corner */
    "  float v = smoothstep(uRadius, uRadius - uSoftness, r);\n"
    "  float vig = mix(1.0 - uStrength, 1.0, v);\n"
    "  o = vec4(c * vig, 1.0);\n"
    "}\n";

struct GlRenderer::Impl {
    SlnGL gl{};
    bool  ok = false;
    GLuint modelProg = 0, postProg = 0;
    GLuint vao = 0, vbo = 0, ebo = 0, emptyVao = 0;
    GLuint fbo = 0, sceneTex = 0;
    int    icount = 0;
    int    w = 0, h = 0;

    GLuint compile(GLenum type, const char *src) {
        GLuint s = gl.CreateShader(type);
        gl.ShaderSource(s, 1, &src, nullptr);
        gl.CompileShader(s);
        GLint okc = 0; gl.GetShaderiv(s, GL_COMPILE_STATUS, &okc);
        if (!okc) {
            char log[1024]; gl.GetShaderInfoLog(s, sizeof log, nullptr, log);
            std::fprintf(stderr, "sleela-native shader: %s\n", log);
        }
        return s;
    }
    GLuint link(const char *vs, const char *fs) {
        GLuint v = compile(GL_VERTEX_SHADER, vs), f = compile(GL_FRAGMENT_SHADER, fs);
        GLuint p = gl.CreateProgram();
        gl.AttachShader(p, v); gl.AttachShader(p, f); gl.LinkProgram(p);
        gl.DeleteShader(v); gl.DeleteShader(f);
        return p;
    }
};

GlRenderer::~GlRenderer() { shutdown(); }

int GlRenderer::init(const Scene &scene, int fb_width, int fb_height) {
    impl_ = new Impl();
    Impl &I = *impl_;
    if (sln_gl_load(&I.gl) != 0) {
        std::fprintf(stderr, "sleela-native: failed to load OpenGL entry points\n");
        return -1;
    }
    I.w = fb_width; I.h = fb_height;
    I.gl.Enable(GL_DEPTH_TEST);

    I.modelProg = I.link(kModelVert, kModelFrag);
    I.postProg  = I.link(kPostVert, kPostFrag);

    /* interleaved VBO: pos(3) normal(3) color(3) */
    std::vector<float> buf;
    buf.reserve(scene.model.vertices.size() * 9);
    for (const auto &v : scene.model.vertices) {
        buf.insert(buf.end(), { v.pos.x, v.pos.y, v.pos.z,
                                v.normal.x, v.normal.y, v.normal.z,
                                v.color.x, v.color.y, v.color.z });
    }
    I.icount = (int)scene.model.indices.size();

    I.gl.GenVertexArrays(1, &I.vao);
    I.gl.BindVertexArray(I.vao);
    I.gl.GenBuffers(1, &I.vbo);
    I.gl.BindBuffer(GL_ARRAY_BUFFER, I.vbo);
    I.gl.BufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(buf.size()*sizeof(float)), buf.data(), GL_STATIC_DRAW);
    I.gl.GenBuffers(1, &I.ebo);
    I.gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, I.ebo);
    I.gl.BufferData(GL_ELEMENT_ARRAY_BUFFER,
                    (GLsizeiptr)(scene.model.indices.size()*sizeof(uint32_t)),
                    scene.model.indices.data(), GL_STATIC_DRAW);
    const GLsizei stride = 9 * sizeof(float);
    I.gl.EnableVertexAttribArray(0); I.gl.VertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,(void*)0);
    I.gl.EnableVertexAttribArray(1); I.gl.VertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,stride,(void*)(3*sizeof(float)));
    I.gl.EnableVertexAttribArray(2); I.gl.VertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,stride,(void*)(6*sizeof(float)));

    /* offscreen colour target for the vignette pass */
    I.gl.GenTextures(1, &I.sceneTex);
    I.gl.BindTexture(GL_TEXTURE_2D, I.sceneTex);
    I.gl.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fb_width, fb_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    I.gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    I.gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    I.gl.GenFramebuffers(1, &I.fbo);
    I.gl.BindFramebuffer(GL_FRAMEBUFFER, I.fbo);
    I.gl.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, I.sceneTex, 0);
    I.gl.BindFramebuffer(GL_FRAMEBUFFER, 0);
    I.gl.GenVertexArrays(1, &I.emptyVao);

    I.ok = true;
    return 0;
}

void GlRenderer::render(const Scene &scene) {
    if (!impl_ || !impl_->ok) return;
    Impl &I = *impl_;
    const float aspect = (I.h > 0) ? (float)I.w / (float)I.h : 1.0f;

    Mat4 model = scene.model.transform();
    Mat4 view  = scene.camera.view();
    Mat4 proj  = scene.camera.projection(aspect);
    Vec3 lp = scene.light.world_position();
    Vec3 eye = scene.camera.eye();

    /* pass 1: lit model into the offscreen target */
    I.gl.BindFramebuffer(GL_FRAMEBUFFER, I.fbo);
    I.gl.Viewport(0, 0, I.w, I.h);
    I.gl.ClearColor(0.03f, 0.03f, 0.05f, 1.0f);
    I.gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    I.gl.UseProgram(I.modelProg);
    I.gl.UniformMatrix4fv(I.gl.GetUniformLocation(I.modelProg, "uModel"), 1, GL_FALSE, model.m);
    I.gl.UniformMatrix4fv(I.gl.GetUniformLocation(I.modelProg, "uView"),  1, GL_FALSE, view.m);
    I.gl.UniformMatrix4fv(I.gl.GetUniformLocation(I.modelProg, "uProj"),  1, GL_FALSE, proj.m);
    I.gl.Uniform3f(I.gl.GetUniformLocation(I.modelProg, "uLightPos"), lp.x, lp.y, lp.z);
    I.gl.Uniform3f(I.gl.GetUniformLocation(I.modelProg, "uLightColor"),
                   scene.light.color.x, scene.light.color.y, scene.light.color.z);
    I.gl.Uniform1f(I.gl.GetUniformLocation(I.modelProg, "uLightI"), scene.light.intensity);
    I.gl.Uniform3f(I.gl.GetUniformLocation(I.modelProg, "uEye"), eye.x, eye.y, eye.z);
    I.gl.BindVertexArray(I.vao);
    I.gl.DrawElements(GL_TRIANGLES, I.icount, GL_UNSIGNED_INT, (void*)0);

    /* pass 2: vignette fullscreen pass to the default framebuffer */
    I.gl.BindFramebuffer(GL_FRAMEBUFFER, 0);
    I.gl.Viewport(0, 0, I.w, I.h);
    I.gl.Clear(GL_COLOR_BUFFER_BIT);
    I.gl.UseProgram(I.postProg);
    I.gl.ActiveTexture(GL_TEXTURE0);
    I.gl.BindTexture(GL_TEXTURE_2D, I.sceneTex);
    I.gl.Uniform1i(I.gl.GetUniformLocation(I.postProg, "uScene"), 0);
    I.gl.Uniform1f(I.gl.GetUniformLocation(I.postProg, "uRadius"),
                   scene.vignette.enabled ? scene.vignette.radius : 2.0f);
    I.gl.Uniform1f(I.gl.GetUniformLocation(I.postProg, "uSoftness"), scene.vignette.softness);
    I.gl.Uniform1f(I.gl.GetUniformLocation(I.postProg, "uStrength"),
                   scene.vignette.enabled ? scene.vignette.strength : 0.0f);
    I.gl.BindVertexArray(I.emptyVao);
    I.gl.DrawArrays(GL_TRIANGLES, 0, 3);   /* fullscreen triangle */
}

void GlRenderer::resize(int w, int h) {
    if (!impl_) return;
    impl_->w = w; impl_->h = h;
    if (impl_->ok) {
        impl_->gl.BindTexture(GL_TEXTURE_2D, impl_->sceneTex);
        impl_->gl.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }
}

void GlRenderer::shutdown() {
    if (impl_) { delete impl_; impl_ = nullptr; }
}

} // namespace sln
