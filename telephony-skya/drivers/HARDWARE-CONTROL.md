# Skya Hardware Control and Product Data Plane

## Unknowns, notes, and working defaults

Where exact hardware facts are not yet available, the profile records the unknown and may additionally carry a Standard/Expected Default. A default is a documented engineering starting assumption, not a discovered fact and not permission to drive hardware.

| Item | Standard/Expected default | Status |
|---|---|---|
| GPIO number | UNASSIGNED / gpio:auto | assumption only |
| GPIO direction | Input / high-impedance | safe planning default |
| GPIO pull | None | assumption only |
| GPIO polarity | Active-high | assumption only |
| Digital logic level | 3.3 V nominal | common planning assumption; verify |
| GPIO current | Unknown; do not source/sink until documented | safety default |
| Power rail | Unknown | never infer from model name |
| USB VBUS | 5 V nominal bus supply | standards-based expectation; not a programmable rail |
| Memory/register address | UNASSIGNED | assumption only |
| Memory size | Unknown | never fabricate capacity |
| Memory access | Read-only until write capability is established | conservative default |
| Cache/DMA coherence | Unknown; explicit flush/invalidate required | conservative software default |
| HID report layout | Discover from descriptor | do not assume byte offsets |
| HID input/output/feature direction | Descriptor-defined | discover at runtime |
| Firmware version | Unknown until enumerated/queried | never infer from model |

These defaults are intentionally marked SKYA_VALUE_DEFAULT_ASSUMPTION. Code refuses hardware writes through an assumed pin/voltage/memory definition and returns -EAGAIN until the value is established.

Linux GPIO guidance favors descriptor-based GPIO access rather than legacy integer GPIO numbers, and GPIO direction must be established before use. HID is self-describing, and Input/Output/Feature fields are defined by descriptors/usages. citeturn0search1turn0search2turn0search0turn0search4

## Evidence

Every hardware fact is classified as UNKNOWN, DOCUMENTED, ENUMERATED, MEASURED, TESTED, or CERTIFIED. An assumption is never silently promoted to an actual value.

## Pins, voltage and control

The hardware profile records power rails, ground, input/output/bidirectional pins, control/reset/clock/data pins, LEDs, buttons and audio pins. Each pin can carry a logical identifier, direction, pull, active polarity, voltage range, current limit, evidence level, and assumption note.

The control backend is the only layer allowed to touch real GPIO, regulators, ADC/DAC, MMIO, USB, HID, I2C, SPI or other board interfaces. The API range-checks voltage and refuses writes based only on an assumption.

## Memory and cache

Profiles describe registers, RAM, flash, EEPROM, NVRAM and DMA regions with address, size, alignment, access permissions, volatility, cache policy and evidence. Unknown addresses and sizes remain unassigned. Explicit flush/invalidate callbacks are used where cache/DMA behavior has not yet been established.

## Product data plane

The data plane provides bounded buffering, producer/consumer locking, condition-variable wakeups, sequence numbers, monotonic timestamps, CRC-32 integrity checking, size validation, backpressure, and audio/HID/control/telemetry/firmware channels.

## Model/version expansion

For every supported model/revision, record exact vendor/model/family, hardware revision, firmware version, interface revision, VID/PID and interface descriptors where applicable, HID report descriptors/usages where applicable, physical pinout only when documented or measured, power limits from authoritative documentation, memory/register maps from authoritative documentation, cache/DMA behavior, and validation status on Linux, Windows 10+, and macOS.

The intended workflow is: documented fact -> enumerated fact -> measured fact -> tested fact, with certification where applicable. The Standard/Expected Default remains visible beside the unresolved item until replaced by evidence.
