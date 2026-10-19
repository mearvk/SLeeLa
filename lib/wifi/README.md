<img align="right" src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Wi-Fi Library

This library provides SLeeLa-native reference models and static capability profiles for Wi-Fi 4, Wi-Fi 5, and Wi-Fi 6, aligned with the companion documentation and C, C++, and Java references in `/wi-fi-4`, `/wi-fi-5`, and `/wi-fi-6`.

## Packages

- [Wi-Fi 4 / IEEE 802.11n](4/README.md)
- [Wi-Fi 5 / IEEE 802.11ac](5/README.md)
- [Wi-Fi 6 / IEEE 802.11ax](6/README.md)

Each version contains a `.sleela` model, a static `wifi.conf` profile, a machine-readable `capabilities.json`, and a small SLeeLa validation example. These are declarative capability/reference layers; they do not drive radio hardware, scan nearby networks, join an access point, or replace operating-system Wi-Fi services.

## Safety And Correctness

Capability maxima are theoretical standard-level limits, not promises about a specific adapter or network. Runtime software must discover actual adapter/AP capabilities, honor the device's regulatory domain, and delegate connection management to supported operating-system APIs. Wi-Fi 6 at 6 GHz is specifically Wi-Fi 6E and must not be inferred from generic Wi-Fi 6 support.

## Running

From the repository root, use the SLeeLa runtime/compiler supported by your checkout, for example:

```sh
sleela run lib/wifi/4/Wifi4Model.sleela
sleela run lib/wifi/5/Wifi5Model.sleela
sleela run lib/wifi/6/Wifi6Model.sleela
```

The validation examples are standalone demonstrations of the model contract. Confirm syntax and runtime behavior against the compiler version installed in your build environment.
