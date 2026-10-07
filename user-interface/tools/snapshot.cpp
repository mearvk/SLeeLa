/* =============================================================================
 * SleelaUI offscreen snapshot tool.
 *
 * Renders a representative Slick Black window with the full control set through
 * the headless backend and writes the Canvas to a PPM image. This gives a
 * display-free, reproducible preview of the toolkit's look -- useful in CI and
 * for documentation -- without needing an X server. (The headless backend uses
 * a built-in 5x7 bitmap font, so text in the snapshot is schematic; the shapes,
 * palette, spacing, and compositing are the real pixels the toolkit produces.)
 *
 * Usage: slui-snapshot [out.ppm]
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

/* ---- dependency-free PNG writer (zlib stored blocks) --------------------
 * Enough of PNG to emit a truecolor 8-bit image with no compression: a single
 * stored (type-0) DEFLATE stream wrapped in a zlib header, with the required
 * CRC-32 and Adler-32 checksums. No libpng, no zlib -- the snapshot stays as
 * self-contained as the rest of the toolkit. */
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
    /* Build the raw scanlines with a 0 filter byte per row. */
    std::vector<uint8_t> raw;
    raw.reserve((size_t)(w * 3 + 1) * h);
    for (int y = 0; y < h; ++y) {
        raw.push_back(0);
        raw.insert(raw.end(), rgb + (size_t)y * w * 3, rgb + (size_t)(y + 1) * w * 3);
    }
    /* zlib stream: 2-byte header + stored DEFLATE blocks + adler32. */
    std::vector<uint8_t> z;
    z.push_back(0x78);
    z.push_back(0x01);
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
    ihdr.push_back(8);   /* bit depth */
    ihdr.push_back(2);   /* colour type: truecolor */
    ihdr.push_back(0);   /* compression */
    ihdr.push_back(0);   /* filter */
    ihdr.push_back(0);   /* interlace */
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
    const char* out = argc > 1 ? argv[1] : "slui-snapshot.ppm";

    auto be = create_backend("com.mearvk.SleelaUI.Snapshot");
    if (!be) {
        std::fprintf(stderr, "snapshot: no backend\n");
        return 1;
    }

    SLUITheme abi;
    theme_fill_preset(&abi, SLUI_THEME_SLICK_BLACK);
    SLUIWindowConfig cfg{};
    cfg.title = "SleelaUI Gallery";
    cfg.width = 520;
    cfg.height = 420;
    cfg.theme = &abi;

    Window win(nullptr, be.get(), cfg);
    Widget* root = win.root();

    auto header = root->add_child(std::make_unique<HeaderBar>());
    header->set_size_request(0, 44);
    auto brand = header->add_child(std::make_unique<Label>("SleelaUI"));
    brand->set_margin(Margin{0, 0, 0, 12});
    brand->set_align(SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    header->add_child(std::make_unique<Spacer>());
    auto gear = header->add_child(std::make_unique<Button>("Settings"));
    gear->set_margin(Margin{6, 12, 6, 0});

    auto col = root->add_child(std::make_unique<Box>(SLUI_ORIENT_VERTICAL, 12));
    col->set_margin(Margin{20, 20, 20, 20});
    col->set_expand(true, true);

    col->add_child(std::make_unique<Label>("Slick Black controls"));
    col->add_child(std::make_unique<Entry>("Type here"));

    auto row = col->add_child(std::make_unique<Box>(SLUI_ORIENT_HORIZONTAL, 8));
    auto save = row->add_child(std::make_unique<Button>("Save"));
    save->set_suggested(true);
    auto discard = row->add_child(std::make_unique<Button>("Discard"));
    discard->set_destructive(true);
    row->add_child(std::make_unique<Spacer>());

    auto tog = col->add_child(std::make_unique<Toggle>("Dark mode", true));
    (void)tog;
    auto sld = col->add_child(std::make_unique<Slider>(0.0, 100.0, 72.0));
    sld->set_expand(true, false);
    col->add_child(std::make_unique<Separator>(SLUI_ORIENT_HORIZONTAL));
    auto dis = col->add_child(std::make_unique<Button>("Unavailable action"));
    dis->set_sensitive(false);

    const Canvas& c = win.render();

    /* Pack to tight RGB. */
    std::vector<uint8_t> rgb((size_t)c.width() * c.height() * 3);
    for (int i = 0; i < c.width() * c.height(); ++i) {
        uint32_t p = c.pixels()[i];
        rgb[(size_t)i * 3 + 0] = (uint8_t)((p >> 16) & 0xFF);
        rgb[(size_t)i * 3 + 1] = (uint8_t)((p >> 8) & 0xFF);
        rgb[(size_t)i * 3 + 2] = (uint8_t)(p & 0xFF);
    }

    std::string path(out);
    bool is_ppm = path.size() > 4 && path.substr(path.size() - 4) == ".ppm";
    if (is_ppm) {
        FILE* f = std::fopen(out, "wb");
        if (!f) {
            std::fprintf(stderr, "snapshot: cannot open %s\n", out);
            return 1;
        }
        std::fprintf(f, "P6\n%d %d\n255\n", c.width(), c.height());
        std::fwrite(rgb.data(), 1, rgb.size(), f);
        std::fclose(f);
    } else {
        if (!png::write(path, c.width(), c.height(), rgb.data())) {
            std::fprintf(stderr, "snapshot: cannot write %s\n", out);
            return 1;
        }
    }
    std::printf("wrote %s (%dx%d)\n", out, c.width(), c.height());
    return 0;
}
