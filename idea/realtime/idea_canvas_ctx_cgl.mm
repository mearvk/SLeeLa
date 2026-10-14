/*
 * idea/realtime/idea_canvas_ctx_cgl.mm
 * Idea System(TM) realtime canvas -- macOS GL context (Cocoa + CGL / NSOpenGL).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Standards used: CGL (Core OpenGL) + NSOpenGLContext, the standard OpenGL
 * bindings on macOS, with an NSWindow for the windowed path. No Metal.
 * A core-profile (OpenGL 4.1 -- the max macOS exposes) context is requested.
 *
 * NOTE: Apple deprecated OpenGL (still shipped and functional through current
 * macOS). We use it because it is the open, cross-platform standard; a Metal
 * backend can be added later behind the same idea_canvas_ctx interface.
 *
 * Objective-C++ (.mm). Compiled only on macOS (guarded). NOT compiled or
 * verified in the Linux Idea System sandbox -- build and run on macOS.
 */
#include "idea_canvas_ctx.h"

#if defined(__APPLE__)

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>

struct idea_ctx {
    NSWindow *window = nil;
    NSOpenGLContext *gl = nil;
    bool windowed = false;
    bool should_close = false;
};

idea_ctx *idea_ctx_create(int width, int height, int windowed, const char *title) {
    @autoreleasepool {
        idea_ctx *c = new idea_ctx();
        c->windowed = windowed != 0;

        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAAccelerated,
            0
        };
        NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        if (!pf) { delete c; return nullptr; }
        c->gl = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
        if (!c->gl) { delete c; return nullptr; }

        if (c->windowed) {
            [NSApplication sharedApplication];
            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
            NSRect frame = NSMakeRect(0, 0, width, height);
            c->window = [[NSWindow alloc] initWithContentRect:frame
                styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                           NSWindowStyleMaskResizable)
                backing:NSBackingStoreBuffered defer:NO];
            [c->window setTitle:[NSString stringWithUTF8String:(title ? title : "Idea System(TM)")]];
            [c->window makeKeyAndOrderFront:nil];
            [c->gl setView:[c->window contentView]];
        }
        [c->gl makeCurrentContext];
        return c;
    }
}

int idea_ctx_make_current(idea_ctx *c) {
    if (!c || !c->gl) return -1;
    [c->gl makeCurrentContext];
    return 0;
}

void idea_ctx_present(idea_ctx *c) {
    if (c && c->gl && c->windowed) [c->gl flushBuffer];
}

int idea_ctx_pump(idea_ctx *c) {
    if (!c) return 0;
    @autoreleasepool {
        NSEvent *ev;
        while ((ev = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil
                            inMode:NSDefaultRunLoopMode dequeue:YES])) {
            if ([ev type] == NSEventTypeAppKitDefined) { /* window close etc. */ }
            [NSApp sendEvent:ev];
        }
        if (c->window && ![c->window isVisible]) c->should_close = true;
    }
    return c->should_close ? 0 : 1;
}

void idea_ctx_destroy(idea_ctx *c) {
    if (!c) return;
    @autoreleasepool {
        [NSOpenGLContext clearCurrentContext];
        if (c->window) [c->window close];
    }
    delete c;
}

idea_ctx_kind idea_ctx_kind_of(void) { return IDEA_CTX_CGL; }
int idea_ctx_supported(void) { return 1; }

#endif /* __APPLE__ */
