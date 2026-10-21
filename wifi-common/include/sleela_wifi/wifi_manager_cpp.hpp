#ifndef SLEELA_WIFI_MANAGER_CPP_HPP
#define SLEELA_WIFI_MANAGER_CPP_HPP
#include "sleela_wifi/wifi_manager.h"
#include <string>
#include <vector>
namespace sleela::wifi {
using Status = wifi_status_t;
struct Network {
    std::string ssid, bssid;
    unsigned channel = 0, frequencyMHz = 0, signalPercent = 0;
    wifi_security_t security = WIFI_SECURITY_UNKNOWN;
    bool inUse = false;
    int protectedManagementFrames = -1;
};
class Credentials final {
public:
    wifi_auth_mode_t mode = WIFI_AUTH_OS_MANAGED;
    std::string profileName, username, password, caCertificateFile, expectedServerName;
    Credentials() = default;
    Credentials(const Credentials&) = delete;
    Credentials& operator=(const Credentials&) = delete;
    Credentials(Credentials&&) = delete;
    Credentials& operator=(Credentials&&) = delete;
    ~Credentials();
    void clear() noexcept;
private:
    static void clearOne(std::string& value) noexcept;
};
class Manager final {
public:
    explicit Manager(const wifi_security_policy_t* policy = nullptr);
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;
    Status scan(std::vector<Network>& out);
    Status select(std::size_t index, Network& out) const;
    Status connect(const Network& network, const Credentials& credentials);
    Status disconnect();
private:
    wifi_manager_t manager_{};
    Status status_ = WIFI_STATUS_BACKEND_UNAVAILABLE;
};
}
#endif
