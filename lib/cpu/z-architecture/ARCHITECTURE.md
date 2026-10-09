# IBM z/Architecture

## Execution model

z/Architecture is a 64-bit architecture with general-purpose registers, condition codes, a program status word, and privileged system state. The implementation supports a selected architecture level rather than one timeless instruction inventory.

## Instruction decode

Decode should use a versioned instruction table with operand format, length, privilege requirements, facility requirements, condition-code effects, exceptions, and storage access metadata. Unknown or disabled encodings must not be interpreted as another instruction.

## Program interruptions

Represent instruction exceptions, addressing and protection exceptions, operation exceptions, and other architected interruption classes according to the selected level. Preserve the architecturally defined old/new state and interruption metadata in the configured system model.

## Privilege and control state

Separate problem-state execution from supervisor/privileged operations. Access to control registers, PSW-changing instructions, translation controls, and other privileged operations is checked against the selected model.

## Storage and address translation

Memory access passes through configurable address translation and protection logic before reaching the storage model. Storage keys and other protection features are enabled only when represented by the selected system profile.

## Multiprocessor and I/O boundary

Inter-CPU ordering, synchronization, channel-subsystem behavior and device I/O require system-level components. Do not approximate these with local CPU state unless the platform contract explicitly defines the behavior.
