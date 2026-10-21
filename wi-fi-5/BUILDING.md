# Wi-Fi 5 Source Build And Test Notes

## Scope

The Wi-Fi 5 profile describes IEEE 802.11ac capabilities. The new management API provides scan, selection, native authentication, connect and disconnect operations through an OS provider. It is not a PHY/MAC implementation or packet sniffer.

## Layout

- src/c/wifi_profile.h and wifi_profile.c: C capability metadata.
- src/c/wifi_manager.h and wifi_manager.c: C integration shim for the shared management API.
- src/c/wifi_manager_example.c: scan, display, select and connect example.
- src/cpp/wifi_profile.hpp and wifi_profile.cpp: C++ profile metadata.
- src/cpp/wifi_manager.hpp and wifi_manager.cpp: C++ integration shim.
- src/cpp/wifi_manager_example.cpp: C++ management example.
- ../wifi-common: shared C/C++ manager, security policy and native providers.
- src/java: Java reference profile.

## Profile-only builds

    cc -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc/c src/c/wifi_profile.c src/c/example.c -o wifi-profile-c
    ./wifi-profile-c

    c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -Isrc/cpp src/cpp/wifi_profile.cpp src/cpp/example.cpp -o wifi-profile-cpp
    ./wifi-profile-cpp

    mkdir -p build/java
    javac -Xlint:all -Werror -d build/java src/java/WifiProfile.java src/java/WifiProfileDemo.java
    java -cp build/java WifiProfileDemo

## Linux Wi-Fi management example

Build the C API with NetworkManager/nmcli:

    cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -I../wifi-common/include -Isrc/c ../wifi-common/src/wifi_manager.c ../wifi-common/src/wifi_backend_nmcli.c src/c/wifi_profile.c src/c/wifi_manager.c src/c/wifi_manager_example.c -o wifi-manager-c
    ./wifi-manager-c

For C++, compile the C core/provider as C objects before linking with the C++ driver:

    mkdir -p build
    cc -std=c11 -D_POSIX_C_SOURCE=200809L -I../wifi-common/include -c ../wifi-common/src/wifi_manager.c -o build/wifi-manager-core.o
    cc -std=c11 -D_POSIX_C_SOURCE=200809L -I../wifi-common/include -c ../wifi-common/src/wifi_backend_nmcli.c -o build/wifi-manager-backend.o
    c++ -std=c++17 -Wall -Wextra -Wpedantic -I../wifi-common/include -Isrc/cpp src/cpp/wifi_profile.cpp src/cpp/wifi_manager.cpp ../wifi-common/src/wifi_manager_cpp.cpp src/cpp/wifi_manager_example.cpp build/wifi-manager-core.o build/wifi-manager-backend.o -o wifi-manager-cpp
    ./wifi-manager-cpp

Windows builds use src/wifi_backend_windows.c and link wlanapi/ole32. macOS builds use src/wifi_backend_macos.mm and link CoreWLAN/Foundation/Security. Native OS profiles handle authentication; this avoids exposing credentials in process arguments. See ../wifi-common/README.md and ../wifi-common/SECURITY.md.

## Notes

Wi-Fi 5 uses 5 GHz in normal 802.11ac operation. Peak PHY rate is theoretical and depends on streams, channel width, modulation/coding and guard interval. Always defer regulatory decisions and actual capability discovery to the OS/driver.
