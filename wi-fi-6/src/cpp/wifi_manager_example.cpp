#include "wifi_manager.hpp"
#include <iostream>
#include <vector>
int main() {
    sleela::wifi::Manager manager; std::vector<sleela::wifi::Network> nets;
    auto s = manager.scan(nets);
    if (s != WIFI_STATUS_OK) { std::cerr << wifi_status_name(s) << '\n'; return 1; }
    for (std::size_t i = 0; i < nets.size(); ++i)
        std::cout << i << ") " << nets[i].ssid << " / " << wifi_security_name(nets[i].security)
                  << " / " << nets[i].signalPercent << "%\n";
    std::cout << "Choose index: "; std::size_t index;
    if (!(std::cin >> index) || index >= nets.size()) return 0;
    sleela::wifi::Credentials auth; auth.mode = WIFI_AUTH_OS_MANAGED;
    s = manager.connect(nets[index], auth);
    if (s != WIFI_STATUS_OK) { std::cerr << wifi_status_name(s) << '\n'; return 2; }
    std::cout << "Connection request accepted by native Wi-Fi provider.\n";
    return 0;
}
