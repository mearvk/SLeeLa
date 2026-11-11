/*
 * trade_models.c - Implementation of the three trading models.
 * See trade_models.h for the contract.
 */
#include "trade_models.h"

#include <math.h>

#include "../advantage.h"

/* ---- validation helpers ------------------------------------------------ */

static int is_amount(double v)   /* finite, 0 <= v <= cap */
{
    return isfinite(v) && v >= 0.0 && v <= TM_MAX_AMOUNT;
}

static int is_positive(double v) /* finite, 0 < v <= cap */
{
    return isfinite(v) && v > 0.0 && v <= TM_MAX_AMOUNT;
}

static int is_rate(double v)     /* finite, 0 <= v < 1 */
{
    return isfinite(v) && v >= 0.0 && v < 1.0;
}

static int is_prob(double v)     /* finite, 0 <= v <= 1 */
{
    return isfinite(v) && v >= 0.0 && v <= 1.0;
}

static int is_days(double v)
{
    return isfinite(v) && v >= 0.0 && v <= TM_MAX_HOLD_DAYS;
}

/* Fill and re-check a result; leaves *out untouched unless everything is
 * finite. */
static tm_status finish(double total_cost, double proceeds, double hold_days,
                        tm_result *out)
{
    tm_result r;

    if (!isfinite(total_cost) || !isfinite(proceeds) ||
        total_cost < 0.0 || proceeds < 0.0) {
        return TM_ERR_OVERFLOW;
    }
    r.total_cost = total_cost;
    r.expected_proceeds = proceeds;
    r.expected_profit = proceeds - total_cost;
    r.return_on_cost = total_cost > 0.0 ? r.expected_profit / total_cost : 0.0;
    r.hold_days = hold_days;
    if (!isfinite(r.expected_profit) || !isfinite(r.return_on_cost)) {
        return TM_ERR_OVERFLOW;
    }
    *out = r;
    return TM_OK;
}

/* ---- 1. Arbitrage ------------------------------------------------------ */

static tm_status arbitrage_check(const tm_arbitrage_in *in)
{
    if (in == NULL) return TM_ERR_ARG;
    if (!is_positive(in->buy_price) || !is_positive(in->sell_price) ||
        !is_positive(in->quantity)) {
        return TM_ERR_RANGE;
    }
    if (!is_rate(in->buy_fee_rate) || !is_rate(in->sell_fee_rate) ||
        !is_rate(in->slippage_rate)) {
        return TM_ERR_RANGE;
    }
    /* The sell leg must keep a positive share of its notional. */
    if (in->sell_fee_rate + in->slippage_rate >= 1.0) return TM_ERR_RANGE;
    if (!is_amount(in->fixed_costs) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->hold_days)) {
        return TM_ERR_RANGE;
    }
    return TM_OK;
}

static double arbitrage_cost(const tm_arbitrage_in *in)
{
    return in->quantity * in->buy_price *
               (1.0 + in->buy_fee_rate + in->slippage_rate) +
           in->fixed_costs + in->holding_cost_per_day * in->hold_days;
}

tm_status tm_arbitrage(const tm_arbitrage_in *in, tm_result *out)
{
    tm_status st = arbitrage_check(in);
    if (st != TM_OK) return st;
    if (out == NULL) return TM_ERR_ARG;

    return finish(arbitrage_cost(in),
                  in->quantity * in->sell_price *
                      (1.0 - in->sell_fee_rate - in->slippage_rate),
                  in->hold_days, out);
}

tm_status tm_arbitrage_breakeven(const tm_arbitrage_in *in, double *out_price)
{
    double v;
    tm_status st = arbitrage_check(in);
    if (st != TM_OK) return st;
    if (out_price == NULL) return TM_ERR_ARG;

    v = arbitrage_cost(in) /
        (in->quantity * (1.0 - in->sell_fee_rate - in->slippage_rate));
    if (!isfinite(v)) return TM_ERR_OVERFLOW;
    *out_price = v;
    return TM_OK;
}

/* ---- 2. Market making ------------------------------------------------- */

tm_status tm_quote(double fair_value, double half_spread_rate,
                   double *out_bid, double *out_ask)
{
    if (out_bid == NULL || out_ask == NULL) return TM_ERR_ARG;
    if (!is_positive(fair_value) || !is_rate(half_spread_rate)) {
        return TM_ERR_RANGE;
    }
    *out_bid = fair_value * (1.0 - half_spread_rate);
    *out_ask = fair_value * (1.0 + half_spread_rate);
    return TM_OK;
}

tm_status tm_marketmaking(const tm_marketmaking_in *in, tm_result *out)
{
    double buy_side, done, stuck, proceeds, cost;

    if (in == NULL || out == NULL) return TM_ERR_ARG;
    if (!is_positive(in->bid) || !is_positive(in->ask) ||
        !is_positive(in->quantity) || in->ask < in->bid) {
        return TM_ERR_RANGE;
    }
    /* fee_rate may be a rebate (negative) but must stay inside (-1, 1). */
    if (!isfinite(in->fee_rate) || in->fee_rate <= -1.0 || in->fee_rate >= 1.0) {
        return TM_ERR_RANGE;
    }
    if (!is_prob(in->fill_probability) || !is_prob(in->unwind_loss_rate)) {
        return TM_ERR_RANGE;
    }
    if (!is_amount(in->fixed_costs) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->hold_days)) {
        return TM_ERR_RANGE;
    }

    buy_side = in->quantity * in->bid * (1.0 + in->fee_rate);
    cost = buy_side + in->fixed_costs + in->holding_cost_per_day * in->hold_days;

    /* Completed round trip: sold at the ask. Stuck: dumped below the bid. */
    done  = in->quantity * in->ask * (1.0 - in->fee_rate);
    stuck = in->quantity * in->bid * (1.0 - in->unwind_loss_rate) *
            (1.0 - in->fee_rate);
    proceeds = in->fill_probability * done +
               (1.0 - in->fill_probability) * stuck;

    return finish(cost, proceeds, in->hold_days, out);
}

/* ---- 3. Refurbishment ------------------------------------------------- */

tm_status tm_refurbishment(const tm_refurb_in *in,
                           const tm_outcome *outcomes, size_t n_outcomes,
                           tm_result *out)
{
    double psum = 0.0, expected_value = 0.0, cost, proceeds;
    size_t i;

    if (in == NULL || outcomes == NULL || out == NULL) return TM_ERR_ARG;
    if (n_outcomes == 0 || n_outcomes > TM_MAX_OUTCOMES) return TM_ERR_ARG;
    if (!is_positive(in->acquire_price) || !is_positive(in->quantity) ||
        !is_amount(in->per_unit_cost) || !is_amount(in->fixed_costs) ||
        !is_rate(in->sell_fee_rate) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->hold_days)) {
        return TM_ERR_RANGE;
    }

    for (i = 0; i < n_outcomes; ++i) {
        if (!is_prob(outcomes[i].probability)) return TM_ERR_PROB;
        if (!is_amount(outcomes[i].unit_value)) return TM_ERR_RANGE;
        psum += outcomes[i].probability;
        expected_value += outcomes[i].probability * outcomes[i].unit_value;
    }
    if (fabs(psum - 1.0) > TM_PROB_TOLERANCE) return TM_ERR_PROB;

    cost = in->quantity * (in->acquire_price + in->per_unit_cost) +
           in->fixed_costs + in->holding_cost_per_day * in->hold_days;
    proceeds = in->quantity * expected_value * (1.0 - in->sell_fee_rate);

    return finish(cost, proceeds, in->hold_days, out);
}

/* ---- 4. Regional arbitrage ------------------------------------------- */

tm_status tm_regional_premium(double buy_price, double sell_price,
                              double *out_premium)
{
    if (out_premium == NULL) return TM_ERR_ARG;
    if (!is_positive(buy_price) || !is_positive(sell_price)) {
        return TM_ERR_RANGE;
    }
    *out_premium = sell_price / buy_price - 1.0;
    return TM_OK;
}

tm_status tm_regional(const tm_regional_in *in, tm_result *out)
{
    double cost, ok_proceeds, lost_proceeds, proceeds;

    if (in == NULL || out == NULL) return TM_ERR_ARG;
    if (!is_positive(in->buy_price) || !is_positive(in->sell_price) ||
        !is_positive(in->quantity)) {
        return TM_ERR_RANGE;
    }
    if (!is_rate(in->buy_fee_rate) || !is_rate(in->sell_fee_rate)) {
        return TM_ERR_RANGE;
    }
    if (!is_amount(in->transport_cost_per_unit) ||
        !is_amount(in->fixed_costs) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->transit_days)) {
        return TM_ERR_RANGE;
    }
    if (!is_prob(in->loss_probability) || !is_prob(in->salvage_rate)) {
        return TM_ERR_RANGE;
    }

    cost = in->quantity * in->buy_price * (1.0 + in->buy_fee_rate) +
           in->quantity * in->transport_cost_per_unit + in->fixed_costs +
           in->holding_cost_per_day * in->transit_days;
    ok_proceeds = in->quantity * in->sell_price * (1.0 - in->sell_fee_rate);
    lost_proceeds = in->quantity * in->buy_price * in->salvage_rate;
    proceeds = (1.0 - in->loss_probability) * ok_proceeds +
               in->loss_probability * lost_proceeds;

    return finish(cost, proceeds, in->transit_days, out);
}

/* ---- 5. Supply aggregation -------------------------------------------- */

tm_status tm_aggregation(const tm_aggregation_in *in,
                         const double *lot_prices, size_t n_lots,
                         tm_result *out)
{
    double purchases = 0.0, cost, expected_sale;
    size_t i;

    if (in == NULL || lot_prices == NULL || out == NULL) return TM_ERR_ARG;
    if (n_lots == 0 || n_lots > TM_MAX_LOTS) return TM_ERR_ARG;
    if (!is_amount(in->set_value) || !is_amount(in->piecemeal_value_per_item) ||
        !is_prob(in->completion_probability) ||
        !is_amount(in->per_item_cost) || !is_amount(in->fixed_costs) ||
        !is_rate(in->sell_fee_rate) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->hold_days)) {
        return TM_ERR_RANGE;
    }
    for (i = 0; i < n_lots; ++i) {
        if (!is_positive(lot_prices[i])) return TM_ERR_RANGE;
        purchases += lot_prices[i];
    }

    cost = purchases + (double)n_lots * in->per_item_cost + in->fixed_costs +
           in->holding_cost_per_day * in->hold_days;
    expected_sale = in->completion_probability * in->set_value +
                    (1.0 - in->completion_probability) *
                        (double)n_lots * in->piecemeal_value_per_item;

    return finish(cost, expected_sale * (1.0 - in->sell_fee_rate),
                  in->hold_days, out);
}

/* ---- 6. Informational asymmetry --------------------------------------- */

tm_status tm_mispricing(double listed_price, double estimated_value,
                        double *out_discount)
{
    if (out_discount == NULL) return TM_ERR_ARG;
    if (!is_positive(listed_price) || !is_positive(estimated_value)) {
        return TM_ERR_RANGE;
    }
    *out_discount = 1.0 - listed_price / estimated_value;
    return TM_OK;
}

tm_status tm_asymmetry(const tm_asymmetry_in *in, tm_result *out)
{
    double w, cost, value_if_won;

    if (in == NULL || out == NULL) return TM_ERR_ARG;
    if (!is_positive(in->listed_price) || !is_amount(in->genuine_value) ||
        !is_amount(in->fallback_value)) {
        return TM_ERR_RANGE;
    }
    if (!is_prob(in->genuine_probability) || !is_prob(in->win_probability)) {
        return TM_ERR_RANGE;
    }
    if (!is_rate(in->buy_fee_rate) || !is_rate(in->sell_fee_rate) ||
        !is_amount(in->verification_cost) || !is_amount(in->attempt_cost) ||
        !is_amount(in->holding_cost_per_day) || !is_days(in->hold_days)) {
        return TM_ERR_RANGE;
    }

    w = in->win_probability;
    /* You always pay to look; you pay to buy, verify and hold only if you win. */
    cost = in->attempt_cost +
           w * (in->listed_price * (1.0 + in->buy_fee_rate) +
                in->verification_cost +
                in->holding_cost_per_day * in->hold_days);
    value_if_won = in->genuine_probability * in->genuine_value +
                   (1.0 - in->genuine_probability) * in->fallback_value;

    return finish(cost, w * value_if_won * (1.0 - in->sell_fee_rate),
                  in->hold_days, out);
}

/* ---- 7. Consignment --------------------------------------------------- */

tm_status tm_consignment(const tm_consignment_in *in, tm_result *out)
{
    double p, cost;

    if (in == NULL || out == NULL) return TM_ERR_ARG;
    if (!is_positive(in->sale_price) || !is_rate(in->commission_rate) ||
        !is_prob(in->sale_probability) || !is_rate(in->platform_fee_rate) ||
        !is_amount(in->labor_cost) || !is_amount(in->fixed_costs) ||
        !is_prob(in->chargeback_probability) ||
        !is_amount(in->chargeback_loss) || !is_days(in->days_to_sell)) {
        return TM_ERR_RANGE;
    }

    p = in->sale_probability;
    /* Labor and fixed costs are paid whether or not it sells; platform fees
     * and chargeback losses arise only if it does. */
    cost = in->labor_cost + in->fixed_costs +
           p * (in->platform_fee_rate * in->sale_price +
                in->chargeback_probability * in->chargeback_loss);

    return finish(cost, p * in->commission_rate * in->sale_price,
                  in->days_to_sell, out);
}

/* ---- 8. Distressed inventory ------------------------------------------ */

tm_status tm_distressed(const tm_distressed_in *in, tm_result *out)
{
    double w, s, lot_cost, cost, proceeds;

    if (in == NULL || out == NULL) return TM_ERR_ARG;
    if (!is_positive(in->lot_price) || !is_positive(in->units) ||
        !is_amount(in->expected_unit_value) ||
        !is_prob(in->sell_through_rate) ||
        !is_amount(in->salvage_unit_value) || !is_rate(in->sell_fee_rate) ||
        !is_amount(in->per_unit_handling_cost) ||
        !is_amount(in->disposal_cost_per_unsold_unit) ||
        !is_amount(in->fixed_costs) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->hold_days) || !is_prob(in->win_probability) ||
        !is_amount(in->pursuit_cost)) {
        return TM_ERR_RANGE;
    }

    w = in->win_probability;
    s = in->sell_through_rate;
    /* Pay to chase every lot; pay for the lot and its clean-out only if won. */
    lot_cost = in->lot_price + in->units * in->per_unit_handling_cost +
               (1.0 - s) * in->units * in->disposal_cost_per_unsold_unit +
               in->fixed_costs + in->holding_cost_per_day * in->hold_days;
    cost = in->pursuit_cost + w * lot_cost;
    proceeds = w * in->units *
               (s * in->expected_unit_value * (1.0 - in->sell_fee_rate) +
                (1.0 - s) * in->salvage_unit_value);

    return finish(cost, proceeds, in->hold_days, out);
}

/* ---- 9. Virtual inventory ---------------------------------------------- */

tm_status tm_virtual(const tm_virtual_in *in, tm_result *out)
{
    double f, source_cost, revenue, cost;

    if (in == NULL || out == NULL) return TM_ERR_ARG;
    if (!is_positive(in->sale_price) || !is_positive(in->source_price) ||
        !is_positive(in->quantity) || !is_rate(in->sale_fee_rate) ||
        !is_rate(in->source_fee_rate) || !is_amount(in->shipping_cost) ||
        !is_rate(in->price_drift_rate) ||
        !is_prob(in->fulfillment_failure_probability) ||
        !is_amount(in->failure_loss) || !is_amount(in->fixed_costs) ||
        !is_days(in->days_to_fulfil)) {
        return TM_ERR_RANGE;
    }

    f = in->fulfillment_failure_probability;
    source_cost = in->quantity * in->source_price *
                  (1.0 + in->price_drift_rate) * (1.0 + in->source_fee_rate) +
                  in->shipping_cost;
    revenue = in->quantity * in->sale_price * (1.0 - in->sale_fee_rate);

    /* Fixed costs are paid regardless; source and shipping only if you
     * fulfil; the failure loss only if you cannot. */
    cost = in->fixed_costs + (1.0 - f) * source_cost + f * in->failure_loss;
    return finish(cost, (1.0 - f) * revenue, in->days_to_fulfil, out);
}

/* ---- 10. Synthetic bundling -------------------------------------------- */

tm_status tm_synthetic(const tm_synthetic_in *in,
                       const tm_component *outputs, size_t n_outputs,
                       tm_result *out)
{
    double value = 0.0, cost;
    size_t i;

    if (in == NULL || outputs == NULL || out == NULL) return TM_ERR_ARG;
    if (n_outputs == 0 || n_outputs > TM_MAX_COMPONENTS) return TM_ERR_ARG;
    if (!is_positive(in->input_cost) || !is_amount(in->fixed_costs) ||
        !is_amount(in->per_output_cost) || !is_rate(in->waste_rate) ||
        !is_rate(in->sell_fee_rate) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->hold_days)) {
        return TM_ERR_RANGE;
    }
    for (i = 0; i < n_outputs; ++i) {
        if (!is_amount(outputs[i].sale_value) ||
            !is_prob(outputs[i].sale_probability) ||
            !is_amount(outputs[i].fallback_value)) {
            return TM_ERR_RANGE;
        }
        /* Waste and the sales fee apply to what sells; the fallback (melt,
         * scrap) is taken as received. */
        value += outputs[i].sale_probability * outputs[i].sale_value *
                     (1.0 - in->waste_rate) * (1.0 - in->sell_fee_rate) +
                 (1.0 - outputs[i].sale_probability) *
                     outputs[i].fallback_value;
    }

    cost = in->input_cost + in->fixed_costs +
           (double)n_outputs * in->per_output_cost +
           in->holding_cost_per_day * in->hold_days;
    return finish(cost, value, in->hold_days, out);
}

/* ---- 11. Event momentum ------------------------------------------------ */

tm_status tm_hype_decay(double initial_excess, double half_life_days,
                        double days, double *out_remaining)
{
    double v;
    if (out_remaining == NULL) return TM_ERR_ARG;
    if (!isfinite(initial_excess) || initial_excess < 0.0 ||
        initial_excess > TM_MAX_AMOUNT || !isfinite(half_life_days) ||
        half_life_days <= 0.0 || !is_days(days)) {
        return TM_ERR_RANGE;
    }
    v = initial_excess * pow(0.5, days / half_life_days);
    if (!isfinite(v)) return TM_ERR_OVERFLOW;
    *out_remaining = v;
    return TM_OK;
}

tm_status tm_momentum(const tm_momentum_in *in,
                      const tm_outcome *exit_prices, size_t n_exit_prices,
                      tm_result *out)
{
    double psum = 0.0, expected_exit = 0.0, cost, exit_proceeds, stuck, L;
    size_t i;

    if (in == NULL || exit_prices == NULL || out == NULL) return TM_ERR_ARG;
    if (n_exit_prices == 0 || n_exit_prices > TM_MAX_OUTCOMES) {
        return TM_ERR_ARG;
    }
    if (!is_positive(in->entry_price) || !is_positive(in->quantity) ||
        !is_rate(in->buy_fee_rate) || !is_rate(in->sell_fee_rate) ||
        !is_rate(in->entry_slippage_rate) || !is_rate(in->exit_slippage_rate) ||
        !is_amount(in->fixed_costs) || !is_amount(in->holding_cost_per_day) ||
        !is_days(in->hold_days) || !is_prob(in->exit_failure_probability) ||
        !is_prob(in->stuck_recovery_rate)) {
        return TM_ERR_RANGE;
    }
    /* The exit leg must keep a positive share of its notional. */
    if (in->sell_fee_rate + in->exit_slippage_rate >= 1.0) return TM_ERR_RANGE;

    for (i = 0; i < n_exit_prices; ++i) {
        if (!is_prob(exit_prices[i].probability)) return TM_ERR_PROB;
        if (!is_amount(exit_prices[i].unit_value)) return TM_ERR_RANGE;
        psum += exit_prices[i].probability;
        expected_exit += exit_prices[i].probability * exit_prices[i].unit_value;
    }
    if (fabs(psum - 1.0) > TM_PROB_TOLERANCE) return TM_ERR_PROB;

    L = in->exit_failure_probability;
    cost = in->quantity * in->entry_price *
               (1.0 + in->buy_fee_rate + in->entry_slippage_rate) +
           in->fixed_costs + in->holding_cost_per_day * in->hold_days;
    exit_proceeds = in->quantity * expected_exit *
                    (1.0 - in->sell_fee_rate - in->exit_slippage_rate);
    stuck = in->quantity * in->entry_price * in->stuck_recovery_rate;

    return finish(cost, (1.0 - L) * exit_proceeds + L * stuck, in->hold_days,
                  out);
}

/* ---- Bridge to the advantage core ------------------------------------- */

tm_status tm_baseline(const tm_result *r, double cost_of_capital_annual,
                      double *out_baseline)
{
    double b;
    if (r == NULL || out_baseline == NULL) return TM_ERR_ARG;
    if (!isfinite(cost_of_capital_annual) || cost_of_capital_annual < 0.0 ||
        cost_of_capital_annual > 10.0) {
        return TM_ERR_RANGE;
    }
    b = r->total_cost *
        (1.0 + cost_of_capital_annual * (r->hold_days / 365.0));
    if (!isfinite(b)) return TM_ERR_OVERFLOW;
    *out_baseline = b;
    return TM_OK;
}

tm_status tm_advantage(const tm_result *r, double cost_of_capital_annual,
                       double *out_advantage)
{
    double baseline, adv;
    tm_status st;

    if (out_advantage == NULL) return TM_ERR_ARG;
    st = tm_baseline(r, cost_of_capital_annual, &baseline);
    if (st != TM_OK) return st;

    adv = advantage_value(r->expected_proceeds, baseline, 1.0);
    if (!isfinite(adv)) return TM_ERR_OVERFLOW;
    *out_advantage = adv;
    return TM_OK;
}

const char *tm_strerror(tm_status st)
{
    switch (st) {
    case TM_OK:           return "ok";
    case TM_ERR_ARG:      return "invalid argument (null pointer or count)";
    case TM_ERR_RANGE:    return "value non-finite or outside its valid range";
    case TM_ERR_PROB:     return "outcome probabilities are not a distribution";
    case TM_ERR_OVERFLOW: return "result was not finite";
    }
    return "unknown error";
}
