#ifndef SLEELA_WI_FI_5_WIFI_PROFILE_HPP
#define SLEELA_WI_FI_5_WIFI_PROFILE_HPP

#include <string>
#include <vector>

namespace sleela::wifi {

struct Profile {
    std::string generation;
    std::string ieeeStandard;
    std::string bands;
    std::vector<unsigned> channelWidthsMHz;
    unsigned maxSpatialStreams;
    unsigned maxQamOrder;
    unsigned theoreticalPeakMbps;
    std::vector<std::string> features;

    bool supportsWidth(unsigned widthMHz) const;
    void print() const;
};

// Static reference metadata, not a hardware probe or a PHY/MAC implementation.
Profile profile();

} // namespace sleela::wifi
#endif
