# Tutorial 03 — Dynamic Memory Guard

## Goal

Give a developer an explicit policy for VM memory growth.

| Mode | Behavior |
|---|---|
| HARD | Never grow beyond the configured hard limit. |
| SLOW_CAREFUL | Grow in bounded steps and permit growth to be deferred under pressure. |
| AGGRESSIVE | Permit prompt bounded growth up to the configured maximum and physical limit. |

## Configure

Attach SleelaVMDynamicMemoryGuard to the VM source and memory options. Reason about initial limit, hard limit, maximum limit, growth step, growth delay, pressure threshold, and explicit growth permission.

## Request flow

Allocation request -> Dynamic Memory Guard -> within current limit / deferred growth / bounded growth / limit reached -> Memory Manager -> allocation result.

Invalid configurations and overflowing requests produce INVALID_REQUEST. The guard cannot exceed the resolved physical/resource limit.

## Native support

C ABI: include/sleela_vm_dynamic_memory_guard.h

C++ facade: include/sleela_vm_dynamic_memory_guard.hpp

SLeeLa policy: SleelaVMDynamicMemoryGuard.sleela

SleelaVMOutput records the selected mode and important limits.
