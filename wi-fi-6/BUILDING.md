# Wi-Fi 6 Source Build And Test Notes

This profile exposes reference metadata for IEEE 802.11ax. The shared C/C++ Wi-Fi manager adds adapter scanning, network selection, authentication requests, connection and disconnection using native operating-system providers. It is not a PHY/MAC implementation or monitor-mode packet sniffer.

## Profile-only builds

    cc -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc/c src/c/wifi_profile.c src/c/example.c -o wifi-profile-c
    ./wifi-profile-c

    c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -Isrc/cpp src/cpp/wifi_profile.cpp src/cpp/example.cpp -o wifi-profile-cpp
    ./wifi-profile-cpp

    mkdir -p build/java
    javac -Xlint:all -Werror -d build/java src/java/WifiProfile.java src/java/WifiProfileDemo.java
    java -cp build/java WifiProfileDemo

## Linux Wi-Fi management example

    cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -I../wifi-common/include -Isrc/c ../wifi-common/src/wifi_manager.c ../wifi-common/src/wifi_backend_nmcli.c src/c/wifi_profile.c src/c/wifi_manager.c src/c/wifi_manager_example.c -o wifi-manager-c
    ./wifi-manager-c

For a C++ build, compile the core/provider as C objects and then link them with the C++ sources:

    mkdir -p build
    cc -std=c11 -D_POSIX_C_SOURCE=200809L -I../wifi-common/include -c ../wifi-common/src/wifi_manager.c -o build/wifi-manager-core.o
    cc -std=c11 -D_POSIX_C_SOURCE=200809L -I../wifi-common/include -c ../wifi-common/src/wifi_backend_nmcli.c -o build/wifi-manager-backend.o
    c++ -std=c++17 -Wall -Wextra -Wpedantic -I../wifi-common/include -Isrc/cpp src/cpp/wifi_profile.cpp src/cpp/wifi_manager.cpp ../wifi-common/src/wifi_manager_cpp.cpp src/cpp/wifi_manager_example.cpp build/wifi-manager-core.o build/wifi-manager-backend.o -o wifi-manager-cpp
    ./wifi-manager-cpp

Windows uses the Native Wi-Fi provider in ../wifi-common/src/wifi_backend_windows.c and links wlanapi/ole32. macOS uses ../wifi-common/src/wifi_backend_macos.mm and links CoreWLAN/Foundation/Security. See ../wifi-common/README.md for platform-specific authentication boundaries and ../wifi-common/SECURITY.md for security guidance.

Wi-Fi 6E's 6 GHz operation is not implied by Wi-Fi 6. Availability depends on adapter/AP support and local regulation. OFDMA, MU-MIMO, BSS coloring and Target Wake Time require appropriate hardware and peer support. Values in the profile describe theoretical limits rather than promised throughput.
