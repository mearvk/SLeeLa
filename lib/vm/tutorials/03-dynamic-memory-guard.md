# Tutorial 03 — Dynamic Memory Guard

The Dynamic Memory Guard gives a VM developer three explicit memory-growth policies:

- HARD — never grow beyond the configured hard limit.
- SLOW_CAREFUL — use bounded growth and allow growth to be deferred under pressure.
- AGGRESSIVE — allow prompt bounded growth up to the configured maximum and physical limit.

Attach SleelaVMDynamicMemoryGuard to the VM source and memory options. Configure initial, hard, maximum, growth-step, delay, pressure, and growth-permission values explicitly. Physical/resource limits remain authoritative.

Request flow: allocation request -> Dynamic Memory Guard -> within limit, deferred growth, bounded growth, or limit reached -> Memory Manager -> allocation result.

Inspect SleelaVMDynamicMemoryGuard.sleela, include/sleela_vm_dynamic_memory_guard.h, and SleelaVMOutput.sleela.
