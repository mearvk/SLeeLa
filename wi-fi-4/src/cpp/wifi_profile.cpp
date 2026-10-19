#include "wifi_profile.hpp"

#include <algorithm>
#include <iostream>

namespace sleela::wifi {

Profile profile() {
    return Profile{
        "Wi-Fi 4", "IEEE 802.11n", "2.4 GHz and 5 GHz, subject to hardware and regulatory support",
        {20, 40}, 4u, 64u, 600u,
        {
        "MIMO spatial streams",
        "OFDM",
        "A-MPDU/A-MSDU aggregation",
        "Short guard interval"
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
