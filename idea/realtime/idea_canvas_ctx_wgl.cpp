/*
 * idea/realtime/idea_canvas_ctx_wgl.cpp
 * Idea System(TM) realtime canvas -- Windows 10+ GL context (Win32 + WGL).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Standards used: Win32 window API + WGL (the standard OpenGL binding on
 * Windows) with the ARB extensions wglCreateContextAttribsARB /
 * wglChoosePixelFormatARB for a modern core-profile context. No D3D.
 *
 * Compiled only on Windows (guarded). NOT compiled or verified in the Linux
 * Idea System sandbox -- build and run it on Windows 10+ with its native
 * toolchain (see build-windows.ps1).
 */
#include "idea_canvas_ctx.h"

#if defined(_WIN32)

#include <windows.h>
#include <GL/gl.h>
#include <cstdio>
#include <cstring>

/* WGL ARB entry points and tokens (from wglext.h; declared here to avoid an
 * extra dependency, standard values). */
typedef HGLRC (WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
typedef BOOL  (WINAPI *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC, const int*, const FLOAT*, UINT, int*, UINT*);
#define WGL_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB       0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB        0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB    0x00000001
#define WGL_DRAW_TO_WINDOW_ARB              0x2001
#define WGL_SUPPORT_OPENGL_ARB             0x2010
#define WGL_DOUBLE_BUFFER_ARB              0x2011
#define WGL_PIXEL_TYPE_ARB                 0x2013
#define WGL_TYPE_RGBA_ARB                  0x202B
#define WGL_COLOR_BITS_ARB                 0x2014
#define WGL_DEPTH_BITS_ARB                 0x2022

struct idea_ctx {
    HWND  hwnd = nullptr;
    HDC   hdc  = nullptr;
    HGLRC hglrc = nullptr;
    bool  windowed = false;
    bool  should_close = false;
};

static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_CLOSE || m == WM_DESTROY) {
        idea_ctx *c = (idea_ctx*)GetWindowLongPtr(h, GWLP_USERDATA);
        if (c) c->should_close = true;
        return 0;
    }
    return DefWindowProc(h, m, w, l);
}

idea_ctx *idea_ctx_create(int width, int height, int windowed, const char *title) {
    idea_ctx *c = new idea_ctx();
    c->windowed = windowed != 0;

    WNDCLASSA wc; memset(&wc, 0, sizeof wc);
    wc.lpfnWndProc = wndproc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "IdeaCanvasWGL";
    wc.style = CS_OWNDC;
    RegisterClassA(&wc);

    DWORD style = windowed ? (WS_OVERLAPPEDWINDOW | WS_VISIBLE) : WS_POPUP;
    c->hwnd = CreateWindowExA(0, wc.lpszClassName, title ? title : "Idea System(TM)",
                              style, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                              nullptr, nullptr, wc.hInstance, nullptr);
    if (!c->hwnd) { delete c; return nullptr; }
    SetWindowLongPtr(c->hwnd, GWLP_USERDATA, (LONG_PTR)c);
    c->hdc = GetDC(c->hwnd);

    /* Bootstrap a legacy context to load the ARB entry points. */
    PIXELFORMATDESCRIPTOR pfd; memset(&pfd, 0, sizeof pfd);
    pfd.nSize = sizeof pfd; pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA; pfd.cColorBits = 32; pfd.cDepthBits = 24;
    int pf = ChoosePixelFormat(c->hdc, &pfd);
    SetPixelFormat(c->hdc, pf, &pfd);
    HGLRC tmp = wglCreateContext(c->hdc);
    wglMakeCurrent(c->hdc, tmp);

    auto wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB) {
        const int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 4,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        c->hglrc = wglCreateContextAttribsARB(c->hdc, nullptr, attribs);
        wglMakeCurrent(c->hdc, nullptr);
        wglDeleteContext(tmp);
        wglMakeCurrent(c->hdc, c->hglrc);
    } else {
        c->hglrc = tmp;  /* fall back to the legacy context */
    }
    if (!c->hglrc) { ReleaseDC(c->hwnd, c->hdc); DestroyWindow(c->hwnd); delete c; return nullptr; }
    return c;
}

int idea_ctx_make_current(idea_ctx *c) {
    if (!c) return -1;
    return wglMakeCurrent(c->hdc, c->hglrc) ? 0 : -1;
}
void idea_ctx_present(idea_ctx *c) { if (c && c->windowed) SwapBuffers(c->hdc); }

int idea_ctx_pump(idea_ctx *c) {
    if (!c) return 0;
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg); DispatchMessage(&msg);
    }
    return c->should_close ? 0 : 1;
}

void idea_ctx_destroy(idea_ctx *c) {
    if (!c) return;
    wglMakeCurrent(c->hdc, nullptr);
    if (c->hglrc) wglDeleteContext(c->hglrc);
    if (c->hdc)   ReleaseDC(c->hwnd, c->hdc);
    if (c->hwnd)  DestroyWindow(c->hwnd);
    delete c;
}

idea_ctx_kind idea_ctx_kind_of(void) { return IDEA_CTX_WGL; }
int idea_ctx_supported(void) { return 1; }

#endif /* _WIN32 */
