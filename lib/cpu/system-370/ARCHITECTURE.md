# IBM System/370 Architecture

## Architectural lineage

System/370 extends the System/360 design lineage. Exact instruction and control behavior depends on architecture level, model, and installed features.

## Instruction pipeline

The implementation should expose fetch, decode, operand access, execution, condition-code update, and interruption delivery as distinct stages for tracing. This is a functional decomposition, not a claim about a particular physical pipeline.

## Addressing and translation

Keep instruction and operand address generation separate from real-storage access. When the selected profile supports dynamic address translation, use an explicit translation-table model, protection checks, and architecturally appropriate translation exceptions. Do not enable translation merely because the machine is labeled System/370.

## PSW and interruptions

Implement the correct System/370 PSW layout for the selected architecture level, including the relevant execution, interruption-mask, addressing, and instruction-address fields. Maintain interruption metadata and state transitions explicitly.

## System boundary

Timers, storage controllers, channels, devices, and multiprocessor facilities belong to the machine/system configuration. Avoid attributing every system feature to the CPU core itself.
