# IBM ESA/390 Architecture

## Architectural scope

ESA/390 belongs to the System/390 generation and must remain distinct from System/370 and z/Architecture. Addressing modes and facilities are selected by architecture profile.

## Execution path

Expose instruction fetch, decode, operand access, execution, condition-code update, storage completion, and interruption delivery as traceable functional stages. This does not imply a particular physical pipeline.

## Address translation

Separate effective-address generation from real-storage access. Where DAT is enabled, apply the selected translation format and storage-protection checks. Translation failures must produce architecturally appropriate outcomes rather than host memory faults.

## Program state

Implement the ESA/390 PSW and relevant control/access state for the configured profile. Preserve interruption metadata and perform defined state transitions. Do not reuse the z/Architecture PSW format by default.

## System integration

Storage controllers, channels, devices, timers, and multiprocessor coordination are configured system components. Their behavior should not be conflated with core instruction semantics.
