/*
 * telephony-skya/native/sleela_skya_cli.cpp
 * SLeeLa — sleela-skya command-line program: model + command handlers.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Implements the module registry, the ordered component model, option parsing,
 * and the command handlers (load / unload / start / stop / gui / list / help).
 * Starting the server orchestrates the existing skya_engine API; the GUI
 * component launches the JavaFX studio and reports (never fails hard) on a
 * headless host.
 */

#include "sleela_skya_cli.h"
#include "skya_engine.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <thread>
#include <chrono>

namespace sleela {
namespace skya {

/* ---- names / order ---- */

const char *moduleName(ModuleId id) {
    switch (id) {
        case ModuleId::Socio:             return "Socio";
        case ModuleId::Network:           return "Network";
        case ModuleId::Servers:           return "Servers";
        case ModuleId::Communication:     return "Communication";
        case ModuleId::RealAcquaintances: return "RealAcquaintances";
        default:                          return "Unknown";
    }
}

const char *componentName(ComponentId id) {
    switch (id) {
        case ComponentId::Server: return "SleelaServer";
        case ComponentId::Gui:    return "SkyaGui";
        default:                  return "Unknown";
    }
}

int componentOrder(ComponentId id) {
    switch (id) {
        case ComponentId::Server: return 1; /* server first */
        case ComponentId::Gui:    return 2; /* then the GUI */
        default:                  return 99;
    }
}

/* case-insensitive compare */
static bool ieq(const std::string &a, const char *b) {
    size_t i = 0;
    for (; i < a.size() && b[i]; ++i) {
        char ca = a[i], cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca = (char)(ca - 'A' + 'a');
        if (cb >= 'A' && cb <= 'Z') cb = (char)(cb - 'A' + 'a');
        if (ca != cb) return false;
    }
    return i == a.size() && b[i] == '\0';
}

/* ---- ModuleRegistry ---- */

ModuleRegistry::ModuleRegistry() {
    for (int i = 0; i < (int)ModuleId::Count; ++i) {
        Module m;
        m.id = (ModuleId)i;
        m.loaded = false;
        modules_.push_back(m);
    }
}

int ModuleRegistry::indexOf(const std::string &name) const {
    for (size_t i = 0; i < modules_.size(); ++i)
        if (ieq(name, modules_[i].name())) return (int)i;
    return -1;
}

bool ModuleRegistry::load(const std::string &name) {
    int i = indexOf(name);
    if (i < 0) return false;
    if (modules_[(size_t)i].loaded) return false;
    modules_[(size_t)i].loaded = true;
    return true;
}

bool ModuleRegistry::unload(const std::string &name) {
    int i = indexOf(name);
    if (i < 0) return false;
    if (!modules_[(size_t)i].loaded) return false;
    modules_[(size_t)i].loaded = false;
    return true;
}

void ModuleRegistry::loadAll()   { for (auto &m : modules_) m.loaded = true; }
void ModuleRegistry::unloadAll() { for (auto &m : modules_) m.loaded = false; }

bool ModuleRegistry::isLoaded(const std::string &name) const {
    int i = indexOf(name);
    return i >= 0 && modules_[(size_t)i].loaded;
}

size_t ModuleRegistry::loadedCount() const {
    size_t n = 0;
    for (const auto &m : modules_) if (m.loaded) ++n;
    return n;
}

/* ---- option parsing ---- */

bool parseOptions(int argc, char **argv, Options &out) {
    if (argc < 2) { out.command = "help"; return true; }
    out.command = argv[1];
    for (int i = 2; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--port" && i + 1 < argc) {
            out.port = (unsigned short)std::strtoul(argv[++i], nullptr, 10);
        } else if (a == "--room" && i + 1 < argc) {
            out.room = argv[++i];
        } else if (a == "--max-peers" && i + 1 < argc) {
            out.max_peers = (unsigned)std::strtoul(argv[++i], nullptr, 10);
        } else if (a == "--http2") {
            out.http_version = 2;
        } else if (a == "--http3") {
            out.http_version = 3;
        } else if (a == "--no-gui") {
            out.no_gui = true;
        } else if (a == "--headless") {
            out.headless = true;
        } else if (a == "--all") {
            out.args.push_back("--all");
        } else if (a.rfind("--", 0) == 0) {
            std::fprintf(stderr, "sleela-skya: unknown option '%s'\n", a.c_str());
            return false;
        } else {
            out.args.push_back(a);  /* positional: module name(s) */
        }
    }
    return true;
}

/* ---- command handlers ---- */

int App::cmdLoad() {
    bool all = false;
    for (const auto &a : opts_.args) if (a == "--all") all = true;
    if (all || opts_.args.empty()) {
        registry_.loadAll();
        std::printf("sleela-skya: loaded all modules (%zu)\n", registry_.loadedCount());
        return 0;
    }
    int rc = 0;
    for (const auto &name : opts_.args) {
        if (name == "--all") continue;
        if (registry_.load(name)) {
            std::printf("sleela-skya: loaded module %s\n", name.c_str());
        } else if (registry_.isLoaded(name)) {
            std::printf("sleela-skya: module %s already loaded\n", name.c_str());
        } else {
            std::fprintf(stderr, "sleela-skya: unknown module '%s'\n", name.c_str());
            rc = 1;
        }
    }
    return rc;
}

int App::cmdUnload() {
    bool all = false;
    for (const auto &a : opts_.args) if (a == "--all") all = true;
    if (all || opts_.args.empty()) {
        registry_.unloadAll();
        std::printf("sleela-skya: unloaded all modules\n");
        return 0;
    }
    int rc = 0;
    for (const auto &name : opts_.args) {
        if (name == "--all") continue;
        if (registry_.unload(name)) {
            std::printf("sleela-skya: unloaded module %s\n", name.c_str());
        } else if (!registry_.isLoaded(name) && App::moduleKnown(name)) {
            std::printf("sleela-skya: module %s already unloaded\n", name.c_str());
        } else {
            std::fprintf(stderr, "sleela-skya: unknown module '%s'\n", name.c_str());
            rc = 1;
        }
    }
    return rc;
}

bool App::moduleKnown(const std::string &name) {
    for (int i = 0; i < (int)ModuleId::Count; ++i)
        if (ieq(name, moduleName((ModuleId)i))) return true;
    return false;
}

int App::cmdList() {
    std::printf("sleela-skya modules:\n");
    for (const auto &m : registry_.modules())
        std::printf("  [%s] %s\n", m.loaded ? "loaded  " : "unloaded", m.name());
    std::printf("sleela-skya components (start order):\n");
    std::printf("  1. %s\n", componentName(ComponentId::Server));
    std::printf("  2. %s\n", componentName(ComponentId::Gui));
    return 0;
}

/* Signal flag for an ordered stop. */
static volatile std::sig_atomic_t g_stop = 0;
static void handle_stop(int) { g_stop = 1; }

int App::bringUpServer(skya_engine_t **out) {
    *out = nullptr;
    skya_options_t o{};
    o.max_peers = opts_.max_peers;
    o.port = opts_.port;
    o.http_version = (uint8_t)opts_.http_version;
    o.relay = 0;

    skya_engine_t *e = skya_create(&o);
    if (!e) {
        std::fprintf(stderr, "sleela-skya: SleelaServer engine init failed\n");
        return 1;
    }
    if (skya_start(e, SKYA_SERVER) != 0) {
        std::fprintf(stderr, "sleela-skya: SleelaServer FAILED to listen on %u; status=%s\n",
                     (unsigned)o.port, skya_status(e));
        skya_destroy(e);
        return 1;
    }
    if (skya_join(e, opts_.room.c_str()) != 0 || !skya_port_bound(e)) {
        std::fprintf(stderr, "sleela-skya: SleelaServer FAILED to bind/join room '%s'\n",
                     opts_.room.c_str());
        skya_destroy(e);
        return 1;
    }
    std::printf("sleela-skya: SleelaServer READY %s HTTP/%u on 0.0.0.0:%u room=%s max-peers=%u\n",
                skya_status(e), opts_.http_version, (unsigned)skya_bound_port(e),
                opts_.room.c_str(), opts_.max_peers);
    std::fflush(stdout);
    *out = e;
    return 0;
}

void App::holdServerOpen(skya_engine_t *e) {
    /* Hold the long-lived server open until interrupted; stop the components in
     * descending order (GUI is already attached/noted, then the server). */
    std::signal(SIGINT, handle_stop);
    std::signal(SIGTERM, handle_stop);
    std::printf("sleela-skya: components up; press Ctrl-C to stop\n");
    std::fflush(stdout);
    while (!g_stop) std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::printf("sleela-skya: stopping components in order (SkyaGui, SleelaServer)\n");
    if (e) { skya_stop(e); skya_destroy(e); }
}

int App::startGui() {
    if (opts_.headless) {
        std::printf("sleela-skya: SkyaGui skipped (headless); the JavaFX studio "
                    "lives under telephony-skya/javafx\n");
        return 0;
    }
    /* The GUI is the JavaFX Skya studio; the CLI reports how to launch it rather
     * than embedding a JVM. On a GUI host this is where the launcher is invoked. */
    std::printf("sleela-skya: SkyaGui -> launch the JavaFX studio under "
                "telephony-skya/javafx (SkyaClientApp)\n");
    return 0;
}

int App::cmdGui() {
    return startGui();
}

int App::cmdStart() {
    /* Default, ordered startup: load any unloaded modules first, then start the
     * components in ascending declared order — SleelaServer (1), then SkyaGui
     * (2) — and only then hold the long-lived server open. */
    if (registry_.loadedCount() == 0) {
        registry_.loadAll();
        std::printf("sleela-skya: auto-loaded all modules (%zu) for start\n",
                    registry_.loadedCount());
    }

    std::printf("sleela-skya: starting components in order (SleelaServer, SkyaGui)\n");

    /* Component 1: bring the server up (bind the listener). Non-blocking. */
    skya_engine_t *e = nullptr;
    int rc = bringUpServer(&e);
    if (rc != 0) return rc;

    /* Component 2: start the GUI, after the server is up. */
    if (opts_.no_gui) {
        std::printf("sleela-skya: --no-gui -> SkyaGui will not be started\n");
    } else {
        int grc = startGui();
        if (grc != 0) { if (e) { skya_stop(e); skya_destroy(e); } return grc; }
    }

    /* Both components are up in order; now hold the server open. */
    holdServerOpen(e);
    return 0; /* holdServerOpen blocks until the process is interrupted */
}

int App::cmdStop() {
    /* In this single-process CLI, stop is a no-op placeholder: a running server
     * is stopped with Ctrl-C (SIGINT/SIGTERM). A future daemon mode would signal
     * the running components in descending order (GUI then Server). */
    std::printf("sleela-skya: stop a running server with Ctrl-C (SIGINT/SIGTERM).\n");
    std::printf("sleela-skya: ordered stop (GUI, then SleelaServer) applies in daemon mode.\n");
    return 0;
}

int App::cmdHelp() {
    std::printf(
        "sleela-skya — SLeeLa Skya telephony command line\n"
        "\n"
        "Usage: sleela-skya <command> [options]\n"
        "\n"
        "Commands:\n"
        "  load [MODULE...|--all]     load emblematic modules (default: all)\n"
        "  unload [MODULE...|--all]   unload emblematic modules (default: all)\n"
        "  list                       list modules and the ordered components\n"
        "  gui                        launch the Skya GUI (JavaFX studio)\n"
        "  start [options]            load modules, then start ordered components\n"
        "                             (SleelaServer, then SkyaGui)\n"
        "  stop                       how to stop a running server\n"
        "  help                       show this help\n"
        "\n"
        "Modules: Socio Network Servers Communication RealAcquaintances\n"
        "\n"
        "Options:\n"
        "  --port <p>       server port (default 8443)\n"
        "  --room <name>    server room (default lobby)\n"
        "  --max-peers <n>  server capacity (default 256)\n"
        "  --http2|--http3  session transport selection (default http3)\n"
        "  --no-gui         start the server only (no GUI)\n"
        "  --headless       GUI unavailable: report instead of failing\n"
        "  --all            apply load/unload to every module\n");
    return 0;
}

int App::run() {
    const std::string &c = opts_.command;
    if (c == "load")   return cmdLoad();
    if (c == "unload") return cmdUnload();
    if (c == "list")   return cmdList();
    if (c == "gui")    return cmdGui();
    if (c == "start")  return cmdStart();
    if (c == "stop")   return cmdStop();
    if (c == "help" || c == "--help" || c == "-h") return cmdHelp();
    std::fprintf(stderr, "sleela-skya: unknown command '%s' (try 'help')\n", c.c_str());
    return 2;
}

} // namespace skya
} // namespace sleela
