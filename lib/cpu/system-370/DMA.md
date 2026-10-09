# System/370 Channel I/O and Data Movement

## Scope

Represent data movement through the configured System/370 channel and device model. Do not assume every system has an identical channel configuration or transfer capability.

## Channel model

Describe supported channel types, device attachment, command execution, status, interruption delivery, and concurrency using documentation for the selected system.

## Storage interaction

Channel operations must access configured storage through validated system interfaces and applicable protection rules. If translation is relevant to the selected channel mode, implement it according to the machine's documented behavior rather than assuming CPU DAT rules automatically apply.

## Error handling

Invalid command chains, unavailable devices, and invalid storage references produce modeled channel/device status or system diagnostics. Never invent device capabilities or transfer rates.
