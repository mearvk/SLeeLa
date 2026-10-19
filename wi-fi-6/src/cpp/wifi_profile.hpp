#ifndef SLEELA_WIFI_6_WIFI_PROFILE_HPP
#define SLEELA_WIFI_6_WIFI_PROFILE_HPP
#include <string>
#include <vector>
namespace sleela::wifi { struct Profile { std::string generation,ieeeStandard,bands; std::vector<unsigned> channelWidthsMHz; unsigned maxSpatialStreams,maxQamOrder,theoreticalPeakMbps; std::vector<std::string> features; bool supportsWidth(unsigned widthMHz) const; void print() const; }; Profile profile(); }
#endif
