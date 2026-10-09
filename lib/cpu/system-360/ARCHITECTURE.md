# IBM System/360 Architecture

## Execution state

System/360 uses 32-bit general-purpose registers, a condition code, and a program status word that carries execution and interruption-control state. The exact model and supported features must be configured explicitly.

## Instruction decoding

Use an instruction table that records opcode encoding, instruction length, operand format, privilege requirements, condition-code effects, storage behavior, exceptions, and model/facility availability. Do not decode later-family extensions in the base profile.

## Interruptions

Model program interruptions, external interruptions, supervisor-call behavior, and I/O interruptions according to the selected architecture contract. Preserve the architecturally defined old/new PSW and interruption metadata in the machine state.

## Storage access

All storage references pass through the configured storage subsystem and protection checks. Do not treat the guest address space as an unbounded host array.

## I/O channels

Channel commands, channel status, device state, and asynchronous completion belong to the system-level channel model. CPU I/O instructions interact with that model through explicit requests and interruption outcomes.

## Family boundaries

Differences among System/360 implementations are captured as model capabilities. System/370 and later architectures require separate profiles rather than automatic inheritance of newer behavior.
