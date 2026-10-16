// ===========================================================================
// xlt_resolve.cpp  --  Native resolution of an XltModel (mirrors the Sleela
// SLXltCompiler + SLXltProducerRegistry).
// ===========================================================================
#include "xlt_resolve.h"

#include <map>
#include <set>

namespace xlt {

namespace {

// The producer registry: known refs per slot. Memory/board carry a channel
// attribute for the agreement check (the native mirror of SLXltProducerRegistry
// defaults). A new part becomes recognizable by adding it here.
const std::set<std::string>& cpuSet() {
    static const std::set<std::string> s = {
        "SLAMDEPYC9575F","SLAMDEPYC9965","SLAMDEPYC9755","SLAMDEPYC9645",
        "SLAMDEPYC9475F","SLAMDEPYC9355","SLAMDEPYC9175F","SLAMDEPYCCPU",
        "SLAMDThreadripper9980X","SLAMDThreadripper9970X","SLAMDThreadripper9960X",
        "SLAMDThreadripperPro9995WX","SLAMDThreadripperPro9985WX","SLAMDThreadripperPro9975WX",
        "SLAMDThreadripperPro9965WX","SLAMDThreadripperPro9955WX","SLAMDThreadripperPro9945WX",
        "SLAMDThreadripperCPU"
    };
    return s;
}
// memory ref -> channel count
const std::map<std::string,int>& memMap() {
    static const std::map<std::string,int> m = {
        {"dram_fpm",1},{"sdram_pc100",1},{"ddr_400",1},{"ddr2_800",1},
        {"ddr3_1600",1},{"ddr4_3200",1},{"ddr5_6000",1},{"ddr5_6400",1},
        {"ddr5_6400_quadChannel",4},{"ddr5_6400_octaChannel",8},{"sram_fast",1}
    };
    return m;
}
// board ref -> routed channels
const std::map<std::string,int>& boardMap() {
    static const std::map<std::string,int> m = { {"trx50",4},{"wrx90",8} };
    return m;
}
const std::set<std::string>& cadenceSet() {
    static const std::set<std::string> s = {"DDR","DDR2","DDR3","DDR4","DDR5"};
    return s;
}
const std::set<std::string>& storageSet() {
    static const std::set<std::string> s = {"nvmeGen4","nvmeGen3","sataSsd","sataHdd"};
    return s;
}
const std::set<std::string>& busSet() {
    static const std::set<std::string> s = {
        "pcieGen5x16","pcieGen4x16","pcieGen3x16","pci33","isa","agp8x"
    };
    return s;
}

void missing(ResolveResult& r, const std::string& detail) {
    r.reasons.push_back("[missing] " + detail);
}
void disagrees(ResolveResult& r, const std::string& detail) {
    r.reasons.push_back("[disagrees] " + detail);
}

} // namespace

ResolveResult resolve(const XltModel& m, const DiagnosticBag& diags) {
    ResolveResult r;

    // Fold any lex/parse errors in first — a malformed sheet cannot be built.
    for (const auto& d : diags.all()) {
        if (d.tier == Tier::Error)
            r.reasons.push_back("[parse] " + d.code + ": " + d.message +
                                (d.line > 0 ? " (line " + std::to_string(d.line) + ")" : ""));
    }

    int boardChannels = 0;

    // 1. CPU (required)
    if (!m.cpu.present) missing(r, "no cpu declared: a model requires a CPU");
    else if (!cpuSet().count(m.cpu.ref)) missing(r, "cpu '" + m.cpu.ref + "' is not a registered SKU");
    else r.builtLines.push_back("cpu: " + m.cpu.ref);

    // 2. board (optional)
    if (m.board.present) {
        auto it = boardMap().find(m.board.ref);
        if (it == boardMap().end()) missing(r, "board '" + m.board.ref + "' is not a registered motherboard");
        else { boardChannels = it->second; r.builtLines.push_back("board: " + m.board.ref + " (" + std::to_string(boardChannels) + "-channel)"); }
    }

    // 3. memory (required) + agreement
    if (!m.memory.present) missing(r, "no memory declared: a model requires main memory");
    else {
        auto it = memMap().find(m.memory.ref);
        if (it == memMap().end()) missing(r, "memory '" + m.memory.ref + "' is not a registered module");
        else {
            int wantChannels = it->second;
            if (boardChannels > 0 && wantChannels > boardChannels) {
                disagrees(r, "memory '" + m.memory.ref + "' needs " + std::to_string(wantChannels) +
                             " channels but board '" + m.board.ref + "' routes only " + std::to_string(boardChannels));
            } else {
                r.builtLines.push_back("memory: " + m.memory.ref + " x" + std::to_string(m.memoryCapMiB) + " MiB");
                // 3b. cadence (optional, rides the memory)
                if (m.cadence.present) {
                    if (!cadenceSet().count(m.cadence.ref))
                        missing(r, "cadence '" + m.cadence.ref + "' is not a registered generation");
                    else
                        r.builtLines.push_back("cadence: refreshes as " + m.cadence.ref);
                }
            }
        }
    }

    // 4. storage (optional)
    if (m.storage.present) {
        if (!storageSet().count(m.storage.ref)) missing(r, "storage '" + m.storage.ref + "' is not a registered device");
        else r.builtLines.push_back("storage: " + m.storage.ref);
    }

    // 5. bus (optional)
    if (m.bus.present) {
        if (!busSet().count(m.bus.ref)) missing(r, "bus '" + m.bus.ref + "' is not a registered system bus");
        else r.builtLines.push_back("bus: " + m.bus.ref);
    }

    r.built = (!r.builtLines.empty() && r.reasons.empty());
    return r;
}

std::string renderResult(const std::string& modelName, const ResolveResult& r) {
    std::string out = "=== XLT model '" + modelName + "' ===\n";
    if (r.built) {
        out += "BUILD SUCCESS -- built " + std::to_string(r.builtLines.size()) + " parts:\n";
        for (const auto& b : r.builtLines) out += "  + " + b + "\n";
    } else {
        out += "NOT BUILT -- congruence report (" + std::to_string(r.reasons.size()) + " issue(s)):\n";
        for (const auto& c : r.reasons) out += "  ! " + c + "\n";
        out += "  (built-so-far list had " + std::to_string(r.builtLines.size()) + " agreeable part(s))\n";
    }
    return out;
}

} // namespace xlt
