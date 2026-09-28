// ===========================================================================
// ledger_core.h -- shared .ledger core: SHA-256, canonical records, prev-hash
// chain, ISO-8601 UTC timestamp, and QR insignia (SVG). Dependency-free (C++17
// stdlib only). Used by BOTH the standalone ledger_tool and the Nordshrift
// compiler so the two produce identical .ledger output. See LEDGER.md.
// ===========================================================================
#ifndef SLEELA_LEDGER_CORE_H
#define SLEELA_LEDGER_CORE_H

#include <string>
#include <vector>
#include <cstdint>

namespace ledger {

constexpr int    LEDGER_VERSION = 1;
inline const char* GENESIS() { return "0000000000000000000000000000000000000000000000000000000000000000"; }

// SHA-256 of arbitrary bytes -> lowercase hex.
std::string sha256_hex(const std::string& data);
// SHA-256 of a file's contents -> lowercase hex; empty string on read error.
std::string sha256_file(const std::string& path);

// UTC ISO-8601 timestamp, e.g. "2026-09-20T00:00:00Z".
std::string utc_iso8601();

// A QR code for `text` rendered as an SVG string (byte mode, EC level 'M').
std::string qr_svg(const std::string& text);

// One ledger record.
struct Record {
    int         seq = 0;
    std::string path;
    std::string sha256;
    std::string prev;
    std::string timestamp_utc;
    std::string timestamp_kind = "utc-iso8601";
    std::string qr_svg_path;
    std::string record_hash;   // filled by finalize()
};

// Canonical bytes of a record (fixed key order, compact, record_hash excluded).
std::string canonical(const Record& r);
// record_hash = sha256_hex(canonical(r)); sets r.record_hash and returns it.
std::string finalize(Record& r);

// JSON header line for a .ledger file.
std::string header_line();
// JSON emission line for a finalized record (canonical keys + record_hash).
std::string record_line(const Record& r);

// QR identity string: "sleela-ledger:1|<seq>|<path>|<sha>|<ts>".
std::string qr_identity(int seq, const std::string& path,
                        const std::string& sha, const std::string& ts);

} // namespace ledger

#endif // SLEELA_LEDGER_CORE_H
