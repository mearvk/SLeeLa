# Wi-Fi 4 Source Build And Test Notes

## Scope

This source series is a small, portable reference library for the known capabilities associated with IEEE 802.11n. It is intended to compile with ordinary C, C++, and Java toolchains and to give downstream SLeeLa code a consistent place to read generation metadata and validate common channel-width inputs.

It is **not** an IEEE 802.11 PHY/MAC implementation, does not scan for networks, does not configure a radio, and does not claim that a particular adapter supports every standard capability. Real capabilities must come from the operating system, driver, firmware, and certified hardware. Regulatory decisions must remain with those components.

## Layout

- `src/c/wifi_profile.h`, `src/c/wifi_profile.c`: C reference API and metadata.
- `src/c/example.c`: C demonstration.
- `src/cpp/wifi_profile.hpp`, `src/cpp/wifi_profile.cpp`: C++ reference API and metadata.
- `src/cpp/example.cpp`: C++ demonstration.
- `src/java/WifiProfile.java`: Java reference class.
- `src/java/WifiProfileDemo.java`: Java demonstration.

## Build And Run

From this directory:

### C (C11)

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc/c src/c/wifi_profile.c src/c/example.c -o wifi-profile-c
./wifi-profile-c
```

### C++ (C++17)

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -Isrc/cpp src/cpp/wifi_profile.cpp src/cpp/example.cpp -o wifi-profile-cpp
./wifi-profile-cpp
```

### Java (Java 8+)

```sh
mkdir -p build/java
javac -Xlint:all -Werror -d build/java src/java/WifiProfile.java src/java/WifiProfileDemo.java
java -cp build/java WifiProfileDemo
```

## Engineering Notes

- The peak rate is a theoretical PHY-layer reference, not promised user throughput.
- Supported widths in this profile describe generation-level capabilities, not a specific adapter or country.
- Wi-Fi 4 features are listed for documentation and planning; enabling them requires hardware and driver support.
- Keep credentials out of source, logs, and diagnostics. Application data should use correctly validated TLS.
- Check the current IEEE 802.11 base standard and relevant amendments for normative requirements. The active consolidated IEEE standard is listed at https://standards.ieee.org/ieee/802.11/10548/.
