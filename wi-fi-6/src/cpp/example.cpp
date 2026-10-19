#include "wifi_profile.hpp"

#include <iostream>

int main() {
    const auto profile = sleela::wifi::profile();
    profile.print();
    std::cout << "\n80 MHz profile support: "
              << (profile.supportsWidth(80u) ? "yes" : "no") << '\n'
              << "Query the operating system/driver for actual adapter capabilities.\n";
    return 0;
}
