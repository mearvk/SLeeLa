#ifndef SLEELA_NAT_AWARE_H
#define SLEELA_NAT_AWARE_H

#include <string>

namespace sleela::server {

enum class NatMode { Direct, PortForward, IPv6, Outbound, Relay };

struct NatConfig {
    NatMode mode = NatMode::Outbound;
    std::string bindHost = "127.0.0.1";
    unsigned short bindPort = 8080;
    std::string publicHost;
    unsigned short publicPort = 0;
    std::string rendezvous;
    unsigned int keepaliveSeconds = 25;
    bool tlsRequired = true;
};

struct NatPlan {
    bool valid = false;
    NatMode mode = NatMode::Outbound;
    std::string summary;
    std::string nextStep;
};

const char *natModeName(NatMode mode);
bool natModeFromString(const std::string &value, NatMode &mode);
bool natConfigFromEnvironment(NatConfig &config, std::string &error);
NatPlan makeNatPlan(const NatConfig &config);

} // namespace sleela::server

#endif
