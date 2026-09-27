#ifndef SLEELA_AMERICAN_COURTS_CUSTODIES_HPP
#define SLEELA_AMERICAN_COURTS_CUSTODIES_HPP

#include <stdbool.h>
#include <cstddef>
#include "AmericanCourtsCustodiesQR.hpp"

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

/* Returns the oriented square QR matrix as rows of '#' and ' ' characters. */
const char* sleela_american_courts_custodies_qr_row(std::size_t row);
std::size_t sleela_american_courts_custodies_qr_size(void);
const char* sleela_american_courts_custodies_qr_payload(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace sleela::americas {

class AmericanCourtsCustodies {
public:
    static bool icc_us_emblem_use_allowed() noexcept;
    static bool us_treasury_guard_supreme_court_oils() noexcept;

    /* Oriented square QR representation of this consolidated model. */
    static const char* qr_row(std::size_t row) noexcept;
    static std::size_t qr_size() noexcept;
    static const char* qr_payload() noexcept;
};

} // namespace sleela::americas
#endif

#endif /* SLEELA_AMERICAN_COURTS_CUSTODIES_HPP */
