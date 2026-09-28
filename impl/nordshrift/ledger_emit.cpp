// ===========================================================================
// ledger_emit.cpp -- implementation of the Nordshrift .ledger hook.
// Uses the shared ledger core so compiler output matches ledger/ledger_tool.
// ===========================================================================
#include "ledger_emit.h"

#include "../../ledger/ledger_core.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace nordshrift {

bool emitLedgerFor(const std::string& artifactPath) {
    std::string sha = ledger::sha256_file(artifactPath);
    if (sha.empty()) {
        return false;
    }
    std::string ts = ledger::utc_iso8601();
    std::string qrPath = artifactPath + ".qr.svg";

    ledger::Record r;
    r.seq = 1;
    r.path = artifactPath;
    r.sha256 = sha;
    r.prev = ledger::GENESIS();
    r.timestamp_utc = ts;
    r.qr_svg_path = qrPath;
    ledger::finalize(r);

    // QR insignia
    {
        std::string svg = ledger::qr_svg(
            ledger::qr_identity(r.seq, r.path, r.sha256, r.timestamp_utc));
        std::ofstream qf(qrPath, std::ios::binary);
        if (qf) qf << svg;
    }

    // .ledger file (header + single record)
    std::string ledgerPath = artifactPath + ".ledger";
    std::ofstream lf(ledgerPath, std::ios::binary);
    if (!lf) {
        return false;
    }
    lf << ledger::header_line() << "\n" << ledger::record_line(r) << "\n";
    return true;
}

} // namespace nordshrift
