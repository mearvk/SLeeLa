# SLeeLa CPU Object-Oriented Design

The CPU model uses composition and explicit couplers.

## Primary objects

- `SLCPUArchitecture` — architectural identity and capabilities.
- `SLCPUModel` — concrete processor instance/profile.
- `SLClock` — frequency, domains, edges and timing.
- `SLPipeline` — stages and instruction movement.
- `SLInstructionFetch` / `SLBranchPredictor` — front-end behavior.
- `SLRegisterFile` — architectural and implementation-visible registers.
- `SLExecutionUnit` — ALU/FPU/vector execution resources.
- `SLMemoryController` — memory transactions and timing.
- `SLL2Cache` / `SLL3Cache` — optional cache levels.
- `SLIOBus` / `SLBusArbiter` — transaction transport and ownership.
- `SLIOTiming` — wait states, latency and ordering.
- `SLDMAController` — programmed memory transfers.
- `SLMMU` / `SLTLB` — address translation.
- `SLInterruptController` — interrupt delivery.
- `SLCoupler` — narrow interface between subsystems.

This prevents the console CPU descriptors from becoming monolithic classes and permits historically accurate differences without duplicating the entire CPU model.