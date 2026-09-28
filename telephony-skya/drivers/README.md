# Skya Drivers

## Canonical model-driver layout

Every model-specific driver is stored individually under:

```
drivers/<brand>/<model>/<version>/
```

Each version directory contains both language implementations:

```
driver.h
driver.c
driver.cpp
```

Example:

```
drivers/yealink/mp45/1.0/
├── driver.h
├── driver.c
└── driver.cpp
```

The version directory identifies the **Skya driver adapter version**. It is not a claim about the device firmware version.

## Current model families

The tree contains individual adapters for the current Yealink, Poly, Jabra, Grandstream, EPOS, Logitech, Fanvil, Snom and Cisco model catalog.

## Language policy

C and C++ implementations are maintained side-by-side for every model. The C API uses the `skya_c_<model>_driver()` symbol; the C++ API uses `skya_<model>_driver()`.

The implementations share the same `skya_phone_driver` contract and capability semantics.

## Hardware identity

Model names alone do not establish hardware identity. Exact VID/PID, interface descriptors, HID reports and vendor-specific controls must be added only after verification.

Windows exposes USB hardware identifiers derived from VID, PID and revision information, and composite devices may expose interface-specific identifiers. citeturn1search2turn1search7

Skya therefore treats the model driver as a logical adapter above the operating-system USB/audio/HID stack. A custom OS driver is not automatically required for a supported USB class; Microsoft recommends using an inbox class driver when it satisfies the device requirements. citeturn1search4turn1search6
