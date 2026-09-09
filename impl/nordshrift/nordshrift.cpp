// ===========================================================================
// nordshrift.cpp  --  The `nordshrift` transpiler driver (NS-SST-0001).
//
// Nordshrift reads a .sst control sheet and drives the triplet transpilation
// of the Sleela sources the sheet names -- each a Wrapper(TM) (the .sleela
// file type: a Sleela source file carrying the metadocument addend, governed
// by SL-META-0001) -- into the target language the sheet selects
// (java | sleela | c).
//
//   nordshrift check <sheet.sst>     lex+parse+validate the sheet; print diagnostics
//   nordshrift build <sheet.sst>     resolve source: files, run the pipeline,
//                                     emit target source (per target-language);
//                                     the sleela target additionally runs on the core
//   nordshrift version
//
// The .sst file is the control surface (Part I-XIII). The Sleela source files
// (Wrapper(TM) files) it points at are the program; those are transpiled
// through the shared Sleela front end and emitted / executed here.
// ===========================================================================
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cstdlib>

#include "sst_lexer.h"
#include "sst_parser.h"
#include "sheet_model.h"
#include "diagnostics.h"
#include "source_resolve.h"
#include "sleela_emit.h"
#include "object_compat.h"
#include "../catalog/sheet_catalog.h"

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
        "  nordshrift check <sheet.sst>              validate a sheet; print diagnostics\n"
        "  nordshrift build <sheet.sst>              transpile the sheet's sources to its target\n"
        "  nordshrift objects                        list the SHEET.sheet object compatibility list\n"
        "  nordshrift relevance --target=java|sleela|c [Object]\n"
        "                                            show direct/model relevance conversions\n"
        "  nordshrift version\n";
    return 2;
}

// Locate SHEET.sheet the same way the Sleela driver does.
static catalog::Catalog loadCatalog() {
    const char* env = std::getenv("SLEELA_SHEET");
    const char* candidates[] = { env, "SHEET.sheet", "../SHEET.sheet",
                                 "../../SHEET.sheet", "../../../SHEET.sheet" };
    for (const char* p : candidates) {
        if (!p || !*p) continue;
        bool ok = false;
        catalog::Catalog c = catalog::parseCatalogFile(p, &ok);
        if (ok) return c;
    }
    return catalog::Catalog{};
}

static bool parseTargetLang(const std::string& s, TargetLang& out) {
    if (s == "java")   { out = TargetLang::Java;   return true; }
    if (s == "sleela") { out = TargetLang::Sleela; return true; }
    if (s == "c")      { out = TargetLang::C;      return true; }
    return false;
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

// `objects`: print the object compatibility list from SHEET.sheet.
static int doObjects() {
    catalog::Catalog cat = loadCatalog();
    if (cat.objectCount() == 0) {
        std::cerr << "nordshrift: could not load SHEET.sheet (set SLEELA_SHEET)\n";
        return 1;
    }
    std::cout << "Nordshrift object compatibility list — from SHEET.sheet\n";
    std::cout << "system depth = " << cat.depth
              << ", congruent-linear-max = " << cat.congruentLinearMax
              << ", complexity-degree-max = " << cat.complexityDegreeMax << "\n";
    std::cout << cat.objectCount() << " objects across "
              << cat.sections.size() << " sections\n\n";
    for (const auto& s : cat.sections) {
        std::cout << "[" << s.name << "]  role=" << s.role
                  << "  (" << s.objects.size() << ")\n";
        for (const auto& name : s.objects) {
            const catalog::Object* o = cat.find(name);
            std::cout << "  - " << name;
            if (o && !o->children.empty()) {
                std::cout << "  children:";
                for (auto& c : o->children) std::cout << " " << c;
            }
            std::cout << "\n";
        }
    }
    return 0;
}

// `relevance --target=T [Object]`: show how catalog objects convert to the
// target as a direct construct or a modeled shape.
static int doRelevance(int argc, char** argv) {
    TargetLang target = TargetLang::Java;
    bool haveTarget = false;
    std::string only;
    for (int i = 2; i < argc; i++) {
        std::string a = argv[i];
        if (a.rfind("--target=", 0) == 0) { haveTarget = parseTargetLang(a.substr(9), target); }
        else if (a == "--target" && i + 1 < argc) { haveTarget = parseTargetLang(argv[++i], target); }
        else only = a;   // an optional single object name
    }
    if (!haveTarget) { std::cerr << "nordshrift: relevance requires --target=java|sleela|c\n"; return usage(); }

    catalog::Catalog cat = loadCatalog();
    if (cat.objectCount() == 0) {
        std::cerr << "nordshrift: could not load SHEET.sheet (set SLEELA_SHEET)\n";
        return 1;
    }
    const char* tn = target == TargetLang::Java ? "java" : target == TargetLang::Sleela ? "sleela" : "c";

    if (!only.empty()) {
        ObjectRelevance r = relevanceOf(cat, only, target);
        if (r.relevance == Relevance::None) {
            std::cerr << "nordshrift: '" << only << "' is not on the compatibility list\n";
            return 1;
        }
        std::cout << only << " -> " << tn << "\n";
        std::cout << "  relevance : " << relevanceName(r.relevance) << "\n";
        std::cout << "  mapping   : " << r.mapping << "\n";
        std::cout << "  role      : " << r.role << "\n";
        std::cout << "  insight   : " << r.note << "\n";
        return 0;
    }

    // whole-catalog relevance table
    auto list = relevanceList(cat, target);
    int direct = 0, model = 0;
    std::cout << "Relevance conversions for target '" << tn << "' ("
              << list.size() << " objects)\n\n";
    for (const auto& r : list) {
        std::cout << (r.relevance == Relevance::Direct ? "  [direct] " : "  [model ] ")
                  << r.object << "  ->  " << r.mapping << "\n";
        if (r.relevance == Relevance::Direct) direct++; else model++;
    }
    std::cout << "\n" << direct << " direct, " << model << " model (for OS-executable compilation)\n";
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) return usage();
    std::string cmd = argv[1];
    if (cmd == "version" || cmd == "--version" || cmd == "-v") { std::cout << kVersion << "\n"; return 0; }
    if (cmd == "check") { if (argc < 3) return usage(); return doCheck(argv[2]); }
    if (cmd == "build") { if (argc < 3) return usage(); return doBuild(argv[2]); }
    if (cmd == "objects") { return doObjects(); }
    if (cmd == "relevance") { return doRelevance(argc, argv); }
    return usage();
}
