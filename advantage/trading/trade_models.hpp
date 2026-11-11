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

inline Checked<tm_result> regional(const tm_regional_in& in) {
    tm_result r{};
    return detail::make(tm_regional(&in, &r), r);
}

// Regional premium = sell/buy - 1 (0.05 = 5%).
inline Checked<double> regionalPremium(double buyPrice, double sellPrice) {
    double p = 0.0;
    return detail::make(tm_regional_premium(buyPrice, sellPrice, &p), p);
}

inline Checked<tm_result> aggregation(const tm_aggregation_in& in,
                                      const std::vector<double>& lotPrices) {
    tm_result r{};
    return detail::make(
        tm_aggregation(&in, lotPrices.data(), lotPrices.size(), &r), r);
}

// One candidate listing (see trade_models.h for the semantics).
inline Checked<tm_result> asymmetry(const tm_asymmetry_in& in) {
    tm_result r{};
    return detail::make(tm_asymmetry(&in, &r), r);
}

// Apparent discount = 1 - listed/estimated (0.6 = listed 60% below estimate).
inline Checked<double> mispricing(double listedPrice, double estimatedValue) {
    double d = 0.0;
    return detail::make(tm_mispricing(listedPrice, estimatedValue, &d), d);
}

inline Checked<tm_result> consignment(const tm_consignment_in& in) {
    tm_result r{};
    return detail::make(tm_consignment(&in, &r), r);
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

// ---- the comparison matrix for the four advanced models, as data -----------

struct ModelProfile {
    const char* model;
    const char* operationalLeverage;
    const char* scalability;
    const char* infrastructure;
    const char* primaryFriction;
};

// Descriptive, not numeric. The infrastructure column names what each model
// needs *conceptually*; this library supplies only the arithmetic, not the
// scrapers, OCR, inventory system or escrow platform.
const std::vector<ModelProfile>& advancedModelMatrix();

}  // namespace trading
}  // namespace advantage

#endif  // ADVANTAGE_TRADE_MODELS_HPP
