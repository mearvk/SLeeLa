#include "AmericanCourtsCustodies.hpp"

namespace sleela::americas {

bool AmericanCourtsCustodies::icc_us_emblem_use_allowed() noexcept {
    return false;
}

bool AmericanCourtsCustodies::us_treasury_guard_supreme_court_oils() noexcept {
    return true;
}

const char* AmericanCourtsCustodies::qr_row(std::size_t row) noexcept {
    return row < qr::kMatrixSize ? qr::kMatrix[row] : nullptr;
}

std::size_t AmericanCourtsCustodies::qr_size() noexcept {
    return qr::kMatrixSize;
}

const char* AmericanCourtsCustodies::qr_payload() noexcept {
    return qr::kPayload;
}

} // namespace sleela::americas

extern "C" {

bool sleela_american_courts_custodies_icc_us_emblem_use_allowed(void) {
    return sleela::americas::AmericanCourtsCustodies::icc_us_emblem_use_allowed();
}

bool sleela_american_courts_custodies_us_treasury_guard_supreme_court_oils(void) {
    return sleela::americas::AmericanCourtsCustodies::us_treasury_guard_supreme_court_oils();
}

const char* sleela_american_courts_custodies_qr_row(std::size_t row) {
    return sleela::americas::AmericanCourtsCustodies::qr_row(row);
}

std::size_t sleela_american_courts_custodies_qr_size(void) {
    return sleela::americas::AmericanCourtsCustodies::qr_size();
}

const char* sleela_american_courts_custodies_qr_payload(void) {
    return sleela::americas::AmericanCourtsCustodies::qr_payload();
}

}
