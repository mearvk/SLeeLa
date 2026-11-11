// trade_models.hpp - C++ layer over the C trading models.
//
// Adds, on top of trade_models.h:
//   * Checked<T>: a result that is either a value or a human-readable error
//     (no exceptions, no sentinel numbers);
//   * the three models behind C++ signatures (std::vector for outcomes);
//   * the strategic trade-off table as data (AssetProfile);
//   * a bridge that records an evaluated trade as evidence in an
//     AdvantageStore, so `counsel` can rank trading approaches.
//
// Still pure and offline: nothing here fetches prices or places orders.
#ifndef ADVANTAGE_TRADE_MODELS_HPP
#define ADVANTAGE_TRADE_MODELS_HPP

#include <string>
#include <vector>

#include "../advantage_store.hpp"
#include "trade_models.h"

namespace advantage {
namespace trading {

// Either a value or an error message. Check ok before using value.
template <class T>
struct Checked {
    bool ok = false;
    T value{};
    std::string error;
};

namespace detail {
template <class T>
Checked<T> make(tm_status st, const T& v) {
    Checked<T> c;
    if (st == TM_OK) {
        c.ok = true;
        c.value = v;
    } else {
        c.error = tm_strerror(st);
    }
    return c;
}
}  // namespace detail

inline Checked<tm_result> arbitrage(const tm_arbitrage_in& in) {
    tm_result r{};
    return detail::make(tm_arbitrage(&in, &r), r);
}

inline Checked<double> arbitrageBreakeven(const tm_arbitrage_in& in) {
    double p = 0.0;
    return detail::make(tm_arbitrage_breakeven(&in, &p), p);
}

inline Checked<tm_result> marketMaking(const tm_marketmaking_in& in) {
    tm_result r{};
    return detail::make(tm_marketmaking(&in, &r), r);
}

struct Quote {
    double bid = 0.0;
    double ask = 0.0;
};

inline Checked<Quote> quote(double fairValue, double halfSpreadRate) {
    Quote q;
    return detail::make(tm_quote(fairValue, halfSpreadRate, &q.bid, &q.ask), q);
}

inline Checked<tm_result> refurbishment(const tm_refurb_in& in,
                                        const std::vector<tm_outcome>& outcomes) {
    tm_result r{};
    return detail::make(
        tm_refurbishment(&in, outcomes.data(), outcomes.size(), &r), r);
}

// Advantage of an evaluated trade after the opportunity cost of its capital.
inline Checked<double> advantageOf(const tm_result& r, double costOfCapital) {
    double a = 0.0;
    return detail::make(tm_advantage(&r, costOfCapital, &a), a);
}

// How an evaluated trade is expressed as evidence.
//   Absolute    : money units as evaluated. Fine within ONE approach, but a
//                 large trade then outweighs a small one.
//   PerUnitCost : divided by the trade's total cost, so reward and baseline
//                 are "per 1 unit of capital deployed". Use this to compare
//                 approaches of very different size (tokens vs furniture).
enum class Scale { Absolute, PerUnitCost };

// Record an evaluated trade as one <observation> on the source with `id`:
//   reward = expected proceeds, baseline = cost + opportunity cost of capital
// (both divided by total cost under Scale::PerUnitCost).
// Returns false and fills `error` if the source is absent, the rate is
// invalid, a per-unit scale is requested for a zero-cost trade, or the source
// is already at the observation limit. The store changes only on success.
bool addEvidence(AdvantageStore& store, const std::string& id,
                 const tm_result& r, double costOfCapital, std::string& error,
                 Scale scale = Scale::Absolute);

// ---- the strategic trade-offs, as data ------------------------------------

struct AssetProfile {
    const char* assetClass;      // "Crypto / Digital" or "Physical"
    const char* examples;
    const char* capitalNeeded;
    const char* liquidity;
    const char* executionSpeed;
    const char* majorRisks;
};

// The two rows of the cross-asset comparison. Descriptive, not numeric: use
// them to choose realistic inputs (hold_days, holding costs, fill odds).
const std::vector<AssetProfile>& profiles();

}  // namespace trading
}  // namespace advantage

#endif  // ADVANTAGE_TRADE_MODELS_HPP
