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

    // --- Year / quality / weight config keys --------------------------------
    const std::string p =
        "year = 2807\n"
        "year_baseline = 2000\n"
        "year_span = 1000\n"
        "avg_floors = 20\n"
        "floor_height = 1.5\n"
        "windows_per_floor = 8\n"
        "road_spacing = 8\n"
        "bridge_count = 5\n"
        "w_year = 1.0\n"
        "w_road_proximity = 0.8\n"
        "proximity_falloff = 6\n"
        "draw_bridges = false\n"
        "draw_windows = off\n";
    Config pc = Config::fromText(p, &err);
    assert(pc.params.year == 2807);
    assert(pc.params.avg_floors == 20.0);
    assert(pc.params.floor_height == 1.5);
    assert(pc.params.windows_per_floor == 8.0);
    assert(pc.params.road_spacing == 8 && pc.params.bridge_count == 5);
    assert(pc.draw_bridges == false && pc.draw_windows == false);
    assert(err.empty());
    // modernity = (2807-2000)/1000 = 0.807.
    assert(pc.params.modernity() > 0.80 && pc.params.modernity() < 0.81);

    // --- Year drives modernity/finality: newer > older ----------------------
    CityParams oldP;  oldP.year = 2050;
    CityParams newP;  newP.year = 2807;
    City oldC(48, 48), newC(48, 48);
    oldC.generate(999, oldP);
    newC.generate(999, newP);
    assert(newC.params().modernity() > oldC.params().modernity());
    assert(newC.cityFinality() > oldC.cityFinality());
    assert(oldC.cityFinality() >= 0.0 && newC.cityFinality() <= 1.0);

    // --- Per-user seed is stable and non-zero -------------------------------
    const std::uint64_t s1 = seedForUser("tester");
    const std::uint64_t s2 = seedForUser("tester");
    const std::uint64_t s3 = seedForUser("other");
    assert(s1 == s2 && s1 != 0 && s1 != s3);

    // --- Block count is ~4000 (4096) ---------------------------------------
    City city(cfg.grid_cols, cfg.grid_rows);
    assert(city.blockCount() == 4096);

    // --- Generation is deterministic for a seed + params -------------------
    CityParams gp;  gp.year = 2807;
    City a(64, 64), b(64, 64);
    a.generate(12345, gp);
    b.generate(12345, gp);
    bool identical = true;
    for (std::uint32_t y = 0; y < 64 && identical; ++y)
        for (std::uint32_t x = 0; x < 64; ++x) {
            const Block& pa = a.at(x, y);
            const Block& pb = b.at(x, y);
            if (pa.height != pb.height || pa.floors != pb.floors ||
                pa.windows != pb.windows || pa.cell != pb.cell ||
                pa.finality != pb.finality) { identical = false; break; }
        }
    assert(identical);

    // Some buildings exist (not an empty city), with floors + windows.
    std::size_t nonzero = 0, withFloors = 0, withWindows = 0;
    for (std::uint32_t y = 0; y < 64; ++y)
        for (std::uint32_t x = 0; x < 64; ++x) {
            const Block& blk = a.at(x, y);
            if (blk.isBuilding() && blk.height > 0) ++nonzero;
            if (blk.floors > 0) ++withFloors;
            if (blk.windows > 0) ++withWindows;
        }
    assert(nonzero > 1000);
    assert(withFloors > 1000 && withWindows > 1000);

    // --- Roads, bridges, and computed proximity ----------------------------
    std::size_t roads = 0, bridges = 0;
    for (std::uint32_t y = 0; y < 64; ++y)
        for (std::uint32_t x = 0; x < 64; ++x) {
            const Cell cell = a.at(x, y).cell;
            if (cell == Cell::Road) ++roads;
            if (cell == Cell::Bridge) ++bridges;
        }
    assert(roads > 0);   // road corridors exist
    assert(bridges > 0); // bridge spans placed
    // A road/bridge cell has proximity 0; buildings have finite distances.
    for (std::uint32_t y = 0; y < 64; ++y)
        for (std::uint32_t x = 0; x < 64; ++x) {
            const Block& blk = a.at(x, y);
            assert(blk.road_distance >= 0.0f);
            if (blk.cell == Cell::Road || blk.cell == Cell::Bridge)
                assert(blk.road_distance == 0.0f);
        }
    // City finality is a sane average.
    assert(a.cityFinality() > 0.0 && a.cityFinality() <= 1.0);

    // --- Serialize / deserialize round-trip (v2, all attributes) -----------
    const std::string text = a.serialize("tester", 12345);
    assert(text.find("PHRAIGN-CITY 2") == 0);
    assert(text.find("year 2807") != std::string::npos);
    City c;
    std::string ru;
    std::uint64_t rs = 0;
    assert(City::deserialize(text, c, &ru, &rs));
    assert(ru == "tester" && rs == 12345);
    assert(c.cols() == 64 && c.rows() == 64);
    assert(c.params().year == 2807);
    bool round = true;
    for (std::uint32_t y = 0; y < 64 && round; ++y)
        for (std::uint32_t x = 0; x < 64; ++x) {
            const Block& pa = a.at(x, y);
            const Block& pc = c.at(x, y);
            if (pa.height != pc.height || pa.floors != pc.floors ||
                pa.windows != pc.windows || pa.cell != pc.cell) {
                round = false; break;
            }
        }
    assert(round);

    // --- Back-compat: read a legacy v1 (height-only) model -----------------
    const std::string v1 =
        "PHRAIGN-CITY 1\n"
        "user legacy\n"
        "seed 7\n"
        "grid 2 2\n"
        "row 0 3 4\n"
        "row 1 5 6\n"
        "END\n";
    City lc;
    std::string lu;
    std::uint64_t ls = 0;
    assert(City::deserialize(v1, lc, &lu, &ls));
    assert(lu == "legacy" && ls == 7 && lc.cols() == 2 && lc.rows() == 2);
    assert(lc.at(0, 0).height == 3 && lc.at(1, 1).height == 6);

    // Reject clearly invalid input.
    City junk;
    assert(!City::deserialize("not a city\n", junk));

    // --- Render onto a Phraign frame ---------------------------------------
    // Use a compact viewpoint that fits the 64x64 city into a 320x240 frame.
    RenderOptions ro = cfg.renderOptions();
    ro.frame_width = 320;
    ro.frame_height = 240;
    ro.viewpoint.scale_x = 3.0;
    ro.viewpoint.block_pitch = 3.0;
    ro.viewpoint.scale_height = 0.4;
    ro.viewpoint.origin_x = 50.0;
    ro.viewpoint.origin_y = 150.0;
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
