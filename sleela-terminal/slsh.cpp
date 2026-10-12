// SleelaTerminal(TM) shell -- L6 CLI / REPL driver (slsh).
// Original SLeeLa work (see NOTICE).
//
// Usage:
//   slsh                 interactive read-eval-print loop
//   slsh -c "script"     run the script string
//   slsh FILE            run the script file

#include "core.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "executor.hpp"
#include "m5.hpp"

#include <algorithm>
#include <cerrno>
#include <csignal>
#include <ctime>
#include <chrono>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace {

using namespace sleela::sh;

pid_t g_shell_pid = 0;
volatile sig_atomic_t g_interrupted = 0;

void sharpQuitSignalHandler(int) {
    if (g_shell_pid != 0 && ::getpid() != g_shell_pid) {
        ::kill(::getpid(), SIGKILL);
    }
}

void interruptSignalHandler(int) {
    g_interrupted = 1;
}

void configureTerminalKeys() {
    struct termios tio;
    if (::tcgetattr(STDIN_FILENO, &tio) != 0) return;
    tio.c_cc[VQUIT] = 0x18; // Ctrl+X -> SIGQUIT
    ::tcsetattr(STDIN_FILENO, TCSANOW, &tio);
}

class RawMode {
public:
    RawMode() {
        if (::tcgetattr(STDIN_FILENO, &saved_) != 0) return;
        termios raw = saved_;
        raw.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
        raw.c_iflag &= static_cast<tcflag_t>(~(IXON | ICRNL));
        raw.c_oflag |= OPOST;
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        active_ = (::tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0);
    }

    ~RawMode() { restore(); }

    void restore() {
        if (active_) {
            ::tcsetattr(STDIN_FILENO, TCSANOW, &saved_);
            active_ = false;
        }
    }

    bool active() const { return active_; }

private:
    termios saved_{};
    bool active_ = false;
};

void writeBytes(const std::string& s) {
    std::size_t off = 0;
    while (off < s.size()) {
        const ssize_t n = ::write(STDOUT_FILENO, s.data() + off, s.size() - off);
        if (n <= 0) return;
        off += static_cast<std::size_t>(n);
    }
}

std::string colourPrompt(const std::string& prompt) {
    // Colour the conventional username@computername: prompt without changing
    // the plain prompt stored by Environment or reported by the prompt command.
    const std::size_t at = prompt.find('@');
    const std::size_t colon = prompt.find(':', at == std::string::npos ? 0 : at + 1);
    if (at == std::string::npos || colon == std::string::npos ||
        colon + 1 >= prompt.size() || prompt[colon + 1] != ' ') {
        return prompt;
    }
    return "\033[1;32m" + prompt.substr(0, at) + "\033[0m@" +
           "\033[1;34m" + prompt.substr(at + 1, colon - at - 1) +
           "\033[0m:\033[0m ";
}

void redrawLine(const std::string& prompt, const std::string& line, std::size_t cursor) {
    writeBytes("\r\x1b[2K" + prompt + line + "\x1b[K");
    const std::size_t right = line.size() - cursor;
    if (right > 0) writeBytes("\x1b[" + std::to_string(right) + "D");
}

bool readByte(char& c) {
    for (;;) {
        const ssize_t n = ::read(STDIN_FILENO, &c, 1);
        if (n == 1) return true;
        if (n < 0 && errno == EINTR) continue;
        return false;
    }
}

// Read one input line. `eof` is set to true when the input stream is closed
// (non-tty EOF, Ctrl+D on an empty line, or a read failure) so the caller can
// terminate the REPL instead of spinning on repeated empty results. A Ctrl+C
// cancel is NOT end-of-input: it returns an empty line with eof left false.
// Tab completion for the interactive line editor. Command-position completion
// combines shell builtins with executable files found on PATH; other positions
// complete filesystem entries, including directory traversal.
struct CompletionCandidate {
    std::string text;
    bool is_directory = false;
};

bool startsWith(const std::string& value, const std::string& prefix) {
    return value.size() >= prefix.size() && value.compare(0, prefix.size(), prefix) == 0;
}

std::string escapeCompletion(const std::string& value) {
    std::string escaped;
    for (char c : value) {
        if (c == ' ' || c == '\t' || c == '\\' || c == '$' || c == '&' ||
            c == '(' || c == ')' || c == '[' || c == ']' || c == '{' ||
            c == '}' || c == ';' || c == '|' || c == '<' || c == '>' ||
            c == '*' || c == '?' || c == '\'' || c == '"') escaped.push_back('\\');
        escaped.push_back(c);
    }
    return escaped;
}

std::vector<CompletionCandidate> completionCandidates(const std::string& line,
                                                       std::size_t cursor,
                                                       std::size_t token_start) {
    const std::string token = line.substr(token_start, cursor - token_start);
    const std::string before_token = line.substr(0, token_start);
    const std::size_t separator = before_token.find_last_of(";|&\n");
    const std::string command_prefix = separator == std::string::npos
        ? before_token : before_token.substr(separator + 1);
    const bool command_position =
        command_prefix.find_first_not_of(" \t") == std::string::npos &&
        token.find('/') == std::string::npos;
    std::vector<std::string> names;
    if (command_position) {
        static const std::vector<std::string> builtins = {
            ":", "[", "bg", "break", "cd", "continue", "echo", "eval", "exec",
            "exit", "export", "false", "fg", "getopts", "help", "jobs", "pwd",
            "read", "return", "set", "shift", "test", "times", "trap", "true",
            "type", "umask", "unalias", "unset", "wait", "footerbar", "prompt",
            "soros", "select"
        };
        for (const auto& name : builtins)
            if (startsWith(name, token)) names.push_back(name);

        const char* path_env = std::getenv("PATH");
        std::istringstream paths(path_env ? path_env : "");
        std::string path_entry;
        while (std::getline(paths, path_entry, ':')) {
            if (path_entry.empty()) path_entry = ".";
            std::error_code ec;
            for (std::filesystem::directory_iterator it(path_entry, ec), end;
                 !ec && it != end; it.increment(ec)) {
                const std::string name = it->path().filename().string();
                if (!startsWith(name, token) || name.empty() || name[0] == '.') continue;
                std::error_code type_ec;
                if (::access(it->path().c_str(), X_OK) == 0 &&
                    !std::filesystem::is_directory(it->path(), type_ec)) names.push_back(name);
            }
        }
    } else {
        const std::size_t slash = token.find_last_of('/');
        const std::string directory_part = slash == std::string::npos ? "" : token.substr(0, slash + 1);
        const std::string base = slash == std::string::npos ? token : token.substr(slash + 1);
        const std::filesystem::path directory = directory_part.empty()
            ? std::filesystem::path(".") : std::filesystem::path(directory_part);
        std::error_code ec;
        for (std::filesystem::directory_iterator it(directory, ec), end;
             !ec && it != end; it.increment(ec)) {
            const std::string name = it->path().filename().string();
            if (!startsWith(name, base)) continue;
            if ((base.empty() || base[0] != '.') && !name.empty() && name[0] == '.') continue;
            std::error_code type_ec;
            const bool is_dir = std::filesystem::is_directory(it->path(), type_ec);
            names.push_back(directory_part + name + (is_dir ? "/" : ""));
        }
    }
    std::sort(names.begin(), names.end());
    names.erase(std::unique(names.begin(), names.end()), names.end());
    std::vector<CompletionCandidate> found;
    for (const auto& name : names) {
        std::error_code ec;
        found.push_back({name, std::filesystem::is_directory(std::filesystem::path(name), ec)});
    }
    return found;
}

void applyTabCompletion(std::string& line, std::size_t& cursor, const std::string& prompt) {
    std::size_t token_start = cursor;
    while (token_start > 0) {
        const char c = line[token_start - 1];
        if (c == ' ' || c == '\t' || c == ';' || c == '|' || c == '&' ||
            c == '(' || c == ')' || c == '<' || c == '>') break;
        --token_start;
    }
    const std::string prefix = line.substr(token_start, cursor - token_start);
    if (prefix.empty()) return;
    const auto matches = completionCandidates(line, cursor, token_start);
    if (matches.empty()) return;

    std::string replacement;
    std::string suffix;
    if (matches.size() == 1) {
        replacement = matches.front().text;
        if (matches.front().is_directory) {
            if (replacement.empty() || replacement.back() != '/') suffix = "/";
        } else suffix = " ";
    } else {
        replacement = matches.front().text;
        for (std::size_t i = 1; i < matches.size(); ++i) {
            std::size_t n = 0;
            while (n < replacement.size() && n < matches[i].text.size() &&
                   replacement[n] == matches[i].text[n]) ++n;
            replacement.resize(n);
        }
        if (replacement.size() <= prefix.size()) {
            writeBytes("\r\n");
            for (std::size_t i = 0; i < matches.size(); ++i) {
                writeBytes(matches[i].text);
                writeBytes((i + 1) % 4 == 0 || i + 1 == matches.size() ? "\r\n" : "\t");
            }
            redrawLine(prompt, line, cursor);
            return;
        }
    }
    replacement = escapeCompletion(replacement) + suffix;
    line.replace(token_start, cursor - token_start, replacement);
    cursor = token_start + replacement.size();
    redrawLine(prompt, line, cursor);
}

std::string readInteractiveLine(const std::string& prompt,
                                std::vector<std::string>& history,
                                bool& eof) {
    eof = false;
    RawMode raw;
    if (!raw.active()) {
        std::string line;
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin, line)) { eof = true; return {}; }
        return line;
    }

    std::string line;
    std::size_t cursor = 0;
    std::size_t history_index = history.size();
    bool have_history_buffer = false;
    std::string saved_line;

    // Paint the complete prompt through the same positioning path used for edits.
    // With an empty input line, cursor == line.size() == 0, so the insertion
    // point is deterministically just after the prompt's trailing colon/space.
    redrawLine(prompt, line, cursor);
    for (;;) {
        char c = 0;
        if (!readByte(c)) { eof = true; return {}; }

        if (c == '\t') { // Tab: command or filesystem completion.
            applyTabCompletion(line, cursor, prompt);
            continue;
        }

        if (c == '\n' || c == '\r') {
            writeBytes("\r\n");
            return line;
        }

        if (c == 0x03) { // Ctrl+C: cancel the current input line (not EOF).
            writeBytes("^C\r\n");
            return {};
        }

        if (c == 0x04) { // Ctrl+D: exit on an empty input line.
            if (line.empty()) {
                writeBytes("\r\n");
                eof = true;
                return {};
            }
            continue;
        }

        if (c == 0x01) { // Home / Ctrl+A
            cursor = 0;
            redrawLine(prompt, line, cursor);
            continue;
        }

        if (c == 0x05) { // End / Ctrl+E
            cursor = line.size();
            redrawLine(prompt, line, cursor);
            continue;
        }

        if (c == 0x7f || c == 0x08) { // Backspace
            if (cursor > 0) {
                line.erase(cursor - 1, 1);
                --cursor;
                redrawLine(prompt, line, cursor);
            }
            continue;
        }

        if (c == 0x0c) { // Ctrl+L
            writeBytes("\x1b[2J\x1b[H");
            redrawLine(prompt, line, cursor);
            continue;
        }

        if (c == '\x1b') {
            char a = 0, b = 0;
            if (!readByte(a)) { eof = true; return {}; }
            if (a != '[' && a != 'O') continue;
            if (!readByte(b)) { eof = true; return {}; }

            if (b == 'A') { // Up: history previous
                if (!history.empty() && history_index > 0) {
                    if (!have_history_buffer) {
                        saved_line = line;
                        have_history_buffer = true;
                    }
                    --history_index;
                    line = history[history_index];
                    cursor = line.size();
                    redrawLine(prompt, line, cursor);
                }
                continue;
            }
            if (b == 'B') { // Down: history next
                if (have_history_buffer) {
                    if (history_index + 1 < history.size()) {
                        ++history_index;
                        line = history[history_index];
                    } else {
                        history_index = history.size();
                        line = saved_line;
                        have_history_buffer = false;
                    }
                    cursor = line.size();
                    redrawLine(prompt, line, cursor);
                }
                continue;
            }
            if (b == 'C') { // Right
                if (cursor < line.size()) ++cursor;
                redrawLine(prompt, line, cursor);
                continue;
            }
            if (b == 'D') { // Left
                if (cursor > 0) --cursor;
                redrawLine(prompt, line, cursor);
                continue;
            }
            if (b == 'H') { // Home
                cursor = 0;
                redrawLine(prompt, line, cursor);
                continue;
            }
            if (b == 'F') { // End
                cursor = line.size();
                redrawLine(prompt, line, cursor);
                continue;
            }

            if (b == '3') { // Delete: ESC [ 3 ~
                char tilde = 0;
                if (readByte(tilde) && tilde == '~' && cursor < line.size()) {
                    line.erase(cursor, 1);
                    redrawLine(prompt, line, cursor);
                }
                continue;
            }
            continue;
        }

        if (static_cast<unsigned char>(c) >= 0x20) {
            line.insert(cursor, 1, c);
            ++cursor;
            redrawLine(prompt, line, cursor);
        }
    }
}

std::string trimTrailingWhitespace(std::string value) {
    while (!value.empty() && (value.back() == ' ' || value.back() == '\t')) value.pop_back();
    return value;
}

bool runPromptCommand(const std::string& line, Environment& env) {
    if (line == "prompt") {
        // Print exactly what the REPL uses, including the trailing space.
        std::cout << "[prompt] " << env.prompt() << "\n";
        std::cout.flush();
        env.setLastStatus(0);
        return true;
    }

    const std::string prefix = "prompt set ";
    if (line.compare(0, prefix.size(), prefix) != 0) return false;

    const std::string args = line.substr(prefix.size());
    const std::size_t split = args.find_first_of(" \t");
    if (split == std::string::npos) {
        std::cerr << "prompt: usage: prompt set originalprompt newprompt\n";
        env.setLastStatus(2);
        return true;
    }

    const std::string original = args.substr(0, split);
    std::size_t new_start = split;
    while (new_start < args.size() && (args[new_start] == ' ' || args[new_start] == '\t')) ++new_start;
    const std::string replacement = args.substr(new_start);

    if (replacement.empty()) {
        std::cerr << "prompt: usage: prompt set originalprompt newprompt\n";
        env.setLastStatus(2);
        return true;
    }

    if (trimTrailingWhitespace(env.prompt()) != trimTrailingWhitespace(original)) {
        std::cerr << "prompt: current prompt does not match '" << original << "'\n";
        env.setLastStatus(1);
        return true;
    }

    env.setPrompt(replacement);
    env.setLastStatus(0);
    return true;
}


const std::vector<std::string>& footerbarTopicCatalog() {
    static const std::vector<std::string> topics = {
        "academia",
        "agriculture",
        "ai-research",
        "anatomy",
        "anthropology",
        "archaeology",
        "astronomy",
        "astrophysics",
        "artificial-intelligence",
        "behavioral-science",
        "biochemistry",
        "biodiversity",
        "bioinformatics",
        "biology",
        "biomedical-engineering",
        "biophysics",
        "biotechnology",
        "botany",
        "business",
        "cancer-research",
        "chemistry",
        "climate-science",
        "cognitive-science",
        "computational-biology",
        "computer-science",
        "conservation",
        "data-science",
        "dentistry",
        "developmental-biology",
        "drug-discovery",
        "ecology",
        "economics",
        "education",
        "electrical-engineering",
        "energy",
        "engineering",
        "environmental-science",
        "epidemiology",
        "ethics",
        "evolution",
        "finance",
        "food-science",
        "forensic-science",
        "genetics",
        "geochemistry",
        "geography",
        "geology",
        "global-health",
        "health-policy",
        "history",
        "immunology",
        "infectious-disease",
        "information-security",
        "materials-science",
        "mathematics",
        "mechanical-engineering",
        "medicine",
        "microbiology",
        "molecular-biology",
        "nanotechnology",
        "neuroscience",
        "nuclear-physics",
        "nutrition",
        "oceanography",
        "oncology",
        "optics",
        "paleontology",
        "pharmacology",
        "pharmacy",
        "philosophy",
        "physics",
        "physiology",
        "plant-science",
        "political-science",
        "psychology",
        "public-health",
        "quantum-computing",
        "quantum-physics",
        "robotics",
        "science-policy",
        "software-engineering",
        "statistics",
        "sustainability",
        "systems-biology",
        "toxicology",
        "veterinary-medicine",
        "virology",
        "volcanology",
        "water-resources",
        "wildlife",
        "anxiety-research",
        "clinical-trials",
        "digital-health",
        "medical-devices",
        "mental-health",
        "pharmacogenomics",
        "regenerative-medicine",
        "stem-cells",
        "surgery",
        "vaccinology"
    };
    return topics;
}

std::filesystem::path footerbarTopicsPath() {
    const char *xdg = std::getenv("XDG_CONFIG_HOME");
    const char *home = std::getenv("HOME");
    std::filesystem::path base = (xdg && *xdg) ? xdg :
        ((home && *home) ? (std::filesystem::path(home) / ".config").string() : ".config");
    return base / "sleela" / "footerbar-topics.conf";
}

std::string lowerTopic(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

bool runFooterbarCommand(const std::string& line, Environment& env) {
    if (line != "footerbar" && line.rfind("footerbar ", 0) != 0) return false;
    std::istringstream input(line);
    std::string command, arg;
    input >> command;
    std::vector<std::string> requested;
    while (input >> arg) requested.push_back(lowerTopic(arg));
    const auto &catalog = footerbarTopicCatalog();
    const auto path = footerbarTopicsPath();
    std::vector<std::string> current;
    {
        std::ifstream in(path);
        while (in >> arg) current.push_back(lowerTopic(arg));
    }
    if (requested.empty() || requested.front() == "status") {
        std::cout << "footerbar topics:";
        if (current.empty()) std::cout << " (none; feed inactive)";
        for (const auto &topic : current) std::cout << " " << topic;
        std::cout << "\nUse: footerbar chemistry pharmacology\n"
                     "Use 'footerbar list' to show the 100 supported topics, or 'footerbar clear' to disable.\n";
        env.setLastStatus(0);
        return true;
    }
    if (requested.size() == 1 && requested[0] == "list") {
        for (std::size_t i = 0; i < catalog.size(); ++i)
            std::cout << catalog[i] << ((i + 1) % 5 == 0 ? "\n" : "\t");
        if (catalog.size() % 5 != 0) std::cout << "\n";
        env.setLastStatus(0);
        return true;
    }
    if (requested.size() == 1 && requested[0] == "clear") {
        std::error_code ec;
        std::filesystem::remove(path, ec);
        std::cout << "footerbar: topic feed disabled\n";
        env.setLastStatus(0);
        return true;
    }
    std::vector<std::string> selected;
    for (const auto &topic : requested) {
        if (std::find(catalog.begin(), catalog.end(), topic) == catalog.end()) {
            std::cerr << "footerbar: unsupported topic '" << topic
                      << "'; use 'footerbar list' to see supported topics\n";
            env.setLastStatus(2);
            return true;
        }
        if (std::find(selected.begin(), selected.end(), topic) == selected.end()) selected.push_back(topic);
    }
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) {
        std::cerr << "footerbar: cannot create config directory: " << ec.message() << "\n";
        env.setLastStatus(1);
        return true;
    }
    std::ofstream out(path, std::ios::trunc);
    if (!out) {
        std::cerr << "footerbar: cannot write " << path << "\n";
        env.setLastStatus(1);
        return true;
    }
    for (const auto &topic : selected) out << topic << "\n";
    std::cout << "footerbar: RSS topic feed enabled for";
    for (const auto &topic : selected) std::cout << " " << topic;
    std::cout << "\n";
    env.setLastStatus(0);
    return true;
}

bool runSorosTimeCommand(const std::string& line, Environment& env) {
    if (line != "soros time") return false;

    constexpr int kSorosBaseYear = 2026;
    constexpr int kSorosBaseTimestamp = 2407;
    constexpr int kGreatMysteryTimestamp = 2607;

    const std::time_t now = std::time(nullptr);
    const std::tm* local = std::localtime(&now);
    if (local == nullptr) {
        std::cerr << "soros time: unable to determine the terminal's local year\n";
        env.setLastStatus(1);
        return true;
    }

    const int civil_year = local->tm_year + 1900;
    const int soros_timestamp = kSorosBaseTimestamp + (civil_year - kSorosBaseYear);

    std::cout << "[soros time] " << soros_timestamp
              << " (civil year " << civil_year << ")\n";
    std::cout << "[soros time] 2026 = 2407; each year after 2026 adds one.\n";
    std::cout << "[soros time] Own National Time: 2407 (2026).\n";
    std::cout << "[soros time] Great Mystery marker: " << kGreatMysteryTimestamp << "+.\n";
    std::cout.flush();
    env.setLastStatus(0);
    return true;
}

int runNormalScript(const std::string& src, Environment& env) {
    std::vector<Token> toks;
    LexError lerr;
    if (!lex(src, toks, lerr)) {
        std::cerr << "slsh: lex error at " << lerr.line << ":" << lerr.col
                  << ": " << lerr.message << "\n";
        return 2;
    }
    ParseError perr;
    NodePtr ast = parse(toks, perr);
    if (!ast) {
        std::cerr << "slsh: parse error at " << perr.line << ":" << perr.col
                  << ": " << perr.message << "\n";
        return 2;
    }
    int status = execute(*ast, env);
    if (env.shouldExit()) return env.exitCode();
    return status;
}

int runScript(const std::string& src, Environment& env) {
    if (runPromptCommand(src, env)) return env.lastStatus();
    if (runFooterbarCommand(src, env)) return env.lastStatus();
    if (runSorosTimeCommand(src, env)) return env.lastStatus();

    int status = 0;
    const M5Runner runner = [](const std::string& nested, Environment& e) {
        return runScript(nested, e);
    };
    if (runM5(src, env, runner, status)) return status;
    return runNormalScript(src, env);
}

int repl(Environment& env) {
    std::vector<std::string> history;
    const std::string startup_banner = "SleelaTerminal(TM) slsh -- original SLeeLa shell. Type 'exit' to leave."; 
    std::cout << startup_banner << '\n' << std::flush;

    // Fade the one-line welcome banner after five seconds without blocking the
    // REPL. Each fade frame repaints only row 1 and saves/restores the live cursor.
    // Once the banner has faded, delete row 1: the terminal shifts the prompt row
    // (including any text already typed) up to row 1 and repaints the screen.
    std::thread([startup_banner]() {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        static const char *const fade_colours[] = {
            "\033[38;5;250m", "\033[38;5;244m",
            "\033[38;5;238m", "\033[38;5;232m"
        };
        for (const char *colour : fade_colours) {
            const std::string frame = std::string("\033[s\033[1;1H\033[2K")
                + colour + startup_banner + "\033[0m\033[u";
            (void)::write(STDOUT_FILENO, frame.data(), frame.size());
            std::this_thread::sleep_for(std::chrono::milliseconds(180));
        }
        // Remove the banner row so the prompt row shifts to the top of the
        // terminal. After deleting row 1, explicitly home the cursor to the
        // beginning of the now-top prompt line; the line editor will position
        // it at the input insertion point on the next redraw.
        const std::string clear_and_promote =
            "\033[s\033[1;1H\033[2K\033[1;1H\033[M\033[1;1H";
        (void)::write(STDOUT_FILENO, clear_and_promote.data(), clear_and_promote.size());
    }).detach();

    for (;;) {
        bool eof = false;
        const std::string line = readInteractiveLine(colourPrompt(env.prompt()), history, eof);
        if (g_interrupted) {
            g_interrupted = 0;
            continue;
        }
        if (eof) return env.exitCode();
        if (line.empty()) continue;

        if (history.empty() || history.back() != line) history.push_back(line);
        runScript(line, env);
        if (env.shouldExit()) return env.exitCode();
    }
}

} // namespace

int main(int argc, char** argv) {
    g_shell_pid = ::getpid();
    ::signal(SIGQUIT, sharpQuitSignalHandler);
    ::signal(SIGINT, interruptSignalHandler);
    configureTerminalKeys();

    sleela::sh::Environment env;

    if (argc >= 3 && std::string(argv[1]) == "-c") {
        std::string script = argv[2];
        for (int i = 3; i < argc; ++i) { script += ' '; script += argv[i]; }
        return runScript(script, env);
    }

    if (argc >= 2) {
        std::ifstream f(argv[1]);
        if (!f) { std::cerr << "slsh: cannot open " << argv[1] << "\n"; return 1; }
        std::ostringstream ss; ss << f.rdbuf();
        return runScript(ss.str(), env);
    }

    // No script argument: if stdin is a terminal, run the interactive REPL;
    // otherwise stdin is a pipe/file, so read it in full and run it as a
    // non-interactive script -- no banner, no prompts -- matching standard
    // shell behaviour (`echo cmd | slsh`).
    if (::isatty(STDIN_FILENO)) {
        return repl(env);
    }

    std::ostringstream ss; ss << std::cin.rdbuf();
    return runScript(ss.str(), env);
}
