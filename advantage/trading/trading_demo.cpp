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
#include "rate_budget.hpp"
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

tm_distressed_in dis(double lotPrice, double units, double unitValue,
                     double sellThrough, double salvage, double sellFee,
                     double handling, double disposal, double fixed,
                     double holdCost, double days, double winP, double pursuit) {
    tm_distressed_in in{};
    in.lot_price = lotPrice;
    in.units = units;
    in.expected_unit_value = unitValue;
    in.sell_through_rate = sellThrough;
    in.salvage_unit_value = salvage;
    in.sell_fee_rate = sellFee;
    in.per_unit_handling_cost = handling;
    in.disposal_cost_per_unsold_unit = disposal;
    in.fixed_costs = fixed;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    in.win_probability = winP;
    in.pursuit_cost = pursuit;
    return in;
}

tm_virtual_in vir(double sale, double source, double qty, double saleFee,
                  double sourceFee, double shipping, double drift,
                  double failP, double failLoss, double fixed, double days) {
    tm_virtual_in in{};
    in.sale_price = sale;
    in.source_price = source;
    in.quantity = qty;
    in.sale_fee_rate = saleFee;
    in.source_fee_rate = sourceFee;
    in.shipping_cost = shipping;
    in.price_drift_rate = drift;
    in.fulfillment_failure_probability = failP;
    in.failure_loss = failLoss;
    in.fixed_costs = fixed;
    in.days_to_fulfil = days;
    return in;
}

tm_synthetic_in syn(double input, double fixed, double perOutput, double waste,
                    double sellFee, double holdCost, double days) {
    tm_synthetic_in in{};
    in.input_cost = input;
    in.fixed_costs = fixed;
    in.per_output_cost = perOutput;
    in.waste_rate = waste;
    in.sell_fee_rate = sellFee;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    return in;
}

tm_momentum_in mom(double entry, double qty, double buyFee, double sellFee,
                   double entrySlip, double exitSlip, double fixed,
                   double holdCost, double days, double failP, double recovery) {
    tm_momentum_in in{};
    in.entry_price = entry;
    in.quantity = qty;
    in.buy_fee_rate = buyFee;
    in.sell_fee_rate = sellFee;
    in.entry_slippage_rate = entrySlip;
    in.exit_slippage_rate = exitSlip;
    in.fixed_costs = fixed;
    in.holding_cost_per_day = holdCost;
    in.hold_days = days;
    in.exit_failure_probability = failP;
    in.stuck_recovery_rate = recovery;
    return in;
}

// n identical outputs.
std::vector<tm_component> repeat(std::size_t n, tm_component c) {
    return std::vector<tm_component>(n, c);
}

void printRateBudgetWalkthrough() {
    std::cout << "== Rate budget (offline walkthrough; no requests are made) ==\n";
    struct Case {
        const char* label;
        std::map<std::string, std::string> headers;
        double now;
    };
    const std::vector<Case> cases = {
        {"40 of 100 left, 30s to reset",
         {{"x-ratelimit-limit", "100"}, {"x-ratelimit-remaining", "40"},
          {"x-ratelimit-reset", "30"}}, 0},
        {"2 left (== reserve): wait out window",
         {{"x-ratelimit-limit", "100"}, {"x-ratelimit-remaining", "2"},
          {"x-ratelimit-reset", "30"}}, 0},
        {"reset sent as epoch time",
         {{"x-ratelimit-limit", "100"}, {"x-ratelimit-remaining", "40"},
          {"x-ratelimit-reset", "1700000030"}}, 1700000000},
        {"server says Retry-After: 5",
         {{"x-ratelimit-remaining", "40"}, {"x-ratelimit-reset", "30"},
          {"retry-after", "5"}}, 0},
        {"Retry-After: 600 (over the 60s cap)",
         {{"retry-after", "600"}}, 0},
        {"malformed remaining",
         {{"x-ratelimit-remaining", "lots"}}, 0},
        {"Retry-After as an HTTP date",
         {{"retry-after", "Wed, 21 Oct 2026 07:28:00 GMT"}}, 0},
    };
    for (const auto& c : cases) {
        RateBudget rb;
        std::string err;
        std::cout << "  " << std::left << std::setw(40) << c.label;
        if (!rb.observe(c.headers, c.now, err)) {
            std::cout << "REJECTED: " << err << "\n";
            continue;
        }
        Delay d = rb.next();
        std::cout << std::right << "wait " << std::setw(9) << d.ms << " ms"
                  << (d.mustStop ? "  -> STOP (exceeds cap)" : "") << "\n";
    }
    std::cout << "\n";
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
    std::cout << "\n== Niche model comparison ==\n";
    for (const auto& m : nicheModelMatrix()) {
        std::cout << "  " << m.model << "\n"
                  << "    capital efficiency: " << m.capitalEfficiency
                  << " | velocity: " << m.velocityOfCapital << "\n"
                  << "    friction: " << m.operationalFriction
                  << " | main threat: " << m.mainThreat << "\n";
    }
    std::cout << "\n";
    printRateBudgetWalkthrough();
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

    store.sources.push_back(makeSource(
        "distressed-digital", "Distressed Inventory: Forced-Liquidation Dips (tokens)",
        "Buy assets dumped into a thin book by forced liquidations; sell once "
        "the market recovers.",
        {{"The market recovers rather than keeps falling.", false},
         {"You can exit near the recovered price without moving it.", true},
         {"You get filled before better-equipped liquidators.", true}},
        {"Size to what the book can absorb on the way out, not the way in.",
         "A dip that keeps dipping is a loss, not a discount."}));
    store.sources.push_back(makeSource(
        "distressed-physical", "Distressed Inventory: Truckload Clean-Outs",
        "Buy whole lots from office movers, evictions or storage auctions; "
        "liquidate the contents piece by piece.",
        {{"The lot contains what it was described to contain.", true},
         {"Sell-through holds at the assumed rate.", true},
         {"You can compete with institutional liquidators for good lots.", true}},
        {"Budget the unsold remainder and its disposal before you bid.",
         "Handling per piece, not the lot price, decides the margin."}));
    store.sources.push_back(makeSource(
        "virtual-digital", "Virtual Inventory: Dealer-Fulfilled Coin Sales",
        "List graded coins at a markup to live pricing; buy from a wholesale "
        "dealer through its API once the customer has paid.",
        {{"The dealer's API terms permit this use and you hold valid access.", true},
         {"Stock is still available when you go to buy.", true},
         {"The customer is told the coin is fulfilled to order.", true}},
        {"The failure branch, not the markup, sets the risk.",
         "Price drift between sale and purchase comes straight out of margin."}));
    store.sources.push_back(makeSource(
        "virtual-physical", "Virtual Inventory: Pick-Up-After-Payment Resale",
        "Resell standard items you do not yet hold, collecting them from the "
        "original seller after your buyer has paid.",
        {{"Your listing is honest about who you are and the lead time.", true},
         {"The original item is still available at pick-up.", false},
         {"The sales channel's rules allow this arrangement.", false}},
        {"If the original sells first, you owe a refund and bear the penalty.",
         "A platform ban ends the model, not just one sale."}));
    store.sources.push_back(makeSource(
        "synthetic-digital", "Synthetic Bundling: Wrap / Unwrap and Estate Breakups",
        "Mint or burn wrapped tokens across chains, or split a coin estate "
        "into collector pieces and melt value.",
        {{"The wrap/unwrap path is sound and the contracts are free of exploitable bugs.", false},
         {"Collectors pay the assumed price for the rare pieces.", true},
         {"Melting or selling the remainder is lawful where you operate.", true}},
        {"Fees and gas are the whole margin on a wrap spread.",
         "Check local law before melting coins: some are protected."}));
    store.sources.push_back(makeSource(
        "synthetic-physical", "Synthetic Bundling: Bespoke Assembled Pieces",
        "Combine salvaged components with clean hardware into a finished piece "
        "sold to a specialist buyer.",
        {{"Component supply is steady and priced as assumed.", true},
         {"The finished piece sells to a premium buyer.", true},
         {"Labor stays within the estimate.", true}},
        {"Labor is usually the largest unmeasured cost.",
         "Count the waste: not every slab and base is usable."}));
    store.sources.push_back(makeSource(
        "momentum-digital", "Event Momentum: News-Driven Micro-Cap Entries",
        "Enter a newly listed or newly newsworthy token and exit while "
        "demand is still elevated.",
        {{"There will be enough exit liquidity when you sell.", false},
         {"You are not the exit liquidity for someone else's promotion.", false},
         {"Fees and slippage on thin books are known.", true}},
        {"Hype decays on a half-life; time held is a cost.",
         "Being unable to exit is the dominant risk, not a footnote."}));
    store.sources.push_back(makeSource(
        "momentum-physical", "Event Momentum: Trend-Driven Vintage Buying",
        "Buy local vintage pieces in a style that is going viral, before "
        "casual sellers reprice them.",
        {{"The trend lasts long enough to resell.", true},
         {"Buyers outside the trend still exist as a fallback.", true},
         {"Your sourcing does not breach any platform's rules.", true}},
        {"Resell early; a trend that has peaked reprices against you.",
         "Hold fewer pieces than you can store and sell."}));

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

    // Distressed: lot, units, unitValue, sellThrough, salvage, sellFee, handling, disposal, fixed, holdCost/day, days, winP, pursuit
    addAny("distressed-digital", "dip recovers", distressed(dis(70000, 100000, .95, .9, .60, .002, 0, 0, 20, 0, .5, .6, 5)));
    addAny("distressed-digital", "partial recovery", distressed(dis(70000, 100000, .80, .9, .60, .002, 0, 0, 20, 0, .5, .6, 5)));
    addAny("distressed-digital", "keeps falling", distressed(dis(70000, 100000, .68, .9, .60, .002, 0, 0, 20, 0, .5, .6, 5)));
    addAny("distressed-digital", "thin exit book", distressed(dis(70000, 100000, .95, .5, .50, .002, 0, 0, 20, 0, .5, .6, 5)));
    addAny("distressed-physical", "40 pieces, 75% sell", distressed(dis(800, 40, 70, .75, 5, .13, 12, 8, 250, 3, 60, .7, 20)));
    addAny("distressed-physical", "better stock ($90)", distressed(dis(800, 40, 90, .75, 5, .13, 12, 8, 250, 3, 60, .7, 20)));
    addAny("distressed-physical", "weak stock, 60% sell", distressed(dis(800, 40, 55, .60, 5, .13, 12, 8, 250, 3, 60, .7, 20)));

    // Virtual: sale, source, qty, saleFee, sourceFee, shipping, drift, failP, failLoss, fixed, days
    addAny("virtual-digital", "3% failure", virtualInventory(vir(1200, 1100, 1, .03, 0, 15, .004, .03, 60, 4, 3)));
    addAny("virtual-digital", "15% failure", virtualInventory(vir(1200, 1100, 1, .03, 0, 15, .004, .15, 60, 4, 3)));
    addAny("virtual-digital", "price drifts 2%", virtualInventory(vir(1200, 1100, 1, .03, 0, 15, .02, .05, 60, 4, 3)));
    addAny("virtual-digital", "thin markup, 8% fail", virtualInventory(vir(1150, 1100, 1, .03, 0, 15, .004, .08, 150, 4, 3)));
    addAny("virtual-physical", "20% gone first", virtualInventory(vir(250, 50, 1, .13, 0, 40, 0, .20, 25, 3, 4)));
    addAny("virtual-physical", "50% gone first", virtualInventory(vir(250, 50, 1, .13, 0, 40, 0, .50, 25, 3, 4)));
    addAny("virtual-physical", "50% gone, costly refund", virtualInventory(vir(250, 50, 1, .13, 0, 40, 0, .50, 100, 3, 4)));
    addAny("virtual-physical", "low price, 30% gone", virtualInventory(vir(120, 50, 1, .13, 0, 40, 0, .30, 60, 3, 4)));

    // Synthetic: input, fixed, perOutput, waste, sellFee, holdCost/day, days (+ outputs {saleValue, saleP, fallback})
    addAny("synthetic-digital", "wrap spread 0.4%", synthetic(syn(100000, 30, 20, 0, .001, 0, .05), {{100400, .95, 99000}}));
    addAny("synthetic-digital", "wrap spread 0.1%", synthetic(syn(100000, 30, 20, 0, .001, 0, .05), {{100100, .95, 99000}}));
    addAny("synthetic-digital", "wrap spread 0.8%", synthetic(syn(100000, 30, 20, 0, .001, 0, .05), {{100800, .95, 99000}}));
    auto estate = [](double rareValue, double rareP) {
        std::vector<tm_component> v = repeat(25, {rareValue, rareP, 30});
        v.push_back({0, 0, 600});  // generic damaged coins: melt value only
        return v;
    };
    addAny("synthetic-digital", "estate: rare $220", synthetic(syn(3000, 50, 8, 0, .12, .5, 20), estate(220, .85)));
    addAny("synthetic-digital", "estate: rare $140", synthetic(syn(3000, 50, 8, 0, .12, .5, 20), estate(140, .85)));
    addAny("synthetic-digital", "estate: 60% of rare sell", synthetic(syn(3000, 50, 8, 0, .12, .5, 20), estate(220, .60)));
    addAny("synthetic-physical", "industrial table $1100", synthetic(syn(270, 40, 90, .10, .12, .5, 21), {{1100, .8, 250}}));
    addAny("synthetic-physical", "table $800", synthetic(syn(270, 40, 90, .10, .12, .5, 21), {{800, .8, 250}}));
    addAny("synthetic-physical", "$800, 25% waste", synthetic(syn(270, 40, 90, .25, .12, .5, 21), {{800, .8, 250}}));
    addAny("synthetic-physical", "$700, 50% sells", synthetic(syn(270, 40, 90, .10, .12, .5, 21), {{700, .5, 250}}));

    // Momentum: entry, qty, buyFee, sellFee, entrySlip, exitSlip, fixed, holdCost/day, days, failP, recovery (+ exits {prob, price})
    addAny("momentum-digital", "average event, 25% stuck", momentum(mom(.02, 500000, .003, .003, .02, .05, 5, 0, .2, .25, .05), {{.3, .05}, {.4, .022}, {.3, .008}}));
    addAny("momentum-digital", "same, 10% stuck", momentum(mom(.02, 500000, .003, .003, .02, .05, 5, 0, .2, .10, .05), {{.3, .05}, {.4, .022}, {.3, .008}}));
    addAny("momentum-digital", "strong event, 20% stuck", momentum(mom(.02, 500000, .003, .003, .02, .05, 5, 0, .2, .20, .05), {{.4, .06}, {.4, .03}, {.2, .01}}));
    addAny("momentum-digital", "weak event, 15% stuck", momentum(mom(.02, 500000, .003, .003, .02, .05, 5, 0, .2, .15, .05), {{.2, .03}, {.4, .018}, {.4, .006}}));
    addAny("momentum-physical", "trend holds", momentum(mom(90, 10, 0, .13, 0, .03, 40, .4, 25, .10, .5), {{.3, 200}, {.4, 150}, {.3, 100}}));
    addAny("momentum-physical", "trend fades", momentum(mom(90, 10, 0, .13, 0, .03, 40, .4, 25, .10, .5), {{.1, 200}, {.3, 120}, {.6, 80}}));
    addAny("momentum-physical", "early in the trend", momentum(mom(90, 10, 0, .13, 0, .03, 40, .4, 25, .10, .5), {{.5, 220}, {.3, 160}, {.2, 110}}));

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

    Checked<double> hd = hypeDecay(1.0, 7.0, 14.0);
    if (hd.ok) {
        std::cout << "Hype decay: a 100% premium with a 7-day half-life leaves "
                  << std::setprecision(0) << hd.value * 100.0
                  << "% after 14 days.\n";
    }

    // ---- counsel ----------------------------------------------------------
    std::cout << "\n== Counsel: ranked trading approaches ==\n"
              << "(score = mean advantage x confidence x trust. Advantage is per 1 unit of\n"
                 " capital deployed and net of its opportunity cost, so tokens and\n"
                 " furniture are comparable regardless of trade size.)\n\n";
    renderCounsel(std::cout, counsel(store));

    return failures == 0 ? 0 : 1;
}
