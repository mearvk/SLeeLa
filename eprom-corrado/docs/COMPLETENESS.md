# eprom-corrado — Driver Completeness

Audit status for the EPROM programmer driver family, using the status concepts
and vocabulary from the repository-level `drivers/DRIVER.COMPLETENESS.md` and
`TERMINOLOGY.md`. This record is deliberately conservative: it does not convert
assumptions into actual hardware values to appear complete.

## Overall status

**Provisional path (software-verified; not yet hardware-complete).**

The image/checksum library, the operator logic (backup / copy / delete), the
`EpromControl` contract, and the three connector transports are implemented and
tested against a fake backend and reference images. The live-hardware USB path
(real TL866 bulk framing to a physical EPROM) is **not** audited here and is
not claimed hardware-complete.

## Device-specific completion checklist

Mapped against `DRIVER.COMPLETENESS.md` §"Device-specific completion":

| # | Item | Status | Evidence / note |
|---|------|--------|-----------------|
| 1 | Exact model & revision | **Actual** | Target: 27C256 (32768×8) EPROM in the Corrado ECU; 27C512 twin-tune variant. From Corrado `corrado_eprom.h`. |
| 2 | Firmware/interface version | **Unknown** | TL866 firmware revision varies per unit; not enumerated here. |
| 3 | Transport | **Actual** | USB bulk via libusb-1.0; endpoints EP_OUT 0x01 / EP_IN 0x81. |
| 4 | USB descriptors | **Actual (IDs)** | TL866A VID 0x04D8 PID 0xE11C; TL866II+ VID 0xA466 PID 0x0A53. Full descriptor set not re-enumerated. |
| 5 | Pin/control mappings | **Not applicable** | Programmer abstracts chip pins; no direct pin control exposed. |
| 6 | Register/memory mappings | **Actual** | Flat 32768-byte image; trailing 2-byte little-endian checksum word. |
| 7 | HID report layouts | **Not applicable** | Bulk device, not HID. |
| 8 | Command/response paths | **Provisional** | TL866 bulk read/write/verify framing is a readable reference in Corrado's drivers; cross-check vs. upstream `minipro` before real flashing. |
| 9 | I/O data paths | **Verified (software)** | read→image, image→write→verify exercised over the fake backend and reference images. |
| 10 | State transitions | **Verified (software)** | open → read/write/erase → blank-check → close modelled and tested. |
| 11 | Error & recovery paths | **Verified (software)** | Explicit failure reasons (ARG/IO/SIZE/CHECKSUM/NO_DEVICE/USB/VERIFY/UNSUPPORTED/TIMEOUT). |
| 12 | Hotplug/reconnect | **Unknown** | Not implemented in this reference. |
| 13 | Cache/DMA | **Not applicable** | No DMA path in the host-side library. |
| 14 | Unsupported-feature reporting | **Actual** | UV/OTP 27C parts report UNSUPPORTED for electrical erase (must use a UV eraser); reusable replacements succeed. |
| 15 | Platform-specific behavior | **Verified (software)** | Linux/macOS/Windows build paths exist; Linux core + vendored-libusb build verified. |
| 16 | Test evidence | **Partial** | Checksum library and connector transports verified against `G60/VR6 *_REFERENCE.bin`; Process transport verified driving the real compiled CLI. No live-chip test. |

## What would raise this to hardware-operational / complete

1. Verify the TL866 bulk framing against a physical unit of a known firmware
   revision (closes item 8 and part of 16).
2. Enumerate the full USB descriptor set and firmware version on real hardware
   (items 2, 4).
3. Add hotplug/reconnect handling and record its behaviour (item 12).
4. Record live read/write/verify evidence on a physical 27C256 / reusable part
   (item 16).

Until then this family is a **provisional path** behind an explicit evidence
boundary — accurate, reusable, and honest about where real-hardware proof is
still owed. Never treat the provisional framing as a verified hardware value.
