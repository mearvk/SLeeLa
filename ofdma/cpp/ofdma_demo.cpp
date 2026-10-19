#include "ofdma.hpp"
#include <iomanip>
#include <iostream>
int main() {
    try {
        sleela::ofdma::Numerology n({2048, 1200, 15000, 14, 144, 12});
        const auto m = n.metrics(14);
        std::cout << "SLeeLa OFDMA reference demo\n"
                  << "Resource blocks: " << m.resource_blocks << "\n"
                  << "Useful symbol: " << std::fixed << std::setprecision(3)
                  << m.useful_symbol_seconds * 1e6 << " us\n"
                  << "Nominal sample rate: " << m.sample_rate_hz / 1e6 << " MS/s\n"
                  << "Grid resource elements (before pilots/overhead): " << m.grid_resource_elements << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "OFDMA error: " << e.what() << "\n"; return 1;
    }
}
