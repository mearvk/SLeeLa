// ===========================================================================
// driver.cpp  --  The `sleela` command-line front end.
//
//   sleela run <file.sleela>     lex -> parse -> compile -> run on the C core
//   sleela version
//
// A .sleela file is a Wrapper(TM): the .sleela file type -- a Sleela source
// file that carries the metadocument addend (governed by SL-META-0001).
//
// This ties the whole pipeline together: source text is lowered to Sleela
// Core bytecode by the compiler, loaded into an SLVM, and executed through
// the core's exchange/run API.
// ===========================================================================
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "../catalog/sheet_catalog.h"
#include "../xclass/xclass_loader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

extern "C" {
#include "../core/sleela_core.h"
}

static const char* kVersion =
    "Sleela 0.1.2 (C/C++ core; SHEET.sheet conducted methods; .xclass input)";

// True if `path` ends with the given (lower-case) extension.
static bool hasExt(const std::string& path, const std::string& ext) {
    if (path.size() < ext.size()) return false;
    return path.compare(path.size() - ext.size(), ext.size(), ext) == 0;
}

// Compile a fully-built Program and run it on the core.
static int compileAndRun(sleela::Program& prog, const catalog::Catalog& cat) {
    SLVM* vm = slvm_new();
    int rc = 0;
    try {
        sleela::compile(prog, vm, &cat);      // lowers AST -> core bytecode (sheet-aware)
        SLResult r = slvm_run(vm);            // execute through the core
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
        "  sleela run <file.sleela>              compile and run a Sleela program\n"
        "  sleela run <file.xclass> [more...]    ingest SecureJDK 28 .xclass file(s)\n"
        "                                        into a Sleela program and run it\n"
        "  sleela xclass [--run|--emit|--info] <file.xclass> [more...]\n"
        "                                        --run  (default) ingest + run\n"
        "                                        --emit  print reconstructed Sleela source\n"
        "                                        --info  print identity/security/provenance\n"
        "  sleela version                        print version\n";
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

// Ingest one or more SecureJDK 28 .xclass files, reconstruct a Program, run it.
static int runXclass(const std::vector<std::string>& paths) {
    catalog::Catalog cat = loadCatalog();
    try {
        sleela::xclass::Loaded loaded = sleela::xclass::loadFiles(paths);
        std::cout << "[xclass] ingested " << loaded.metas.size()
                  << " SecureJDK 28 class(es):\n";
        for (const auto& m : loaded.metas)
            std::cout << "[xclass]   " << sleela::xclass::infoLine(m) << "\n";
        return compileAndRun(loaded.program, cat);
    } catch (const std::exception& ex) {
        std::cerr << "sleela: " << ex.what() << "\n";
        return 1;
    }
}

static int runFile(const std::string& path) {
    // SecureJDK 28 .xclass input: reconstruct a Program, then run it.
    if (hasExt(path, ".xclass")) return runXclass({path});

    std::string src;
    if (!readFile(path, src)) {
        std::cerr << "sleela: cannot open '" << path << "'\n";
        return 1;
    }

    catalog::Catalog cat = loadCatalog();
    try {
        sleela::Lexer lexer(src);
        auto tokens = lexer.tokenize();

        sleela::Parser parser(std::move(tokens));
        sleela::Program prog = parser.parseProgram();

        return compileAndRun(prog, cat);
    } catch (const std::exception& ex) {
        std::cerr << "sleela: " << ex.what() << "\n";
        return 1;
    }
}

// `sleela xclass [--emit|--info|--run] <file.xclass> [more.xclass ...]`
static int xclassCmd(int argc, char** argv) {
    enum { RUN, EMIT, INFO } mode = RUN;
    std::vector<std::string> files;
    for (int i = 2; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--emit") mode = EMIT;
        else if (a == "--info") mode = INFO;
        else if (a == "--run") mode = RUN;
        else if (a.rfind("--", 0) == 0) { std::cerr << "sleela: unknown option " << a << "\n"; return 2; }
        else files.push_back(a);
    }
    if (files.empty()) { std::cerr << "sleela xclass: no .xclass files given\n"; return 2; }

    if (mode == RUN) return runXclass(files);

    try {
        sleela::xclass::Loaded loaded = sleela::xclass::loadFiles(files);
        if (mode == EMIT) {
            std::cout << loaded.emitted;
        } else { // INFO
            for (const auto& m : loaded.metas) {
                std::cout << sleela::xclass::infoLine(m) << "\n";
                if (!m.sourceFile.empty()) std::cout << "  source    : " << m.sourceFile << "\n";
                if (!m.edition.empty())    std::cout << "  edition   : " << m.edition << "\n";
                if (!m.signatureHex.empty())
                    std::cout << "  signature : " << m.signatureAlg << ":" << m.signatureHex
                              << (m.signed_ ? " (signed)" : "") << "\n";
            }
        }
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "sleela: " << ex.what() << "\n";
        return 1;
    }
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
        // `sleela run a.xclass b.xclass ...` ingests several into one Program.
        if (hasExt(argv[2], ".xclass")) {
            std::vector<std::string> files;
            for (int i = 2; i < argc; i++) files.push_back(argv[i]);
            return runXclass(files);
        }
        return runFile(argv[2]);
    }
    if (cmd == "xclass") {
        if (argc < 3) return usage();
        return xclassCmd(argc, argv);
    }
    // Convenience: `sleela file.sleela` (a Wrapper(TM)) / `sleela file.xclass`
    // behave like `run`.
    if (hasExt(cmd, ".sleela") || hasExt(cmd, ".xclass")) {
        return runFile(cmd);
    }
    return usage();
}
