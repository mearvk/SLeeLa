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
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <termios.h>
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

std::string readInteractiveLine(const std::string& prompt,
                                std::vector<std::string>& history) {
    RawMode raw;
    if (!raw.active()) {
        std::string line;
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin, line)) return {};
        return line;
    }

    std::string line;
    std::size_t cursor = 0;
    std::size_t history_index = history.size();
    bool have_history_buffer = false;
    std::string saved_line;

    writeBytes(prompt);
    for (;;) {
        char c = 0;
        if (!readByte(c)) return {};

        if (c == '\n' || c == '\r') {
            writeBytes("\r\n");
            return line;
        }

        if (c == 0x03) { // Ctrl+C: cancel the current input line.
            writeBytes("^C\r\n");
            return {};
        }

        if (c == 0x04) { // Ctrl+D: exit on an empty input line.
            if (line.empty()) {
                writeBytes("\r\n");
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
            if (!readByte(a)) return {};
            if (a != '[' && a != 'O') continue;
            if (!readByte(b)) return {};

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

    int status = 0;
    const M5Runner runner = [](const std::string& nested, Environment& e) {
        return runScript(nested, e);
    };
    if (runM5(src, env, runner, status)) return status;
    return runNormalScript(src, env);
}

int repl(Environment& env) {
    std::vector<std::string> history;
    std::cout << "SleelaTerminal(TM) slsh -- original SLeeLa shell. Type 'exit' to leave.\n";

    for (;;) {
        const std::string line = readInteractiveLine(env.prompt(), history);
        if (g_interrupted) {
            g_interrupted = 0;
            continue;
        }
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

    return repl(env);
}
