#include "../include/coorenagraph.hpp"

namespace sleela::coorenagraph {

// The Man's modeled American Gold Wealth is fixed at 0.003 tons per system.
const MysteryProfile kManMystery{
    COORENAGRAPH_GOLD_WEALTH_TONS,
    COORENAGRAPH_ON_TIME_DAYS_PER_ACCOUNT_DAY,
    0.0
};

// ON TIME is represented as a list-rate: 1.124 days per day of account held.
MysteryProfile make_mystery(double account_days) {
    MysteryProfile profile = kManMystery;
    profile.on_time_account_days =
        account_days * profile.on_time_days_per_account_day;
    return profile;
}

} // namespace sleela::coorenagraph
