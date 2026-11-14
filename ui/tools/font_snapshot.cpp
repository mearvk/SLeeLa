/* =============================================================================
 * SleelaUI font-effects snapshot tool.
 *
 * Renders several SleelaUI fonts on the warm #2B1608 base, each showing a
 * different quality effect or stack -- drop shadow, soft glow, outline, relief
 * emboss/engrave, a gradient fill, and a lit sheen -- so the font effects are
 * visible display-free in CI and docs.
 *
 * Usage: slui-font-snapshot [out.png]
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"
#include "sleela_ui_draw.h"
#include "sleela_ui_font.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

/* ---- dependency-free PNG writer (zlib stored blocks) -------------------- */
namespace png {
uint32_t T[256];
bool R = false;
void I() {
    for (uint32_t n = 0; n < 256; ++n) {
        uint32_t c = n;
        for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        T[n] = c;
    }
    R = true;
}
uint32_t crc(const uint8_t* p, size_t n, uint32_t c = 0xFFFFFFFFu) {
    if (!R) I();
    for (size_t i = 0; i < n; ++i) c = T[(c ^ p[i]) & 0xFF] ^ (c >> 8);
    return c;
}
void p32(std::vector<uint8_t>& v, uint32_t x) {
    v.push_back(x >> 24);
    v.push_back(x >> 16);
    v.push_back(x >> 8);
    v.push_back(x);
}
void ch(std::vector<uint8_t>& o, const char* t, const std::vector<uint8_t>& d) {
    p32(o, (uint32_t)d.size());
    size_t s = o.size();
    o.insert(o.end(), t, t + 4);
    o.insert(o.end(), d.begin(), d.end());
    uint32_t c = crc(o.data() + s, o.size() - s) ^ 0xFFFFFFFFu;
    p32(o, c);
}
uint32_t ad(const uint8_t* p, size_t n) {
    uint32_t a = 1, b = 0;
    for (size_t i = 0; i < n; ++i) {
        a = (a + p[i]) % 65521;
        b = (b + a) % 65521;
    }
    return b << 16 | a;
}
bool w(const std::string& path, int W, int H, const uint8_t* rgb) {
    std::vector<uint8_t> raw;
    for (int y = 0; y < H; ++y) {
        raw.push_back(0);
        raw.insert(raw.end(), rgb + (size_t)y * W * 3, rgb + (size_t)(y + 1) * W * 3);
    }
    std::vector<uint8_t> z = {0x78, 1};
    size_t off = 0;
    while (off < raw.size()) {
        size_t b = std::min<size_t>(65535, raw.size() - off);
        bool last = off + b >= raw.size();
        z.push_back(last ? 1 : 0);
        z.push_back(b & 0xFF);
        z.push_back(b >> 8 & 0xFF);
        uint16_t nl = ~b;
        z.push_back(nl & 0xFF);
        z.push_back(nl >> 8 & 0xFF);
        z.insert(z.end(), raw.begin() + off, raw.begin() + off + b);
        off += b;
    }
    uint32_t a = ad(raw.data(), raw.size());
    z.push_back(a >> 24);
    z.push_back(a >> 16);
    z.push_back(a >> 8);
    z.push_back(a);
    std::vector<uint8_t> o = {0x89, 'P', 'N', 'G', 13, 10, 26, 10};
    std::vector<uint8_t> ih;
    p32(ih, W);
    p32(ih, H);
    ih.push_back(8);
    ih.push_back(2);
    ih.push_back(0);
    ih.push_back(0);
    ih.push_back(0);
    ch(o, "IHDR", ih);
    ch(o, "IDAT", z);
    ch(o, "IEND", {});
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return false;
    std::fwrite(o.data(), 1, o.size(), f);
    std::fclose(f);
    return true;
}
} // namespace png

int main(int argc, char** argv) {
    const char* out = argc > 1 ? argv[1] : "docs/font.png";
    const int W = 560, H = 360;
    SLUIDrawContext* dc = slui_draw_create(W, H, SLUI_BUFFER_SINGLE);
    slui_draw_clear(dc, 0x2B1608FF); /* the warm base */

    SLUIColor fg = slui_rgb(0xFF, 0xF0, 0xDC);
    int y = 54;
    const int step = 54;

    /* 1. plain */
    {
        SLUIFont* f = slui_font_create("system", 30);
        slui_font_set_weight(f, SLUI_FONT_BOLD);
        slui_font_draw(dc, f, "Plain", 28, y, fg);
        slui_font_destroy(f);
    }
    y += step;
    /* 2. drop shadow */
    {
        SLUIFont* f = slui_font_create("system", 30);
        slui_font_set_weight(f, SLUI_FONT_BOLD);
        slui_font_set_quality(f, SLUI_QUALITY_HIGH);
        slui_font_add_effect(f, slui_fx_drop_shadow(2, 4, 4, slui_rgb(0, 0, 0)));
        slui_font_draw(dc, f, "Drop shadow", 28, y, fg);
        slui_font_destroy(f);
    }
    y += step;
    /* 3. warm glow (light emission look) */
    {
        SLUIFont* f = slui_font_create("system", 30);
        slui_font_set_weight(f, SLUI_FONT_BOLD);
        slui_font_set_quality(f, SLUI_QUALITY_ULTRA);
        slui_font_add_effect(f, slui_fx_glow(7, 1.1, slui_rgb(0xFF, 0xB0, 0x50)));
        slui_font_draw(dc, f, "Warm glow", 28, y, fg);
        slui_font_destroy(f);
    }
    y += step;
    /* 4. outline */
    {
        SLUIFont* f = slui_font_create("system", 30);
        slui_font_set_weight(f, SLUI_FONT_BOLD);
        slui_font_add_effect(f, slui_fx_outline(2.0, slui_rgb(0x20, 0x10, 0x06)));
        slui_font_draw(dc, f, "Outline", 28, y, fg);
        slui_font_destroy(f);
    }
    y += step;
    /* 5. relief emboss + gradient fill */
    {
        SLUIFont* f = slui_font_create("system", 30);
        slui_font_set_weight(f, SLUI_FONT_BLACK);
        slui_font_set_quality(f, SLUI_QUALITY_HIGH);
        slui_font_add_effect(f, slui_fx_relief(SLUI_FONT_EMBOSSED, 2.5, 1.0));
        slui_font_add_effect(
            f, slui_fx_gradient_fill(slui_rgb(0xFF, 0xE6, 0xC0), slui_rgb(0xD8, 0x90, 0x40)));
        slui_font_draw(dc, f, "Embossed", 28, y, fg);
        slui_font_destroy(f);
    }
    y += step;
    /* 6. the works: shadow + glow + outline + light sheen */
    {
        SLUIFont* f = slui_font_create("system", 30);
        slui_font_set_weight(f, SLUI_FONT_BLACK);
        slui_font_set_quality(f, SLUI_QUALITY_ULTRA);
        slui_font_add_effect(f, slui_fx_drop_shadow(2, 4, 5, slui_rgb(0, 0, 0)));
        slui_font_add_effect(f, slui_fx_glow(6, 0.8, slui_rgb(0xFF, 0x9A, 0x3C)));
        slui_font_add_effect(f, slui_fx_outline(1.5, slui_rgb(0x20, 0x10, 0x06)));
        slui_font_add_effect(f, slui_fx_light(-1, -1, 1.2, slui_rgb(255, 255, 255)));
        slui_font_draw(dc, f, "SleelaUI", 28, y, fg);
        slui_font_destroy(f);
    }

    std::vector<uint8_t> rgb((size_t)W * H * 3);
    for (int i = 0; i < W * H; ++i) {
        SLUIColor p = slui_draw_get_pixel(dc, i % W, i / W);
        rgb[(size_t)i * 3 + 0] = (p >> 24) & 0xFF;
        rgb[(size_t)i * 3 + 1] = (p >> 16) & 0xFF;
        rgb[(size_t)i * 3 + 2] = (p >> 8) & 0xFF;
    }
    png::w(out, W, H, rgb.data());
    std::printf("wrote %s (%dx%d)\n", out, W, H);
    slui_draw_destroy(dc);
    return 0;
}
