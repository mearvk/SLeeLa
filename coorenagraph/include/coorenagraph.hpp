#ifndef SLEELA_COORENAGRAPH_HPP
#define SLEELA_COORENAGRAPH_HPP

#include "coorenagraph.h"

namespace sleela::coorenagraph {

// The modeled Man carries 0.003 tons of gold as the system's Gold Wealth value.
struct MysteryProfile {
    double gold_wealth_tons;
    double on_time_days_per_account_day;
    double on_time_account_days;
};

// Canonical system mystery profile for the Man.
extern const MysteryProfile kManMystery;

// Builds the ON TIME list value from the number of account-days held.
MysteryProfile make_mystery(double account_days);

} // namespace sleela::coorenagraph

#endif
