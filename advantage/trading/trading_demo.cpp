// trading_demo.cpp - Evaluate the three trading models across digital and
// physical assets, record each result as evidence, and let `counsel` rank the
// six approaches.
//
// ALL NUMBERS BELOW ARE ILLUSTRATIVE. They are not market data, quotes, fee
// schedules or forecasts. Replace them with your own before drawing any
// conclusion. Nothing here fetches prices or places orders.
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../counsel.hpp"
#include "trade_models.hpp"

using namespace advantage;
using namespace advantage::trading;

namespace {

constexpr double kCostOfCapital = 0.08;  // illustrative annual opportunity cost

struct Scenario {
    std::string sourceId;
    std::string label;
    std::function<Checked<tm_result>()> evaluate;
};

SourceAdvantage makeSource(
    const std::string& id, const std::string& name, const std::string& desc,
    const std::vector<std::pair<std::string, bool>>& assumptions,
    const std::vector<std::string>& knowledge) {
    SourceAdvantage s;
    s.id = id;
    s.kind = "trading";
    s.name = name;
    s.description = desc;
    for (const auto& a : assumptions) {
        s.assumptions.push_back(a.first);
        s.assumptionHeld.push_back(a.second);
    }
    s.knowledge = knowledge;
    return s;
}

tm_arbitrage_in arb(double buy, double sell, double qty, double buyFee,
                    double sellFee, double slip, double fixed, double holdCost,
                    double days) {
    tm_arbitrage_in in{};
    in.buy_price = buy;
    in.sell_price = sell;
    in.quantity = qty;
    in.buy_fee_rate = buyFee;
    in.sell_fee_rate = sellFee;
    in.slippage_rate = slip;
    in.fixed_costs = fixed;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    return in;
}

tm_marketmaking_in mm(double bid, double ask, double qty, double fee,
                      double fill, double unwind, double fixed, double holdCost,
                      double days) {
    tm_marketmaking_in in{};
    in.bid = bid;
    in.ask = ask;
    in.quantity = qty;
    in.fee_rate = fee;
    in.fill_probability = fill;
    in.unwind_loss_rate = unwind;
    in.fixed_costs = fixed;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    return in;
}

tm_refurb_in refurb(double acquire, double qty, double perUnit, double fixed,
                    double sellFee, double holdCost, double days) {
    tm_refurb_in in{};
    in.acquire_price = acquire;
    in.quantity = qty;
    in.per_unit_cost = perUnit;
    in.fixed_costs = fixed;
    in.sell_fee_rate = sellFee;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    return in;
}

void printProfiles() {
    std::cout << "== Strategic trade-offs ==\n";
    for (const auto& p : profiles()) {
        std::cout << "  " << p.assetClass << " (" << p.examples << ")\n"
                  << "    capital: " << p.capitalNeeded
                  << " | liquidity: " << p.liquidity
                  << " | speed: " << p.executionSpeed << "\n"
                  << "    major risks: " << p.majorRisks << "\n";
    }
    std::cout << "\n";
}

}  // namespace

int main() {
    printProfiles();

    // ---- the six approaches (model x asset class) -------------------------
    AdvantageStore store;
    store.version = "trading-demo";
    store.sources.push_back(makeSource(
        "arbitrage-digital", "Digital Arbitrage (DEX -> CEX)",
        "Same token, two venues: buy where it is cheap, sell where it is dear.",
        {{"Both venues can settle within the time the gap lasts.", true},
         {"Smart contracts and bridges are free of exploitable bugs.", false},
         {"The gap exceeds fees, slippage and gas.", true}},
        {"Compute the break-even sell price before acting.",
         "A gap that narrows mid-transfer turns a gain into a loss."}));
    store.sources.push_back(makeSource(
        "arbitrage-physical", "Physical Arbitrage (regional -> premium relist)",
        "Buy underpriced local listings; relist on premium venues.",
        {{"Listings can be gathered within each platform's terms of service.",
          false},
         {"The item matches its description and is not misrepresented.", true},
         {"Shipping and handling are priced in.", true}},
        {"Shipping, fees and storage days are the margin's silent thieves.",
         "Verify authenticity and condition before buying."}));
    store.sources.push_back(makeSource(
        "marketmaking-digital", "Digital Market Making",
        "Quote both sides on an order book or AMM and earn the spread.",
        {{"Fill probability holds at the quoted spread.", true},
         {"Inventory can be unwound near the bid when quotes go stale.", true},
         {"Fees are known and stable.", true}},
        {"Wider spreads raise margin and lower fill odds.",
         "Unfilled inventory is where market making loses."}));
    store.sources.push_back(makeSource(
        "marketmaking-physical", "Physical Market Making (cash for estate lots)",
        "Be the instant buyer for people clearing a house; resell at market.",
        {{"Lots resell at the assumed prices within the assumed time.", true},
         {"Stuck inventory can be dumped to a dealer at about half cost.", false},
         {"Sellers have clear title to what they sell.", true}},
        {"Speed and certainty are what the seller is paying you for.",
         "Storage days accrue against every unsold lot."}));
    store.sources.push_back(makeSource(
        "refurbishment-coins", "Refurbishment: Graded Coins",
        "Buy raw numismatic coins, have them professionally graded, sell slabs.",
        {{"Raw coins are authentic.", false},
         {"Grade outcomes follow the assumed distribution.", true},
         {"Grading fees and turnaround are as quoted.", true}},
        {"The outcome is a distribution; judge it by expected value, not best case.",
         "Most raw coins grade low; a few carry the profit."}));
    store.sources.push_back(makeSource(
        "refurbishment-furniture", "Refurbishment: Mid-Century Furniture",
        "Repair or upcycle distressed pieces to unlock premium value.",
        {{"Restoration cost stays within the estimate.", true},
         {"A premium buyer exists for the finished piece.", true},
         {"Original finishes and veneers are recoverable.", true}},
        {"Budget the not-restorable branch up front.",
         "Labour is usually the largest unmeasured cost."}));

    // ---- illustrative scenarios -------------------------------------------
    // Arbitrage inputs: buy, sell, qty, buyFee, sellFee, slip, fixed, holdCost/day, days
    // Market making:    bid, ask, qty, fee, fillProb, unwindLoss, fixed, holdCost/day, days
    // Refurbishment:    acquire, qty, perUnitCost, fixed, sellFee, holdCost/day, days
    std::vector<Scenario> scenarios;
    auto addArb = [&](const std::string& id, const std::string& label,
                      tm_arbitrage_in in) {
        scenarios.push_back({id, label, [in] { return arbitrage(in); }});
    };
    auto addMm = [&](const std::string& id, const std::string& label,
                     tm_marketmaking_in in) {
        scenarios.push_back({id, label, [in] { return marketMaking(in); }});
    };
    auto addRf = [&](const std::string& id, const std::string& label,
                     tm_refurb_in in, std::vector<tm_outcome> out) {
        scenarios.push_back(
            {id, label, [in, out] { return refurbishment(in, out); }});
    };

    addArb("arbitrage-digital", "gap 1.2%",  arb(1.000, 1.012, 5000, .003, .001, .001, 8, 0, 0.01));
    addArb("arbitrage-digital", "gap 1.5%",  arb(1.000, 1.015, 5000, .003, .001, .001, 8, 0, 0.01));
    addArb("arbitrage-digital", "gap 0.9%",  arb(1.000, 1.009, 5000, .003, .001, .001, 8, 0, 0.01));
    addArb("arbitrage-digital", "gap closed to 0.4%", arb(1.000, 1.004, 5000, .003, .001, .001, 8, 0, 0.01));

    addArb("arbitrage-physical", "dresser 80 -> 160",  arb(80, 160, 1, 0, .13, .02, 25, .5, 14));
    addArb("arbitrage-physical", "dresser 120 -> 170", arb(120, 170, 1, 0, .13, .02, 25, .5, 14));
    addArb("arbitrage-physical", "dresser 60 -> 150",  arb(60, 150, 1, 0, .13, .02, 25, .5, 14));
    addArb("arbitrage-physical", "dresser 90 -> 140",  arb(90, 140, 1, 0, .13, .02, 25, .5, 14));

    addMm("marketmaking-digital", "tight spread, fills 90%", mm(.9985, 1.0015, 20000, .0005, .90, .01, 2, 0, 0.01));
    addMm("marketmaking-digital", "tight spread, fills 70%", mm(.9985, 1.0015, 20000, .0005, .70, .01, 2, 0, 0.01));
    addMm("marketmaking-digital", "wide spread, fills 85%",  mm(.9970, 1.0030, 20000, .0005, .85, .01, 2, 0, 0.01));

    addMm("marketmaking-physical", "estate lot, resells 80%", mm(1000, 4000, 1, .05, .80, .50, 150, 6, 45));
    addMm("marketmaking-physical", "estate lot, resells 50%", mm(1000, 4000, 1, .05, .50, .50, 150, 6, 45));
    addMm("marketmaking-physical", "estate lot, weak demand", mm(1000, 2500, 1, .05, .30, .50, 150, 6, 45));

    addRf("refurbishment-coins", "good luck",  refurb(40, 50, 35, 120, .12, 1, 90),
          {{.10, 600}, {.30, 120}, {.45, 55}, {.15, 25}});
    addRf("refurbishment-coins", "poor luck",  refurb(40, 50, 35, 120, .12, 1, 90),
          {{.02, 600}, {.18, 120}, {.50, 55}, {.30, 25}});
    addRf("refurbishment-coins", "dearer buy", refurb(30, 50, 35, 120, .12, 1, 90),
          {{.05, 400}, {.25, 100}, {.50, 55}, {.20, 25}});
    addRf("refurbishment-coins", "cheaper buy", refurb(25, 50, 35, 120, .12, 1, 90),
          {{.08, 500}, {.32, 110}, {.40, 55}, {.20, 25}});

    addRf("refurbishment-furniture", "straightforward", refurb(150, 1, 120, 30, .10, .5, 30),
          {{.6, 600}, {.3, 300}, {.1, 100}});
    addRf("refurbishment-furniture", "labour-heavy", refurb(220, 1, 200, 30, .10, .5, 30),
          {{.4, 600}, {.4, 300}, {.2, 100}});
    addRf("refurbishment-furniture", "cheap find", refurb(100, 1, 80, 30, .10, .5, 21),
          {{.5, 550}, {.35, 300}, {.15, 100}});

    // ---- evaluate, record evidence ----------------------------------------
    std::cout << "== Scenario evaluations (ILLUSTRATIVE inputs) ==\n"
              << std::fixed << std::setprecision(2);
    int failures = 0;
    for (const auto& sc : scenarios) {
        Checked<tm_result> r = sc.evaluate();
        std::cout << "  " << std::left << std::setw(24) << sc.sourceId
                  << std::setw(24) << sc.label;
        if (!r.ok) {
            std::cout << "REJECTED: " << r.error << "\n";
            ++failures;
            continue;
        }
        Checked<double> adv = advantageOf(r.value, kCostOfCapital);
        std::string err;
        if (!adv.ok || !addEvidence(store, sc.sourceId, r.value, kCostOfCapital, err,
                                           Scale::PerUnitCost)) {
            std::cout << "ERROR: " << (adv.ok ? err : adv.error) << "\n";
            ++failures;
            continue;
        }
        std::cout << std::right << "cost " << std::setw(9) << r.value.total_cost
                  << "  proceeds " << std::setw(9) << r.value.expected_proceeds
                  << "  profit " << std::setw(8) << r.value.expected_profit
                  << "  advantage " << std::setw(8) << adv.value << "\n";
    }

    // One worked break-even, since it is the first question to ask.
    Checked<double> be = arbitrageBreakeven(arb(1.000, 1.012, 5000, .003, .001, .001, 8, 0, 0.01));
    if (be.ok) {
        std::cout << "\nBreak-even sell price for the digital arbitrage above: "
                  << std::setprecision(4) << be.value << " (buy at 1.0000)\n";
    }

    // ---- counsel ----------------------------------------------------------
    std::cout << "\n== Counsel: ranked trading approaches ==\n"
              << "(score = mean advantage x confidence x trust. Advantage is per 1 unit of\n"
                 " capital deployed and net of its opportunity cost, so tokens and\n"
                 " furniture are comparable regardless of trade size.)\n\n";
    renderCounsel(std::cout, counsel(store));

    return failures == 0 ? 0 : 1;
}
