# ESA/390 Channel I/O and Data Movement

## Scope

Data movement is modeled through the configured System/390 channel and device subsystem. Available channels and device capabilities depend on the actual system configuration.

## Channel contract

Represent command execution, status, device state, completion, and interruption delivery as explicit operations. Do not assume all systems expose identical channel models.

## Storage safety

Validate channel command structures and storage references against configured storage and protection rules. Apply translation to channel operations only according to the target system's documented behavior.

## Error handling

Malformed commands, invalid storage references, and unavailable devices must produce modeled status or diagnostics. Do not invent transfer rates or device features.
