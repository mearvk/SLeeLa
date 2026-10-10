# Universal Serial Bus — Specification (usb)

USB is a tiered-star bus: a host controller (root hub) with devices and hubs
below it. `lib/os/usb` models the finite set of USB specification generations
and device classes by name.

## Generations and signalling rate

| Generation (marketing) | Signalling rate | Era name |
|---|---:|---|
| USB 1.1 (Full Speed) | 12 Mb/s | USB 1.x |
| USB 2.0 (High Speed) | 480 Mb/s | USB 2 |
| USB 3.2 Gen 1 | 5 Gb/s | formerly USB 3.0 / 3.1 Gen 1 |
| USB 3.2 Gen 2 | 10 Gb/s | formerly USB 3.1 Gen 2 |
| USB 3.2 Gen 2x2 | 20 Gb/s | two-lane Type-C |
| USB4 | 20 / 40 Gb/s | tunnels PCIe/DisplayPort; Thunderbolt-compatible |

The USB-IF has repeatedly renamed the 3.x tier; the SLeeLa classes carry both
the ordinal and the current marketing name.

## Connectors

- **Type-A** — the classic rectangular host plug.
- **Type-B / Micro-B** — device-side legacy connectors.
- **Type-C** — reversible, required for USB 3.2 Gen 2x2 and USB4, carries USB
  Power Delivery and alternate modes.

## Device classes (abbreviated)

| Class | Examples |
|---|---|
| HID | keyboard, mouse, game controller |
| Mass Storage | flash drive, external SSD/HDD |
| Hub | downstream port expander |
| Audio / Video | headset, webcam |
| Host Controller | xHCI root hub on the mainboard |

## Power Delivery

USB-PD negotiates higher voltages/currents over Type-C (up to 240 W with EPR).
`SLUSBDevice.hasPowerDelivery()` reflects whether a port advertises PD.

## Manufacturers

Host-controller (xHCI) silicon comes from **Intel**, **AMD**, **ASMedia**,
**VIA**, **Renesas**, **Texas Instruments**, and **Fresco Logic**;
`SLUSBCatalog` exposes controllers, hubs, and class devices by name. On a UEFI
machine the controller is enumerated by firmware so USB keyboards/boot media
work before the OS loads.
