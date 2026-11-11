/*
 * trade_models.h - Three cross-asset trading models, as pure C math.
 *
 *   1. Arbitrage      : same asset, two venues, price gap (DEX->CEX, or
 *                       regional listing -> premium relist).
 *   2. Market making  : be the instant buyer and seller; earn the spread,
 *                       bear the risk of inventory that does not turn over.
 *   3. Refurbishment  : buy raw/distressed, improve, sell into a higher tier;
 *                       the result is a distribution of outcomes, not one.
 *
 * The models are asset-agnostic: a token and a mid-century dresser differ only
 * in the numbers (fees, holding cost, days held, fill probability).
 *
 * Every function is PURE: no I/O, no network, no clock, no global state. It
 * does not fetch prices and it does not place orders. Callers supply quotes
 * (from wherever they legitimately obtain them) and receive an evaluation.
 *
 * Care taken (fail closed):
 *   - every input must be finite and inside a stated range, else an error
 *     status is returned and the output struct is left untouched;
 *   - amounts are capped at TM_MAX_AMOUNT so products cannot overflow;
 *   - results are re-checked for finiteness before being returned.
 *
 * Models that involve chance (loss, completion, winning a race, a sale
 * happening) report EXPECTED values: total_cost and expected_proceeds are
 * probability-weighted, so a single real trade will land above or below them.
 *
 * Costs are modelled explicitly so a "profitable-looking" spread that fees,
 * slippage, storage and time would consume is reported honestly as a loss.
 *
 * This is arithmetic, not financial advice. Outputs are only as good as the
 * inputs.
 */
#ifndef ADVANTAGE_TRADE_MODELS_H
#define ADVANTAGE_TRADE_MODELS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TM_MAX_AMOUNT 1e12     /* cap on prices, quantities and cost totals */
#define TM_MAX_OUTCOMES 16     /* cap on refurbishment outcome branches     */
#define TM_MAX_LOTS 256        /* cap on items in one aggregation           */
#define TM_MAX_COMPONENTS 64   /* cap on parts in one synthetic bundle      */
#define TM_PROB_TOLERANCE 1e-6 /* outcome probabilities must sum to 1 +/- this */
#define TM_MAX_HOLD_DAYS 36500.0

typedef enum {
    TM_OK = 0,
    TM_ERR_ARG = -1,   /* NULL pointer or count out of range              */
    TM_ERR_RANGE = -2, /* a value is non-finite or outside its valid range */
    TM_ERR_PROB = -3,  /* outcome probabilities do not form a distribution */
    TM_ERR_OVERFLOW = -4 /* a computed result was not finite              */
} tm_status;

/* Common evaluation result (all three models). Money units are the caller's. */
typedef struct {
    double total_cost;        /* everything paid: purchase, fees, slippage,
                                 fixed and holding costs                     */
    double expected_proceeds; /* what is expected back, net of selling costs */
    double expected_profit;   /* expected_proceeds - total_cost              */
    double return_on_cost;    /* expected_profit / total_cost (0 if no cost) */
    double hold_days;         /* time capital is tied up                     */
} tm_result;

/* ---- 1. Arbitrage ------------------------------------------------------ */
typedef struct {
    double buy_price;           /* per unit, venue A                     (> 0) */
    double sell_price;          /* per unit, venue B                     (> 0) */
    double quantity;            /* units                                 (> 0) */
    double buy_fee_rate;        /* fraction of buy notional          [0, 1)    */
    double sell_fee_rate;       /* fraction of sell notional         [0, 1)    */
    double slippage_rate;       /* adverse fraction, applied per leg [0, 1)    */
    double fixed_costs;         /* gas / shipping / transfer, total      (>= 0) */
    double holding_cost_per_day;/* storage etc., total per day           (>= 0) */
    double hold_days;           /* days capital is tied up      [0, MAX_HOLD]  */
} tm_arbitrage_in;

tm_status tm_arbitrage(const tm_arbitrage_in *in, tm_result *out);

/* Lowest sell price per unit at which the arbitrage breaks even. */
tm_status tm_arbitrage_breakeven(const tm_arbitrage_in *in, double *out_price);

/* ---- 2. Market making / liquidity provision ---------------------------- */
typedef struct {
    double bid;                 /* price paid per unit                   (> 0) */
    double ask;                 /* price received per unit        (>= bid)     */
    double quantity;            /* units per round trip                  (> 0) */
    double fee_rate;            /* per side; negative = rebate     (-1, 1)     */
    double fill_probability;    /* P(round trip completes at the ask)  [0, 1]  */
    double unwind_loss_rate;    /* if NOT filled, fraction of bid lost
                                   when stuck inventory is dumped      [0, 1]  */
    double fixed_costs;         /* total                                 (>= 0) */
    double holding_cost_per_day;/* total per day                         (>= 0) */
    double hold_days;           /* expected days inventory is carried          */
} tm_marketmaking_in;

/* Symmetric quote around a fair value: bid = fair*(1-h), ask = fair*(1+h). */
tm_status tm_quote(double fair_value, double half_spread_rate,
                   double *out_bid, double *out_ask);

tm_status tm_marketmaking(const tm_marketmaking_in *in, tm_result *out);

/* ---- 3. Value-add refurbishment ---------------------------------------- */
typedef struct {
    double probability;         /* chance of this outcome            [0, 1]    */
    double unit_value;          /* sale value per unit in this case  (>= 0)    */
} tm_outcome;

typedef struct {
    double acquire_price;       /* per unit, raw / distressed            (> 0) */
    double quantity;            /* units                                 (> 0) */
    double per_unit_cost;       /* grading fee, parts, labour, per unit  (>= 0) */
    double fixed_costs;         /* total                                 (>= 0) */
    double sell_fee_rate;       /* fraction of sale value            [0, 1)    */
    double holding_cost_per_day;/* total per day                         (>= 0) */
    double hold_days;           /* days capital is tied up                      */
} tm_refurb_in;

/*
 * `outcomes` lists every branch (e.g. grade results, or restored / not
 * restorable). Probabilities must each be in [0,1] and sum to 1 within
 * TM_PROB_TOLERANCE, otherwise TM_ERR_PROB.
 */
tm_status tm_refurbishment(const tm_refurb_in *in,
                           const tm_outcome *outcomes, size_t n_outcomes,
                           tm_result *out);

/* ---- 4. Regional / geofenced arbitrage --------------------------------- */
/*
 * Buy where an asset is cheap, move it, sell where it is dear. The edge is
 * spatial; the friction is logistics. For tokens the "move" may be a capital-
 * control or conversion barrier (a regional premium); for furniture it is a
 * truck. Either way the move can fail or damage the goods.
 */
typedef struct {
    double buy_price;            /* per unit, origin region               (> 0) */
    double sell_price;           /* per unit, destination region          (> 0) */
    double quantity;             /* units                                 (> 0) */
    double buy_fee_rate;         /* fraction of buy notional          [0, 1)    */
    double sell_fee_rate;        /* fraction of sell notional         [0, 1)    */
    double transport_cost_per_unit; /* freight / conversion per unit      (>= 0) */
    double fixed_costs;          /* total                                 (>= 0) */
    double holding_cost_per_day; /* total per day in transit              (>= 0) */
    double transit_days;         /* days capital is tied up      [0, MAX_HOLD]  */
    double loss_probability;     /* P(damaged / blocked / not completed) [0, 1] */
    double salvage_rate;         /* if lost: fraction of buy value
                                    recovered                           [0, 1] */
} tm_regional_in;

tm_status tm_regional(const tm_regional_in *in, tm_result *out);

/* Regional premium = sell_price / buy_price - 1 (e.g. 0.05 = 5% premium). */
tm_status tm_regional_premium(double buy_price, double sell_price,
                              double *out_premium);

/* ---- 5. Fragmented supply aggregation ----------------------------------- */
/*
 * Buy many small items from many sellers, assemble a cohesive set, sell the
 * set at a premium. The risk is that the set never completes; unsold pieces
 * then fetch only their piecemeal value.
 */
typedef struct {
    double set_value;            /* sale value of the COMPLETE set        (>= 0) */
    double piecemeal_value_per_item; /* value of one item sold alone      (>= 0) */
    double completion_probability;   /* P(the set is completed)          [0, 1] */
    double per_item_cost;        /* sorting, cleaning, travel, per item   (>= 0) */
    double fixed_costs;          /* total                                 (>= 0) */
    double sell_fee_rate;        /* fraction of sale value            [0, 1)    */
    double holding_cost_per_day; /* storage, total per day                (>= 0) */
    double hold_days;            /* the "patience" time          [0, MAX_HOLD]  */
} tm_aggregation_in;

/* lot_prices: acquisition price of each item, 1..TM_MAX_LOTS entries (> 0). */
tm_status tm_aggregation(const tm_aggregation_in *in,
                         const double *lot_prices, size_t n_lots,
                         tm_result *out);

/* ---- 6. Informational asymmetry ----------------------------------------- */
/*
 * A listing is mislabeled, mispriced or misunderstood. One evaluation is one
 * CANDIDATE listing: you pay to look at it (attempt_cost) every time, you only
 * buy it if you win the race against other buyers (win_probability), and what
 * you bought is only valuable if your read of it was right
 * (genuine_probability).
 */
typedef struct {
    double listed_price;         /* what the seller asks                  (> 0) */
    double genuine_value;        /* sale value if your read is right      (>= 0) */
    double fallback_value;       /* sale value if your read is wrong      (>= 0) */
    double genuine_probability;  /* P(your read is right)             [0, 1]    */
    double win_probability;      /* P(you get it before competitors)  [0, 1]    */
    double buy_fee_rate;         /* fraction of listed price          [0, 1)    */
    double sell_fee_rate;        /* fraction of sale value            [0, 1)    */
    double verification_cost;    /* authentication / inspection, if won   (>= 0) */
    double attempt_cost;         /* scanning / OCR / tooling per candidate (>= 0) */
    double holding_cost_per_day; /* total per day                         (>= 0) */
    double hold_days;            /* days capital is tied up      [0, MAX_HOLD]  */
} tm_asymmetry_in;

tm_status tm_asymmetry(const tm_asymmetry_in *in, tm_result *out);

/* Apparent discount = 1 - listed_price / estimated_value (needs both > 0). */
tm_status tm_mispricing(double listed_price, double estimated_value,
                        double *out_discount);

/* ---- 7. Consignment / zero inventory ------------------------------------ */
/*
 * Match buyers and sellers without buying or storing anything: earn a
 * commission when it sells. Almost no capital is tied up, so "return on cost"
 * can look enormous -- the real inputs are your TIME (labor_cost) and the
 * counterparty risk (chargeback / fraud), which are modelled explicitly.
 */
typedef struct {
    double sale_price;           /* expected sale price of the item       (> 0) */
    double commission_rate;      /* your share of the sale price      [0, 1)    */
    double sale_probability;     /* P(it sells)                       [0, 1]    */
    double platform_fee_rate;    /* fees YOU bear, fraction of sale   [0, 1)    */
    double labor_cost;           /* staging, photos, listing, messaging;
                                    value your time                     (>= 0) */
    double fixed_costs;          /* total                                 (>= 0) */
    double chargeback_probability;/* P(sale reversed / defrauded | sold) [0, 1] */
    double chargeback_loss;      /* amount you lose if it is              (>= 0) */
    double days_to_sell;         /* days until it sells          [0, MAX_HOLD]  */
} tm_consignment_in;

tm_status tm_consignment(const tm_consignment_in *in, tm_result *out);

/* ---- 8. Distressed inventory & liquidation ------------------------------ */
/*
 * Buy from a seller who values speed and certainty over price (forced
 * liquidation, eviction clean-out, storage auction, flash crash), then sell
 * the contents on. Institutional liquidators compete for the best lots, so
 * the model carries a win probability and a cost to pursue every lot.
 */
typedef struct {
    double lot_price;            /* total price of the whole lot          (> 0) */
    double units;                /* items in the lot                      (> 0) */
    double expected_unit_value;  /* resale value per unit that sells      (>= 0) */
    double sell_through_rate;    /* fraction of units that sell at that
                                    value                               [0, 1] */
    double salvage_unit_value;   /* per unit for the rest (scrap / dump)  (>= 0) */
    double sell_fee_rate;        /* fraction of sale value            [0, 1)    */
    double per_unit_handling_cost;/* sorting, cleaning, listing, per unit (>= 0) */
    double disposal_cost_per_unsold_unit; /* hauling away what will not sell (>= 0) */
    double fixed_costs;          /* truck, crew, total                    (>= 0) */
    double holding_cost_per_day; /* storage, total per day                (>= 0) */
    double hold_days;            /* days to clear the lot        [0, MAX_HOLD]  */
    double win_probability;      /* P(you get the lot)                [0, 1]    */
    double pursuit_cost;         /* cost of chasing the lot, paid either way (>= 0) */
} tm_distressed_in;

tm_status tm_distressed(const tm_distressed_in *in, tm_result *out);

/* ---- 9. Virtual inventory / sell-before-source -------------------------- */
/*
 * Take the customer's payment first and source the item afterwards. The edge
 * is that the customer's money funds the purchase; the risk is that you cannot
 * fulfil (item gone, price jumped, source refuses you) after being paid.
 *
 * Because the sale funds the purchase, "return on cost" here is measured on
 * total outlay, NOT on your own capital. Your capital at risk is mainly
 * fixed_costs plus the failure branch (failure_loss), which is why the
 * failure probability is the most important number in this model.
 *
 * This model assumes an HONEST sale: the buyer is told it is fulfilled to
 * order with a stated lead time. It makes no allowance for misrepresentation.
 */
typedef struct {
    double sale_price;           /* per unit, charged to your customer    (> 0) */
    double source_price;         /* per unit, quoted by the source now    (> 0) */
    double quantity;             /* units                                 (> 0) */
    double sale_fee_rate;        /* your sales channel's fee          [0, 1)    */
    double source_fee_rate;      /* fee on the purchase               [0, 1)    */
    double shipping_cost;        /* total per order                       (>= 0) */
    double price_drift_rate;     /* adverse move in source price between
                                    sale and purchase                   [0, 1)    */
    double fulfillment_failure_probability; /* P(cannot fulfil)       [0, 1]    */
    double failure_loss;         /* what you lose if you cannot: non-refundable
                                    fees, penalties, goodwill, total    (>= 0) */
    double fixed_costs;          /* total                                 (>= 0) */
    double days_to_fulfil;       /* sale -> delivery             [0, MAX_HOLD]  */
} tm_virtual_in;

tm_status tm_virtual(const tm_virtual_in *in, tm_result *out);

/* ---- 10. Synthetic bundling (assemble / disassemble) -------------------- */
/*
 * Turn one thing into many (break up a collection: sell the rare parts to
 * specialists, melt the rest) or many into one (assemble mismatched parts into
 * a finished piece). Both are "pay for inputs, transform at a cost, sell
 * outputs", so one structure covers them. Wrap/unwrap of a token is the
 * degenerate case: one output, tiny transform cost, small spread.
 */
typedef struct {
    double sale_value;           /* value of this output if it sells      (>= 0) */
    double sale_probability;     /* P(it sells at that value)         [0, 1]    */
    double fallback_value;       /* value if it does not (e.g. melt)      (>= 0) */
} tm_component;

typedef struct {
    double input_cost;           /* total paid for the inputs             (> 0) */
    double fixed_costs;          /* tools, setup, total                   (>= 0) */
    double per_output_cost;      /* labor / hardware / grading, per output (>= 0) */
    double waste_rate;           /* fraction of output value lost to
                                    material waste or defects           [0, 1)    */
    double sell_fee_rate;        /* fraction of sale value            [0, 1)    */
    double holding_cost_per_day; /* total per day                         (>= 0) */
    double hold_days;            /* days including assembly      [0, MAX_HOLD]  */
} tm_synthetic_in;

/* outputs: 1..TM_MAX_COMPONENTS entries. */
tm_status tm_synthetic(const tm_synthetic_in *in,
                       const tm_component *outputs, size_t n_outputs,
                       tm_result *out);

/* ---- 11. Event-driven momentum / hype cycle ------------------------------ */
/*
 * Enter on a news or trend shock and exit while demand is still elevated. The
 * profit is entirely in the exit-price distribution and in whether you can exit
 * at all; both are the caller's honest estimates. Hype decays, so time held is
 * a cost, not a neutral input. High failure rates are normal in this model:
 * thin or manipulated markets can leave you holding something you cannot sell
 * (exit_failure_probability), often at near-zero recovery.
 */
typedef struct {
    double entry_price;          /* per unit                              (> 0) */
    double quantity;             /* units                                 (> 0) */
    double buy_fee_rate;         /* fraction of entry notional        [0, 1)    */
    double sell_fee_rate;        /* fraction of exit notional         [0, 1)    */
    double entry_slippage_rate;  /* adverse fill on the way in        [0, 1)    */
    double exit_slippage_rate;   /* adverse fill on the way out       [0, 1)    */
    double fixed_costs;          /* total                                 (>= 0) */
    double holding_cost_per_day; /* total per day                         (>= 0) */
    double hold_days;            /* planned holding time         [0, MAX_HOLD]  */
    double exit_failure_probability; /* P(cannot exit at all)         [0, 1]    */
    double stuck_recovery_rate;  /* if stuck: fraction of entry value
                                    recovered                           [0, 1]    */
} tm_momentum_in;

/* exit_prices: branches that start with tm_outcome{probability, exit price per
 * unit}; probabilities must sum to 1 (they describe the exits that WERE
 * possible; the failure to exit is separate). */
tm_status tm_momentum(const tm_momentum_in *in,
                      const tm_outcome *exit_prices, size_t n_exit_prices,
                      tm_result *out);

/* Excess price above baseline remaining after `days`, given a half-life:
 * remaining = initial_excess * 0.5^(days / half_life_days). */
tm_status tm_hype_decay(double initial_excess, double half_life_days,
                        double days, double *out_remaining);

/* ---- Bridge to the advantage core -------------------------------------- */
/*
 * Express an evaluated trade as an advantage: reward = expected proceeds,
 * baseline = total cost plus the opportunity cost of tying that capital up
 * for hold_days at `cost_of_capital_annual` (a fraction, e.g. 0.05, in
 * [0, 10]). The advantage is what the trade earns beyond simply having paid
 * its costs AND beyond what the capital could have earned elsewhere.
 */
tm_status tm_baseline(const tm_result *r, double cost_of_capital_annual,
                      double *out_baseline);
tm_status tm_advantage(const tm_result *r, double cost_of_capital_annual,
                       double *out_advantage);

const char *tm_strerror(tm_status st);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADVANTAGE_TRADE_MODELS_H */
