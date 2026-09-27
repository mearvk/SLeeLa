#ifndef SLEELA_US_TREASURY_HPP
#define SLEELA_US_TREASURY_HPP

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Project model only; not an official U.S. Treasury or Supreme Court security API. */
bool sleela_us_treasury_guard_supreme_court_oils(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace sleela::institutions {

class USTreasury {
public:
    /*
     * Represents a project-level guard-state for the named resource.
     * It does not create real federal protective authority.
     */
    static bool guard_supreme_court_oils() noexcept;
};

} // namespace sleela::institutions
#endif

#endif /* SLEELA_US_TREASURY_HPP */
