#include "wifi_profile.hpp"

#include <algorithm>
#include <iostream>

namespace sleela::wifi {

Profile profile() {
    return Profile{
        "Wi-Fi 6", "IEEE 802.11ax", "2.4 GHz and 5 GHz; 6 GHz for Wi-Fi 6E where permitted",
        {20, 40, 80, 160}, 8u, 1024u, 9608u,
        {
        "OFDMA uplink/downlink",
        "MU-MIMO uplink/downlink",
        "1024-QAM",
        "BSS coloring/spatial reuse",
        "Target Wake Time"
        }
    };
}

bool Profile::supportsWidth(unsigned widthMHz) const {
    return std::find(channelWidthsMHz.begin(), channelWidthsMHz.end(), widthMHz) != channelWidthsMHz.end();
}

void Profile::print() const {
    std::cout << generation << " (" << ieeeStandard << ")\n"
              << "Bands: " << bands << "\n"
              << "Maximum theoretical PHY rate: " << theoreticalPeakMbps
              << " Mbit/s (not application throughput)\n"
              << "Maximum standard spatial streams: " << maxSpatialStreams
              << "; highest listed modulation: " << maxQamOrder << "-QAM\n"
              << "Reference features:\n";
    for (const auto& feature : features) std::cout << "  - " << feature << '\n';
}

} // namespace sleela::wifi
