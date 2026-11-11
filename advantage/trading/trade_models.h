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
