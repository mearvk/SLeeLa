# Skya Hardware Control and Product Data Plane

The driver stack now has a concrete boundary for model/version evidence, pin and power metadata, memory, cache policy, and product data movement.

## Evidence

A product name does not establish a board pinout. Every hardware fact is classified as UNKNOWN, DOCUMENTED, ENUMERATED, MEASURED, TESTED, or CERTIFIED. A driver must not invent a GPIO number, voltage rail, USB endpoint, HID report, register address, memory size, or firmware command from a product name alone.

## Pins and power

The hardware profile records power rails, ground, input/output/bidirectional pins, control/reset/clock/data pins, LEDs, buttons and audio pins. Each pin can carry a logical identifier, direction, pull, active polarity, voltage range, current limit and evidence level.

The control backend is the only layer allowed to touch real GPIO, regulators, ADC/DAC, MMIO, USB, HID, I2C, SPI or other board interfaces. Voltage writes are range-checked first. Unknown or unspecified voltage rails cannot be driven through this API.

For USB HID devices, report fields must be discovered from descriptors and HID usages rather than guessed from a model name. USB-IF describes HID as self-describing and defines Input, Output and Feature data items. The current HID Usage Tables provide the usage meanings. citeturn1search0turn1search2

## Memory and cache

Profiles describe registers, RAM, flash, EEPROM, NVRAM and DMA regions with address, size, alignment, access permissions, volatility, cache policy and evidence. Explicit flush/invalidate callbacks allow the backend to implement the correct platform and bus rules instead of assuming DMA coherence.

## Product data plane

The new data plane provides a bounded queue, producer/consumer locking, condition-variable wakeups, sequence numbers, monotonic timestamps, CRC-32 integrity checking, size validation, backpressure, and separate audio/HID/control/telemetry/firmware channels.

This is above the existing runtime queue and below model-level policy. It is intended to carry actual product data without bypassing the hardware boundary.

## Model/version expansion

For every supported model and revision, record exact vendor/model/family, hardware revision, firmware version, interface revision, VID/PID and interface descriptors where applicable, HID report descriptors and usage mappings where applicable, physical pinout only when documented or measured, power limits from authoritative documentation, memory/register maps from authoritative documentation, cache/DMA behavior, and validation status on Linux, Windows 10+, and macOS.

USB power-related controls should use the applicable USB Power Delivery or Power Device specification rather than treating a USB port as an arbitrary programmable voltage source. USB-IF maintains the current USB PD and Power Device specifications. citeturn1search5turn1search13

## Safety boundary

The API deliberately does not guess physical pins or enable arbitrary voltage writes. A board-specific backend must establish the exact electrical limits and implement the callbacks before hardware control is enabled.
