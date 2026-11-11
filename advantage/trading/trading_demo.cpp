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

tm_regional_in reg(double buy, double sell, double qty, double buyFee,
                   double sellFee, double perUnitTransport, double fixed,
                   double holdCost, double days, double lossP, double salvage) {
    tm_regional_in in{};
    in.buy_price = buy;
    in.sell_price = sell;
    in.quantity = qty;
    in.buy_fee_rate = buyFee;
    in.sell_fee_rate = sellFee;
    in.transport_cost_per_unit = perUnitTransport;
    in.fixed_costs = fixed;
    in.holding_cost_per_day = holdCost;
    in.transit_days = days;
    in.loss_probability = lossP;
    in.salvage_rate = salvage;
    return in;
}

tm_aggregation_in agg(double setValue, double piecemeal, double completeP,
                      double perItem, double fixed, double sellFee,
                      double holdCost, double days) {
    tm_aggregation_in in{};
    in.set_value = setValue;
    in.piecemeal_value_per_item = piecemeal;
    in.completion_probability = completeP;
    in.per_item_cost = perItem;
    in.fixed_costs = fixed;
    in.sell_fee_rate = sellFee;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    return in;
}

tm_asymmetry_in asy(double listed, double genuine, double fallback,
                    double genuineP, double winP, double buyFee, double sellFee,
                    double verify, double attempt, double holdCost, double days) {
    tm_asymmetry_in in{};
    in.listed_price = listed;
    in.genuine_value = genuine;
    in.fallback_value = fallback;
    in.genuine_probability = genuineP;
    in.win_probability = winP;
    in.buy_fee_rate = buyFee;
    in.sell_fee_rate = sellFee;
    in.verification_cost = verify;
    in.attempt_cost = attempt;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    return in;
}

tm_consignment_in con(double sale, double commission, double saleP,
                      double platformFee, double labor, double fixed,
                      double cbP, double cbLoss, double days) {
    tm_consignment_in in{};
    in.sale_price = sale;
    in.commission_rate = commission;
    in.sale_probability = saleP;
    in.platform_fee_rate = platformFee;
    in.labor_cost = labor;
    in.fixed_costs = fixed;
    in.chargeback_probability = cbP;
    in.chargeback_loss = cbLoss;
    in.days_to_sell = days;
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
    std::cout << "\n== Advanced model comparison ==\n";
    for (const auto& m : advancedModelMatrix()) {
        std::cout << "  " << m.model << "\n"
                  << "    leverage: " << m.operationalLeverage
                  << " | scalability: " << m.scalability << "\n"
                  << "    infrastructure: " << m.infrastructure
                  << " | friction: " << m.primaryFriction << "\n";
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

    store.sources.push_back(makeSource(
        "regional-digital", "Regional Arbitrage: Country Premium (tokens)",
        "Buy a token where it is cheap; sell on a country-exclusive venue "
        "where local demand or capital controls hold the price higher.",
        {{"The premium persists long enough to cross the barrier.", true},
         {"Moving funds across the border is lawful for you and permitted "
          "by both venues.", false},
         {"Conversion and withdrawal costs are known.", true}},
        {"A premium that cannot be crossed is a price, not an opportunity.",
         "Check the legality of the route before the arithmetic."}));
    store.sources.push_back(makeSource(
        "regional-physical", "Regional Arbitrage: Estate Sale -> Urban Resale",
        "Source premium furniture where supply is plentiful; sell where it is "
        "scarce and dense demand pays for pre-assembled quality.",
        {{"The destination market pays the assumed price.", true},
         {"Pieces survive transport without material damage.", true},
         {"Truck, labour and fuel costs are as estimated.", true}},
        {"Logistics, not price, decides the margin.",
         "Price the damaged-in-transit branch before you load the truck."}));
    store.sources.push_back(makeSource(
        "aggregation-coins", "Supply Aggregation: Sorted Coin Sets",
        "Buy small unsorted lots, sort by mint mark or era, sell verified sets.",
        {{"A matching set can actually be assembled from available lots.", true},
         {"Sorting and attribution are done correctly.", true},
         {"Lots are authentic and lawfully sold.", false}},
        {"The premium is paid for patience and completeness.",
         "An incomplete set sells for little more than its parts."}));
    store.sources.push_back(makeSource(
        "aggregation-furniture", "Supply Aggregation: Matched Furniture Sets",
        "Collect matching pieces one by one from different sellers into a "
        "complete set for designers who pay a premium.",
        {{"Matching pieces keep appearing at acceptable prices.", true},
         {"Designers will pay the assumed premium for a complete set.", true},
         {"Storage stays cheap while the set is assembled.", true}},
        {"Buy toward the set you can finish, not the piece that is cheapest.",
         "Storage days are the cost of patience."}));
    store.sources.push_back(makeSource(
        "asymmetry-digital", "Informational Asymmetry: Mispriced Listings (tokens)",
        "Find listings mispriced by error or panic, faster than other buyers.",
        {{"Your read of the mispricing is more often right than wrong.", false},
         {"You can win the race against faster, better-funded bots.", false},
         {"The listing is real and the asset is what it claims to be.", true}},
        {"Count the candidates you looked at and lost, not just the one you won.",
         "Most apparent bargains are noise or traps."}));
    store.sources.push_back(makeSource(
        "asymmetry-physical", "Informational Asymmetry: Mislabeled Furniture",
        "Spot listings whose title hides what the piece really is.",
        {{"You can recognise the real thing from photos and a description.", true},
         {"Few other buyers are looking at the same weak listings.", true},
         {"Authentication is available before you resell.", true}},
        {"A typo is only an edge until others search for it too.",
         "Authenticate before you pay the premium price in your head."}));
    store.sources.push_back(makeSource(
        "consignment-digital", "Consignment: Local OTC / Escrow Desk",
        "Match local cash buyers with digital-asset holders for a fee, holding "
        "no inventory of your own.",
        {{"Escrow is sound and funds cannot be reversed after release.", true},
         {"Counterparties are who they say they are.", false},
         {"The service is lawful where you operate.", false}},
        {"Your product is trust; price it as a risk, not a convenience.",
         "One reversed transfer can erase many commissions."}));
    store.sources.push_back(makeSource(
        "consignment-physical", "Consignment: Staging and Sales for Movers",
        "Photograph, list and negotiate for people clearing a home; split the "
        "proceeds on pickup; never store the goods.",
        {{"Owners keep their word on price, pickup and payment.", true},
         {"Your listings and profile convert at the assumed rate.", true},
         {"Disputes after pickup are rare and bounded.", true}},
        {"Your capital is your time; value it.",
         "Client acquisition, not arithmetic, is the usual limit."}));

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

    // Generic adder for any model: the caller has already evaluated it.
    auto addAny = [&](const std::string& id, const std::string& label,
                      Checked<tm_result> r) {
        scenarios.push_back({id, label, [r] { return r; }});
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

    // Regional:    buy, sell, qty, buyFee, sellFee, transport/unit, fixed, holdCost/day, days, lossP, salvage
    addAny("regional-digital", "premium 5%", regional(reg(1.000, 1.050, 10000, .001, .0025, .002, 15, 0, 0.1, .03, .95)));
    addAny("regional-digital", "premium 2%", regional(reg(1.000, 1.020, 10000, .001, .0025, .002, 15, 0, 0.1, .03, .95)));
    addAny("regional-digital", "premium faded to 0.5%", regional(reg(1.000, 1.005, 10000, .001, .0025, .002, 15, 0, 0.1, .03, .95)));
    addAny("regional-digital", "5% but 10% blocked", regional(reg(1.000, 1.050, 10000, .001, .0025, .002, 15, 0, 0.1, .10, .95)));

    addAny("regional-physical", "300 -> 700 x4", regional(reg(300, 700, 4, 0, .10, 60, 120, 2, 3, .05, .5)));
    addAny("regional-physical", "300 -> 520 x4", regional(reg(300, 520, 4, 0, .10, 60, 120, 2, 3, .05, .5)));
    addAny("regional-physical", "300 -> 450 x4", regional(reg(300, 450, 4, 0, .10, 60, 120, 2, 3, .05, .5)));
    addAny("regional-physical", "300 -> 700, damage 15%", regional(reg(300, 700, 4, 0, .10, 60, 120, 2, 3, .15, .5)));

    // Aggregation: setValue, piecemeal/item, completeP, perItem, fixed, sellFee, holdCost/day, days (+ lot prices)
    const std::vector<double> coinLots = {45, 60, 38, 52, 70, 41, 55, 49};
    const std::vector<double> chairLots = {85, 110, 95, 70, 120, 90};
    addAny("aggregation-coins", "set 1100, 60% completes", aggregation(agg(1100, 60, .60, 12, 40, .12, 1, 60), coinLots));
    addAny("aggregation-coins", "set 1100, 30% completes", aggregation(agg(1100, 60, .30, 12, 40, .12, 1, 60), coinLots));
    addAny("aggregation-coins", "set 700, 60% completes", aggregation(agg(700, 60, .60, 12, 40, .12, 1, 60), coinLots));
    addAny("aggregation-furniture", "set 2400, 70% completes", aggregation(agg(2400, 140, .70, 15, 60, .10, .5, 45), chairLots));
    addAny("aggregation-furniture", "set 2400, 40% completes", aggregation(agg(2400, 140, .40, 15, 60, .10, .5, 45), chairLots));
    addAny("aggregation-furniture", "set 1400, 70% completes", aggregation(agg(1400, 140, .70, 15, 60, .10, .5, 45), chairLots));

    // Asymmetry (per candidate): listed, genuine, fallback, genuineP, winP, buyFee, sellFee, verify, attempt, holdCost/day, days
    addAny("asymmetry-digital", "bots win most races", asymmetry(asy(2000, 3500, 1500, .15, .10, .003, .002, 5, .5, 0, 0.01)));
    addAny("asymmetry-digital", "better read, faster", asymmetry(asy(2000, 3500, 1500, .40, .30, .003, .002, 5, .5, 0, 0.01)));
    addAny("asymmetry-digital", "middling edge", asymmetry(asy(2000, 3500, 1500, .25, .20, .003, .002, 5, .5, 0, 0.01)));
    addAny("asymmetry-physical", "'old wooden table'", asymmetry(asy(60, 900, 70, .05, .80, 0, .13, 10, .2, .5, 20)));
    addAny("asymmetry-physical", "same, 2% are genuine", asymmetry(asy(60, 900, 70, .02, .80, 0, .13, 10, .2, .5, 20)));
    addAny("asymmetry-physical", "'midcenury' typo", asymmetry(asy(120, 1200, 150, .25, .50, 0, .13, 10, .2, .5, 20)));

    // Consignment: sale, commission, saleP, platformFee, labor, fixed, chargebackP, chargebackLoss, days
    addAny("consignment-digital", "1%, weak escrow", consignment(con(5000, .01, .90, 0, 15, 2, .02, 5000, 0.1)));
    addAny("consignment-digital", "1%, sound escrow", consignment(con(5000, .01, .90, 0, 15, 2, .002, 5000, 0.1)));
    addAny("consignment-digital", "2%, sound escrow", consignment(con(5000, .02, .90, 0, 15, 2, .002, 5000, 0.1)));
    addAny("consignment-digital", "2%, weak escrow", consignment(con(5000, .02, .90, 0, 15, 2, .02, 5000, 0.1)));
    addAny("consignment-physical", "1200 sale, 75% sells", consignment(con(1200, .35, .75, .10, 90, 10, .03, 200, 21)));
    addAny("consignment-physical", "1200 sale, 50% sells", consignment(con(1200, .35, .50, .10, 90, 10, .03, 200, 21)));
    addAny("consignment-physical", "600 sale, 75% sells", consignment(con(600, .35, .75, .10, 90, 10, .03, 200, 21)));

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
