// ===========================================================================
// nordshrift.cpp  --  The `nordshrift` transpiler driver (NS-SST-0001).
//
// Nordshrift reads a .sst control sheet and drives the triplet transpilation
// of the Sleela sources the sheet names, into the target language the sheet
// selects (java | sleela | c).
//
//   nordshrift check <sheet.sst>     lex+parse+validate the sheet; print diagnostics
//   nordshrift build <sheet.sst>     resolve source: files, run the pipeline,
//                                     emit target source (per target-language);
//                                     the sleela target additionally runs on the core
//   nordshrift version
//
// The .sst file is the control surface (Part I-XIII). The Sleela source files
// it points at are the program; those are transpiled through the shared Sleela
// front end and emitted / executed here.
// ===========================================================================
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "sst_lexer.h"
#include "sst_parser.h"
#include "sheet_model.h"
#include "diagnostics.h"
#include "source_resolve.h"
#include "sleela_emit.h"

// Shared Sleela front end + core for parsing/executing the source programs.
#include "../frontend/lexer.h"
#include "../frontend/parser.h"
#include "../frontend/compiler.h"
extern "C" {
#include "../core/sleela_core.h"
}

using namespace nordshrift;

static const char* kVersion =
    "Nordshrift 1.0 (NS-SST-0001 driver; triplet: java | sleela | c)";

static int usage() {
    std::cerr <<
        "Usage:\n"
        "  nordshrift check <sheet.sst>   validate a sheet; print diagnostics\n"
        "  nordshrift build <sheet.sst>   transpile the sheet's sources to its target\n"
        "  nordshrift version\n";
    return 2;
}

static bool readFile(const std::string& path, std::string& out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    std::ostringstream ss; ss << f.rdbuf(); out = ss.str();
    return true;
}

static std::string dirOf(const std::string& path) {
    size_t s = path.find_last_of('/');
    return s == std::string::npos ? "." : path.substr(0, s);
}
static std::string langName(TargetLang l) {
    return l == TargetLang::Java ? "java" : l == TargetLang::Sleela ? "sleela" : "c";
}

// Load + parse + (lightly) validate a sheet. Returns false if it could not be
// read; diagnostics carry every lexical/parse/semantic issue.
static bool loadSheet(const std::string& path, Sheet& sheet, DiagnosticBag& diags) {
    std::string src;
    if (!readFile(path, src)) { std::cerr << "nordshrift: cannot open '" << path << "'\n"; return false; }
    auto toks = lex(src, path, diags);
    sheet = parseSheet(toks, path, diags);
    return true;
}

// `check`: validate only, print all diagnostics + a summary.
static int doCheck(const std::string& path) {
    Sheet sheet; DiagnosticBag diags;
    if (!loadSheet(path, sheet, diags)) return 1;
    std::cout << diags.render();
    std::cout << "sheet '" << sheet.meta.name << "' — "
              << diags.errorCount() << " error(s), "
              << diags.warningCount() << " warning(s)\n";
    if (!diags.hasErrors())
        std::cout << "OK: target-language = " << langName(sheet.target.language) << "\n";
    return diags.hasErrors() ? 1 : 0;
}

// Run one emitted Sleela program on the C core (the sleela-target arm).
static int runSleelaOnCore(const std::string& sleelaSrc) {
    SLVM* vm = slvm_new();
    int rc = 0;
    try {
        sleela::Lexer lx(sleelaSrc);
        sleela::Parser ps(lx.tokenize());
        sleela::Program prog = ps.parseProgram();
        sleela::compile(prog, vm);
        if (slvm_run(vm) == SLR_ERROR) {
            const char* e = slvm_error(vm);
            std::cerr << "nordshrift: sleela runtime error: " << (e ? e : "unknown") << "\n";
            rc = 1;
        }
    } catch (const std::exception& ex) {
        std::cerr << "nordshrift: while running emitted Sleela: " << ex.what() << "\n";
        rc = 1;
    }
    slvm_free(vm);
    return rc;
}

// `build`: resolve sources, transpile each into the sheet's target language.
static int doBuild(const std::string& path) {
    Sheet sheet; DiagnosticBag diags;
    if (!loadSheet(path, sheet, diags)) return 1;

    // Resolve the source file set (§V) unless the sheet omitted a source block.
    std::string sheetDir = dirOf(path);
    std::vector<std::string> sources;
    if (sheet.source.present)
        sources = resolveSources(sheet.source, sheetDir, diags);

    // Report diagnostics; halt on errors (NSS-E are fatal at end of phase).
    std::cout << diags.render();
    if (diags.hasErrors()) {
        std::cerr << "nordshrift: build halted — " << diags.errorCount() << " error(s)\n";
        return 1;
    }

    TargetLang lang = sheet.target.language;
    std::cout << "nordshrift: building sheet '" << sheet.meta.name
              << "' -> " << langName(lang)
              << " (" << sources.size() << " source file(s))\n";

    int rc = 0;
    for (const auto& srcPath : sources) {
        std::string code;
        if (!readFile(srcPath, code)) {
            std::cerr << "nordshrift: cannot read source '" << srcPath << "'\n";
            rc = 1; continue;
        }
        sleela::Program prog;
        try {
            sleela::Lexer lx(code);
            sleela::Parser ps(lx.tokenize());
            prog = ps.parseProgram();
        } catch (const std::exception& ex) {
            // Phase 1-7 source error, reported against the Sleela source file.
            std::cerr << "NSS-E (error): " << srcPath << ": " << ex.what() << "\n";
            rc = 1; continue;
        }

        std::string emitted = emitProgram(prog, lang, sheet.target.packageRoot);
        std::cout << "\n// ==== " << srcPath << "  ->  " << langName(lang) << " ====\n";
        std::cout << emitted;

        // The sleela target is executable: run it on the C core to prove it out.
        if (lang == TargetLang::Sleela) {
            std::cout << "// ---- executing on the Sleela core ----\n";
            if (runSleelaOnCore(emitted) != 0) rc = 1;
        }
    }
    return rc;
}

int main(int argc, char** argv) {
    if (argc < 2) return usage();
    std::string cmd = argv[1];
    if (cmd == "version" || cmd == "--version" || cmd == "-v") { std::cout << kVersion << "\n"; return 0; }
    if (cmd == "check") { if (argc < 3) return usage(); return doCheck(argv[2]); }
    if (cmd == "build") { if (argc < 3) return usage(); return doBuild(argv[2]); }
    return usage();
}
