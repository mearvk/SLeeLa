#ifndef SLEELA_CHEMISTRY_H
#define SLEELA_CHEMISTRY_H

#include <cstdint>
#include <string>
#include <vector>

namespace sleela {
namespace chemistry {

// A bounded 0..24 inference level. It measures model support, not truth.
struct InferenceScore {
    int level;
    double confidence;
    double uncertainty;
};

struct RatioNorm {
    std::string name;
    double value;
    double weight;
};

struct SubjectNorm {
    std::string subject;
    std::vector<std::string> observations;
    std::vector<std::string> bonds;
    std::vector<std::string> signals;
    std::vector<RatioNorm> ratios;
    std::vector<std::string> conferrers;
    std::vector<std::string> symmetry;
    InferenceScore score;
};

// Safe ratio: returns 0 for an undefined denominator.
double ratio(double numerator, double denominator);

// Compare two positive quantities as a bounded similarity ratio [0,1].
double similarity(double a, double b);

// Deterministically seeded stochastic perturbation for ranking competing
// hypotheses. This is a ranking aid, never a measurement.
double stochastic_factor(std::uint64_t seed);

// Convert convergent evidence ratios into the project 0..24 inference scale.
InferenceScore evaluate(double observation,
                        double bond,
                        double valence,
                        double similarity_score,
                        double signal,
                        double symmetry,
                        std::uint64_t seed = 0);

// Build a subject norm from normalized evidence. The returned structure keeps
// observation, inference, similarity and uncertainty conceptually separate.
SubjectNorm presume(const std::string& subject,
                    double observation,
                    double bond,
                    double valence,
                    double similarity_score,
                    double signal,
                    double symmetry,
                    std::uint64_t seed = 0);

// Ordered names used by the causal-symmetry model.
const std::vector<std::string>& causal_order();

} // namespace chemistry
} // namespace sleela

#endif
