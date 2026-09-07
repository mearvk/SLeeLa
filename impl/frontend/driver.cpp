// ===========================================================================
// driver.cpp  --  The `sleela` command-line front end.
//
//   sleela run <file.sleela>     lex -> parse -> compile -> run on the C core
//   sleela version
//
// This ties the whole pipeline together: source text is lowered to Sleela
// Core bytecode by the compiler, loaded into an SLVM, and executed through
// the core's exchange/run API.
// ===========================================================================
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "../catalog/sheet_catalog.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

extern "C" {
#include "../core/sleela_core.h"
}

static const char* kVersion = "Sleela 0.1.1 (C/C++ core; SHEET.sheet conducted methods)";

// Locate SHEET.sheet: honor $SLEELA_SHEET, else probe common relative paths up
// from the working dir / build tree. Returns an empty catalog if not found
// (the conducted-method built-ins then resolve against nothing but still work).
static catalog::Catalog loadCatalog() {
    const char* env = std::getenv("SLEELA_SHEET");
    const char* candidates[] = {
        env,
        "SHEET.sheet",
        "../SHEET.sheet",
        "../../SHEET.sheet",
        "../../../SHEET.sheet",
    };
    for (const char* p : candidates) {
        if (!p || !*p) continue;
        bool ok = false;
        catalog::Catalog c = catalog::parseCatalogFile(p, &ok);
        if (ok) return c;
    }
    return catalog::Catalog{};
}

static int usage() {
    std::cerr <<
        "Usage:\n"
        "  sleela run <file.sleela>   compile and run a Sleela program\n"
        "  sleela version             print version\n";
    return 2;
}

static bool readFile(const std::string& path, std::string& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    out = ss.str();
    return true;
}

static int runFile(const std::string& path) {
    std::string src;
    if (!readFile(path, src)) {
        std::cerr << "sleela: cannot open '" << path << "'\n";
        return 1;
    }

    catalog::Catalog cat = loadCatalog();

    SLVM* vm = slvm_new();
    int rc = 0;
    try {
        sleela::Lexer lexer(src);
        auto tokens = lexer.tokenize();

        sleela::Parser parser(std::move(tokens));
        sleela::Program prog = parser.parseProgram();

        sleela::compile(prog, vm, &cat);  // lowers AST -> core bytecode (sheet-aware)

        SLResult r = slvm_run(vm);        // execute through the core
        if (r == SLR_ERROR) {
            const char* e = slvm_error(vm);
            std::cerr << "sleela: runtime error: " << (e ? e : "unknown") << "\n";
            rc = 1;
        }
    } catch (const std::exception& ex) {
        std::cerr << "sleela: " << ex.what() << "\n";
        rc = 1;
    }
    slvm_free(vm);
    return rc;
}

int main(int argc, char** argv) {
    if (argc < 2) return usage();
    std::string cmd = argv[1];

    if (cmd == "version" || cmd == "--version" || cmd == "-v") {
        std::cout << kVersion << "\n";
        return 0;
    }
    if (cmd == "run") {
        if (argc < 3) return usage();
        return runFile(argv[2]);
    }
    // Convenience: `sleela file.sleela` behaves like `sleela run file.sleela`.
    if (cmd.size() > 7 && cmd.substr(cmd.size() - 7) == ".sleela") {
        return runFile(cmd);
    }
    return usage();
}
