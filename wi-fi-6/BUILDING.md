# Wi-Fi 6 Source Build And Test Notes

This portable source series exposes static reference metadata for IEEE 802.11ax. It is not a radio/PHY/MAC implementation, network scanner, or adapter configuration utility. Generation-level capabilities do not imply that a particular adapter, access point, channel, or regulatory domain supports them.

## Build And Run

C11: `cc -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc/c src/c/wifi_profile.c src/c/example.c -o wifi-profile-c && ./wifi-profile-c`

C++17: `c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -Isrc/cpp src/cpp/wifi_profile.cpp src/cpp/example.cpp -o wifi-profile-cpp && ./wifi-profile-cpp`

Java 8+: `mkdir -p build/java && javac -Xlint:all -Werror -d build/java src/java/WifiProfile.java src/java/WifiProfileDemo.java && java -cp build/java WifiProfileDemo`

Peak rates are theoretical PHY-layer values, not promised throughput. OFDMA, MU-MIMO, BSS coloring, and Target Wake Time require appropriate radio, firmware, driver, and peer support. Wi-Fi 6E's 6 GHz operation is subject to device and local regulatory rules. See the active consolidated IEEE 802.11 standard: https://standards.ieee.org/ieee/802.11/10548/.
