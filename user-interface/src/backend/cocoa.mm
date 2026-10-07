/* =============================================================================
 * SleelaUI Cocoa backend (macOS / Darwin).
 *
 * The real host surface on macOS: an NSWindow with a custom NSView that draws
 * the toolkit's Canvas via a CGImage, Core Text for glyph coverage rendered
 * into a one-channel (grayscale) CGBitmapContext, and the AppKit run loop for
 * input. No SwiftUI, no AppKit controls -- SleelaUI owns its pixels and only
 * asks Cocoa to show them, exactly as the X11 and Win32 backends do.
 *
 * Compiled as Objective-C++ (.mm). "Correct, not styled": translate native
 * input into SLUIEvent, present the Canvas; the core owns all look and feel.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#import <Cocoa/Cocoa.h>
#import <CoreText/CoreText.h>

#include "slui_backend.hpp"
#include "slui_window.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace slui {
namespace {
uint32_t map_keycode(unsigned short kc);
uint32_t map_flags(NSEventModifierFlags f);
} // namespace
} // namespace slui

/* The view holds a back-pointer to the portable Window so it can pull the
 * Canvas on draw and push events on input. */
@interface SLUIView : NSView {
@public
    slui::Window* owner;
    const uint32_t* framePixels;
    int frameW;
    int frameH;
}
@end

@implementation SLUIView
- (BOOL)isFlipped {
    return YES; /* top-left origin, matching the Canvas */
}
- (BOOL)acceptsFirstResponder {
    return YES;
}
- (void)drawRect:(NSRect)dirtyRect {
    (void)dirtyRect;
    if (!owner) return;
    const slui::Canvas& c = owner->render();
    framePixels = c.pixels();
    frameW = c.width();
    frameH = c.height();
    if (frameW <= 0 || frameH <= 0) return;

    CGColorSpaceRef cs = CGColorSpaceCreateDeviceRGB();
    CGContextRef ctx =
        (CGContextRef)[[NSGraphicsContext currentContext] CGContext];
    CGDataProviderRef prov = CGDataProviderCreateWithData(
        NULL, framePixels, (size_t)frameW * frameH * 4, NULL);
    /* Canvas packs 0xAARRGGBB; request matching host-endian ARGB. */
    CGImageRef img = CGImageCreate(
        frameW, frameH, 8, 32, (size_t)frameW * 4, cs,
        kCGBitmapByteOrder32Little | kCGImageAlphaNoneSkipFirst, prov, NULL,
        false, kCGRenderingIntentDefault);
    CGContextDrawImage(ctx, CGRectMake(0, 0, frameW, frameH), img);
    CGImageRelease(img);
    CGDataProviderRelease(prov);
    CGColorSpaceRelease(cs);
}
@end

/* Window delegate: forwards close/resize to the portable Window. */
@interface SLUIWindowDelegate : NSObject <NSWindowDelegate> {
@public
    slui::Window* owner;
}
@end

@implementation SLUIWindowDelegate
- (BOOL)windowShouldClose:(NSWindow*)sender {
    (void)sender;
    if (owner) {
        SLUIEvent ev{};
        ev.type = SLUI_EVENT_CLOSE;
        ev.window = reinterpret_cast<SLUIWindow*>(owner);
        owner->handle_event(ev);
    }
    return NO; /* the core decides whether to really close */
}
- (void)windowDidResize:(NSNotification*)note {
    NSWindow* w = [note object];
    NSRect r = [[w contentView] bounds];
    if (owner) {
        SLUIEvent ev{};
        ev.type = SLUI_EVENT_RESIZE;
        ev.width = (int)r.size.width;
        ev.height = (int)r.size.height;
        ev.window = reinterpret_cast<SLUIWindow*>(owner);
        owner->handle_event(ev);
        [[w contentView] setNeedsDisplay:YES];
    }
}
@end

namespace slui {
namespace {

class CocoaWindow : public NativeWindow {
public:
    CocoaWindow(NSWindow* w, SLUIView* v, SLUIWindowDelegate* d)
        : window_(w), view_(v), delegate_(d) {}
    ~CocoaWindow() override {
        [window_ close];
    }
    void set_title(const std::string& t) override {
        [window_ setTitle:[NSString stringWithUTF8String:t.c_str()]];
    }
    void show() override {
        [window_ makeKeyAndOrderFront:nil];
    }
    void hide() override { [window_ orderOut:nil]; }
    int width() const override { return (int)[[window_ contentView] bounds].size.width; }
    int height() const override { return (int)[[window_ contentView] bounds].size.height; }
    void request_redraw() override { [view_ setNeedsDisplay:YES]; }
    void present(const Canvas&) override { [view_ setNeedsDisplay:YES]; }

    NSWindow* window() const { return window_; }

private:
    NSWindow* window_;
    SLUIView* view_;
    SLUIWindowDelegate* delegate_;
};

class CocoaBackend : public Backend {
public:
    explicit CocoaBackend(const std::string&) {
        @autoreleasepool {
            [NSApplication sharedApplication];
            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        }
    }
    ~CocoaBackend() override {
        if (font_) CFRelease(font_);
    }

    SLUIBackend id() const override { return SLUI_BACKEND_COCOA; }

    std::unique_ptr<NativeWindow> create_window(Window* owner,
                                                const std::string& title,
                                                int width, int height,
                                                bool resizable) override {
        @autoreleasepool {
            NSUInteger style = NSWindowStyleMaskTitled |
                               NSWindowStyleMaskClosable |
                               NSWindowStyleMaskMiniaturizable;
            if (resizable) style |= NSWindowStyleMaskResizable;
            NSRect frame = NSMakeRect(0, 0, width, height);
            NSWindow* win =
                [[NSWindow alloc] initWithContentRect:frame
                                            styleMask:style
                                              backing:NSBackingStoreBuffered
                                                defer:NO];
            [win setTitle:[NSString stringWithUTF8String:title.c_str()]];
            [win center];

            SLUIView* view = [[SLUIView alloc] initWithFrame:frame];
            view->owner = owner;
            [win setContentView:view];

            SLUIWindowDelegate* del = [[SLUIWindowDelegate alloc] init];
            del->owner = owner;
            [win setDelegate:del];

            [NSApp activateIgnoringOtherApps:YES];
            owners_.push_back(owner);
            return std::make_unique<CocoaWindow>(win, view, del);
        }
    }

    int pump(bool block) override {
        @autoreleasepool {
            int dispatched = 0;

            /* Any animated widget? Pick the fastest refresh so we wake on a
             * frame timer and advance throbbers/canvas views while idle. */
            double fps = 0.0;
            bool animated = false;
            for (Window* o : owners_) {
                double f = 0.0;
                if (o && o->has_animation(&f)) {
                    animated = true;
                    if (f > fps) fps = f;
                }
            }
            if (fps <= 0.0) fps = 60.0;

            NSDate* until;
            if (!block) until = [NSDate distantPast];
            else if (animated)
                until = [NSDate dateWithTimeIntervalSinceNow:1.0 / fps];
            else until = [NSDate distantFuture];

            for (;;) {
                NSEvent* ev = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                 untilDate:until
                                                    inMode:NSDefaultRunLoopMode
                                                   dequeue:YES];
                if (!ev) break;
                route_event(ev);
                [NSApp sendEvent:ev];
                ++dispatched;
                until = [NSDate distantPast];
            }

            if (animated) {
                double now = now_seconds();
                double dt = last_anim_ > 0 ? now - last_anim_ : 1.0 / fps;
                last_anim_ = now;
                if (dt > 0.25) dt = 0.25;
                for (Window* o : owners_)
                    if (o) o->animation_tick(dt, nullptr);
            }
            return dispatched;
        }
    }

    void quit(int code) override {
        quit_ = true;
        exit_code_ = code;
        [NSApp stop:nil];
    }
    bool should_quit(int* code) const override {
        if (quit_ && code) *code = exit_code_;
        return quit_;
    }

    /* ---- fonts (Core Text grayscale coverage) -------------------------- */
    bool set_font(const std::string& family, int size_pt) override {
        std::string key = family + "@" + std::to_string(size_pt);
        if (key == font_key_ && font_) return true;
        if (font_) {
            CFRelease(font_);
            font_ = nullptr;
        }
        CFStringRef name = CFStringCreateWithCString(
            NULL,
            (family == "system" || family.empty()) ? "Helvetica Neue"
                                                    : family.c_str(),
            kCFStringEncodingUTF8);
        font_ = CTFontCreateWithName(name, size_pt, NULL);
        CFRelease(name);
        if (!font_) return false;
        ascent_ = CTFontGetAscent(font_);
        descent_ = CTFontGetDescent(font_);
        line_height_ = ascent_ + descent_ + CTFontGetLeading(font_);
        font_key_ = key;
        advance_cache_.clear();
        return true;
    }
    double font_ascent() const override { return ascent_; }
    double font_descent() const override { return descent_; }
    double font_line_height() const override { return line_height_; }

    bool rasterize_glyph(uint32_t cp, GlyphBitmap* out) override {
        if (!font_) return false;
        UniChar chars[2];
        CGGlyph glyphs[2];
        CFIndex n = encode_utf16(cp, chars);
        if (!CTFontGetGlyphsForCharacters(font_, chars, glyphs, n)) return false;
        CGGlyph glyph = glyphs[0];

        CGRect bb = CTFontGetBoundingRectsForGlyphs(font_, kCTFontOrientationDefault,
                                                    &glyph, NULL, 1);
        int w = (int)ceil(bb.size.width) + 2;
        int h = (int)ceil(bb.size.height) + 2;
        if (w <= 0 || h <= 0) {
            out->width = out->height = 0;
            out->advance = glyph_advance(cp);
            out->coverage.clear();
            return true;
        }
        std::vector<uint8_t> gray((size_t)w * h, 0);
        CGColorSpaceRef cs = CGColorSpaceCreateDeviceGray();
        CGContextRef ctx = CGBitmapContextCreate(gray.data(), w, h, 8, w, cs,
                                                 kCGImageAlphaNone);
        CGColorSpaceRelease(cs);
        CGContextSetGrayFillColor(ctx, 1.0, 1.0);
        CGPoint origin = CGPointMake(-bb.origin.x + 1, -bb.origin.y + 1);
        CTFontDrawGlyphs(font_, &glyph, &origin, 1, ctx);
        CGContextRelease(ctx);

        out->left = (int)floor(bb.origin.x) - 1;
        out->top = (int)ceil(bb.origin.y + bb.size.height) + 1;
        out->width = w;
        out->height = h;
        out->advance = glyph_advance(cp);
        /* The gray context is bottom-up; flip rows into the Canvas's top-down
         * coverage buffer. */
        out->coverage.assign((size_t)w * h, 0);
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                out->coverage[(size_t)y * w + x] = gray[(size_t)(h - 1 - y) * w + x];
        return true;
    }

    double glyph_advance(uint32_t cp) override {
        if (!font_) return 8.0;
        auto it = advance_cache_.find(cp);
        if (it != advance_cache_.end()) return it->second;
        UniChar chars[2];
        CGGlyph glyphs[2];
        CFIndex n = encode_utf16(cp, chars);
        double adv = 8.0;
        if (CTFontGetGlyphsForCharacters(font_, chars, glyphs, n)) {
            CGSize advs;
            CTFontGetAdvancesForGlyphs(font_, kCTFontOrientationHorizontal,
                                       &glyphs[0], &advs, 1);
            adv = advs.width;
        }
        advance_cache_[cp] = adv;
        return adv;
    }

private:
    static CFIndex encode_utf16(uint32_t cp, UniChar out[2]) {
        if (cp <= 0xFFFF) {
            out[0] = (UniChar)cp;
            return 1;
        }
        cp -= 0x10000;
        out[0] = (UniChar)(0xD800 + (cp >> 10));
        out[1] = (UniChar)(0xDC00 + (cp & 0x3FF));
        return 2;
    }

    void route_event(NSEvent* ev) {
        NSWindow* win = [ev window];
        if (!win) return;
        NSView* cv = [win contentView];
        if (![cv isKindOfClass:[SLUIView class]]) return;
        SLUIView* view = (SLUIView*)cv;
        Window* owner = view->owner;
        if (!owner) return;

        SLUIEvent e{};
        e.window = reinterpret_cast<SLUIWindow*>(owner);
        NSPoint p = [ev locationInWindow];
        NSPoint local = [view convertPoint:p fromView:nil];
        e.x = (int)local.x;
        e.y = (int)local.y;

        switch ([ev type]) {
        case NSEventTypeMouseMoved:
        case NSEventTypeLeftMouseDragged:
            e.type = SLUI_EVENT_POINTER_MOVE;
            e.modifiers = map_flags([ev modifierFlags]);
            owner->handle_event(e);
            [view setNeedsDisplay:YES];
            break;
        case NSEventTypeLeftMouseDown:
            e.type = SLUI_EVENT_POINTER_DOWN;
            e.button = SLUI_BUTTON_LEFT;
            owner->handle_event(e);
            [view setNeedsDisplay:YES];
            break;
        case NSEventTypeLeftMouseUp:
            e.type = SLUI_EVENT_POINTER_UP;
            e.button = SLUI_BUTTON_LEFT;
            owner->handle_event(e);
            [view setNeedsDisplay:YES];
            break;
        case NSEventTypeKeyDown:
        case NSEventTypeKeyUp: {
            e.type = [ev type] == NSEventTypeKeyDown ? SLUI_EVENT_KEY_DOWN
                                                     : SLUI_EVENT_KEY_UP;
            e.keysym = map_keycode([ev keyCode]);
            e.modifiers = map_flags([ev modifierFlags]);
            owner->handle_event(e);
            if ([ev type] == NSEventTypeKeyDown) {
                NSString* s = [ev characters];
                if ([s length] > 0) {
                    SLUIEvent te{};
                    te.type = SLUI_EVENT_TEXT;
                    te.window = e.window;
                    te.codepoint = [s characterAtIndex:0];
                    if (te.codepoint >= 0x20 && te.codepoint != 0x7F)
                        owner->handle_event(te);
                }
            }
            [view setNeedsDisplay:YES];
            break;
        }
        default:
            break;
        }
    }

    CTFontRef font_ = nullptr;
    std::string font_key_;
    double ascent_ = 12, descent_ = 3, line_height_ = 16;
    bool quit_ = false;
    int exit_code_ = 0;
    std::unordered_map<uint32_t, double> advance_cache_;
    std::vector<Window*> owners_;
    double last_anim_ = 0.0;
    static double now_seconds() {
        return CFAbsoluteTimeGetCurrent();
    }
};

uint32_t map_keycode(unsigned short kc) {
    switch (kc) {
    case 36: return SLUI_KEY_ENTER;     /* Return   */
    case 76: return SLUI_KEY_ENTER;     /* KP Enter */
    case 53: return SLUI_KEY_ESCAPE;
    case 51: return SLUI_KEY_BACKSPACE;
    case 48: return SLUI_KEY_TAB;
    case 49: return SLUI_KEY_SPACE;
    case 123: return SLUI_KEY_LEFT;
    case 124: return SLUI_KEY_RIGHT;
    case 126: return SLUI_KEY_UP;
    case 125: return SLUI_KEY_DOWN;
    case 115: return SLUI_KEY_HOME;
    case 119: return SLUI_KEY_END;
    case 117: return SLUI_KEY_DELETE;
    default: return SLUI_KEY_UNKNOWN;
    }
}

uint32_t map_flags(NSEventModifierFlags f) {
    uint32_t m = 0;
    if (f & NSEventModifierFlagShift) m |= SLUI_MOD_SHIFT;
    if (f & NSEventModifierFlagControl) m |= SLUI_MOD_CTRL;
    if (f & NSEventModifierFlagOption) m |= SLUI_MOD_ALT;
    if (f & NSEventModifierFlagCommand) m |= SLUI_MOD_SUPER;
    return m;
}

} // namespace

std::unique_ptr<Backend> create_backend(const std::string& app_id) {
    return std::make_unique<CocoaBackend>(app_id);
}

} // namespace slui
