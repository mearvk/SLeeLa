#include "wifi_profile.hpp"

#include <iostream>

int main() {
    const auto profile = sleela::wifi::profile();
    profile.print();
    std::cout << "\n40 MHz profile support: "
              << (profile.supportsWidth(40u) ? "yes" : "no") << '\n'
              << "Query the operating system/driver for actual adapter capabilities.\n";
    return 0;
}
