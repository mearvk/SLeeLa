/* =============================================================================
 * SleelaUI mood / wash / tanor snapshot tool.
 *
 * Renders several Excellent-Wash moods on the warm #2B1608 base as soft radial
 * glows (lightless-bulb tanor), each placed in millimetres and coloured by a
 * mood sampled through Calculus-8, plus the same mood at two refresh phases to
 * show the "a little left" freshening. Display-free preview for CI and docs.
 *
 * Usage: slui-mood-snapshot [out.png]
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"
#include "sleela_ui_draw.h"
#include "sleela_ui_mood.h"

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

/* Paint a lightless-bulb tanor glow: a soft radial wash around (cx,cy). */
static void glow(SLUIDrawContext* dc, double cx, double cy, double radius,
                 SLUIColor color) {
    SLUIColor edge = (color & 0xFFFFFF00u) | 0x00u; /* fade alpha to 0 */
    slui_draw_set_blend(dc, SLUI_BLEND_ADD);
    slui_draw_radial_gradient(dc, cx, cy, radius, color, edge);
    slui_draw_set_blend(dc, SLUI_BLEND_OVER);
}

int main(int argc, char** argv) {
    const char* out = argc > 1 ? argv[1] : "docs/mood.png";
    const int W = 720, H = 320;
    SLUIDrawContext* dc = slui_draw_create(W, H, SLUI_BUFFER_SINGLE);
    slui_draw_clear(dc, 0x2B1608FF);

    SLUICalculus8* calc = slui_calc8_create();
    /* nudge a few differentiables so the mood sits mid-wash */
    slui_calc8_set(calc, 2, 0.7);
    slui_calc8_set(calc, 40, 0.3);
    slui_calc8_set(calc, 88, 0.6);

    const SLUIMoodKind kinds[] = {SLUI_MOOD_WARM, SLUI_MOOD_COOL, SLUI_MOOD_TENDER,
                                  SLUI_MOOD_RADIANT};
    const int n = 4;
    double dpi = 96.0;
    for (int i = 0; i < n; ++i) {
        SLUIMood* m = slui_mood_create(kinds[i]);
        slui_mood_set_calculus(m, calc);
        /* place each glow in mm: evenly across, 14mm above the row, the bulbs
         * sit a little left per the tanor's freshing bias. */
        double fx = 110.0 + i * 170.0;
        double fy = 120.0;
        SLUILight bulb = slui_light_for_text(fx, fy, slui_mm(0, 0, 14.0), dpi, m,
                                             0.0, SLUI_LIGHT_EMITTER,
                                             SLUI_POLARITY_LIGHT, 90, 1.0);
        glow(dc, bulb.x, bulb.y, 90, bulb.color);
        slui_mood_destroy(m);
    }

    /* Bottom row: the WARM mood at three refresh phases, showing the pattern
     * freshen "a little left" over time. */
    SLUIMood* warm = slui_mood_create(SLUI_MOOD_WARM);
    slui_mood_set_calculus(warm, calc);
    for (int t = 0; t < 3; ++t) {
        double time_s = t * 0.6; /* across a 0.5Hz refresh */
        SLUIColor c = slui_mood_color(warm, time_s);
        double fx = 150.0 + t * 220.0;
        glow(dc, fx, 250.0, 80, c);
    }
    slui_mood_destroy(warm);

    slui_calc8_destroy(calc);

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
