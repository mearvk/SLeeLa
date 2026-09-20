// ===========================================================================
// ledger_emit.h -- Nordshrift hook to emit a .ledger next to each .sleela
// artifact, using the shared ledger core (ledger/ledger_core.*). Keeps the
// compiler's ledger output byte-identical to the standalone ledger_tool.
// ===========================================================================
#ifndef NORDSHRIFT_LEDGER_EMIT_H
#define NORDSHRIFT_LEDGER_EMIT_H

#include <string>

namespace nordshrift {

// Emit a single-record .ledger for `artifactPath` (an emitted .sleela file):
//   - writes "<artifactPath>.ledger" (JSON-lines: header + one record),
//   - writes a QR insignia SVG next to it as "<artifactPath>.qr.svg".
// The record's sha256 is over the artifact file's bytes; the timestamp is UTC
// ISO-8601; the chain uses genesis as prev (one record per artifact file).
// Returns true on success. Best-effort: a failure here does not abort the build.
bool emitLedgerFor(const std::string& artifactPath);

} // namespace nordshrift

#endif // NORDSHRIFT_LEDGER_EMIT_H
