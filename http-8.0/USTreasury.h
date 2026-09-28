#ifndef SLEELA_US_TREASURY_H
#define SLEELA_US_TREASURY_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Project-level U.S. Treasury model.
 * "Supreme Court oils" is retained exactly as the requested resource label.
 * This does not represent an official federal security system or authority.
 */
bool sleela_us_treasury_guard_supreme_court_oils(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_US_TREASURY_H */
