// trade_examples.cpp
//
// Consumes the advantage C/C++ model as a whole:
//   * advantage::value / advantage::mean  (C core advantage_value/advantage_mean
//     via the C++ wrapper in advantage.hpp) to score each trade,
//   * advantage::sha256_hex (transport/digest) to content-address each
//     presumed internet source, the same way the acquisition layer keys data.
//
// It then builds:
//   1. Positive trade examples from PRESUMED / ILLUSTRATIVE internet sources
//      (not live feeds -- labelled as presumed throughout),
//   2. A relevance/importance sequence for four audiences toward a Trading
//      Method -- Traders, Consumers, Helpers, Healers -- using the framing
//      TV = a Source, Entertainment = a Captain, Health = a Brand,
//   3. Eight models ranked A..D where D = most "Cement" (foundational,
//      concrete) and A = most "Brand" (elevated, brand-level).
//
// Build:  see advantage/examples/Makefile  (links advantage.c + digest.cpp)

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#if defined(_WIN32)
#  include <io.h>
#  define ADV_ISATTY(fd) _isatty(fd)
#else
#  include <unistd.h>
#  define ADV_ISATTY(fd) isatty(fd)
#endif

#include "../advantage.hpp"            // advantage::value / advantage::mean
#include "../transport/digest.h"       // advantage::sha256_hex

namespace {

// Advantage(TM) brand line, in the customary red + classic (ANSI SGR 31), the
// same treatment SLeeLa and Sleelvac use for their product identity. Presentation
// only and TTY-gated: emitted solely to an interactive terminal, so piped or
// captured output stays plain and machine-parsable. fd 1 = stdout.
void brandAdvantage(std::ostream& o, int fd) {
    if (ADV_ISATTY(fd) == 0) return;
    o << "\033[31m" << "Advantage\u2122" << "\033[0m\n";
}

// ----- helpers --------------------------------------------------------------

std::string sourceId(const std::string& url) {
    // Content-address the presumed source string with the real digest code,
    // then show a short prefix as its stable object id.
    std::string h = advantage::sha256_hex(
        reinterpret_cast<const uint8_t*>(url.data()), url.size());
    return h.empty() ? "<nohash>" : h.substr(0, 12);
}

// A positive trade example drawn from a presumed internet source.
struct Trade {
    std::string name;
    std::string presumedSource;      // illustrative URL, NOT a live feed
    std::vector<double> rewards;     // realized per-leg returns (presumed)
    std::vector<double> baselines;   // the do-nothing / benchmark per leg
    double weight;
};

// ----- 1. trade examples ----------------------------------------------------

std::vector<Trade> buildTrades() {
    return {
        {"index-ladder",
         "https://presumed.example/markets/sp500-weekly.json",
         {1.8, 2.2, 1.1, 2.6}, {1.0, 1.0, 1.0, 1.0}, 1.0},
        {"staples-basket",
         "https://presumed.example/retail/consumer-staples.csv",
         {0.9, 1.3, 1.5, 1.2}, {0.8, 0.8, 0.8, 0.8}, 0.9},
        {"aid-logistics",
         "https://presumed.example/ngo/relief-supply-futures.json",
         {1.2, 0.7, 1.9, 1.4}, {0.9, 0.9, 0.9, 0.9}, 0.8},
        {"care-devices",
         "https://presumed.example/health/med-device-demand.json",
         {2.0, 1.6, 2.4, 1.1}, {1.1, 1.1, 1.1, 1.1}, 1.0},
    };
}

void printTrades(const std::vector<Trade>& trades) {
    std::cout << "== 1. Positive trade examples (presumed internet sources) ==\n";
    std::cout << "   (sources are illustrative placeholders, content-addressed "
                 "with the real SHA-256 digest code)\n";
    for (const auto& t : trades) {
        double adv = advantage::mean(t.rewards, t.baselines, t.weight);
        // A single headline advantage for the best leg vs its baseline too.
        double best = advantage::value(
            *std::max_element(t.rewards.begin(), t.rewards.end()),
            t.baselines.empty() ? 0.0 : t.baselines.front(), t.weight);
        std::cout << "  - " << std::left << std::setw(16) << t.name
                  << " src[" << sourceId(t.presumedSource) << "]"
                  << "  mean-advantage=" << std::fixed << std::setprecision(3) << adv
                  << "  best-leg-advantage=" << best
                  << (adv > 0.0 ? "  (POSITIVE)" : "")
                  << "\n";
    }
    std::cout << "\n";
}

// ----- 2. audience relevance sequence --------------------------------------

struct Audience {
    std::string name;
    std::string framing;     // the presumed framing preset
    int relevance;           // 1..10 toward the Trading Method
    std::string role;        // its place in the sequence
};

void printAudiences() {
    std::cout << "== 2. Relevance & importance sequence toward a Trading Method ==\n";
    std::cout << "   framing: TV = a Source, Entertainment = a Captain, "
                 "Health = a Brand\n";
    // Ordered sequence: who the method reaches first and how strongly.
    std::vector<Audience> seq = {
        {"Traders",   "Entertainment as a Captain (leads the play)", 10,
         "originate and steer the method"},
        {"Consumers", "TV as a Source (receive the signal)",          8,
         "receive and validate demand"},
        {"Helpers",   "TV as a Source + Captain's relay",             6,
         "distribute and support execution"},
        {"Healers",   "Health as a Brand (restore and vouch)",        7,
         "repair trust and certify outcomes"},
    };
    int step = 1;
    for (const auto& a : seq) {
        std::cout << "  " << step++ << ". " << std::left << std::setw(10) << a.name
                  << " relevance=" << a.relevance << "/10"
                  << "  [" << a.framing << "]\n"
                  << "       role: " << a.role << "\n";
    }
    std::cout << "\n";
}

// ----- 3. eight models ranked A..D (D = most Cement, A = most Brand) --------

struct Model {
    std::string name;
    char tier;            // 'A'..'D'
    std::string nature;   // Cement <-> Brand characterization
    double rewardProxy;   // a presumed strength used with the advantage math
    double baseline;
};

// Tier semantics (explicit):
//   D = most Cement : foundational, concrete, load-bearing infrastructure.
//   C = structural  : concrete made useful; plumbing above the foundation.
//   B = market- facing : productized, sold, visible.
//   A = most Brand  : elevated, identity-level, what the audience trusts by name.
const char* tierMeaning(char t) {
    switch (t) {
        case 'D': return "most Cement  (foundational / concrete)";
        case 'C': return "structural   (concrete made useful)";
        case 'B': return "market-facing (productized / sold)";
        case 'A': return "most Brand   (elevated / identity-level)";
    }
    return "unknown";
}

std::vector<Model> buildModels() {
    // Two models per tier, eight total.
    return {
        // D - most Cement
        {"settlement-rail",     'D', "clearing & settlement bedrock",     1.05, 1.0},
        {"custody-vault",       'D', "asset custody / cold store",         1.08, 1.0},
        // C - structural
        {"liquidity-plumbing",  'C', "order routing & matching",           1.25, 1.0},
        {"risk-ledger",         'C', "position & exposure accounting",     1.30, 1.0},
        // B - market-facing
        {"retail-storefront",   'B', "consumer trading surface",           1.60, 1.0},
        {"advisory-desk",       'B', "guided / helped execution",          1.55, 1.0},
        // A - most Brand
        {"trust-mark",          'A', "the name audiences trade on",        2.10, 1.0},
        {"care-seal",           'A', "health-grade assurance brand",       2.00, 1.0},
    };
}

void printModels(const std::vector<Model>& models) {
    std::cout << "== 3. Eight models, ranked A..D "
                 "(D = most Cement, A = most Brand) ==\n";
    // Print grouped D -> A so the "Cement up to Brand" progression is visible.
    for (char tier : std::string("DCBA")) {
        std::cout << "  Tier " << tier << " - " << tierMeaning(tier) << ":\n";
        for (const auto& m : models) {
            if (m.tier != tier) continue;
            double adv = advantage::value(m.rewardProxy, m.baseline, 1.0);
            std::cout << "      - " << std::left << std::setw(18) << m.name
                      << " advantage=" << std::fixed << std::setprecision(3) << adv
                      << "  (" << m.nature << ")\n";
        }
    }
    std::cout << "\n";
}

}  // namespace

int main() {
    brandAdvantage(std::cout, 1);   // red Advantage(TM) brand, TTY-gated
    std::cout << "=== advantage trade examples (consuming the C/C++ model) ===\n\n";

    auto trades = buildTrades();
    printTrades(trades);
    printAudiences();
    auto models = buildModels();
    printModels(models);

    // A whole-of-model roll-up: mean advantage across all trades, proving the
    // C core is doing the arithmetic end to end.
    std::vector<double> allR, allB;
    for (const auto& t : trades)
        for (size_t i = 0; i < t.rewards.size() && i < t.baselines.size(); ++i) {
            allR.push_back(t.rewards[i]);
            allB.push_back(t.baselines[i]);
        }
    double overall = advantage::mean(allR, allB, 1.0);
    std::cout << "== roll-up ==\n";
    std::cout << "  overall mean-advantage across all presumed trades = "
              << std::fixed << std::setprecision(3) << overall
              << (overall > 0.0 ? "  (net POSITIVE)" : "") << "\n";

    return 0;
}
