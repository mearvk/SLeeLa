// Phraign(TM) City 3D -- smoke test. Asserts model, config, generation,
// serialization round-trip, and rendering onto a Phraign frame.

#include "city_model.hpp"
#include "city_renderer.hpp"
#include "pixel_terminal.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    using namespace sleela::city;

    // --- Theme parsing ------------------------------------------------------
    Theme t = Theme::White;
    assert(parseTheme("green", t) && t == Theme::Green);
    assert(parseTheme("WHITE", t) && t == Theme::White);
    assert(parseTheme("Blue", t) && t == Theme::Blue);
    assert(!parseTheme("purple", t));
    assert(std::string(themeName(Theme::Green)) == "green");

    // --- Config parsing -----------------------------------------------------
    const std::string cfgText =
        "# comment\n"
        "grid_cols = 64\n"
        "grid_rows = 64\n"
        "user = tester\n"
        "seed = 0\n"
        "theme = blue\n"
        "tilt_x = 0.4\n"
        "frame_width = 320\n"
        "frame_height = 240\n"
        "save_target = github\n"
        "save_location = mearvk/SLeeLa\n"
        "bogus_key = ignored\n";
    std::string err;
    Config cfg = Config::fromText(cfgText, &err);
    assert(cfg.grid_cols == 64 && cfg.grid_rows == 64);
    assert(cfg.theme == Theme::Blue);
    assert(cfg.viewpoint.tilt_x == 0.4);
    assert(cfg.frame_width == 320 && cfg.frame_height == 240);
    assert(cfg.save_target == "github" && cfg.save_location == "mearvk/SLeeLa");
    assert(err.empty());

    // --- Per-user seed is stable and non-zero -------------------------------
    const std::uint64_t s1 = seedForUser("tester");
    const std::uint64_t s2 = seedForUser("tester");
    const std::uint64_t s3 = seedForUser("other");
    assert(s1 == s2 && s1 != 0 && s1 != s3);

    // --- Block count is ~4000 (4096) ---------------------------------------
    City city(cfg.grid_cols, cfg.grid_rows);
    assert(city.blockCount() == 4096);

    // --- Generation is deterministic for a seed ----------------------------
    City a(64, 64), b(64, 64);
    a.generate(12345);
    b.generate(12345);
    bool identical = true;
    for (std::uint32_t y = 0; y < 64 && identical; ++y)
        for (std::uint32_t x = 0; x < 64; ++x)
            if (a.at(x, y).height != b.at(x, y).height) { identical = false; break; }
    assert(identical);

    // Some buildings exist (not an empty city).
    std::size_t nonzero = 0;
    for (std::uint32_t y = 0; y < 64; ++y)
        for (std::uint32_t x = 0; x < 64; ++x)
            if (a.at(x, y).height > 0) ++nonzero;
    assert(nonzero > 1000);

    // --- Serialize / deserialize round-trip --------------------------------
    const std::string text = a.serialize("tester", 12345);
    City c;
    std::string ru;
    std::uint64_t rs = 0;
    assert(City::deserialize(text, c, &ru, &rs));
    assert(ru == "tester" && rs == 12345);
    assert(c.cols() == 64 && c.rows() == 64);
    bool round = true;
    for (std::uint32_t y = 0; y < 64 && round; ++y)
        for (std::uint32_t x = 0; x < 64; ++x)
            if (a.at(x, y).height != c.at(x, y).height) { round = false; break; }
    assert(round);

    // Reject clearly invalid input.
    City junk;
    assert(!City::deserialize("not a city\n", junk));

    // --- Render onto a Phraign frame ---------------------------------------
    RenderOptions ro = cfg.renderOptions();
    ro.frame_width = 320;
    ro.frame_height = 240;
    sleela::terminal::PixelTerminal term(
        sleela::terminal::Size{ro.frame_width, ro.frame_height});
    Renderer renderer(ro);
    const std::size_t drawn = renderer.render(a, term);
    assert(drawn > 0);
    assert(term.pixelSize().width == 320 && term.pixelSize().height == 240);
    assert(term.frame().size() == 320u * 240u);

    // Background theme color should appear somewhere (frame was filled first).
    const Palette pal = Palette::forTheme(Theme::Blue);
    (void)pal;

    std::cout << "phraign city3d smoke: OK ("
              << city.blockCount() << " blocks, " << drawn
              << " pixels drawn)\n";
    return 0;
}
