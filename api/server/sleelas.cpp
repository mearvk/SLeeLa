// SLeeLa Server Launcher (sleelas)
// Max Rupplin - MEARVK LLC - 2026
//
// Native cross-platform launcher for the SLeeLa Server Edition.
// The executable starts the authoritative server reference:
//   server-edition/moral/2/src/Server.sleela
//
// It intentionally launches the existing SLeeLa engine rather than
// reimplementing the Server.sleela program.

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include "nat_aware.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

static fs::path executable_dir(const char *argv0) {
#if defined(_WIN32)
    char buf[MAX_PATH]; DWORD n = GetModuleFileNameA(nullptr, buf, MAX_PATH);
    if (n > 0 && n < MAX_PATH) return fs::path(buf).parent_path();
#elif defined(__APPLE__)
    char buf[4096]; uint32_t size = sizeof(buf);
    extern int _NSGetExecutablePath(char *, uint32_t *);
    if (_NSGetExecutablePath(buf, &size) == 0) return fs::canonical(fs::path(buf)).parent_path();
#else
    char buf[4096]; ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (n > 0) { buf[n] = 0; return fs::path(buf).parent_path(); }
#endif
    std::error_code ec; return fs::absolute(fs::path(argv0 ? argv0 : "sleelas"), ec).parent_path();
}

static bool regular_file(const fs::path &p) { std::error_code ec; return fs::is_regular_file(p, ec); }

static fs::path locate_root(const fs::path &exe_dir) {
    if (const char *env = std::getenv("SLEELA_ROOT"); env && *env) {
        fs::path p(env); if (regular_file(p / "server-edition/moral/2/src/Server.sleela")) return p;
    }
    std::vector<fs::path> candidates = {exe_dir, exe_dir.parent_path(), exe_dir.parent_path().parent_path(), fs::current_path()};
    for (const auto &p : candidates) if (regular_file(p / "server-edition/moral/2/src/Server.sleela")) return p;
    return {};
}

static fs::path locate_engine(const fs::path &root) {
#if defined(_WIN32)
    const char *name = "sleela.exe";
#else
    const char *name = "sleela";
#endif
    if (const char *env = std::getenv("SLEELA_BIN"); env && *env && regular_file(env)) return fs::absolute(env);
    for (const auto &p : {root / "bin" / name, root / "impl" / "build" / name}) if (regular_file(p)) return p;
    return {};
}

static bool server_is_running(const fs::path &state, const fs::path &lock) {
    std::error_code ec;
    if (!fs::is_directory(lock, ec)) return false;
    // A stale lock must not permanently disable the server. The owner file
    // contains the launcher PID and is checked on platforms where possible.
    const fs::path owner = lock / "owner";
    std::ifstream in(owner); long long pid = 0; in >> pid;
    if (!in || pid <= 0) return false;
#if defined(_WIN32)
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, static_cast<DWORD>(pid));
    if (!h) return false;
    DWORD code = 0; bool alive = GetExitCodeProcess(h, &code) && code == STILL_ACTIVE;
    CloseHandle(h); return alive;
#else
    return kill(static_cast<pid_t>(pid), 0) == 0;
#endif
}

static bool acquire_lock(const fs::path &lock) {
    std::error_code ec;
    if (fs::create_directory(lock, ec)) {
        std::ofstream out(lock / "owner", std::ios::trunc);
#if defined(_WIN32)
        out << static_cast<unsigned long long>(GetCurrentProcessId()) << "\n";
#else
        out << static_cast<long long>(getpid()) << "\n";
#endif
        return static_cast<bool>(out);
    }
    return false;
}

static void help() {
    std::cout << "SLeeLa Server Launcher (sleelas)\n"
              << "Starts the SLeeLa Server Edition reference program.\n\n"
              << "Usage: sleelas [--tick] [--foreground] [--design-activity SCIENCE C R O S R I] [--help]\n\n"
              << "  --tick          add a timestamped self-input before the server pass\n"
              << "  --foreground    show launcher status\n"
              << "  --design-activity SCIENCE C R O S R I  emit the common Design Activity record\n"
              << "  --help          show this help\n";
}

static std::string utc_now() {
    auto now = std::chrono::system_clock::now(); std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    char buf[64]; std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm); return buf;
}

static int run_design_activity(const fs::path &root, const fs::path &engine, const std::vector<std::string> &activity) {
#if defined(_WIN32)
    std::string cmd = "\"" + engine.string() + "\" design-activity";
    for (const auto &arg : activity) cmd += " \"" + arg + "\"";
    STARTUPINFOA si{}; PROCESS_INFORMATION pi{}; si.cb = sizeof(si);
    std::string mutable_cmd = cmd;
    if (!CreateProcessA(nullptr, mutable_cmd.data(), nullptr, nullptr, FALSE, 0,
                        nullptr, root.string().c_str(), &si, &pi)) {
        std::cerr << "sleelas: CreateProcess failed: " << GetLastError() << "\n";
        return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 1; GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
    return static_cast<int>(code);
#else
    pid_t pid = fork();
    if (pid < 0) { std::perror("sleelas: fork"); return 1; }
    if (pid == 0) {
        if (chdir(root.c_str()) != 0) std::exit(126);
        std::vector<std::string> args = {engine.string(), "design-activity"};
        args.insert(args.end(), activity.begin(), activity.end());
        std::vector<char*> av;
        for (auto &arg : args) av.push_back(arg.data());
        av.push_back(nullptr);
        execv(engine.c_str(), av.data());
        std::perror("sleelas: exec"); std::exit(127);
    }
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) { std::perror("sleelas: waitpid"); return 1; }
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
#endif
}

#if defined(_WIN32)
static int run_engine(const fs::path &root, const fs::path &engine, const fs::path &server) {
    std::string cmd = "\"" + engine.string() + "\" run \"" + server.string() + "\"";
    STARTUPINFOA si{}; PROCESS_INFORMATION pi{}; si.cb = sizeof(si); std::string mutable_cmd = cmd;
    if (!CreateProcessA(nullptr, mutable_cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, root.string().c_str(), &si, &pi)) {
        std::cerr << "sleelas: CreateProcess failed: " << GetLastError() << "\n"; return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE); DWORD code = 1; GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread); CloseHandle(pi.hProcess); return static_cast<int>(code);
}
#else
static int run_engine(const fs::path &root, const fs::path &engine, const fs::path &server) {
    pid_t pid = fork(); if (pid < 0) { std::perror("sleelas: fork"); return 1; }
    if (pid == 0) {
        if (chdir(root.c_str()) != 0) std::exit(126);
        std::vector<std::string> args = {engine.string(), "run", server.string()}; std::vector<char*> av;
        for (auto &s : args) av.push_back(s.data()); av.push_back(nullptr); execv(engine.c_str(), av.data());
        std::perror("sleelas: exec"); std::exit(127);
    }
    int status = 0; if (waitpid(pid, &status, 0) < 0) { std::perror("sleelas: waitpid"); return 1; }
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
#endif

int main(int argc, char **argv) {
    bool tick = false, foreground = false, natPlanOnly = false, designActivity = false;
    std::vector<std::string> activityArgs;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--help" || a == "-h") { help(); return 0; }
        if (a == "--tick") { tick = true; continue; }
        if (a == "--foreground" || a == "-f") { foreground = true; continue; }
        if (a == "--nat-plan") { natPlanOnly = true; continue; }
        if (a == "--design-activity") {
            if (i + 7 >= argc) { std::cerr << "sleelas: --design-activity requires science plus six 0-100 scores\\n"; return 2; }
            designActivity = true;
            for (int j = 1; j <= 7; ++j) activityArgs.push_back(argv[i + j]);
            i += 7;
            continue;\n        }
        std::cerr << "sleelas: unknown option '" << a << "' (use --help)\n"; return 2;
    }
    sleela::server::NatConfig natConfig;
    std::string natError;
    if (!sleela::server::natConfigFromEnvironment(natConfig, natError)) {
        std::cerr << "sleelas: invalid NAT configuration: " << natError << "\\n";
        return 2;
    }
    if (designActivity) {
        const fs::path root = locate_root(executable_dir(argv[0]));
        if (root.empty()) { std::cerr << "sleelas: SLeeLa root not found; set SLEELA_ROOT\\n"; return 1; }
        const fs::path engine = locate_engine(root);
        if (engine.empty()) { std::cerr << "sleelas: SLeeLa engine not found; set SLEELA_BIN or build impl/\\n"; return 1; }
        return run_design_activity(root, engine, activityArgs);
    }
    if (natPlanOnly) {
        const auto plan = sleela::server::makeNatPlan(natConfig);
        std::cout << "sleelas: NAT mode = " << sleela::server::natModeName(plan.mode) << "\\n"
                  << "sleelas: plan = " << plan.summary << "\\n"
                  << "sleelas: next step = " << plan.nextStep << "\\n";
        return 0;
    }
    const fs::path root = locate_root(executable_dir(argv[0]));
    if (root.empty()) { std::cerr << "sleelas: SLeeLa root not found; set SLEELA_ROOT\n"; return 1; }
    const char *level_env = std::getenv("SLEELA_SERVER_LEVEL");
    const std::string level = level_env && *level_env ? level_env : "2";
    fs::path server;
    if (level == "2") server = root / "server-edition/moral/2/src/Server.sleela";
    else if (level == "3") server = root / "server-edition/moral/3/src/Server.sleela";
    else { std::cerr << "sleelas: unsupported SLEELA_SERVER_LEVEL '" << level << "' (supported: 2, 3)\n"; return 2; }
    const fs::path engine = locate_engine(root);
    if (engine.empty()) { std::cerr << "sleelas: SLeeLa engine not found; set SLEELA_BIN or build impl/\n"; return 1; }
    const fs::path inbox = root / "server-edition/inbox/requests.txt";
    const fs::path state = root / "server-edition/state";
    const fs::path lock = state / ".sleelas.lock";
    const fs::path log = state / "results.log";
    std::error_code ec; fs::create_directories(inbox.parent_path(), ec); fs::create_directories(state, ec);
    if (ec) { std::cerr << "sleelas: cannot create server state: " << ec.message() << "\n"; return 1; }

    if (server_is_running(state, lock)) {
        std::cout << "sleelas: Server Edition is already running; not starting another instance.\n";
        return 0;
    }
    // Remove a lock whose recorded owner is no longer alive, then acquire it atomically.
    fs::remove_all(lock, ec);
    if (!acquire_lock(lock)) {
        if (server_is_running(state, lock)) {
            std::cout << "sleelas: Server Edition is already running; not starting another instance.\n"; return 0;
        }
        std::cerr << "sleelas: unable to acquire server instance lock\n"; return 3;
    }
    struct Unlock { fs::path p; ~Unlock() { std::error_code e; fs::remove_all(p, e); } } unlock{lock};
    if (!regular_file(inbox)) { std::ofstream out(inbox); out << "hello\n"; }
    if (tick) { std::ofstream out(inbox, std::ios::app); out << "tick " << utc_now() << "\n"; }
    if (!std::getenv("SLEELA_SHA256_MANIFEST")) {
        fs::path manifest = root / "security/important-sha256-manifest.json";
#if defined(_WIN32)
        if (regular_file(manifest)) _putenv_s("SLEELA_SHA256_MANIFEST", manifest.string().c_str());
#else
        if (regular_file(manifest)) setenv("SLEELA_SHA256_MANIFEST", manifest.c_str(), 0);
#endif
    }
    if (foreground) std::cout << "sleelas: starting Server.sleela using " << engine << "\n";
    const int rc = run_engine(root, engine, server);
    if (rc != 0) { std::cerr << "sleelas: server reference exited with status " << rc << "\n"; return rc; }
    std::cout << "sleelas: processed server-edition/moral/2/src/Server.sleela -> " << log << "\n";
    return 0;
}