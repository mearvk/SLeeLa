# Driver Completeness

## Completion boundary

SLeeLa distinguishes shared driver infrastructure from device-specific completeness.

### Shared infrastructure

The common driver layer provides:

- hardware profile metadata;
- pin, voltage and memory abstractions;
- cache/DMA policy;
- bounded data-plane queues;
- frame sequencing and timestamps;
- CRC-32 integrity checking;
- producer/consumer synchronization;
- mutex and condition-variable locking;
- blocking/timed receive;
- backpressure;
- start/stop/destroy lifecycle;
- platform transport boundaries.

### Device-specific completion

A concrete driver must additionally account for the applicable:

1. exact model and hardware revision;
2. firmware/interface version where discoverable;
3. transport;
4. USB/Bluetooth/network/audio/HID descriptors;
5. pin and control mappings when hardware-level control is exposed;
6. register or memory mappings when applicable;
7. HID report layouts and usages when applicable;
8. command and response paths;
9. input/output data paths;
10. state transitions;
11. error and recovery paths;
12. hotplug/reconnect behavior;
13. cache/DMA requirements;
14. unsupported-feature reporting;
15. platform-specific behavior;
16. test evidence.

## Audit status

The absence of TODO/STUB/ENOSYS markers is not, by itself, proof of completeness.

Use these status concepts:

- **Generic implementation** — reusable behavior that does not claim every model-specific detail.
- **Provisional path** — an implementation operating behind an explicit evidence boundary.
- **Verified path** — a path supported by documented, enumerated, measured or tested evidence.
- **Hardware-operational** — required device-specific operations are implemented for the documented scope.
- **Hardware-complete** — the applicable control, data, state, error, recovery, synchronization and hardware-mapping paths have been audited and remaining gaps are explicitly recorded.

Never convert an assumption into an actual hardware value merely to make a driver appear complete.
