// counsel.cpp - Implementation of counsel (see counsel.hpp).
#include "counsel.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "advantage.hpp"

namespace advantage {

namespace {

std::string fmt(double v) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3) << v;
    return ss.str();
}

Counsel assess(const SourceAdvantage& s, const CounselOptions& opts) {
    Counsel c;
    c.source = &s;
    c.evidenceCount = s.evidence.size();

    // Hand the evidence to the C core as flat series.
    std::vector<double> rewards, baselines;
    rewards.reserve(s.evidence.size());
    baselines.reserve(s.evidence.size());
    for (const auto& ob : s.evidence) {
        rewards.push_back(ob.reward);
        baselines.push_back(ob.baseline);
    }
    double mean = 0.0;
    // Series are built together, so lengths always match.
    mean_checked(rewards, baselines, s.evidenceWeight, mean);
    c.meanAdvantage = mean;
    c.spread = advantage_stddev(rewards.data(), baselines.data(),
                                rewards.size(), s.evidenceWeight);
    c.confidence = advantage_confidence(c.evidenceCount, opts.prior);

    // Trust from assumptions. Guard against a store whose parallel vector was
    // not populated: treat missing status as held.
    c.assumptionsTotal = s.assumptions.size();
    for (std::size_t i = 0; i < s.assumptions.size(); ++i) {
        bool held = i >= s.assumptionHeld.size() ? true : s.assumptionHeld[i];
        if (held) c.assumptionsHeld++;
    }
    c.trust = c.assumptionsTotal == 0
                  ? 1.0
                  : static_cast<double>(c.assumptionsHeld) /
                        static_cast<double>(c.assumptionsTotal);

    c.score = c.meanAdvantage * c.confidence * c.trust;

    // Extreme inputs can overflow to inf/NaN. A NaN would also break the
    // strict weak ordering the ranking sort relies on, so neutralize it:
    // an unusable score is reported as 0 with an explicit caution.
    const bool finite = std::isfinite(c.meanAdvantage) &&
                        std::isfinite(c.spread) && std::isfinite(c.score);
    if (!finite) {
        c.meanAdvantage = 0.0;
        c.spread = 0.0;
        c.score = 0.0;
        c.cautions.push_back(
            "evidence values overflowed the math; score withheld (set to 0)");
    }

    // What to verify: doubted assumptions first, then the held ones (held is
    // not the same as proven).
    for (std::size_t i = 0; i < s.assumptions.size(); ++i) {
        bool held = i >= s.assumptionHeld.size() ? true : s.assumptionHeld[i];
        if (!held) c.toVerify.push_back("DOUBTED: " + s.assumptions[i]);
    }
    for (std::size_t i = 0; i < s.assumptions.size(); ++i) {
        bool held = i >= s.assumptionHeld.size() ? true : s.assumptionHeld[i];
        if (held) c.toVerify.push_back("held: " + s.assumptions[i]);
    }

    // Cautions -- say plainly why the number may mislead.
    if (c.evidenceCount == 0) {
        c.cautions.push_back("no evidence yet: advantage is unknown, not zero");
    } else if (c.evidenceCount < opts.minEvidence) {
        c.cautions.push_back("thin evidence (" +
                             std::to_string(c.evidenceCount) + " < " +
                             std::to_string(opts.minEvidence) +
                             " observations): score is heavily discounted");
    }
    if (c.evidenceCount > 0 && c.meanAdvantage < 0.0) {
        c.cautions.push_back("evidence shows a disadvantage on average");
    }
    if (c.evidenceCount >= 2 && c.meanAdvantage != 0.0 &&
        c.spread > (c.meanAdvantage < 0 ? -c.meanAdvantage : c.meanAdvantage)) {
        c.cautions.push_back("inconsistent: spread exceeds the average gain");
    }
    if (c.assumptionsHeld < c.assumptionsTotal) {
        c.cautions.push_back(std::to_string(c.assumptionsTotal -
                                            c.assumptionsHeld) +
                             " assumption(s) doubted: verify before relying");
    }
    if (!s.remoteKnowledge.empty()) {
        c.cautions.push_back(std::to_string(s.remoteKnowledge.size()) +
                             " remote line(s) present but untrusted and "
                             "excluded from this counsel");
    }
    return c;
}

}  // namespace

std::vector<Counsel> counsel(const AdvantageStore& store,
                             const CounselOptions& opts) {
    std::vector<Counsel> out;
    out.reserve(store.sources.size());
    for (const auto& s : store.sources) out.push_back(assess(s, opts));

    std::stable_sort(out.begin(), out.end(),
                     [](const Counsel& a, const Counsel& b) {
                         if (a.score != b.score) return a.score > b.score;
                         return a.source->id < b.source->id;
                     });
    return out;
}

void renderCounsel(std::ostream& os, const std::vector<Counsel>& ranked) {
    int rank = 1;
    for (const auto& c : ranked) {
        const SourceAdvantage& s = *c.source;
        os << rank++ << ". " << s.name << "  [id=" << s.id << "]  score="
           << fmt(c.score) << "\n";
        os << "   mean=" << fmt(c.meanAdvantage) << "  spread=" << fmt(c.spread)
           << "  confidence=" << fmt(c.confidence) << " (n="
           << c.evidenceCount << ")  trust=" << fmt(c.trust) << " ("
           << c.assumptionsHeld << "/" << c.assumptionsTotal << " held)\n";

        os << "   draw on:\n";
        if (s.knowledge.empty()) os << "     (no authored knowledge)\n";
        for (const auto& k : s.knowledge) os << "     * " << k << "\n";

        os << "   verify first:\n";
        if (c.toVerify.empty()) os << "     (no assumptions recorded)\n";
        for (const auto& v : c.toVerify) os << "     - " << v << "\n";

        if (!c.cautions.empty()) {
            os << "   cautions:\n";
            for (const auto& w : c.cautions) os << "     ! " << w << "\n";
        }
        os << "\n";
    }
}

}  // namespace advantage
