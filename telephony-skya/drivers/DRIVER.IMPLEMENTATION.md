# Skya™ Driver Implementation Status

## Purpose

This document records the first executable cross-platform driver layer for Skya™ Telephony.

The implementation is intentionally layered:

    OS device enumeration
        -> Skya platform adapter
        -> common driver registry
        -> vendor driver probe
        -> standard audio fallback
        -> runtime capability record

## Implemented

### Common C/C++ layer

- `include/skya_phone_driver.h`
- `include/skya_platform_driver.h`
- `include/skya_builtin_drivers.h`
- `src/skya_driver_registry.cpp`
- `src/skya_builtin_drivers.cpp`

The registry now contains vendor adapters for:

- Yealink
- Poly
- Jabra
- Grandstream
- EPOS
- Logitech
- Fanvil
- Snom
- Cisco

A standard audio fallback is also present for USB Audio and Bluetooth transports.

### Linux

`platform/linux/skya_linux_driver.cpp`

The Linux adapter enumerates ALSA-visible devices through `/proc/asound` as a first discovery layer and converts them into the common Skya platform-device structure.

It then performs:

    platform device
      -> Skya logical device
      -> registry probe
      -> capabilities

This is intentionally an initial user-space discovery implementation. It is not a replacement for the Linux kernel or ALSA driver.

### Windows 10+

`platform/windows/skya_windows_driver.cpp`

The Windows adapter uses the Windows audio endpoint architecture and WASAPI/MMDevice interfaces to enumerate active audio endpoints, capture endpoint identity/friendly-name information, and pass the result into the common Skya registry.

Windows' Audio Endpoint Builder creates and manages application-facing endpoints; Skya consumes those endpoints rather than replacing the Windows audio stack.

### macOS

`platform/macos/skya_macos_driver.cpp`

The macOS adapter uses Core Audio's hardware-device enumeration to obtain active audio-device identities and feeds those devices into the common Skya registry.

A DriverKit/AudioDriverKit system driver is deliberately not introduced for ordinary devices. That layer will be added only for hardware that genuinely requires a system-driver implementation.

## Load Sequence

    enumerate
      -> identify
      -> registry probe
      -> capability query
      -> runtime activation

The platform adapter does not automatically claim a device is fully supported merely because it was enumerated.

## Vendor Support Model

Current vendor entries provide the common identity/probe/capability boundary. Hardware-specific HID reports, SIP control protocols, firmware operations, and proprietary feature APIs remain separate implementation work.

Therefore:

- cataloged does not mean hardware-certified;
- discovered does not mean all capabilities work;
- audio-working does not mean call-control-working;
- vendor adapter presence does not mean every model in that vendor family is supported.

## Next Driver Implementation Layer

The next concrete layer should add, in order:

1. exact device identity matching;
2. USB VID/PID and OS device-instance capture;
3. HID report/usage discovery;
4. mute/answer/end/hold/volume operations;
5. hotplug and reconnect callbacks;
6. per-model capability declarations;
7. vendor-specific controls;
8. SIP/network phone adapters;
9. hardware test records;
10. platform-specific packaging/signing where required.

## Platform Principles

Linux: consume ALSA/HID/USB/Bluetooth facilities before considering kernel code.

Windows 10+: consume PnP, class drivers, audio endpoints, WASAPI, and HID before considering a custom driver.

macOS: consume Core Audio and standard device interfaces before introducing DriverKit/AudioDriverKit.

These principles align the Skya driver layer with the operating systems rather than attempting to replace their established device stacks.

## Important Status

This is the first executable driver foundation, not a claim of universal hardware support. The implementation now provides a real structure into which tested device-specific behavior can be added.
