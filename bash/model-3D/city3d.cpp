// Phraign(TM) City 3D -- command-line driver.
//
// Usage:
//   city3d generate --config <file> [--out <model.city>] [--ppm <image.ppm>]
//   city3d render   --model  <file> [--config <file>] [--ppm <image.ppm>]
//   city3d save     --model  <file> --config <file>   [--message <msg>]
//
// generate : build a city from the config (per-user seed) and optionally write
//            the model file and/or a PPM preview of the Phraign frame.
// render   : load a saved model and render it (optionally with a view config).
// save     : print the save plan for the configured target (github/server/local).
//
// The renderer draws onto a Phraign native pixel frame; --ppm dumps that exact
// per-pixel frame to a portable image so it can be viewed anywhere.

#include "city_model.hpp"
#include "city_renderer.hpp"
#include "cityscape_model.hpp"
#include "pixel_terminal.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

using namespace sleela::city;

std::string readFile(const std::string& path, bool& ok) {
    std::ifstream f(path, std::ios::binary);
    if (!f) { ok = false; return {}; }
    std::ostringstream ss;
    ss << f.rdbuf();
    ok = true;
    return ss.str();
}

bool writeFile(const std::string& path, const std::string& data) {
    std::ofstream f(path, std::ios::binary);
    if (!f) return false;
    f << data;
    return static_cast<bool>(f);
}

// Dump the Phraign frame (0xRRGGBBAA words) as a binary PPM (P6) image.
bool writePPM(const std::string& path,
              const sleela::terminal::PixelTerminal& term) {
    const auto w = term.pixelSize().width;
    const auto h = term.pixelSize().height;
    const auto& frame = term.frame();
    if (frame.size() < w * h) return false;

    std::ofstream f(path, std::ios::binary);
    if (!f) return false;
    f << "P6\n" << w << " " << h << "\n255\n";
    for (std::size_t i = 0; i < w * h; ++i) {
        const std::uint32_t v = frame[i].value;
        const char rgb[3] = {
            static_cast<char>((v >> 24) & 0xFF),
            static_cast<char>((v >> 16) & 0xFF),
            static_cast<char>((v >> 8) & 0xFF),
        };
        f.write(rgb, 3);
    }
    return static_cast<bool>(f);
}

std::string argValue(int argc, char** argv, const std::string& key) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (key == argv[i]) return argv[i + 1];
    }
    return {};
}

bool hasArg(int argc, char** argv, const std::string& key) {
    for (int i = 1; i < argc; ++i) if (key == argv[i]) return true;
    return false;
}

int usage() {
    std::cerr <<
        "usage:\n"
        "  city3d generate --config <file> [--out <model.city>] [--ppm <img.ppm>]\n"
        "  city3d render   --model  <file> [--config <file>] [--ppm <img.ppm>]\n"
        "  city3d save     --model  <file> --config <file> [--message <msg>]\n";
    return 2;
}

Config loadConfig(const std::string& path) {
    Config cfg;
    if (path.empty()) return cfg;
    bool ok = false;
    const std::string text = readFile(path, ok);
    if (!ok) {
        std::cerr << "warning: cannot read config '" << path
                  << "', using defaults\n";
        return cfg;
    }
    std::string err;
    cfg = Config::fromText(text, &err);
    if (!err.empty()) std::cerr << "config note: " << err << "\n";
    return cfg;
}

int doGenerate(int argc, char** argv) {
    const Config cfg = loadConfig(argValue(argc, argv, "--config"));
    const std::uint64_t seed = cfg.seed ? cfg.seed : seedForUser(cfg.user);

    City city(cfg.grid_cols, cfg.grid_rows);
    city.generate(seed, cfg.params);

    // The general cityscape graph: radix/diameter/randomness, composed with the
    // Year/IQ/Legislature drivers, from the center of centricity.
    CityscapeModel model;
    model.build(seed, cfg.radix(), cfg.diameter(), cfg.randomness(), cfg.params,
                cfg.grid_cols, cfg.grid_rows);

    std::cout << "generated city: " << cfg.grid_cols << "x" << cfg.grid_rows
              << " = " << city.blockCount() << " blocks; user='" << cfg.user
              << "' seed=" << seed << " theme=" << themeName(cfg.theme) << "\n";
    std::cout << "  year=" << cfg.params.year
              << " modernity=" << cfg.params.modernity()
              << "  iq=" << cfg.params.iq
              << " designQuality=" << cfg.params.designQuality()
              << "  legislature=" << legislatureName(cfg.params.legislature)
              << "\n  finality=" << city.cityFinality()
              << " (city quality of condition, 0..1)\n";
    std::cout << "  cityscape graph: radix=" << model.radix()
              << " diameter=" << model.diameter()
              << " randomness=" << model.randomness()
              << " nodes=" << model.nodes().size()
              << " edges=" << model.edgeCount()
              << " cylinderPairs=" << model.cylinders().size()
              << "\n  moralSymmetry=" << model.moralSymmetry()
              << " (spheres graced by the cylinders; positive orientation)\n";

    const std::string out = argValue(argc, argv, "--out");
    if (!out.empty()) {
        if (writeFile(out, city.serialize(cfg.user, seed)))
            std::cout << "wrote model: " << out << "\n";
        else { std::cerr << "error: cannot write model '" << out << "'\n"; return 1; }
    }

    const std::string ppm = argValue(argc, argv, "--ppm");
    if (!ppm.empty()) {
        sleela::terminal::PixelTerminal term(
            sleela::terminal::Size{cfg.frame_width, cfg.frame_height});
        Renderer renderer(cfg.renderOptions());
        const std::size_t drawn = renderer.render(city, model, term);
        if (writePPM(ppm, term))
            std::cout << "wrote preview: " << ppm << " (" << drawn
                      << " pixels drawn)\n";
        else { std::cerr << "error: cannot write ppm '" << ppm << "'\n"; return 1; }
    }
    return 0;
}

int doRender(int argc, char** argv) {
    const std::string model = argValue(argc, argv, "--model");
    if (model.empty()) return usage();
    bool ok = false;
    const std::string text = readFile(model, ok);
    if (!ok) { std::cerr << "error: cannot read model '" << model << "'\n"; return 1; }

    City city;
    std::string user;
    std::uint64_t seed = 0;
    if (!City::deserialize(text, city, &user, &seed)) {
        std::cerr << "error: '" << model << "' is not a valid PHRAIGN-CITY model\n";
        return 1;
    }

    Config cfg = loadConfig(argValue(argc, argv, "--config"));
    std::cout << "loaded model: " << city.cols() << "x" << city.rows()
              << " = " << city.blockCount() << " blocks; user='" << user
              << "' seed=" << seed << " theme=" << themeName(cfg.theme) << "\n";
    std::cout << "  year=" << city.params().year
              << " iq=" << city.params().iq
              << " legislature=" << legislatureName(city.params().legislature)
              << " finality=" << city.cityFinality()
              << " (city quality of condition, 0..1)\n";

    // Rebuild the general cityscape graph from the model's params + the config
    // dimensions (radix/diameter/randomness), so the render includes the
    // cylinder pairs and the spheres they grace.
    // radix/diameter/randomness come from the loaded model's params (or config
    // overrides if the model predates them / config is supplied).
    CityscapeModel gmodel;
    gmodel.build(seed, city.params().radix, city.params().diameter,
                 city.params().randomness, city.params(),
                 city.cols(), city.rows());

    sleela::terminal::PixelTerminal term(
        sleela::terminal::Size{cfg.frame_width, cfg.frame_height});
    Renderer renderer(cfg.renderOptions());
    const std::size_t drawn = renderer.render(city, gmodel, term);
    std::cout << "rendered " << drawn << " pixels into a "
              << term.pixelSize().width << "x" << term.pixelSize().height
              << " Phraign frame (graph nodes=" << gmodel.nodes().size()
              << " edges=" << gmodel.edgeCount() << ")\n";

    const std::string ppm = argValue(argc, argv, "--ppm");
    if (!ppm.empty()) {
        if (writePPM(ppm, term)) std::cout << "wrote preview: " << ppm << "\n";
        else { std::cerr << "error: cannot write ppm '" << ppm << "'\n"; return 1; }
    }
    return 0;
}

int doSave(int argc, char** argv) {
    const std::string model = argValue(argc, argv, "--model");
    const Config cfg = loadConfig(argValue(argc, argv, "--config"));
    if (model.empty()) return usage();

    const std::string msg = hasArg(argc, argv, "--message")
        ? argValue(argc, argv, "--message")
        : "Save Phraign city model";

    std::cout << "save target: " << cfg.save_target << "\n";
    if (cfg.save_target == "github") {
        const std::string slug = cfg.save_location.empty()
            ? "<owner>/<repo>" : cfg.save_location;
        std::cout << "plan: commit '" << model << "' to GitHub repo " << slug
                  << " with message: \"" << msg << "\"\n"
                  << "  git add " << model << " && git commit -m \"" << msg
                  << "\" && git push\n";
    } else if (cfg.save_target == "server") {
        const std::string url = cfg.save_location.empty()
            ? "<public-server-url>" : cfg.save_location;
        std::cout << "plan: upload '" << model << "' to public server " << url
                  << "\n  (e.g. curl -T " << model << " " << url << ")\n";
    } else {
        const std::string dest = cfg.save_location.empty()
            ? model : cfg.save_location;
        std::cout << "plan: keep '" << model << "' locally at " << dest << "\n";
    }
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) return usage();
    const std::string cmd = argv[1];
    if (cmd == "generate") return doGenerate(argc, argv);
    if (cmd == "render")   return doRender(argc, argv);
    if (cmd == "save")     return doSave(argc, argv);
    return usage();
}
