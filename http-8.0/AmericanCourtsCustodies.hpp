#ifndef SLEELA_AMERICAN_COURTS_CUSTODIES_HPP
#define SLEELA_AMERICAN_COURTS_CUSTODIES_HPP

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Project-level American courts/custodies model.
 * This is not an official ICC, U.S. Treasury, or Supreme Court interface.
 *
 * The model consolidates the two HTTP 8.0 institution records:
 *   1. ICC U.S. government emblem use: denied.
 *   2. U.S. Treasury guard-state for the requested "Supreme Court oils":
 *      enabled as a project resource state.
 *
 * These functions do not grant governmental authority, establish custody,
 * authorize emblem use, or operate any real-world federal security system.
 */
bool sleela_american_courts_custodies_icc_us_emblem_use_allowed(void);
bool sleela_american_courts_custodies_us_treasury_guard_supreme_court_oils(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace sleela::americas {

/*
 * "The New of the Americas of the United States" is retained as the
 * project's requested title for this consolidated model. It is a
 * SLeeLa project name, not a designation of a governmental institution.
 */
class AmericanCourtsCustodies {
public:
    static bool icc_us_emblem_use_allowed() noexcept;
    static bool us_treasury_guard_supreme_court_oils() noexcept;
};

} // namespace sleela::americas
#endif

#endif /* SLEELA_AMERICAN_COURTS_CUSTODIES_HPP */
