// counsel.hpp - Turn the knowledge store plus its evidence into counsel:
//               a ranked, caveated answer to "which advantage should I draw
//               on, and what must I check before I do?"
//
// This is the join between the two halves of the module:
//   * the C math core   (advantage_mean / advantage_stddev / confidence)
//   * the knowledge store (assumptions, knowledge strings, evidence)
//
// Wisdom built into the scoring -- the score never claims more than the
// evidence supports:
//   score = meanAdvantage * confidence * trust
//     meanAdvantage : average (reward - baseline) * weight over the evidence
//     confidence    : n / (n + prior). Two lucky results earn little trust.
//     trust         : fraction of the source's assumptions currently held.
//                     An advantage resting on doubted assumptions is worth
//                     less until those assumptions are verified.
//   * No evidence is NOT zero advantage: it is "unknown", scored 0 and
//     flagged, so an untested source is never confused with a bad one.
//   * Remote (fetched) knowledge is never used in scoring or advice; it is
//     only counted, because it is untrusted.
//
// Pure and offline: no I/O, no network.
#ifndef ADVANTAGE_COUNSEL_HPP
#define ADVANTAGE_COUNSEL_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

#include "advantage_store.hpp"

namespace advantage {

struct CounselOptions {
    // Observations at which confidence is one half (see advantage_confidence).
    double prior = 3.0;
    // Below this many observations the source is flagged as thinly evidenced.
    std::size_t minEvidence = 3;
};

struct Counsel {
    const SourceAdvantage* source = nullptr;  // valid while the store lives

    std::size_t evidenceCount = 0;
    double meanAdvantage = 0.0;   // from the C core
    double spread = 0.0;          // sample std-dev of per-observation advantage
    double confidence = 0.0;      // 0 with no evidence
    double trust = 1.0;           // held assumptions / total (1.0 if none)
    double score = 0.0;           // meanAdvantage * confidence * trust

    std::size_t assumptionsHeld = 0;
    std::size_t assumptionsTotal = 0;

    // Assumptions to verify before relying on this source, doubted ones
    // first (prefixed "DOUBTED: "), then held ones (prefixed "held: ").
    std::vector<std::string> toVerify;

    // Plain-language reasons the score should be taken with care.
    std::vector<std::string> cautions;
};

// Compute counsel for every source, ranked best-first by score (ties broken
// by id for determinism). Never mutates the store.
std::vector<Counsel> counsel(const AdvantageStore& store,
                             const CounselOptions& opts = {});

// Write a human-readable rendering: for each ranked source, the score and its
// parts, what to draw on (authored knowledge), what to verify first
// (doubted assumptions, then held ones), and cautions.
void renderCounsel(std::ostream& os, const std::vector<Counsel>& ranked);

}  // namespace advantage

#endif  // ADVANTAGE_COUNSEL_HPP
