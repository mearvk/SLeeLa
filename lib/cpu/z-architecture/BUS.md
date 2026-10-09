# z/Architecture Storage and I/O Interfaces

## Storage access

The CPU model issues typed storage operations through a system interface that performs address translation, protection checks, and storage access. The system model supplies installed storage, address spaces, protection controls, and platform-specific attributes.

## I/O boundary

Mainframe I/O may involve a channel subsystem and architected I/O instructions rather than simple memory-mapped device access. Model the channel subsystem and device state separately, with explicit command, completion, status, interruption and error paths as supported by the selected system profile.

## Ordering and synchronization

Implement architected serialization, memory ordering and synchronization instructions according to the selected architecture level. Multiprocessor behavior must be coordinated with a shared system model; local CPU simulation alone cannot guarantee system-wide ordering.

## Faults

Addressing, protection, translation, and device/I/O errors must be represented with architected interruption or system error outcomes where defined. Never let invalid guest addresses access host memory directly.
