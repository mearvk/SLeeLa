/* =============================================================================
 * SleelaUI throbber snapshot tool.
 *
 * Renders three water-like Throbbers -- calm, medium, and vigorous, at
 * different widths and base hues -- advanced through 200 frames of the flow
 * field, and writes the result to a PNG (via the same dependency-free encoder
 * as the main snapshot tool). A display-free way to see the throbber's motion
 * and predictive colouring in CI and docs.
 *
 * Usage: slui-throbber-snapshot [out.png]
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "slui_backend.hpp"
#include "slui_theme.hpp"
#include "slui_widget.hpp"
#include "slui_window.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

using namespace slui;

/* ---- dependency-free PNG writer (zlib stored blocks); see tools/snapshot.cpp */
namespace png {
uint32_t crc_table[256];
bool crc_ready = false;
void crc_init() {
    for (uint32_t n = 0; n < 256; ++n) {
        uint32_t c = n;
        for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        crc_table[n] = c;
    }
    crc_ready = true;
}
uint32_t crc32(const uint8_t* p, size_t n, uint32_t crc = 0xFFFFFFFFu) {
    if (!crc_ready) crc_init();
    for (size_t i = 0; i < n; ++i) crc = crc_table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    return crc;
}
void put32(std::vector<uint8_t>& v, uint32_t x) {
    v.push_back((x >> 24) & 0xFF);
    v.push_back((x >> 16) & 0xFF);
    v.push_back((x >> 8) & 0xFF);
    v.push_back(x & 0xFF);
}
void chunk(std::vector<uint8_t>& out, const char* type,
           const std::vector<uint8_t>& data) {
    put32(out, (uint32_t)data.size());
    size_t start = out.size();
    out.insert(out.end(), type, type + 4);
    out.insert(out.end(), data.begin(), data.end());
    uint32_t c = crc32(out.data() + start, out.size() - start);
    put32(out, c ^ 0xFFFFFFFFu);
}
uint32_t adler32(const uint8_t* p, size_t n) {
    uint32_t a = 1, b = 0;
    for (size_t i = 0; i < n; ++i) {
        a = (a + p[i]) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}
bool write(const std::string& path, int w, int h, const uint8_t* rgb) {
    std::vector<uint8_t> raw;
    raw.reserve((size_t)(w * 3 + 1) * h);
    for (int y = 0; y < h; ++y) {
        raw.push_back(0);
        raw.insert(raw.end(), rgb + (size_t)y * w * 3, rgb + (size_t)(y + 1) * w * 3);
    }
    std::vector<uint8_t> z = {0x78, 0x01};
    size_t off = 0;
    while (off < raw.size()) {
        size_t block = std::min<size_t>(65535, raw.size() - off);
        bool last = (off + block >= raw.size());
        z.push_back(last ? 1 : 0);
        z.push_back(block & 0xFF);
        z.push_back((block >> 8) & 0xFF);
        uint16_t nlen = (uint16_t)~block;
        z.push_back(nlen & 0xFF);
        z.push_back((nlen >> 8) & 0xFF);
        z.insert(z.end(), raw.begin() + off, raw.begin() + off + block);
        off += block;
    }
    uint32_t ad = adler32(raw.data(), raw.size());
    z.push_back((ad >> 24) & 0xFF);
    z.push_back((ad >> 16) & 0xFF);
    z.push_back((ad >> 8) & 0xFF);
    z.push_back(ad & 0xFF);
    std::vector<uint8_t> out = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    std::vector<uint8_t> ihdr;
    put32(ihdr, w);
    put32(ihdr, h);
    ihdr.push_back(8);
    ihdr.push_back(2);
    ihdr.push_back(0);
    ihdr.push_back(0);
    ihdr.push_back(0);
    chunk(out, "IHDR", ihdr);
    chunk(out, "IDAT", z);
    chunk(out, "IEND", {});
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return false;
    std::fwrite(out.data(), 1, out.size(), f);
    std::fclose(f);
    return true;
}
} // namespace png

int main(int argc, char** argv) {
    const char* out = argc > 1 ? argv[1] : "docs/throbber.png";
    auto be = create_backend("com.mearvk.SleelaUI.ThrobberSnapshot");
    if (!be) {
        std::fprintf(stderr, "throbber-snapshot: no backend\n");
        return 1;
    }
    SLUITheme abi;
    theme_fill_preset(&abi, SLUI_THEME_SLICK_BLACK);
    SLUIWindowConfig cfg{};
    cfg.title = "SleelaUI Throbber";
    cfg.width = 460;
    cfg.height = 190;
    cfg.theme = &abi;
    Window win(nullptr, be.get(), cfg);
    Widget* root = win.root();

    auto col = root->add_child(std::make_unique<Box>(SLUI_ORIENT_VERTICAL, 18));
    col->set_margin(Margin{22, 24, 22, 24});
    col->set_expand(true, true);

    struct Spec { int w, h; double intensity, hue; };
    const Spec specs[] = {{400, 28, 0.25, 205.0}, /* calm, water-blue   */
                          {320, 28, 0.60, 150.0}, /* medium, green water */
                          {400, 28, 0.95, 30.0}}; /* vigorous, warm      */
    for (const auto& s : specs) {
        auto t = col->add_child(std::make_unique<Throbber>(s.w));
        t->set_size_request(s.w, s.h);
        auto* th = static_cast<Throbber*>(t);
        th->set_intensity(s.intensity);
        th->set_hue(s.hue);
    }

    for (int f = 0; f < 200; ++f) {
        win.animation_tick(1.0 / 60.0, nullptr);
        win.render();
    }
    const Canvas& c = win.render();

    std::vector<uint8_t> rgb((size_t)c.width() * c.height() * 3);
    for (int i = 0; i < c.width() * c.height(); ++i) {
        uint32_t p = c.pixels()[i];
        rgb[(size_t)i * 3 + 0] = (uint8_t)((p >> 16) & 0xFF);
        rgb[(size_t)i * 3 + 1] = (uint8_t)((p >> 8) & 0xFF);
        rgb[(size_t)i * 3 + 2] = (uint8_t)(p & 0xFF);
    }
    if (!png::write(out, c.width(), c.height(), rgb.data())) {
        std::fprintf(stderr, "throbber-snapshot: cannot write %s\n", out);
        return 1;
    }
    std::printf("wrote %s (%dx%d)\n", out, c.width(), c.height());
    return 0;
}
