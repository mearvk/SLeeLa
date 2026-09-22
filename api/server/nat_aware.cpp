#include "nat_aware.h"

#include <cstdlib>
#include <sstream>

namespace sleela::server {
namespace {
bool parsePort(const char *text, unsigned short &out) {
    if (!text || !*text) return false;
    char *end = nullptr;
    unsigned long value = std::strtoul(text, &end, 10);
    if (*end != '\0' || value == 0 || value > 65535) return false;
    out = static_cast<unsigned short>(value);
    return true;
}
bool parseBool(const char *text, bool &out) {
    if (!text) return false;
    std::string v(text);
    if (v == "1" || v == "true" || v == "yes") { out = true; return true; }
    if (v == "0" || v == "false" || v == "no") { out = false; return true; }
    return false;
}
}

const char *natModeName(NatMode mode) {
    switch (mode) {
        case NatMode::Direct: return "direct";
        case NatMode::PortForward: return "port-forward";
        case NatMode::IPv6: return "ipv6";
        case NatMode::Outbound: return "outbound";
        case NatMode::Relay: return "relay";
    }
    return "outbound";
}

bool natModeFromString(const std::string &value, NatMode &mode) {
    if (value == "direct") { mode = NatMode::Direct; return true; }
    if (value == "port-forward") { mode = NatMode::PortForward; return true; }
    if (value == "ipv6") { mode = NatMode::IPv6; return true; }
    if (value == "outbound") { mode = NatMode::Outbound; return true; }
    if (value == "relay") { mode = NatMode::Relay; return true; }
    return false;
}

bool natConfigFromEnvironment(NatConfig &config, std::string &error) {
    if (const char *v = std::getenv("SLEELA_NAT_MODE"))
        if (!natModeFromString(v, config.mode)) {
            error = "SLEELA_NAT_MODE must be direct, port-forward, ipv6, outbound, or relay";
            return false;
        }
    if (const char *v = std::getenv("SLEELA_NAT_BIND_HOST")) config.bindHost = v;
    if (const char *v = std::getenv("SLEELA_NAT_BIND_PORT"))
        if (!parsePort(v, config.bindPort)) { error = "SLEELA_NAT_BIND_PORT must be 1..65535"; return false; }
    if (const char *v = std::getenv("SLEELA_NAT_PUBLIC_HOST")) config.publicHost = v;
    if (const char *v = std::getenv("SLEELA_NAT_PUBLIC_PORT"))
        if (!parsePort(v, config.publicPort)) { error = "SLEELA_NAT_PUBLIC_PORT must be 1..65535"; return false; }
    if (const char *v = std::getenv("SLEELA_NAT_RENDEZVOUS")) config.rendezvous = v;
    if (const char *v = std::getenv("SLEELA_NAT_KEEPALIVE")) {
        char *end = nullptr; unsigned long n = std::strtoul(v, &end, 10);
        if (*end != '\0' || n < 5 || n > 3600) { error = "SLEELA_NAT_KEEPALIVE must be 5..3600 seconds"; return false; }
        config.keepaliveSeconds = static_cast<unsigned int>(n);
    }
    if (const char *v = std::getenv("SLEELA_NAT_TLS_REQUIRED"))
        if (!parseBool(v, config.tlsRequired)) { error = "SLEELA_NAT_TLS_REQUIRED must be true/false"; return false; }

    if (config.mode == NatMode::PortForward && (config.publicHost.empty() || config.publicPort == 0)) {
        error = "port-forward mode requires SLEELA_NAT_PUBLIC_HOST and SLEELA_NAT_PUBLIC_PORT"; return false;
    }
    if ((config.mode == NatMode::Outbound || config.mode == NatMode::Relay) && config.rendezvous.empty()) {
        error = "outbound/relay mode requires SLEELA_NAT_RENDEZVOUS"; return false;
    }
    if ((config.mode == NatMode::Outbound || config.mode == NatMode::Relay) && !config.tlsRequired) {
        error = "outbound/relay mode requires TLS"; return false;
    }
    return true;
}

NatPlan makeNatPlan(const NatConfig &config) {
    NatPlan plan; plan.valid = true; plan.mode = config.mode; std::ostringstream s;
    switch (config.mode) {
        case NatMode::Direct:
            s << "direct listener; NAT traversal is not supplied by sleelas";
            plan.nextStep = "Use only when the host already has a reachable address and firewall path."; break;
        case NatMode::PortForward:
            s << "mapping " << config.publicHost << ":" << config.publicPort
              << " -> " << config.bindHost << ":" << config.bindPort;
            plan.nextStep = "Configure the router/firewall mapping and publish the external endpoint."; break;
        case NatMode::IPv6:
            s << "native IPv6 listener on " << config.bindHost << ":" << config.bindPort;
            plan.nextStep = "Permit the service port in the IPv6 firewall."; break;
        case NatMode::Outbound:
            s << "persistent outbound rendezvous connection to " << config.rendezvous;
            plan.nextStep = "Use a trusted public rendezvous/reverse service with authenticated TLS."; break;
        case NatMode::Relay:
            s << "authenticated relay path through " << config.rendezvous;
            plan.nextStep = "Use a public relay when direct inbound or reverse connectivity is unavailable."; break;
    }
    plan.summary = s.str(); return plan;
}
} // namespace sleela::server
