# Skya Platform Driver Coverage

## Windows 10+

The Windows platform layer enumerates active audio endpoints through Windows MMDevice APIs and routes model selection through the shared registry. Product control uses the shared skya_product_transport callback boundary so vendor HID/USB command encoding can be supplied without duplicating model state logic.

Expected transport stack:
- MMDevice/WASAPI for audio;
- HID/WinUSB/vendor transport for device controls;
- Windows device notifications for hotplug/reconnect;
- UTF-8 normalized model metadata at the Skya boundary.

## macOS

The macOS platform layer enumerates Core Audio devices and routes model selection through the shared registry. Product control uses the same transport boundary, with IOHID/USB/vendor transport backends responsible for device-specific reports.

Expected transport stack:
- Core Audio for audio;
- IOHID/USB/vendor transport for controls;
- device notifications for hotplug/reconnect.

## Linux

Linux remains the native reference platform using ALSA/platform enumeration and the same product-runtime interface.

## Completeness rule

A host implementation is operationally complete only when its platform transport can actually send the applicable device control and receive the applicable response. Enumeration alone is not hardware-control certification.
