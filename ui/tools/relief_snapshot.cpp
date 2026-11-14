/* =============================================================================
 * SleelaUI relief / lighting snapshot tool.
 *
 * Renders the lighting system on the deep warm #2B1608 base: a row of panels
 * with different relief profiles (flat, rounded, bevel, engraved, embossed)
 * lit by a warm directional emitter, plus a point light source and a shadow
 * emitter, with soft cast shadows. A display-free way to see the quality of the
 * light, the darkness, and the relief in CI and docs.
 *
 * Usage: slui-relief-snapshot [out.png]
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"
#include "sleela_ui_draw.h"
#include "sleela_ui_light.h"

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
    const char* out = argc > 1 ? argv[1] : "docs/relief.png";

    const int W = 760, H = 300;
    SLUIDrawContext* dc = slui_draw_create(W, H, SLUI_BUFFER_SINGLE);

    /* The deep warm base colour #2B1608 as the floor. */
    slui_draw_clear(dc, 0x2B1608FF);

    /* A scene: a warm directional sun emitter (top-left), a point light source
     * (upper area), and a shadow emitter pooling darkness bottom-right. */
    SLUILightScene* scene = slui_light_scene_create();
    slui_light_scene_set_ambient(scene, 0.55, slui_rgb(0xFF, 0xE8, 0xCC));
    slui_light_scene_add_emitter(
        scene, slui_light_directional(-0.8, 0.9, 1.1, slui_rgb(0xFF, 0xE6, 0xC0)));
    SLUILight pt = slui_light_point(W * 0.5, 40, 420, 0.9, slui_rgb(0xFF, 0xD0, 0x90));
    slui_light_scene_add_source(scene, /*anchor=*/1, pt);
    slui_light_scene_add_emitter(
        scene, slui_shadow_point(W - 60, H - 40, 300, 0.6, slui_rgb(0x08, 0x03, 0x00)));

    const SLUIReliefProfile profiles[] = {SLUI_RELIEF_FLAT, SLUI_RELIEF_ROUNDED,
                                          SLUI_RELIEF_BEVEL, SLUI_RELIEF_ENGRAVED,
                                          SLUI_RELIEF_EMBOSSED};
    const int n = 5;
    int pad = 24;
    int pw = (W - pad * (n + 1)) / n;
    int ph = 150;
    int py = 70;
    for (int i = 0; i < n; ++i) {
        int px = pad + i * (pw + pad);
        SLUIMaterial m = slui_material(profiles[i], 7.0);
        m.gloss = 0.45;
        m.occlusion = 0.5;
        m.edge_px = 10.0;
        /* a warm panel colour lifted from the base family */
        slui_light_panel(dc, (SLUIRect){px, py, pw, ph}, 14.0,
                         slui_rgb(0x7A, 0x45, 0x1C), m, scene);
    }

    std::vector<uint8_t> rgb((size_t)W * H * 3);
    for (int i = 0; i < W * H; ++i) {
        SLUIColor p = slui_draw_get_pixel(dc, i % W, i / W);
        rgb[(size_t)i * 3 + 0] = (p >> 24) & 0xFF; /* note: get_pixel is RRGGBBAA */
        rgb[(size_t)i * 3 + 1] = (p >> 16) & 0xFF;
        rgb[(size_t)i * 3 + 2] = (p >> 8) & 0xFF;
    }
    png::w(out, W, H, rgb.data());
    std::printf("wrote %s (%dx%d)\n", out, W, H);
    slui_light_scene_destroy(scene);
    slui_draw_destroy(dc);
    return 0;
}
