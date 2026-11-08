// enrich.hpp - Safe, opt-in enrichment of an AdvantageStore from remote
//              knowledge sources, via the secure transport layer.
//
// SAFETY POSTURE (all gates default-closed):
//   * Enrichment is OFF unless a policy is explicitly passed.
//   * Only schemes in the allowlist are permitted; the default allowlist is
//     { "https", "file" } -- no plaintext http, no ssh/sftp by default.
//   * Only hosts in `allowedHosts` are permitted; the default is EMPTY, which
//     allows nothing. A caller must name every host it trusts.
//   * TLS certificate + hostname verification stays ON (inherited from the
//     transport layer's secure defaults).
//   * Each fetch is bounded by a byte cap and a maximum number of knowledge
//     lines; oversized responses are rejected.
//   * Fetched text is SANITIZED (control chars stripped, length-capped) and
//     stored in SourceAdvantage::remoteKnowledge -- kept separate from
//     authored knowledge so untrusted remote data can never masquerade as
//     authored content.
//   * All downloads go through the content-addressed store, so enrichment is
//     idempotent and integrity-checked (optional sha256 pin per source).
//
// Nothing here can reach the network unless the caller both links transport
// and passes a policy that names the schemes and hosts it trusts.
#ifndef ADVANTAGE_ENRICH_HPP
#define ADVANTAGE_ENRICH_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "advantage_store.hpp"

namespace advantage {

struct EnrichPolicy {
    // Must be true or enrichment is a no-op. Forces the caller to opt in.
    bool enabled = false;

    // Directory for the content-addressed download store.
    std::string storeDir = "./advantage-store";

    // Permitted URL schemes. Default: https + file only.
    std::vector<std::string> allowedSchemes = {"https", "file"};

    // Permitted hosts (exact match, case-insensitive). EMPTY => deny all
    // network hosts. file:// has no host and is governed by allowedSchemes.
    std::vector<std::string> allowedHosts;

    // Per-source download cap and sanitization limits.
    std::size_t maxBytesPerSource = 1u * 1024u * 1024u;  // 1 MiB
    std::size_t maxLinesPerSource = 1000;
    std::size_t maxLineLength     = 2000;

    // If true, a single source failing aborts the whole enrichment with an
    // error. If false (default), failures are collected per-source and
    // enrichment continues -- a safer, more resilient default.
    bool failFast = false;
};

struct EnrichReport {
    std::size_t sourcesConsidered = 0;   // sourceAdvantages with a URL
    std::size_t fetchesAttempted = 0;
    std::size_t fetchesSucceeded = 0;
    std::size_t fetchesFromCache = 0;
    std::size_t linesAdded = 0;
    std::vector<std::string> warnings;   // per-source skips/failures
    bool ok = true;                      // false only if failFast tripped
};

// Enrich `store` in place by fetching declared knowledgeSources that pass the
// policy gates. Returns a report; `store` is mutated only with sanitized,
// clearly-attributed remote knowledge. Safe to call repeatedly (idempotent).
EnrichReport enrich(AdvantageStore& store, const EnrichPolicy& policy);

}  // namespace advantage

#endif  // ADVANTAGE_ENRICH_HPP
