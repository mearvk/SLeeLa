/*
 * idea/sleela-native/sln_gl_loader.h
 * Sleela-Native -- original minimal OpenGL function loader (replaces GLEW/GLAD).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Clean-room original. Resolves the handful of modern-GL entry points the
 * renderer needs through the platform's standard getProcAddress:
 *   - Windows : wglGetProcAddress (+ GetProcAddress on opengl32.dll)
 *   - macOS   : symbols are available directly (framework); loader is a no-op
 *   - Linux   : eglGetProcAddress / glXGetProcAddress / dlsym
 *
 * We declare only what we use. The function-pointer typedefs mirror the public
 * OpenGL registry signatures (which are specification, not someone's source).
 *
 * NOTE: not compiled/verified in the headless sandbox (no GL). Written to the
 * standard GL ABI.  SPDX-License-Identifier: MIT (see PROVENANCE.md)
 */
#ifndef SLN_GL_LOADER_H
#define SLN_GL_LOADER_H

#include <cstddef>
#include <cstdint>

/* Minimal GL types (from the OpenGL spec; stable ABI). */
typedef unsigned int   GLenum;
typedef unsigned int   GLuint;
typedef int            GLint;
typedef int            GLsizei;
typedef float          GLfloat;
typedef char           GLchar;
typedef unsigned char  GLboolean;
typedef ptrdiff_t      GLsizeiptr;
typedef ptrdiff_t      GLintptr;
typedef unsigned int   GLbitfield;
typedef void           GLvoid;

/* Just the subset we call. */
typedef GLuint (*PFN_glCreateShader)(GLenum);
typedef void   (*PFN_glShaderSource)(GLuint, GLsizei, const GLchar* const*, const GLint*);
typedef void   (*PFN_glCompileShader)(GLuint);
typedef void   (*PFN_glGetShaderiv)(GLuint, GLenum, GLint*);
typedef void   (*PFN_glGetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef GLuint (*PFN_glCreateProgram)(void);
typedef void   (*PFN_glAttachShader)(GLuint, GLuint);
typedef void   (*PFN_glLinkProgram)(GLuint);
typedef void   (*PFN_glUseProgram)(GLuint);
typedef void   (*PFN_glDeleteShader)(GLuint);
typedef GLint  (*PFN_glGetUniformLocation)(GLuint, const GLchar*);
typedef void   (*PFN_glUniform1f)(GLint, GLfloat);
typedef void   (*PFN_glUniform3f)(GLint, GLfloat, GLfloat, GLfloat);
typedef void   (*PFN_glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*);
typedef void   (*PFN_glGenVertexArrays)(GLsizei, GLuint*);
typedef void   (*PFN_glBindVertexArray)(GLuint);
typedef void   (*PFN_glGenBuffers)(GLsizei, GLuint*);
typedef void   (*PFN_glBindBuffer)(GLenum, GLuint);
typedef void   (*PFN_glBufferData)(GLenum, GLsizeiptr, const void*, GLenum);
typedef void   (*PFN_glEnableVertexAttribArray)(GLuint);
typedef void   (*PFN_glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
typedef void   (*PFN_glDrawElements)(GLenum, GLsizei, GLenum, const void*);
typedef void   (*PFN_glDrawArrays)(GLenum, GLint, GLsizei);
typedef void   (*PFN_glGenFramebuffers)(GLsizei, GLuint*);
typedef void   (*PFN_glBindFramebuffer)(GLenum, GLuint);
typedef void   (*PFN_glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint);
typedef void   (*PFN_glGenTextures)(GLsizei, GLuint*);
typedef void   (*PFN_glBindTexture)(GLenum, GLuint);
typedef void   (*PFN_glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
typedef void   (*PFN_glTexParameteri)(GLenum, GLenum, GLint);
typedef void   (*PFN_glActiveTexture)(GLenum);
typedef void   (*PFN_glUniform1i)(GLint, GLint);
typedef void   (*PFN_glViewport)(GLint, GLint, GLsizei, GLsizei);
typedef void   (*PFN_glClear)(GLbitfield);
typedef void   (*PFN_glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
typedef void   (*PFN_glEnable)(GLenum);

/* The resolved pointers (defined in sln_gl_loader.cpp). */
struct SlnGL {
    PFN_glCreateShader       CreateShader;
    PFN_glShaderSource       ShaderSource;
    PFN_glCompileShader      CompileShader;
    PFN_glGetShaderiv        GetShaderiv;
    PFN_glGetShaderInfoLog   GetShaderInfoLog;
    PFN_glCreateProgram      CreateProgram;
    PFN_glAttachShader       AttachShader;
    PFN_glLinkProgram        LinkProgram;
    PFN_glUseProgram         UseProgram;
    PFN_glDeleteShader       DeleteShader;
    PFN_glGetUniformLocation GetUniformLocation;
    PFN_glUniform1f          Uniform1f;
    PFN_glUniform3f          Uniform3f;
    PFN_glUniformMatrix4fv   UniformMatrix4fv;
    PFN_glGenVertexArrays    GenVertexArrays;
    PFN_glBindVertexArray    BindVertexArray;
    PFN_glGenBuffers         GenBuffers;
    PFN_glBindBuffer         BindBuffer;
    PFN_glBufferData         BufferData;
    PFN_glEnableVertexAttribArray EnableVertexAttribArray;
    PFN_glVertexAttribPointer VertexAttribPointer;
    PFN_glDrawElements       DrawElements;
    PFN_glDrawArrays         DrawArrays;
    PFN_glGenFramebuffers    GenFramebuffers;
    PFN_glBindFramebuffer    BindFramebuffer;
    PFN_glFramebufferTexture2D FramebufferTexture2D;
    PFN_glGenTextures        GenTextures;
    PFN_glBindTexture        BindTexture;
    PFN_glTexImage2D         TexImage2D;
    PFN_glTexParameteri      TexParameteri;
    PFN_glActiveTexture      ActiveTexture;
    PFN_glUniform1i          Uniform1i;
    PFN_glViewport           Viewport;
    PFN_glClear              Clear;
    PFN_glClearColor         ClearColor;
    PFN_glEnable             Enable;
};

/* Resolve every pointer above. Returns 0 on success, <0 if any is missing. */
int sln_gl_load(SlnGL *gl);

#endif /* SLN_GL_LOADER_H */
