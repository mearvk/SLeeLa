// SleelaTerminal(TM) shell -- L6 CLI / REPL driver (slsh).
// Original SLeeLa work (see NOTICE).
//
// Usage:
//   slsh                 interactive read-eval-print loop
//   slsh -c "script"     run the script string
//   slsh FILE            run the script file
//
// The driver is thin: it only wires input into the pipeline lex -> parse ->
// execute, printing structured diagnostics on lexer/parser errors.

#include "core.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "executor.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

using namespace sleela::sh;

// Run one script (which may contain many lines/commands). Returns the exit
// status of the last command (or the requested exit code).
int runScript(const std::string& src, Environment& env) {
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

int repl(Environment& env) {
    std::string line;
    std::cout << "SleelaTerminal(TM) slsh -- original SLeeLa shell. Type 'exit' to leave.\n";
    for (;;) {
        std::cout << "slsh$ " << std::flush;
        if (!std::getline(std::cin, line)) { std::cout << "\n"; break; }
        if (line.empty()) continue;
        runScript(line, env);
        if (env.shouldExit()) return env.exitCode();
    }
    return env.lastStatus();
}

} // namespace

int main(int argc, char** argv) {
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
