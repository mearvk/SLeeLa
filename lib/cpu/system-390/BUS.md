# ESA/390 Storage, Channel, and System Interfaces

## Storage access

Route effective addresses through profile-appropriate translation, protection, and real-storage access. Validate address ranges and permissions before accessing host-backed memory.

## Channel I/O

Model channel operations, device status, asynchronous completion, and I/O interruptions through explicit system interfaces. Channel configuration depends on the target system.

## System boundary

Timers, external interruption sources, storage controllers, channels, and devices belong to the system model. Separate I/O waiting and transfer timing from CPU instruction timing unless model-specific documentation defines the relationship.

## Safety

Never convert a guest address directly into an unchecked host pointer. Invalid storage or device requests must return modeled errors or interruption outcomes.
