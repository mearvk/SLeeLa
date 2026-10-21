#include "sleela_wifi/wifi_manager_cpp.hpp"
#include <algorithm>
#include <cstring>
#include <utility>
namespace sleela::wifi {
void Credentials::clearOne(std::string& value) noexcept {
    if (!value.empty()) {
        wifi_secure_zero(&value[0], value.size());
        value.clear();
    }
}
void Credentials::clear() noexcept {
    clearOne(password); clearOne(username); clearOne(caCertificateFile);
    clearOne(expectedServerName); clearOne(profileName);
}
Credentials::~Credentials() { clear(); }
Manager::Manager(const wifi_security_policy_t* policy) {
    status_ = wifi_manager_init(&manager_, wifi_backend_default(), nullptr, policy);
}
Status Manager::scan(std::vector<Network>& out) {
    wifi_network_t raw[WIFI_MAX_SCAN_RESULTS]; std::size_t count = 0;
    Status s; out.clear(); if (status_ != WIFI_STATUS_OK) return status_;
    s = wifi_manager_scan(&manager_, raw, WIFI_MAX_SCAN_RESULTS, &count);
    if (s != WIFI_STATUS_OK && s != WIFI_STATUS_BUFFER_TOO_SMALL) return s;
    out.reserve(std::min(count, (std::size_t)WIFI_MAX_SCAN_RESULTS));
    for (std::size_t i = 0; i < std::min(count, (std::size_t)WIFI_MAX_SCAN_RESULTS); ++i) {
        Network n; n.ssid = raw[i].ssid; n.bssid = raw[i].bssid;
        n.channel = raw[i].channel; n.frequencyMHz = raw[i].frequency_mhz;
        n.signalPercent = raw[i].signal_percent; n.security = raw[i].security;
        n.inUse = raw[i].in_use != 0; n.protectedManagementFrames = raw[i].protected_management_frames;
        out.push_back(std::move(n));
    }
    return s;
}
Status Manager::select(std::size_t index, Network& out) const {
    wifi_network_t r{}; Status s = wifi_manager_select(&manager_, index, &r);
    if (s != WIFI_STATUS_OK) return s;
    out.ssid = r.ssid; out.bssid = r.bssid; out.channel = r.channel;
    out.frequencyMHz = r.frequency_mhz; out.signalPercent = r.signal_percent;
    out.security = r.security; out.inUse = r.in_use != 0;
    out.protectedManagementFrames = r.protected_management_frames; return WIFI_STATUS_OK;
}
Status Manager::connect(const Network& n, const Credentials& c) {
    wifi_network_t raw{}; wifi_auth_t a{};
    if (status_ != WIFI_STATUS_OK) return status_;
    if (n.ssid.empty() || n.ssid.size() > WIFI_SSID_MAX_BYTES ||
        n.bssid.empty() || n.bssid.size() >= WIFI_BSSID_TEXT_SIZE)
        return WIFI_STATUS_INVALID_ARGUMENT;
    std::memcpy(raw.ssid, n.ssid.c_str(), n.ssid.size() + 1);
    std::memcpy(raw.bssid, n.bssid.c_str(), n.bssid.size() + 1);
    raw.channel = n.channel; raw.frequency_mhz = n.frequencyMHz;
    raw.signal_percent = n.signalPercent; raw.security = n.security;
    raw.in_use = n.inUse ? 1 : 0; raw.protected_management_frames = n.protectedManagementFrames;
    a.mode = c.mode; a.profile_name = c.profileName.empty() ? nullptr : c.profileName.c_str();
    a.username = c.username.empty() ? nullptr : c.username.c_str();
    a.password = c.password.empty() ? nullptr : c.password.c_str();
    a.ca_certificate_file = c.caCertificateFile.empty() ? nullptr : c.caCertificateFile.c_str();
    a.expected_server_name = c.expectedServerName.empty() ? nullptr : c.expectedServerName.c_str();
    return wifi_manager_connect(&manager_, &raw, &a);
}
Status Manager::disconnect() {
    return status_ == WIFI_STATUS_OK ? wifi_manager_disconnect(&manager_) : status_;
}
}
