# System/370 Storage and I/O Interfaces

## Storage path

Route CPU storage references through a system interface that separates virtual/logical address generation, optional DAT translation, real-storage access, and protection checks.

## Channel I/O

Represent channel operations, channel/device status, completion, and I/O interruptions through explicit channel and device abstractions. Available channel types and behavior depend on the configured system.

## Storage safety

Validate address ranges, operand sizes, access permissions, and translation results before touching host memory. Guest storage must never become an unrestricted host pointer.

## Integration

Storage controllers, timers, external interruption sources, and device transfer behavior are system-level components. Keep their timing separate from intrinsic instruction timing unless a model's documentation defines the interaction.
