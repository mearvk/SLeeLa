# Skya Driver Audit and Completion Record

This document defines the final audit boundary for `telephony-skya/drivers`.

## Implemented shared infrastructure

The driver family now has concrete infrastructure for:

- hardware identity and evidence metadata;
- pin role, direction, polarity, pull and voltage constraints;
- memory/register region metadata and access checks;
- cache policy and DMA-coherency requirements;
- bounded data-plane buffering;
- producer/consumer message passing;
- mutex and condition-variable locking;
- blocking and timed receive;
- backpressure;
- sequence numbers and monotonic timestamps;
- CRC-32 frame validation;
- start/stop/destroy lifecycle;
- platform-specific transport boundaries.

The same reusable hardware-profile and data-plane sources are also exposed under the repository-level `/drivers` directory.

## Device-specific audit boundary

The infrastructure above is not a declaration that every telephone/headset model has verified hardware mappings.

For each model, an audit must record:

- vendor, model, hardware revision and firmware/interface version when discoverable;
- transport and operating-system path;
- exact identifiers when documented and verified;
- descriptor and HID-report evidence;
- command, input, output and feature mappings;
- pin/voltage/register mappings when applicable;
- buffer and synchronization behavior;
- error and recovery behavior;
- hotplug/reconnect behavior;
- unsupported capabilities;
- platform test evidence;
- remaining assumptions.

## Status vocabulary

Use the terminology in the repository `TERMINOLOGY.md`.

In particular:

- **Generic Implementation** means reusable code without a model-specific completeness claim.
- **Verified Path** means the path has supporting evidence.
- **Hardware-Operational** means required device-specific operations are implemented for the stated scope.
- **Hardware-Complete** means the applicable paths have been audited and remaining gaps are explicitly recorded.
- **Full Driver** means a concrete model implementation whose completeness claim identifies its scope and evidence.

A source file is not considered a stub merely because it contains generic infrastructure. Conversely, the absence of a literal stub marker does not prove hardware completeness.
