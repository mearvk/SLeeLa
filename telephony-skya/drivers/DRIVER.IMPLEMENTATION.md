# Skya Driver Implementation

## Model-specific driver layer

Skya now separates three levels of driver matching:

1. **Exact model driver** — matches a specific vendor/model string.
2. **Vendor-family driver** — matches the vendor when no exact model driver matches.
3. **Standard-audio fallback** — matches standards-based USB/Bluetooth audio when no vendor match is available.

Exact model drivers are registered first so that a known model receives its model-specific capability policy before falling back to the broader vendor adapter.

Current model adapters include:

- Yealink MP45
- Yealink MP50
- Poly Blackwire 5220
- Jabra Evolve2 40
- Grandstream GUV3000
- EPOS IMPACT SC 600
- Logitech Zone Wired 2
- Fanvil X4U-V2
- Snom A330D
- Cisco 321

The model implementation lives in:

- `drivers/include/skya_model_drivers.h`
- `drivers/src/skya_model_drivers.cpp`

The registry order is:

```
exact model
    -> vendor family
    -> standard audio
```

## Capability policy

Model identification is not certification.

A model driver may establish the capability policy appropriate to the device class, but the platform layer must still determine the actual transport and available interfaces. USB HID call-control capabilities are only exposed when the discovered transport is `SKYA_TRANSPORT_USB_HID`. Fanvil X4U-V2 is treated as a SIP/network telephone and its telephone controls are exposed only for SIP/network transport.

## Evidence boundary

The initial model set is grounded in manufacturer product specifications. For example, Yealink documents MP45 and MP50 as USB phones with call controls, while HP Poly documents Blackwire 5220 USB call answer/end, mute, and volume controls, and Jabra documents Evolve2 40 USB-A/USB-C variants and Jabra Direct support.

Manufacturer documentation:

- https://www.yealink.com/en/product-detail/microsoft-teams-phone-mp45
- https://www.yealink.com/website-service/attachment/product_resource/documents/20220525/202205250648006524a5aea68480e91546caf0511b83f.pdf
- https://support.hp.com/us-en/product/product-specs/blackwire-5200-series/model/2101719547
- https://www.jabra.com/business/office-headsets/jabra-evolve/jabra-evolve2-40

These sources establish documented device characteristics; they do not replace physical Skya compatibility testing.

## Next model-driver work

For each model, the next implementation layer should add:

- exact USB VID/PID where documented and verified;
- USB interface descriptors;
- HID report discovery;
- answer/end/mute/hold/volume mappings;
- LED and busy-light controls;
- firmware query;
- hotplug/reconnect behavior;
- unsupported-feature reporting;
- Linux test record;
- Windows 10+ test record;
- macOS test record;
- hardware revision and firmware matrix;
- official vendor documentation/download links.

No model should be marked `TESTED` or `CERTIFIED` until the exact hardware and software combination has been exercised.
