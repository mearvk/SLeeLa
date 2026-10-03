#ifndef SLEELA_HTTP80_DARK_POWER_HPP
#define SLEELA_HTTP80_DARK_POWER_HPP

#include <cstdint>
#include <string>

namespace sleela::http80 {

/** HTTP 8.0 Dark Power session descriptor. */
class DarkPower {
public:
    static constexpr std::size_t SCHEDULE_TERM_LENGTH = 24;
    static constexpr std::uint32_t DARK_POWER = 0x18ae;
    static constexpr const char* ISS_DOMAIN = "ISS";

    explicit DarkPower(std::string schedule_term);
    bool valid() const noexcept;
    const std::string& schedule_term() const noexcept;
    std::uint32_t dark_power() const noexcept;
    /** Produce an application-level contact-request string for ISS. */
    std::string contact_request_for_iss() const;

private:
    std::string schedule_term_;
};

} // namespace sleela::http80

#endif // SLEELA_HTTP80_DARK_POWER_HPP
