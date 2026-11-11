// trade_models.cpp - See trade_models.hpp.
#include "trade_models.hpp"

namespace advantage {
namespace trading {

bool addEvidence(AdvantageStore& store, const std::string& id,
                 const tm_result& r, double costOfCapital, std::string& error,
                 Scale scale) {
    SourceAdvantage* target = nullptr;
    for (auto& s : store.sources) {
        if (s.id == id) { target = &s; break; }
    }
    if (target == nullptr) {
        error = "no source with id '" + id + "'";
        return false;
    }
    if (target->evidence.size() >= kMaxObservationsPerSource) {
        error = "source '" + id + "' is at the observation limit";
        return false;
    }

    double baseline = 0.0;
    tm_status st = tm_baseline(&r, costOfCapital, &baseline);
    if (st != TM_OK) {
        error = tm_strerror(st);
        return false;
    }

    Observation ob;
    ob.reward = r.expected_proceeds;
    ob.baseline = baseline;
    if (scale == Scale::PerUnitCost) {
        if (!(r.total_cost > 0.0)) {
            error = "cannot scale a zero-cost trade per unit of cost";
            return false;
        }
        ob.reward /= r.total_cost;
        ob.baseline /= r.total_cost;
    }
    target->evidence.push_back(ob);
    error.clear();
    return true;
}

const std::vector<AssetProfile>& profiles() {
    static const std::vector<AssetProfile> table = {
        {"Crypto / Digital",
         "tokens and coins on DEX / CEX venues",
         "low to medium",
         "ultra-high (seconds)",
         "microseconds to seconds",
         "high volatility; smart-contract bugs"},
        {"Physical",
         "furniture; numismatic coins",
         "low to medium",
         "low (days to weeks)",
         "hours to days",
         "storage costs; fraud; shipping fees"},
    };
    return table;
}

const std::vector<ModelProfile>& advancedModelMatrix() {
    static const std::vector<ModelProfile> table = {
        {"Regional Arbitrage", "high (spatial friction)", "medium",
         "transport + region-specific data sources",
         "logistics and moving costs"},
        {"Supply Aggregation", "very high (patience premium)", "medium",
         "inventory management",
         "sorting time and storage"},
        {"Informational Asymmetry", "extreme (alpha generation)", "high",
         "keyword search + image/text recognition",
         "high competition from other buyers and bots"},
        {"Consignment / Zero-Inventory", "maximum capital efficiency", "high",
         "escrow and listing management",
         "trust and client acquisition"},
    };
    return table;
}

}  // namespace trading
}  // namespace advantage
