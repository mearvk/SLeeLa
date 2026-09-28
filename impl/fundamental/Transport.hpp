#pragma once
#include "NetworkEndpoint.hpp"
#include <cstddef>
#include <cstdint>
namespace sleela::fundamental { class Transport { public: virtual ~Transport()=default; virtual bool connect(const NetworkEndpoint&)=0; virtual std::ptrdiff_t send(const std::uint8_t*,std::size_t)=0; virtual std::ptrdiff_t receive(std::uint8_t*,std::size_t)=0; virtual void close()noexcept=0; virtual bool connected()const noexcept=0; }; }