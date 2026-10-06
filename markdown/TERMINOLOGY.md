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


## Driver Completeness and Implementation Definitions

**Driver Infrastructure** — The shared mechanisms that allow drivers to represent hardware, communicate with devices, transport data, synchronize concurrent operations, validate state, and expose control functions. Infrastructure being implemented does not by itself prove that every device-specific driver is complete.

**Device Binding** — The device-specific portion of a driver that connects a detected vendor/model/interface to its actual commands, descriptors, transport, capabilities, and hardware behavior.

**Generic Implementation** — A reusable implementation that provides common behavior without claiming to encode every model-specific hardware detail. Generic code must not be mistaken for a verified device binding.

**Model-Specific Implementation** — Code and metadata explicitly tied to a documented product model, hardware revision, firmware family, or interface revision.

**Hardware-Operational** — A driver state in which the required device-specific operations have been implemented and are supported by sufficient hardware documentation, enumeration, measurement, or testing.

**Hardware-Complete** — A stronger status indicating that the relevant device-specific controls, data paths, state transitions, error handling, synchronization, and known hardware mappings have been implemented and verified to the defined scope.

**Stub Method** — A method whose body is primarily a placeholder, no-op, fixed dummy result, or incomplete path rather than an implementation of its intended operation.

**Placeholder Implementation** — Code intentionally present to reserve an interface or control path before the actual implementation is known or available.

**No-Op** — An operation that intentionally performs no state change. A no-op is not automatically a stub; its intent must be documented.

**Unimplemented Operation** — An operation for which the required behavior has not yet been provided. It must be distinguished from a valid unsupported-device response.

**Unsupported Operation** — A defined operation that the detected device or interface does not provide. This is different from an implementation that has simply not been written.

**Capability Gap** — A known difference between the capabilities required by the driver contract and those currently implemented or supported by a specific device.

**Implementation Gap** — A known area where code required by the driver contract remains incomplete, provisional, or generic.

**Audit** — A systematic review of driver source, model metadata, interfaces, error paths, and hardware assumptions to determine whether implementation requirements are actually satisfied.

**Driver Audit** — An audit focused specifically on identifying stubs, placeholders, generic-only paths, unsupported operations, missing model bindings, missing hardware mappings, incomplete error handling, and unverified assumptions.

**Stub Audit** — A targeted search and review for placeholder patterns such as TODO/FIXME markers, dummy returns, empty handlers, unimplemented errors, fixed success responses, and other evidence that an intended operation is not actually implemented. Search results alone are not proof of completeness; source behavior must be reviewed.

**Implementation Audit** — Review of whether each declared driver operation has a meaningful implementation, correct state handling, appropriate error behavior, and an identified hardware/transport path.

**Hardware Binding Audit** — Review of whether a model's GPIO, voltage, memory, HID, transport, firmware, controls, and other hardware-specific properties are established rather than merely assumed.

**Evidence Level** — The strength of support for a hardware or implementation claim, such as documented, enumerated, measured, tested, or certified.

**Verified Path** — A code path whose required behavior has been established against the applicable interface, documentation, enumeration, or test evidence.

**Provisional Path** — A code path that is usable for development or architecture work but contains assumptions or unverified hardware-specific details.

**Generic Fallback** — A deliberately defined behavior used when a device-specific implementation is unavailable. It must be clearly marked and must not be represented as model-specific hardware knowledge.

**Model Coverage** — The set of vendor/model/hardware-revision combinations for which the driver has an identified and documented binding.

**Transport Implementation** — The actual mechanism used to exchange data or commands with the device, such as USB HID, USB control/bulk/interrupt transfer, serial, network, or another supported bus/interface.

**Command Path** — The complete route by which a requested operation is encoded, transmitted to the device, handled by the device, and its result returned to the caller.

**Data Path** — The complete route by which operational data is acquired, buffered, transported, validated, and delivered to its consumer.

**State Path** — The mechanisms that maintain and transition driver/device state, including initialization, ready, active, suspended, error, recovery, and shutdown conditions where applicable.

**Error Path** — The defined behavior for transport failures, invalid data, unsupported operations, timeouts, device removal, concurrency failures, and hardware faults.

**Recovery Path** — The logic used to restore a driver/device to a usable state after a recoverable failure.

**Concurrency Path** — The locking, condition-variable, queueing, ownership, and ordering mechanisms required when multiple threads or execution contexts access driver state.

**Hardware Control Path** — The complete path from a software control request through validation and synchronization to the physical device operation.

**Control Completeness** — The degree to which a declared control operation has a real command path, state path, error path, and hardware binding rather than only an API declaration.

**Data-Plane Completeness** — The degree to which operational data can actually travel through acquisition, buffering, synchronization, integrity checking, transport, and delivery.

**Control-Plane Completeness** — The degree to which configuration, commands, state changes, management operations, and responses have complete implementation paths.

**Model Metadata** — Structured information describing a vendor, model, hardware revision, firmware version, interface revision, capabilities, and hardware mappings.

**Model/Version Knowledge** — Information established about a particular device's vendor, model, hardware revision, firmware version, and interface revision. Unknown values must remain explicitly unknown or provisional.

**Hardware Mapping** — The relationship between logical driver functions and physical/device-specific resources such as pins, registers, reports, endpoints, controls, and memory.

**Pin Mapping** — The relationship between a logical pin function and an actual GPIO/pin identifier, electrical properties, direction, polarity, voltage, and current limits.

**Register Mapping** — The relationship between a logical device function and its documented hardware register or address.

**HID Mapping** — The relationship between a logical control or data field and the corresponding HID usage, report, report field, report ID, and direction.

**Transport Mapping** — The relationship between a logical driver operation and the physical or logical communication endpoint used to perform it.

**Default-Backed Implementation** — An implementation that can operate using explicitly labeled Standard/Expected Default values. It is provisional until the relevant hardware facts are validated.

**Assumption Boundary** — The point in an implementation where an assumed value is prevented from being silently treated as an actual hardware fact.

**Safety Default** — A conservative value or behavior selected to reduce the risk of unsafe hardware interaction when a property is unknown. A safety default is not evidence that the hardware actually has that property.

**Completeness Claim** — A statement that a driver or subsystem is implemented to a defined scope. Such a claim should identify its evidence and must not be inferred merely from the presence of source files.

**Scope of Completeness** — The explicitly defined set of models, interfaces, operations, hardware features, firmware versions, platforms, and test conditions covered by a completeness claim.

**Full Driver** — In SLeeLa/Skya terminology, a driver whose required shared infrastructure and defined device-specific binding are implemented for its stated scope, including meaningful command/data paths and appropriate error and concurrency handling. The term does not imply that every possible model or hardware revision is supported.

## Implementation Status Rule

The following distinctions are mandatory:

- **Infrastructure implemented** does not mean every **device binding** is complete.
- **Generic implementation** does not mean **model-specific implementation**.
- **Known unsupported** does not mean **unimplemented**.
- **Default-backed** does not mean **hardware-verified**.
- **Source presence** does not mean **implementation completeness**.
- **No obvious stub marker** does not prove that a driver is free of stubs or placeholders.
- A completeness claim must identify its **Scope of Completeness** and supporting **Evidence Level**.

A driver should not be described as hardware-complete until the applicable device-specific paths have been audited and the remaining assumptions, unsupported operations, implementation gaps, and hardware mappings are explicitly recorded.


## Slecompiler™ Terms

**Slecompiler™** — The SLeeLa native-artifact analysis product. It performs read-only
static inspection, decoding, control-flow analysis, library analysis, and SLIR-oriented
analysis without executing the analyzed artifact as part of the default pipeline.

**Artifact** — A native input presented to Slecompiler for analysis, such as an
executable, shared library, archive, relocatable object, kernel module, firmware image,
or other binary data.

**Observed Evidence** — A fact directly established by bytes, headers, symbols,
descriptors, or other authoritative artifact metadata available to the analysis.

**Derived Analysis** — A result computed from observed evidence by an analysis
algorithm, such as a recovered control-flow relationship or dependency edge.

**Unknown** — A property not established by the available artifact or implementation.
Unknown is not a permission to substitute a guess.

**SLIR** — Slecompiler's lifted intermediate representation between native decoding
and higher-level analysis.

**Analysis VM** — The Slecompiler representation/execution model used to operate on
lifted analysis material; it is not a mechanism for executing the original analyzed
artifact.

**Control-Flow Graph (CFG)** — A graph representing recovered basic-block and branch
relationships. CFG recovery can be incomplete when control flow is indirect,
unsupported, malformed, or otherwise not statically recoverable.

**Static Analysis** — Analysis performed from an artifact's available representation
without executing that artifact.

**API Exemplar** — A small buildable program demonstrating one defined analysis use
case. Slecompiler currently provides inspection, CFG/decode, library, graph, and
SLIR/VM exemplars.
