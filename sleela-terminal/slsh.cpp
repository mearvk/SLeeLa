// SleelaTerminal(TM) shell -- L6 CLI / REPL driver (slsh).
// Original SLeeLa work (see NOTICE).
//
// Usage:
//   slsh                 interactive read-eval-print loop
//   slsh -c "script"     run the script string
//   slsh FILE            run the script file
//
// The driver wires input through the M5 orchestration layer and then the
// established lex -> parse -> execute pipeline.

#include "core.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "executor.hpp"
#include "m5.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <csignal>

namespace {

using namespace sleela::sh;

// Ctrl+X is the SleelaTerminal sharp-quit key. The PTY is configured so Ctrl+X
// is delivered as SIGQUIT to the foreground process group. slsh itself catches
// SIGQUIT and survives; a foreground child inherits the handler but sees a
// different PID and immediately receives SIGKILL. This returns control to the
// slsh prompt without disposing of the terminal window.
pid_t g_shell_pid = 0;

void sharpQuitSignalHandler(int) {
    if (g_shell_pid != 0 && ::getpid() != g_shell_pid) {
        ::kill(::getpid(), SIGKILL);
    }
}

void configureSharpQuitKey() {
    struct termios tio;
    if (::tcgetattr(STDIN_FILENO, &tio) != 0) return;
    tio.c_cc[VQUIT] = 0x18; // Ctrl+X
    ::tcsetattr(STDIN_FILENO, TCSANOW, &tio);
}

bool runPromptCommand(const std::string& line, Environment& env) {
    std::istringstream in(line);
    std::string command;
    std::string subcommand;
    std::string original;
    if (!(in >> command >> subcommand >> original) ||
        command != "prompt" || subcommand != "set") {
        return false;
    }

    std::string replacement;
    std::getline(in, replacement);
    if (!replacement.empty() && replacement.front() == ' ') replacement.erase(0, 1);

    if (replacement.empty()) {
        std::cerr << "prompt: usage: prompt set originalprompt newprompt\n";
        env.setLastStatus(2);
        return true;
    }

    if (original != env.prompt()) {
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
    std::string line;
    std::cout << "SleelaTerminal(TM) slsh -- original SLeeLa shell. Type 'exit' to leave.\n";
    for (;;) {
        std::cout << env.prompt() << std::flush;
        if (!std::getline(std::cin, line)) { std::cout << "\n"; break; }
        if (line.empty()) continue;

        runScript(line, env);
        if (env.shouldExit()) return env.exitCode();
    }
    return env.lastStatus();
}

} // namespace

int main(int argc, char** argv) {
    g_shell_pid = ::getpid();
    ::signal(SIGQUIT, sharpQuitSignalHandler);
    configureSharpQuitKey();

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
