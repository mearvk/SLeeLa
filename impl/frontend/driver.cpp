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
#include "../transition/mem_model.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

extern "C" {
#include "../core/sleela_core.h"
#include "../transition/stp_client.h"
#include "../transition/stp_crypto_optional.h"
}

static const char* kVersion = "Sleela 0.1.2 (C/C++ core; SHEET.sheet conducted methods; STP-0001 secure transition)";

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
        "  sleela run [--secure [--remote=host:port] [--pin=<hex>]] <file.sleela>\n"
        "                             compile and run a Sleela program; with --secure,\n"
        "                             transition under SecureJDK 28 supervision over the\n"
        "                             STP-0001 pipe (local by default, or --remote), and\n"
        "                             fall back to a local safe-trim on failure.\n"
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

// Options for a secure (STP-0001) transition run.
struct SecureOpts {
    bool secure = false;
    std::string remote;      // "host:port" or empty (local pipe)
    std::string pin;         // supervisor Ed25519 pubkey hex (64 chars) or empty
};

// Attempt the SecureJDK 28 transition for a parsed program. Returns true if the
// program is ADMITTED (proceed supervised); false if it must run as safe-trim
// (in which case *cap is set to the trimmed thread cap and the failure has been
// recorded for Admin review).
static bool secureTransition(const sleela::Program& prog, const std::string& path,
                             const SecureOpts& opts, const catalog::Catalog& cat,
                             int* safeTrimCap) {
    // Compute the memory model + parse digest.
    long long sysdepth = cat.depth ? cat.depth : 3024;
    long long degreemax = cat.complexityDegreeMax ? cat.complexityDegreeMax : 4;
    stp_mem_model mm = sleela::analyze_mem_model(prog, sysdepth, degreemax);

    // Parse digest = SHA-256 over a canonical summary of the program shape.
    std::ostringstream canon;
    canon << "sleela-parse|" << path << "|g=" << mm.globals << "|f=" << mm.functions
          << "|c=" << mm.code_len << "|t=" << mm.max_threads
          << "|l=" << mm.locks << "|m=" << mm.mailboxes;
    std::string cs = canon.str();
    uint8_t digest[32];
    stp_sha256_portable(reinterpret_cast<const uint8_t*>(cs.data()), cs.size(), digest);

    char program_id[17];
    stp_hex_local(digest, 8, program_id);   // first 8 bytes -> 16 hex chars

    stp_config cfg; std::memset(&cfg, 0, sizeof cfg);
    std::string host, portStr;
    if (!opts.remote.empty()) {
        auto colon = opts.remote.find(':');
        host = opts.remote.substr(0, colon);
        portStr = colon == std::string::npos ? "8443" : opts.remote.substr(colon + 1);
        cfg.remote_host = host.c_str();
        cfg.remote_port = std::atoi(portStr.c_str());
    }
    if (!opts.pin.empty()) cfg.sup_pubkey_hex = opts.pin.c_str();

    std::string sourceName = path;
    stp_outcome outcome;
    stp_transition(&cfg, program_id, sourceName.c_str(), digest, &mm, &outcome);

    if (outcome.kind == STP_ADMITTED) {
        std::cout << "[sleela] SECURE TRANSITION: ADMITTED by SecureJDK 28\n"
                  << "[sleela]   region  = " << outcome.region_name
                  << " (class " << outcome.region_class << ")\n"
                  << "[sleela]   threads = " << outcome.granted_threads
                  << ", observer = " << outcome.observer_endpoint << "\n"
                  << "[sleela]   ack tag verified = " << (outcome.ack_verified ? "yes" : "no")
                  << " — running UNDER SUPERVISION\n";
        *safeTrimCap = 0;   // no trim; supervised
        return true;
    }

    // FALLBACK: record for Admin review, apply the safe trim.
    std::cout << "[sleela] SECURE TRANSITION: FALLBACK (" << outcome.reason;
    if (outcome.detail[0]) std::cout << ": " << outcome.detail;
    std::cout << ")\n"
              << "[sleela]   continuing LOCALLY as a secure safe-trim"
              << " (thread cap " << STP_SAFE_TRIM_MAX_THREADS << ")\n";
    stp_record_failure(&cfg, program_id, sourceName.c_str(), digest, &mm,
                       outcome.reason, outcome.detail,
                       opts.remote.empty() ? "local-pipe" : "remote-tls");
    *safeTrimCap = STP_SAFE_TRIM_MAX_THREADS;
    return false;
}

static int runFile(const std::string& path, const SecureOpts& opts = SecureOpts{}) {
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

        // Secure transition to SecureJDK 28 (optional): request supervision of
        // the memory model; on failure fall back to a local safe-trim run.
        if (opts.secure) {
            int safeTrimCap = 0;
            bool admitted = secureTransition(prog, path, opts, cat, &safeTrimCap);
            if (!admitted && safeTrimCap > 0) slvm_set_thread_cap(vm, safeTrimCap);
        }

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
        SecureOpts opts;
        std::string file;
        for (int i = 2; i < argc; i++) {
            std::string a = argv[i];
            if (a == "--secure") opts.secure = true;
            else if (a.rfind("--remote=", 0) == 0) { opts.secure = true; opts.remote = a.substr(9); }
            else if (a.rfind("--pin=", 0) == 0) opts.pin = a.substr(6);
            else if (a.rfind("--", 0) == 0) { std::cerr << "sleela: unknown option " << a << "\n"; return usage(); }
            else file = a;
        }
        if (file.empty()) return usage();
        return runFile(file, opts);
    }
    // Convenience: `sleela file.sleela` behaves like `sleela run file.sleela`.
    if (cmd.size() > 7 && cmd.substr(cmd.size() - 7) == ".sleela") {
        return runFile(cmd);
    }
    return usage();
}
