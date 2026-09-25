# TERMINOLOGY

## Purpose

This glossary defines the general and technical language used by the SLeeLa / Skya driver infrastructure.

## General Definitions

**Actual Value** — A value established from authoritative documentation, device enumeration, measurement, or completed testing.

**Assumption** — A provisional value used to continue design or software development when the actual value is not known.

**Default** — A value selected when no more specific value is available.

**Standard/Expected Default** — A clearly labeled best-guess value based on standards, common device architecture, or normal engineering practice. It is not manufacturer-specific fact.

**Unknown** — A property for which sufficient evidence is not currently available.

**Evidence** — Information supporting a hardware or software claim.

**Validation** — Checking that a proposed value or behavior is compatible with the actual device and environment.

**Verification** — Demonstrating that an implementation satisfies defined requirements.

**Control** — An operation that changes device state or requests a device action.

**Data Plane** — The path through which operational product data is transported.

**Control Plane** — The path through which configuration, state changes, commands, and management operations are transported.

## Technical Definitions

**GPIO** — General-Purpose Input/Output: a digital hardware signal normally configurable as input or output. Modern Linux drivers should generally use GPIO descriptors/functions rather than hard-coded global integer GPIO numbers. citeturn0search1turn0search3

**GPIO Number** — A numeric identifier for a GPIO line. It is platform-specific. Modern Linux GPIO architecture favors descriptors rather than hard-coded global numbers. citeturn0search3turn0search8

**GPIO Offset** — The hardware-relative line position within a GPIO controller.

**GPIO Descriptor** — An opaque Linux kernel representation of a GPIO line obtained through the descriptor consumer API. citeturn0search1

**Pin Role** — The intended electrical/software purpose of a pin: power, ground, input, output, control, reset, clock, data, LED, button, audio, or bidirectional.

**Direction** — Whether a digital signal is input, output, or bidirectional.

**Pull-Up** — A resistor or equivalent bias tending to hold a digital signal high when not actively driven.

**Pull-Down** — A resistor or equivalent bias tending to hold a digital signal low when not actively driven.

**Active-High** — Logical assertion represented by a high physical signal.

**Active-Low** — Logical assertion represented by a low physical signal. Linux GPIO descriptors can represent active-low semantics so consumers can use logical rather than raw values. citeturn0search2

**Voltage Rail** — A power supply node with a defined voltage range and electrical limits.

**Logic Level** — The voltage range interpreted as digital low/high. A 3.3 V nominal value is a planning default here when a device-specific logic voltage is not established; it is not a claim about a particular product.

**VBUS** — The USB bus power supply. 5 V nominal is a standards-based planning expectation for ordinary USB VBUS; it is not an arbitrary programmable rail.

**Current Limit** — The maximum permitted current associated with a pin, rail, regulator, or interface.

**Memory Region** — A defined addressable area such as registers, RAM, flash, EEPROM, NVRAM, or DMA-visible memory.

**Register** — A hardware-defined addressable location used for control, status, configuration, or data.

**Memory Map** — The documented relationship between device addresses and registers or memory regions.

**Cache Policy** — Rules governing how data is cached, written back, written through, or treated as device memory.

**DMA** — Direct Memory Access, in which a device or controller transfers data to or from memory without requiring CPU copying for every byte.

**Cache Coherency** — Whether CPU caches and device/DMA accesses observe a consistent view of memory without explicit synchronization.

**Flush** — An operation ensuring pending cached writes reach the required visibility point.

**Invalidate** — An operation discarding stale cached copies so subsequent reads obtain current data.

**HID** — Human Interface Device, a USB device class that describes device controls and data through descriptors and usages. HID is intentionally self-describing. citeturn0search0turn0search4

**HID Report Descriptor** — The descriptor describing the fields and organization of HID reports.

**HID Input** — Data provided by the device to the host.

**HID Output** — Data sent from the host to the device, such as control or indicator state.

**HID Feature** — Configuration/state information exchanged through HID feature reports. USB HID defines Input, Output, and Feature items separately. citeturn0search24turn0search25

**Descriptor-Driven** — An implementation strategy in which the driver discovers capabilities and data layout from authoritative descriptors instead of assuming a model-specific layout.

**Probe** — The process of determining whether a driver can operate a detected device.

**Enumeration** — Discovering devices and their descriptors/interfaces through the operating system or bus.

**Capability** — A function a device/driver exposes, such as audio input, audio output, mute, volume, call control, display, or firmware query.

**Model** — The manufacturer's product designation.

**Hardware Revision** — A physical hardware revision of a product or board.

**Firmware Version** — The version identifier of software stored or executed by the device.

**Interface Revision** — The revision of a communication interface or protocol exposed by the device.

**Buffer** — Temporary storage used to absorb differences in producer and consumer timing.

**Ring Buffer** — A bounded circular buffer in which head and tail positions wrap around.

**Backpressure** — A mechanism preventing an overloaded consumer path from silently accepting unlimited data; the Skya data plane reports a full queue with -EAGAIN.

**Sequence Number** — A monotonically increasing identifier assigned to a data frame.

**CRC-32** — A 32-bit cyclic redundancy check used by the Skya data plane to detect accidental payload corruption.

**Lock** — A synchronization mechanism protecting shared state from concurrent access.

**Condition Variable** — A synchronization primitive allowing a thread to sleep until shared state changes.

**Data Frame** — A header plus payload representing one unit of product data in the Skya data plane.

**Telemetry** — Operational information about device state, performance, errors, or environment.

**Firmware Channel** — The data-plane channel reserved for firmware-related transport and operations.

## Skya-specific Rule

When a value is unknown, source code may carry a Standard/Expected Default, but it must also carry SKYA_VALUE_DEFAULT_ASSUMPTION and an assumption note. Hardware write paths reject assumed pin, voltage, and memory definitions until the value is replaced or explicitly validated.

Unknown != Default != Actual.

A default helps development proceed; it does not manufacture a hardware fact.
