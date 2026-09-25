# Skya Known-Product Driver Implementation

This release promotes the known-product model files from capability-only adapters to a shared, concrete product-runtime architecture.

## Known product set

Yealink: MP45, MP50, UH42, UH44, UH46, WH64, WH68.
Poly: Blackwire 3320, Blackwire 5220, Savi 8200, Savi 8400, Voyager 4320, Voyager 5200.
Jabra: Biz 1500, Evolve2 40, Evolve3 65, Evolve3 75, Speak2 55, Speak2 75.
Grandstream: GRP26xx, GUV3000, GUV3005, GXP21xx.
EPOS: IMPACT 1000, IMPACT SC 200, IMPACT SC 600, SDW 5000.
Logitech: H570e, Zone 305, Zone Vibe, Zone Wired 2, Zone Wireless 2.
Fanvil: V63, V64, V65, V66, X210i-V2, X4U-V2, X5U-V2, X6U-V2.
Snom: A330D, A330M.
Cisco: 321, 322.

## Runtime implementation

skya_product_runtime.h and skya_product_runtime.c provide validated model/vendor matching, per-device capability policy, answer/end/hold/mute/volume/dial/busy-light operation state, transport callback dispatch, firmware query and descriptor retrieval, hotplug state, reset state, operation accounting, Linux/Windows 10+/macOS host selection, and compatibility with the existing hardware profile and bounded data plane.

The model files remain responsible for exact product identity. The runtime is responsible for the common operational state machine and the platform/vendor transport boundary.

## Hardware evidence boundary

The runtime does not fabricate VID/PID values, HID report IDs, report byte offsets, register maps, firmware formats, or vendor-specific command packets. Those must be supplied by a verified transport backend or discovered from the device. A model being implemented does not mean that every hardware revision has been physically tested.

## Platform coverage

Linux uses the existing ALSA/platform layer. Windows 10+ uses the existing Windows audio enumeration layer and is designed for WASAPI/MMDevice plus vendor HID/USB transport backends. macOS uses the existing Core Audio enumeration layer and is designed for Core Audio plus IOHID/USB transport backends.

SKYA_HOST_WINDOWS_10 and SKYA_HOST_MACOS are explicit runtime targets so model drivers do not need duplicated product logic merely because the host operating system changes.
