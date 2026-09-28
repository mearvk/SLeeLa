#include "DarkPower.hpp"

#include <utility>

namespace sleela::http80 {

DarkPower::DarkPower(std::string schedule_term)
    : schedule_term_(std::move(schedule_term)) {}

bool DarkPower::valid() const noexcept {
    return schedule_term_.size() == SCHEDULE_TERM_LENGTH;
}

const std::string& DarkPower::schedule_term() const noexcept {
    return schedule_term_;
}

std::uint32_t DarkPower::dark_power() const noexcept {
    return DARK_POWER;
}

std::string DarkPower::contact_request_for_iss() const {
    if (!valid()) return {};
    return std::string("CONTACT-REQUEST|DOMAIN=") + ISS_DOMAIN +
           "|SCHEDULE-TERM=" + schedule_term_ +
           "|DARK-POWER=0x18ae";
}

} // namespace sleela::http80
