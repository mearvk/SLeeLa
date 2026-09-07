// ===========================================================================
// nordshrift.cpp  --  The `nordshrift` command-line front end.
//
//   nordshrift emit --target=java   file.sst   print Java source
//   nordshrift emit --target=sleela file.sst   print Sleela source
//   nordshrift emit --target=c      file.sst   print C source
//   nordshrift run                  file.sst   emit Sleela and run on the core
//   nordshrift version
//
// `run` closes the loop end-to-end: a .sst Style Sheet is lowered to Sleela
// source, which is compiled to Sleela Core bytecode and executed in-process by
// the C/C++ core -- the "runs into Sleela" arm of the triple manifold.
// ===========================================================================
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "sst_lexer.h"
#include "sst_parser.h"
#include "sst_sema.h"
#include "emitters.h"

// Sleela front end + core, for the `run` path.
#include "../frontend/lexer.h"
#include "../frontend/parser.h"
#include "../frontend/compiler.h"
extern "C" {
#include "../core/sleela_core.h"
}

static const char* kVersion = "Nordshrift 0.1.0 (triple manifold: Java / Sleela / C)";

static int usage() {
    std::cerr <<
        "Usage:\n"
        "  nordshrift emit --target=java|sleela|c <file.sst>   emit target source\n"
        "  nordshrift run <file.sst>                            emit Sleela and run on the core\n"
        "  nordshrift version\n";
    return 2;
}

static bool readFile(const std::string& path, std::string& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    std::ostringstream ss; ss << f.rdbuf(); out = ss.str();
    return true;
}

// Parse + analyze a .sst file into a Sheet.
static nordshrift::Sheet loadSheet(const std::string& src) {
    nordshrift::Lexer lexer(src);
    nordshrift::Parser parser(lexer.tokenize());
    nordshrift::Sheet sheet = parser.parseSheet();
    nordshrift::analyze(sheet);
    return sheet;
}

// Run a piece of Sleela source through the Sleela front end + C core.
static int runSleelaSource(const std::string& sleelaSrc) {
    SLVM* vm = slvm_new();
    int rc = 0;
    try {
        sleela::Lexer lx(sleelaSrc);
        sleela::Parser ps(lx.tokenize());
        sleela::Program prog = ps.parseProgram();
        sleela::compile(prog, vm);
        SLResult r = slvm_run(vm);
        if (r == SLR_ERROR) {
            const char* e = slvm_error(vm);
            std::cerr << "nordshrift: Sleela runtime error: " << (e ? e : "unknown") << "\n";
            rc = 1;
        }
    } catch (const std::exception& ex) {
        std::cerr << "nordshrift: while running emitted Sleela: " << ex.what() << "\n";
        rc = 1;
    }
    slvm_free(vm);
    return rc;
}

static int doEmit(const std::string& target, const std::string& path) {
    std::string src;
    if (!readFile(path, src)) { std::cerr << "nordshrift: cannot open '" << path << "'\n"; return 1; }
    try {
        nordshrift::Sheet sheet = loadSheet(src);
        if (target == "java")        std::cout << nordshrift::emitJava(sheet);
        else if (target == "sleela") std::cout << nordshrift::emitSleela(sheet);
        else if (target == "c")      std::cout << nordshrift::emitC(sheet);
        else { std::cerr << "nordshrift: unknown target '" << target << "' (use java|sleela|c)\n"; return 2; }
    } catch (const std::exception& ex) {
        std::cerr << "nordshrift: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

static int doRun(const std::string& path) {
    std::string src;
    if (!readFile(path, src)) { std::cerr << "nordshrift: cannot open '" << path << "'\n"; return 1; }
    std::string sleelaSrc;
    try {
        nordshrift::Sheet sheet = loadSheet(src);
        sleelaSrc = nordshrift::emitSleela(sheet);
    } catch (const std::exception& ex) {
        std::cerr << "nordshrift: " << ex.what() << "\n";
        return 1;
    }
    return runSleelaSource(sleelaSrc);
}

int main(int argc, char** argv) {
    if (argc < 2) return usage();
    std::string cmd = argv[1];

    if (cmd == "version" || cmd == "--version" || cmd == "-v") {
        std::cout << kVersion << "\n";
        return 0;
    }
    if (cmd == "emit") {
        std::string target, file;
        for (int i = 2; i < argc; i++) {
            std::string a = argv[i];
            if (a.rfind("--target=", 0) == 0) target = a.substr(9);
            else if (a == "--target" && i + 1 < argc) target = argv[++i];
            else file = a;
        }
        if (target.empty() || file.empty()) return usage();
        return doEmit(target, file);
    }
    if (cmd == "run") {
        if (argc < 3) return usage();
        return doRun(argv[2]);
    }
    return usage();
}
