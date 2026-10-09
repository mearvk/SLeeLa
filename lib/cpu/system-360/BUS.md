# System/360 Storage and I/O Interfaces

## Storage interface

CPU loads and stores pass through a system storage interface that validates address ranges, access size, alignment requirements, and protection settings before accessing configured main storage.

## Channel subsystem

System/360 I/O is modeled through channel and device abstractions where the selected system configuration supports them. Represent command execution, channel status, device status, asynchronous completion, and I/O interruptions explicitly.

## System integration

Storage controllers, channels, devices, timers, and external interruption sources belong to the machine model. Their throughput and latency are configured separately from CPU instruction semantics.

## Faults

Invalid or protected storage accesses and invalid I/O operations must produce documented machine-level errors or modeled interruptions. Never allow guest addresses to access arbitrary host memory.
