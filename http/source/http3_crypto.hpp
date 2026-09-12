#ifndef SLEELA_HTTP3_CRYPTO_HPP
#define SLEELA_HTTP3_CRYPTO_HPP

#include "crypto_openssl.h"
#include "crypto_route_session.h"

#include <array>
#include <cstdint>
#include <stdexcept>

namespace sleela::http3 {

class RouteSession final {
public:
    RouteSession() = default;
    ~RouteSession() { http3_route_session_clear(&session_); }

    RouteSession(const RouteSession&) = delete;
    RouteSession& operator=(const RouteSession&) = delete;

    std::array<std::uint8_t, 32> establish(
        const http3_route_plan_t& plan,
        const std::array<std::uint8_t, 32>& peer_public_key,
        std::uint64_t key_epoch)
    {
        if (http3_route_session_establish(&session_, &plan,
                                          peer_public_key.data(), key_epoch) != 0) {
            throw std::runtime_error("HTTP/3 route session establishment failed");
        }
        return public_key();
    }

    std::array<std::uint8_t, 32> rekey(
        const http3_route_plan_t& old_plan,
        const http3_route_plan_t& new_plan,
        const std::array<std::uint8_t, 32>& peer_public_key,
        std::uint64_t key_epoch)
    {
        if (http3_route_session_rekey(&session_, &old_plan, &new_plan,
                                      peer_public_key.data(), key_epoch) != 0) {
            throw std::runtime_error("HTTP/3 route session rekey failed");
        }
        return public_key();
    }

    std::array<std::uint8_t, 32> country_key(std::uint32_t country_id) const
    {
        std::array<std::uint8_t, 32> key{};
        if (http3_route_session_derive_country_key(&session_, country_id,
                                                   key.data()) != 0) {
            throw std::runtime_error("HTTP/3 country key derivation failed");
        }
        return key;
    }

    void clear() noexcept { http3_route_session_clear(&session_); }

private:
    std::array<std::uint8_t, 32> public_key() const
    {
        std::array<std::uint8_t, 32> key{};
        std::copy(std::begin(session_.ephemeral_public_key),
                  std::end(session_.ephemeral_public_key), key.begin());
        return key;
    }

    http3_route_session_t session_{};
};

} // namespace sleela::http3

#endif
