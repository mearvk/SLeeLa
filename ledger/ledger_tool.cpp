// ===========================================================================
// ledger_tool.cpp -- standalone .ledger generator/verifier (C++ front end over
// ledger_core). Mirrors ledger/ledger.py. Usage:
//   ledger_tool build --out FILE.ledger [--insignia-dir DIR] [--root DIR] FILES...
//   ledger_tool verify FILE.ledger
// ===========================================================================
#include "ledger_core.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static int do_build(int argc, char** argv) {
    std::string out, insignia_dir, root;
    std::vector<std::string> files;
    for (int i = 0; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--out" && i + 1 < argc) out = argv[++i];
        else if (a == "--insignia-dir" && i + 1 < argc) insignia_dir = argv[++i];
        else if (a == "--root" && i + 1 < argc) root = argv[++i];
        else files.push_back(a);
    }
    if (out.empty() || files.empty()) {
        std::cerr << "ledger_tool: build needs --out and files\n";
        return 2;
    }
    std::ofstream lf(out, std::ios::binary);
    if (!lf) { std::cerr << "ledger_tool: cannot write " << out << "\n"; return 1; }
    lf << ledger::header_line() << "\n";
    std::string prev = ledger::GENESIS();
    std::string ts = ledger::utc_iso8601();
    int seq = 0;
    for (const auto& f : files) {
        ++seq;
        std::string rel = f;
        if (!root.empty()) rel = fs::relative(f, root).string();
        std::string sha = ledger::sha256_file(f);
        std::string qrpath = insignia_dir.empty() ? "" : (insignia_dir + "/" + rel + ".svg");
        ledger::Record r;
        r.seq = seq; r.path = rel; r.sha256 = sha; r.prev = prev;
        r.timestamp_utc = ts; r.qr_svg_path = qrpath;
        ledger::finalize(r);
        prev = r.record_hash;
        if (!insignia_dir.empty()) {
            std::string svg = ledger::qr_svg(ledger::qr_identity(seq, rel, sha, ts));
            fs::path dest = fs::path(insignia_dir) / (rel + ".svg");
            fs::create_directories(dest.parent_path());
            std::ofstream sf(dest, std::ios::binary);
            sf << svg;
        }
        lf << ledger::record_line(r) << "\n";
    }
    std::cout << "ledger_tool: wrote " << out << " (" << seq << " records)\n";
    return 0;
}

static int do_verify(const std::string& path) {
    std::ifstream f(path);
    if (!f) { std::cerr << "ledger_tool: cannot read " << path << "\n"; return 1; }
    std::string line;
    std::getline(f, line); // header
    std::string prev = ledger::GENESIS();
    int breaks = 0, n = 0;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        ++n;
        // crude field extraction sufficient for verification
        auto field = [&](const std::string& key) -> std::string {
            std::string pat = "\"" + key + "\":\"";
            size_t p = line.find(pat);
            if (p == std::string::npos) return "";
            p += pat.size();
            size_t q = line.find('"', p);
            return line.substr(p, q - p);
        };
        auto intfield = [&](const std::string& key) -> int {
            std::string pat = "\"" + key + "\":";
            size_t p = line.find(pat); if (p == std::string::npos) return 0;
            p += pat.size(); return std::atoi(line.c_str() + p);
        };
        ledger::Record r;
        r.seq = intfield("seq");
        r.path = field("path");
        r.sha256 = field("sha256");
        r.prev = field("prev");
        r.timestamp_utc = field("timestamp_utc");
        r.timestamp_kind = field("timestamp_kind");
        r.qr_svg_path = field("qr_svg");
        std::string want = ledger::sha256_hex(ledger::canonical(r));
        std::string got = field("record_hash");
        if (got != want) { std::cout << "ledger_tool: record " << n << " record_hash MISMATCH\n"; ++breaks; }
        if (r.prev != prev) { std::cout << "ledger_tool: record " << n << " prev-link BROKEN\n"; ++breaks; }
        prev = want;
    }
    if (breaks == 0) { std::cout << "ledger_tool: OK -- " << n << " records, chain intact\n"; return 0; }
    std::cout << "ledger_tool: FAIL -- " << breaks << " problem(s)\n";
    return 1;
}

int main(int argc, char** argv) {
    if (argc < 2) { std::cerr << "usage: ledger_tool build|verify ...\n"; return 2; }
    std::string cmd = argv[1];
    if (cmd == "build") return do_build(argc - 2, argv + 2);
    if (cmd == "verify" && argc >= 3) return do_verify(argv[2]);
    std::cerr << "usage: ledger_tool build|verify ...\n";
    return 2;
}
