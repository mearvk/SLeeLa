// rate_budget.hpp - C++ layer over rate_budget.h.
//
// Takes the lowercase-keyed header map that advantage::HttpResponse carries
// (but does not depend on the transport library), keeps the budget state, and
// reports how long to wait. Pure: it never sleeps and never makes a request.
#ifndef ADVANTAGE_RATE_BUDGET_HPP
#define ADVANTAGE_RATE_BUDGET_HPP

#include <map>
#include <string>

#include "rate_budget.h"

namespace advantage {
namespace trading {

struct Delay {
    bool ok = false;         // true: wait `ms` then proceed
    bool mustStop = false;   // true: the required wait exceeds maxWaitMs
    double ms = 0.0;         // the wait the budget calls for
    std::string error;
};

struct BudgetOptions {
    double reserve = 2;           // requests held back, never spent
    double minIntervalMs = 100;   // floor between requests
    double maxWaitMs = 60000;     // beyond this, stop instead of sleeping
};

class RateBudget {
public:
    RateBudget() { rb_init(&state_); }

    // Feed one response's headers (keys must be lowercase, as HttpResponse
    // provides). `nowEpochSeconds` is only needed when the server sends the
    // reset as an absolute epoch time: a reset value above 1e9 is treated as
    // epoch and converted to seconds-from-now (never negative). Returns false
    // with `error` set if a present header is malformed; state is then left
    // unchanged.
    bool observe(const std::map<std::string, std::string>& headers,
                 double nowEpochSeconds, std::string& error) {
        const std::string* limit = find(headers, "x-ratelimit-limit");
        const std::string* remaining = find(headers, "x-ratelimit-remaining");
        const std::string* reset = find(headers, "x-ratelimit-reset");
        const std::string* retry = find(headers, "retry-after");

        std::string resetText;
        if (reset != nullptr) {
            double v = 0.0;
            if (rb_parse_nonneg(reset->c_str(), &v) != RB_OK) {
                error = "x-ratelimit-reset: " +
                        std::string(rb_strerror(RB_ERR_PARSE));
                return false;
            }
            if (v > 1e9) v = v > nowEpochSeconds ? v - nowEpochSeconds : 0.0;
            resetText = std::to_string(v);
            reset = &resetText;
        }

        rb_status st = rb_observe(&state_,
                                  limit ? limit->c_str() : nullptr,
                                  remaining ? remaining->c_str() : nullptr,
                                  reset ? reset->c_str() : nullptr,
                                  retry ? retry->c_str() : nullptr);
        if (st != RB_OK) {
            error = rb_strerror(st);
            return false;
        }
        error.clear();
        return true;
    }

    Delay next(const BudgetOptions& o = {}) const {
        Delay d;
        double ms = 0.0;
        rb_status st = rb_next_delay_ms(&state_, o.reserve, o.minIntervalMs,
                                        o.maxWaitMs, &ms);
        d.ms = ms;
        if (st == RB_OK) {
            d.ok = true;
        } else if (st == RB_ERR_WAIT) {
            d.mustStop = true;
            d.error = rb_strerror(st);
        } else {
            d.error = rb_strerror(st);
        }
        return d;
    }

    const rb_state& state() const { return state_; }

private:
    static const std::string* find(
        const std::map<std::string, std::string>& h, const char* key) {
        auto it = h.find(key);
        return it == h.end() ? nullptr : &it->second;
    }

    rb_state state_;
};

}  // namespace trading
}  // namespace advantage

#endif  // ADVANTAGE_RATE_BUDGET_HPP
