// ===========================================================================
// nordshrift.cpp -- The `nordshrift` SST driver (NS-SST-0001).
//
// Nordshrift reads a .sst control sheet and drives source compilation to the
// selected target. For target-language=sleela, Nordshrift now uses the same
// Sleelvac™ compiler/artifact boundary as the direct compiler and writes a
// persistent runnable .sleela Core artifact. That artifact is loadable by the
// Sleela runtime without a second front-end compilation.
//
// Nordshrift 2.0 also links the common semantic subject model used by the
// Math, Physics, Economics, Chemistry, and Financial libraries.
// ===========================================================================
#include <cstdio>
#include <filesystem>
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
#include "component_manifest.h"
#include "object_compat.h"
#include "subject_model.h"
#include "ledger_emit.h"
#include "../catalog/sheet_catalog.h"
#include "../frontend/lexer.h"
#include "../frontend/parser.h"
#include "../frontend/compiler.h"
#include "../frontend/artifact.h"
#include "../frontend/version.h"
extern "C" {
#include "../core/sleela_core.h"
}

using namespace nordshrift;

static const char* kVersion =
    "Nordshrift 2.0 (NS-SST-0001; semantic subject model; Sleelvac™ runnable .sleela target)";

static int usage() {
    std::cerr <<
        "Usage:\n"
        "  nordshrift check <sheet.sst>              validate a sheet; print diagnostics\n"
        "  nordshrift build <sheet.sst>              compile the sheet's sources to its target\n"
        "  nordshrift objects                        list the SHEET.sheet object compatibility list\n"
        "  nordshrift relevance --target=java|sleela|c [Object]\n"
        "                                            show direct/model relevance conversions\n"
        "  nordshrift version\n";
    return 2;
}

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

static bool loadSheet(const std::string& path, Sheet& sheet, DiagnosticBag& diags) {
    std::string src;
    if (!readFile(path, src)) {
        std::cerr << "nordshrift: cannot open '" << path << "'\n";
        return false;
    }
    auto toks = lex(src, path, diags);
    sheet = parseSheet(toks, path, diags);
    return true;
}

static void reportComponentSeries(const Sheet& sheet) {
    if (sheet.network.present) {
        std::cout << "network: " << sheet.network.objects.size()
                  << " object(s)";
        if (!sheet.network.objects.empty()) {
            std::cout << " [";
            for (size_t k = 0; k < sheet.network.objects.size(); k++)
                std::cout << (k ? ", " : "") << networkObjectName(sheet.network.objects[k]);
            std::cout << "]";
        }
        std::cout << ", " << sheet.network.transports.size() << " transport(s)";
        if (!sheet.network.transports.empty()) {
            std::cout << " [";
            for (size_t k = 0; k < sheet.network.transports.size(); k++)
                std::cout << (k ? ", " : "") << networkTransportName(sheet.network.transports[k]);
            std::cout << "]";
        }
        std::cout << ", address-family=" << networkAddressFamilyName(sheet.network.addressFamily)
                  << ", tls=" << (sheet.network.tls ? "true" : "false") << "\n";
    }
    if (sheet.finance.present) {
        std::cout << "finance: " << sheet.finance.objects.size() << " object(s)";
        if (!sheet.finance.objects.empty()) {
            std::cout << " [";
            for (size_t k = 0; k < sheet.finance.objects.size(); k++)
                std::cout << (k ? ", " : "") << financeObjectName(sheet.finance.objects[k]);
            std::cout << "]";
        }
        std::cout << ", currency="
                  << (sheet.finance.hasCurrency ? sheet.finance.currency : std::string("(unset)"))
                  << ", period=" << financePeriodName(sheet.finance.period)
                  << ", discounting=" << financeDiscountingName(sheet.finance.discounting) << "\n";
    }
    for (const auto& s : sheet.subjects) {
        std::cout << "subject '" << s.identity << "'";
        if (!s.domain.empty()) std::cout << " (domain=" << s.domain << ")";
        std::cout << ": " << s.quantities.size() << " quantity, "
                  << s.assumptions.size() << " assumption, "
                  << s.relations.size() << " relation, "
                  << s.transformations.size() << " transformation, "
                  << s.comparisons.size() << " comparison, "
                  << s.evidence.size() << " evidence, "
                  << s.explanations.size() << " explanation, "
                  << s.workplan.size() << " todo\n";
        // Canonical chain rendering (Subject -> Quantity -> ... -> Validation).
        std::cout << "  chain: Subject";
        if (!s.quantities.empty())       std::cout << " -> Quantity/Unit";
        if (!s.assumptions.empty())      std::cout << " -> Assumption";
        if (!s.relations.empty())        std::cout << " -> Relation/Formula";
        if (!s.transformations.empty())  std::cout << " -> Transformation -> Result";
        if (!s.comparisons.empty())      std::cout << " -> ComparativeNorm";
        if (!s.evidence.empty())         std::cout << " -> Evidence";
        if (!s.explanations.empty())     std::cout << " -> Explanation";
        if (!s.workplan.empty())         std::cout << " -> Validation";
        std::cout << "\n";
    }
}

// Semantic validation of declared subjects (dependency and provenance checks).
// Warnings only: a subject sheet remains valid, but the ideals in
// NORDSHRIFT.md ("provenance before trust", "explicit dependency before hidden
// coupling") are surfaced so a modeled result is not silently trusted.
static void validateSubjects(const Sheet& sheet, DiagnosticBag& diags) {
    using namespace nordshrift::semantic;
    // Set of declared subject identities for dependency resolution.
    std::vector<std::string> known;
    for (const auto& s : sheet.subjects) known.push_back(s.identity);
    auto isKnown = [&](const std::string& id) {
        for (const auto& k : known) if (k == id) return true;
        return isFoundationalDomain(id);   // e.g. "math" is a foundational dep
    };
    for (const auto& s : sheet.subjects) {
        for (const auto& dep : s.dependencies)
            if (!isKnown(dep))
                diags.warning("NSS-W-SUB-010", sheet.file, s.quantities.empty() ? 0 : 0,
                              "subject '" + s.identity + "' depends on undeclared subject '" + dep + "'",
                              "SST-SUB-DEPENDENCY");
        // A modeled/inferred/derived quantity without any supporting relation,
        // assumption, or evidence is a bare claim.
        for (const auto& q : s.quantities) {
            bool needsSupport = q.status == EvidenceStatus::Modeled ||
                                q.status == EvidenceStatus::Inferred ||
                                q.status == EvidenceStatus::Derived;
            bool hasSupport = !s.relations.empty() || !s.assumptions.empty() ||
                              !s.evidence.empty();
            if (needsSupport && !hasSupport)
                diags.warning("NSS-W-SUB-011", sheet.file, 0,
                              "subject '" + s.identity + "' quantity '" + q.identity +
                              "' is " + evidenceStatusName(q.status) +
                              " but has no supporting relation/assumption/evidence",
                              "SST-SUB-PROVENANCE");
        }
    }
}

static int doCheck(const std::string& path) {
    Sheet sheet; DiagnosticBag diags;
    if (!loadSheet(path, sheet, diags)) return 1;
    validateSubjects(sheet, diags);
    std::cout << diags.render();
    std::cout << "sheet '" << sheet.meta.name << "' — "
              << diags.errorCount() << " error(s), "
              << diags.warningCount() << " warning(s)\n";
    if (!diags.hasErrors()) {
        std::cout << "OK: target-language = " << langName(sheet.target.language) << "\n";
        reportComponentSeries(sheet);
    }
    return diags.hasErrors() ? 1 : 0;
}

static std::string runnablePathFor(const std::string& srcPath) {
    std::filesystem::path src(srcPath);
    std::filesystem::path outDir = src.parent_path() / "build";
    std::filesystem::create_directories(outDir);
    return (outDir / (src.stem().string() + ".sleela")).string();
}

static int doBuild(const std::string& path) {
    Sheet sheet; DiagnosticBag diags;
    if (!loadSheet(path, sheet, diags)) return 1;

    std::string sheetDir = dirOf(path);
    std::vector<std::string> sources;
    if (sheet.source.present)
        sources = resolveSources(sheet.source, sheetDir, diags);

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

        sleela::VersionResolution vr = sleela::resolveSyntaxVersion(code);
        if (vr.isError()) {
            std::cerr << "NSS-E (error): " << srcPath << ": " << vr.message << "\n";
            rc = 1; continue;
        }
        if (vr.isWarning())
            std::cerr << "NSS-W (warning): " << srcPath << ": " << vr.message << "\n";

        sleela::Program prog;
        try {
            sleela::Lexer lx(code);
            sleela::Parser ps(lx.tokenize());
            prog = ps.parseProgram();
        } catch (const std::exception& ex) {
            std::cerr << "NSS-E (error): " << srcPath << ": " << ex.what() << "\n";
            rc = 1; continue;
        }

        if (lang == TargetLang::Sleela) {
            const std::string output = runnablePathFor(srcPath);
            try {
                sleela::compileToArtifact(prog, output, nullptr, vr.declared);
                std::cout << "nordshrift: " << srcPath << " -> " << output
                          << " (runnable Sleela Core artifact)\n";
                // Additionally emit a .ledger (QR insignia + SHA-256 chain +
                // ISO-8601 UTC timestamp) next to the artifact, via the shared
                // ledger core (matches the standalone ledger tool).
                if (nordshrift::emitLedgerFor(output)) {
                    std::cout << "nordshrift: " << output << " -> " << output
                              << ".ledger (+ " << output << ".qr.svg)\n";
                }
            } catch (const std::exception& ex) {
                std::cerr << "NSS-E (error): " << srcPath << ": " << ex.what() << "\n";
                rc = 1;
            }
            continue;
        }

        std::string emitted = emitProgram(prog, lang, sheet.target.packageRoot);
        std::cout << "\n// ==== " << srcPath << "  ->  " << langName(lang) << " ====\n";
        std::cout << emitted;
    }

    // Emit a component/subject manifest artifact from the declared network,
    // finance, and subject series, so declaring those blocks has a real,
    // inspectable build effect. Written next to the sheet for the sleela
    // target; printed for java/c (matching the source emission behavior).
    if (hasComponentManifest(sheet)) {
        std::string manifest = emitComponentManifest(sheet, lang, sheet.target.packageRoot);
        if (lang == TargetLang::Sleela) {
            std::filesystem::path outDir = std::filesystem::path(sheetDir) / "build";
            std::filesystem::create_directories(outDir);
            const char* ext = ".sleela";
            std::filesystem::path out = outDir / (std::string("ComponentManifest") + ext);
            std::ofstream mf(out, std::ios::binary);
            if (mf) {
                mf << manifest;
                std::cout << "nordshrift: manifest -> " << out.string()
                          << " (network/finance/subject component manifest)\n";
            } else {
                std::cerr << "nordshrift: cannot write manifest '" << out.string() << "'\n";
                rc = 1;
            }
        } else {
            std::cout << "\n// ==== component manifest  ->  " << langName(lang) << " ====\n";
            std::cout << manifest;
        }
    }
    return rc;
}

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

static int doRelevance(int argc, char** argv) {
    TargetLang target = TargetLang::Java;
    bool haveTarget = false;
    std::string only;
    for (int i = 2; i < argc; i++) {
        std::string a = argv[i];
        if (a.rfind("--target=", 0) == 0) haveTarget = parseTargetLang(a.substr(9), target);
        else if (a == "--target" && i + 1 < argc) haveTarget = parseTargetLang(argv[++i], target);
        else only = a;
    }
    if (!haveTarget) {
        std::cerr << "nordshrift: relevance requires --target=java|sleela|c\n";
        return usage();
    }

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
