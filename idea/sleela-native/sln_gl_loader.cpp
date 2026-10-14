/*
 * idea/sleela-native/sln_gl_loader.cpp
 * Sleela-Native -- original OpenGL function loader implementation.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 * Clean-room original. SPDX-License-Identifier: MIT (see PROVENANCE.md)
 *
 * NOT compiled/verified in the headless sandbox; written to the standard
 * platform getProcAddress ABIs.
 */
#include "sln_gl_loader.h"

#include <cstring>

/* Platform getProcAddress. */
#if defined(_WIN32)
#  include <windows.h>
static void *gl_sym(const char *name) {
    void *p = (void *)wglGetProcAddress(name);
    if (p == nullptr || p == (void*)1 || p == (void*)2 ||
        p == (void*)3 || p == (void*)-1) {
        static HMODULE gl = LoadLibraryA("opengl32.dll");
        p = gl ? (void *)GetProcAddress(gl, name) : nullptr;
    }
    return p;
}
#elif defined(__APPLE__)
/* On macOS the GL symbols are available directly via the framework; resolve
 * through dlsym on the current image. */
#  include <dlfcn.h>
static void *gl_sym(const char *name) { return dlsym(RTLD_DEFAULT, name); }
#else
/* Linux: prefer eglGetProcAddress, fall back to dlsym on libGL. */
#  include <dlfcn.h>
extern "C" void (*eglGetProcAddress(const char *))(void);
static void *gl_sym(const char *name) {
    void *p = nullptr;
    void (*ep)(void) = eglGetProcAddress(name);
    if (ep) p = (void *)ep;
    if (!p) {
        static void *lib = dlopen("libGL.so.1", RTLD_NOW | RTLD_GLOBAL);
        if (lib) p = dlsym(lib, name);
    }
    return p;
}
#endif

#define LOAD(field, glname)                                   \
    gl->field = (decltype(gl->field))gl_sym(glname);          \
    if (!gl->field) missing++;

int sln_gl_load(SlnGL *gl) {
    if (!gl) return -1;
    std::memset(gl, 0, sizeof(*gl));
    int missing = 0;
    LOAD(CreateShader, "glCreateShader");
    LOAD(ShaderSource, "glShaderSource");
    LOAD(CompileShader, "glCompileShader");
    LOAD(GetShaderiv, "glGetShaderiv");
    LOAD(GetShaderInfoLog, "glGetShaderInfoLog");
    LOAD(CreateProgram, "glCreateProgram");
    LOAD(AttachShader, "glAttachShader");
    LOAD(LinkProgram, "glLinkProgram");
    LOAD(UseProgram, "glUseProgram");
    LOAD(DeleteShader, "glDeleteShader");
    LOAD(GetUniformLocation, "glGetUniformLocation");
    LOAD(Uniform1f, "glUniform1f");
    LOAD(Uniform3f, "glUniform3f");
    LOAD(UniformMatrix4fv, "glUniformMatrix4fv");
    LOAD(GenVertexArrays, "glGenVertexArrays");
    LOAD(BindVertexArray, "glBindVertexArray");
    LOAD(GenBuffers, "glGenBuffers");
    LOAD(BindBuffer, "glBindBuffer");
    LOAD(BufferData, "glBufferData");
    LOAD(EnableVertexAttribArray, "glEnableVertexAttribArray");
    LOAD(VertexAttribPointer, "glVertexAttribPointer");
    LOAD(DrawElements, "glDrawElements");
    LOAD(DrawArrays, "glDrawArrays");
    LOAD(GenFramebuffers, "glGenFramebuffers");
    LOAD(BindFramebuffer, "glBindFramebuffer");
    LOAD(FramebufferTexture2D, "glFramebufferTexture2D");
    LOAD(GenTextures, "glGenTextures");
    LOAD(BindTexture, "glBindTexture");
    LOAD(TexImage2D, "glTexImage2D");
    LOAD(TexParameteri, "glTexParameteri");
    LOAD(ActiveTexture, "glActiveTexture");
    LOAD(Uniform1i, "glUniform1i");
    LOAD(Viewport, "glViewport");
    LOAD(Clear, "glClear");
    LOAD(ClearColor, "glClearColor");
    LOAD(Enable, "glEnable");
    return missing == 0 ? 0 : -missing;
}
