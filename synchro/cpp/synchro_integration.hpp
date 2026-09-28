#pragma once

#include "../c/synchro_integration.h"
#include "synchro.hpp"

#include <cstdint>
#include <stdexcept>

namespace sleela::synchro {

class Integration {
    synchro_integration integration_{};

public:
    explicit Integration(std::size_t window = 1024) {
        if (synchro_integration_init(&integration_, window) != 0)
            throw std::runtime_error("unable to initialize Synchro integration");
    }

    ~Integration() { synchro_integration_free(&integration_); }

    Integration(const Integration&) = delete;
    Integration& operator=(const Integration&) = delete;

    std::vector<std::uint8_t> prepare(std::uint64_t sentNs,
                                       std::uint32_t& sequence) {
        std::vector<std::uint8_t> packet(16);
        if (synchro_integration_prepare(&integration_, packet.data(),
                                        sentNs, &sequence) != 16)
            throw std::runtime_error("unable to prepare Synchro packet");
        return packet;
    }

    void acknowledge(const std::uint8_t* packet,
                     std::uint32_t sequence,
                     std::uint64_t nowNs,
                     std::uint64_t sentNs) {
        if (synchro_integration_ack(&integration_, packet, sequence,
                                    nowNs, sentNs) != 0)
            throw std::runtime_error("invalid Synchro acknowledgement");
    }

    void timeout() {
        if (synchro_integration_timeout(&integration_) != 0)
            throw std::runtime_error("unable to record Synchro timeout");
    }

    const synchro_stats& stats() const {
        return *synchro_integration_stats(&integration_);
    }
};

} // namespace sleela::synchro
