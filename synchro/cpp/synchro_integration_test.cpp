#include "synchro_integration.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
int main() {
    sleela::synchro::Integration integration(8);
    std::uint32_t sequence = 0;
    const std::uint64_t sent = 1000000000ULL;
    const std::uint64_t now = 1005000000ULL;
    const auto packet = integration.prepare(sent, sequence);
    assert(packet.size() == 16);
    assert(sequence == 1);
    integration.acknowledge(packet.data(), sequence, now, 0);
    assert(integration.stats().sent == 1);
    assert(integration.stats().acked == 1);
    assert(integration.stats().lost == 0);
    integration.timeout();
    assert(integration.stats().sent == 2);
    assert(integration.stats().lost == 1);
    std::cout << "synchro C++ integration: PASS\n";
    return 0;
}
