#include "wifi_profile.hpp"

#include <iostream>

int main() {
    const auto p = sleela::wifi::profile();
    p.print();
    std::cout << "\nReference check: 80 MHz channel "
              << (p.supportsWidth(80u) ? "is listed as supported" : "is not listed as supported")
              << " by this generation profile.\n"
              << "Note: query the operating system/driver for actual adapter capabilities.\n";
    return 0;
}
