# Remaining Model Driver Expansion

This expansion adds model-specific adapters for the remaining models in the current Skya driver catalog.

## Added models

### Yealink
- UH42
- UH44
- UH46
- WH64
- WH68

### Poly
- Blackwire 3320
- Savi 8200
- Savi 8400
- Voyager 4320
- Voyager 5200

### Jabra
- Biz 1500
- Evolve3 65
- Evolve3 75
- SPEAK2 55
- SPEAK2 75

### Grandstream
- GUV3005
- GXP21xx family
- GRP26xx family

### EPOS
- IMPACT SC 200
- IMPACT 1000
- SDW 5000

### Logitech
- Zone Wireless 2
- Zone Vibe
- H570e
- Zone 305

### Fanvil
- X5U-V2
- X6U-V2
- V63
- V64
- V65
- V66
- X210i-V2

### Snom
- A330M

### Cisco
- 322

## Matching policy

These drivers use exact model/family string matching against the identity supplied by the platform discovery layer.

USB models receive the standard audio capability profile and expose answer/end only when the discovered transport is USB HID.

SIP/network models receive telephone controls only when the discovered transport is SIP or network.

The driver does not claim a physical HID report, USB VID/PID, firmware interface, or vendor-specific command sequence merely from the product name.

## Evidence

The current manufacturer/compatibility evidence supports expanding the catalog in these areas. Yealink currently lists UH42/UH44/UH46 and WH64/WH68 in its headset portfolio. citeturn0search0

Grandstream's current compatibility documentation identifies GUV3005, Poly Blackwire families, Jabra Biz/Evolve families, and EPOS IMPACT SC 200/600 USB devices as compatible USB headset classes, while also documenting Bluetooth and EHS device classes. citeturn0search1turn0search2

## Status

These are MODEL-IDENTIFIED / CATALOGED adapters. Physical validation is still required before a device can be marked TESTED or CERTIFIED.

The next hardware-specific layer should establish:

1. exact VID/PID;
2. USB interface descriptors;
3. HID report descriptors;
4. physical button mappings;
5. LED/busylight controls;
6. firmware query/update behavior;
7. hotplug and reconnect behavior;
8. Linux validation;
9. Windows 10+ validation;
10. macOS validation.
