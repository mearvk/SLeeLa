/* =============================================================================
 * SleelaUI headless backend.
 *
 * A backend with no window system at all: it renders the Canvas into memory and
 * synthesises no input. It exists so the toolkit's portable core (layout, paint,
 * theme, rasterizer) can be built and smoke-tested on a CI runner with no X
 * server / no display, on every OS. It uses a built-in 5x7 bitmap font so glyph
 * metrics and coverage are deterministic and dependency-free.
 *
 * Selected by compiling with SLUI_BACKEND_HEADLESS_BUILD and linking this file
 * instead of a real backend. Max Rupplin -- MEARVK LLC -- 2026.
 * ===========================================================================*/
#include "slui_backend.hpp"
#include "slui_window.hpp"

#include <cstdint>
#include <cstring>

namespace slui {
namespace {

/* A minimal 5x7 glyph set: enough printable ASCII to measure and paint text in
 * tests. Unknown scalars fall back to a filled box so coverage is non-zero. */
const int kGlyphW = 5, kGlyphH = 7, kAdvance = 6;

/* Each glyph is 7 rows of 5 bits (MSB-first). Only a handful are distinct; the
 * default pattern is a hollow box so measurement and blitting are exercised. */
const uint8_t kBox[kGlyphH] = {0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F};
const uint8_t kSpace[kGlyphH] = {0, 0, 0, 0, 0, 0, 0};

class HeadlessWindow : public NativeWindow {
public:
    HeadlessWindow(int w, int h) : width_(w), height_(h) {}
    void set_title(const std::string& t) override { title_ = t; }
    void show() override { visible_ = true; }
    void hide() override { visible_ = false; }
    int width() const override { return width_; }
    int height() const override { return height_; }
    void request_redraw() override { dirty_ = true; }
    void present(const Canvas& canvas) override {
        last_ = canvas.width();
        (void)canvas;
        dirty_ = false;
    }
    bool visible_ = false;
    bool dirty_ = true;
    int last_ = 0;

private:
    int width_, height_;
    std::string title_;
};

class HeadlessBackend : public Backend {
public:
    SLUIBackend id() const override { return SLUI_BACKEND_UNKNOWN; }

    std::unique_ptr<NativeWindow> create_window(Window*, const std::string&,
                                                int w, int h, bool) override {
        return std::make_unique<HeadlessWindow>(w, h);
    }
    int pump(bool) override { return 0; }
    void quit(int code) override {
        quit_ = true;
        code_ = code;
    }
    bool should_quit(int* code) const override {
        if (quit_ && code) *code = code_;
        return quit_;
    }

    bool set_font(const std::string&, int size) override {
        size_ = size > 0 ? size : 11;
        return true;
    }
    double font_ascent() const override { return kGlyphH; }
    double font_descent() const override { return 2.0; }
    double font_line_height() const override { return kGlyphH + 4; }

    bool rasterize_glyph(uint32_t cp, GlyphBitmap* out) override {
        const uint8_t* rows = (cp == ' ') ? kSpace : kBox;
        out->left = 0;
        out->top = kGlyphH;
        out->width = kGlyphW;
        out->height = kGlyphH;
        out->advance = kAdvance;
        out->coverage.assign(static_cast<size_t>(kGlyphW) * kGlyphH, 0);
        for (int y = 0; y < kGlyphH; ++y)
            for (int x = 0; x < kGlyphW; ++x)
                if (rows[y] & (1 << (kGlyphW - 1 - x)))
                    out->coverage[static_cast<size_t>(y) * kGlyphW + x] = 255;
        return true;
    }
    double glyph_advance(uint32_t) override { return kAdvance; }

private:
    bool quit_ = false;
    int code_ = 0;
    int size_ = 11;
};

} // namespace

std::unique_ptr<Backend> create_backend(const std::string&) {
    return std::make_unique<HeadlessBackend>();
}

} // namespace slui
