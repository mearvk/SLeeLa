#ifndef SLEELA_SKYA_CLI_H
#define SLEELA_SKYA_CLI_H
/*
 * telephony-skya/native/sleela_skya_cli.h
 * SLeeLa — sleela-skya command-line program: module + component model.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * The sleela-skya CLI loads/unloads the emblematic Skya modules (the /lib
 * telephony-skya Master Classes) and starts the default, ordered Skya
 * components — the GUI and the Sleela/Skya server — through the existing
 * skya_engine API. The model here is deliberately small and inspectable:
 *
 *   Module    — a named, loadable unit (Socio, Network, Servers, Communication,
 *               RealAcquaintances). Loading is bookkeeping + readiness; it never
 *               opens a socket or a firewall by itself.
 *   Component — a named, startable runtime piece with a declared start order
 *               (GUI, Server). Components start in ascending order and stop in
 *               descending order.
 *   App       — the CLI application: a ModuleRegistry + a ComponentSet + the
 *               command dispatch (load / unload / start / stop / gui / list).
 */

#include <string>
#include <vector>

/* Forward declaration so the header need not include the engine ABI. */
struct skya_engine;
typedef struct skya_engine skya_engine_t;

namespace sleela {
namespace skya {

/* ---- Modules (mirror the /lib/telephony-skya emblematic Master Classes) ---- */

enum class ModuleId {
    Socio = 0,
    Network,
    Servers,
    Communication,
    RealAcquaintances,
    Count
};

const char *moduleName(ModuleId id);

/* A loadable module: name + loaded flag. Loading is readiness bookkeeping only;
 * authoritative networking/firewall lifecycle stays in the native subsystem. */
struct Module {
    ModuleId id;
    bool loaded = false;
    const char *name() const { return moduleName(id); }
};

/* Registry of the five emblematic modules. Load/unload by name or all at once. */
class ModuleRegistry {
public:
    ModuleRegistry();
    bool load(const std::string &name);     /* true if a module changed state */
    bool unload(const std::string &name);
    void loadAll();
    void unloadAll();
    bool isLoaded(const std::string &name) const;
    size_t loadedCount() const;
    const std::vector<Module> &modules() const { return modules_; }
private:
    std::vector<Module> modules_;
    int indexOf(const std::string &name) const; /* -1 when unknown */
};

/* ---- Components (the ordered runtime pieces the CLI can start) ---- */

enum class ComponentId {
    Server = 0,   /* the Sleela/Skya comm server (started first)   */
    Gui    = 1,   /* the GUI front end (started after the server)  */
    Count
};

const char *componentName(ComponentId id);
int componentOrder(ComponentId id);   /* ascending start order */

/* A startable component with a declared start order. */
struct Component {
    ComponentId id;
    bool running = false;
    const char *name() const { return componentName(id); }
    int order() const { return componentOrder(id); }
};

/* ---- Options parsed from argv ---- */

struct Options {
    std::string command;          /* load|unload|start|stop|gui|list|help     */
    std::vector<std::string> args;/* command arguments (e.g. module names)     */
    unsigned short port = 8443;   /* server port                               */
    std::string room = "lobby";   /* server room                               */
    unsigned max_peers = 256;     /* server capacity                           */
    unsigned http_version = 3;    /* 2 or 3                                     */
    bool no_gui = false;          /* `start --no-gui` brings up server only    */
    bool headless = false;        /* GUI unavailable: report, do not fail hard */
};

/* Parse argv into Options. Returns false on a parse error (message on stderr). */
bool parseOptions(int argc, char **argv, Options &out);

/* ---- The application ---- */

class App {
public:
    explicit App(const Options &opts) : opts_(opts) {}

    int run();                    /* dispatch opts_.command; process exit code */

    /* command handlers (also unit-testable) */
    int cmdLoad();
    int cmdUnload();
    int cmdStart();               /* start the default, ordered components     */
    int cmdStop();
    int cmdGui();
    int cmdList();
    static int cmdHelp();

    /* Is `name` one of the known emblematic modules? (case-insensitive) */
    static bool moduleKnown(const std::string &name);

    ModuleRegistry &registry() { return registry_; }

private:
    Options opts_;
    ModuleRegistry registry_;

    /* Bring the server component up (bind the listener) without blocking.
     * On success returns 0 and sets *out to the live engine; the caller then
     * holds it open with holdServerOpen(). Implemented in the .cpp so the
     * engine ABI include stays out of the header. */
    int bringUpServer(skya_engine_t **out);
    /* Hold the long-lived server open until interrupted, then stop/destroy it
     * (ordered stop). Blocks. */
    void holdServerOpen(skya_engine_t *e);
    /* Start the GUI component. In this build the GUI is the JavaFX studio under
     * telephony-skya/javafx; headless hosts report rather than fail hard. */
    int startGui();
};

} // namespace skya
} // namespace sleela
#endif
