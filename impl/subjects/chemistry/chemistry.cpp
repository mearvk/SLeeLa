#include "chemistry.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace sleela {
namespace chemistry {

namespace {
double clamp01(double x) {
    if (!std::isfinite(x)) return 0.0;
    return std::max(0.0, std::min(1.0, x));
}
}

double ratio(double numerator, double denominator) {
    if (!std::isfinite(numerator) || !std::isfinite(denominator) || denominator == 0.0) return 0.0;
    return numerator / denominator;
}

double similarity(double a, double b) {
    if (!std::isfinite(a) || !std::isfinite(b)) return 0.0;
    const double scale = std::max({std::fabs(a), std::fabs(b), 1.0});
    return clamp01(1.0 - std::fabs(a - b) / scale);
}

double stochastic_factor(std::uint64_t seed) {
    // SplitMix64-style deterministic mixing. The output is deliberately
    // narrow: stochastic ranking may perturb a hypothesis, but cannot create
    // evidence or overturn a strong contradictory observation.
    std::uint64_t x = seed + 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x ^= x >> 31;
    const double u = static_cast<double>(x & 0xFFFFFFFFULL) / 4294967295.0;
    return 0.98 + 0.04 * u;
}

InferenceScore evaluate(double observation,
                        double bond,
                        double valence,
                        double similarity_score,
                        double signal,
                        double symmetry,
                        std::uint64_t seed) {
    const double evidence[6] = {
        clamp01(observation), clamp01(bond), clamp01(valence),
        clamp01(similarity_score), clamp01(signal), clamp01(symmetry)
    };
    // Observation and valence receive the strongest weights. Similarity and
    // symmetry are supporting evidence, not substitutes for observation.
    const double weights[6] = {0.24, 0.20, 0.22, 0.13, 0.12, 0.09};
    double weighted = 0.0;
    for (int i = 0; i < 6; ++i) weighted += evidence[i] * weights[i];

    const double stochastic = stochastic_factor(seed);
    const double support = clamp01(weighted * stochastic);
    int level = static_cast<int>(std::floor(support * 25.0));
    if (level > 24) level = 24;

    // Uncertainty remains explicit and cannot fall to zero merely because a
    // model receives a high score.
    const double spread = std::max({evidence[0], evidence[1], evidence[2],
                                    evidence[3], evidence[4], evidence[5]}) -
                          std::min({evidence[0], evidence[1], evidence[2],
                                    evidence[3], evidence[4], evidence[5]});
    const double uncertainty = clamp01(0.55 * (1.0 - support) + 0.45 * spread);
    return {level, support, uncertainty};
}

SubjectNorm presume(const std::string& subject,
                    double observation,
                    double bond,
                    double valence,
                    double similarity_score,
                    double signal,
                    double symmetry,
                    std::uint64_t seed) {
    SubjectNorm result;
    result.subject = subject;
    result.observations.push_back("normalized observation score");
    result.bonds.push_back("bond compatibility");
    result.signals.push_back("available experimental signal");
    result.conferrers.push_back("periodic/structural analogy");
    result.conferrers.push_back("valence consistency");
    result.conferrers.push_back("convergent signal support");
    result.symmetry.push_back("causal evidence ordering");
    result.symmetry.push_back("compound-family similarity");
    result.ratios = {
        {"observation", clamp01(observation), 0.24},
        {"bond", clamp01(bond), 0.20},
        {"valence", clamp01(valence), 0.22},
        {"similarity", clamp01(similarity_score), 0.13},
        {"signal", clamp01(signal), 0.12},
        {"symmetry", clamp01(symmetry), 0.09}
    };
    result.score = evaluate(observation, bond, valence, similarity_score,
                            signal, symmetry, seed);
    return result;
}

const std::vector<std::string>& causal_order() {
    static const std::vector<std::string> order = {
        "observation",
        "measurement",
        "structure",
        "bond",
        "valence",
        "geometry",
        "electronic_state",
        "reaction_relationship",
        "compound_family_similarity",
        "thermodynamic_consequence",
        "astronomical_context"
    };
    return order;
}

} // namespace chemistry
} // namespace sleela
