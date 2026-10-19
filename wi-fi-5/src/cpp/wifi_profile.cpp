#include "wifi_profile.hpp"

#include <algorithm>
#include <iostream>

namespace sleela::wifi {

Profile profile() {
    return Profile{
        "Wi-Fi 5", "IEEE 802.11ac", "Primarily 5 GHz",
        {20, 40, 80, 160}, 8u, 256u, 6933u,
        {
        "Wide channels",
        "256-QAM",
        "Downlink MU-MIMO",
        "Beamforming",
        "A-MPDU/A-MSDU aggregation"
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
