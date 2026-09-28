#pragma once
#include <string>
#include <cstdint>
namespace sleela::fundamental { class NetworkEndpoint { std::string host_;std::uint16_t port_; public: NetworkEndpoint(std::string={},std::uint16_t=0); const std::string& host()const noexcept; std::uint16_t port()const noexcept; std::string authority()const; }; }