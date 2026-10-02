# Tutorial 02 — Startup Module and System Harness

## Goal

Understand how a compiled SLeeLa VM definition becomes a Startup Module and reaches VM READY without treating native C/C++ as the source authority.

## Startup sequence

SLeeLa System Startup Module -> System Harness -> compiled SLeeLa C ABI plus C++ runtime OR compatible .sleela VM module -> bounded bootstrap calls -> VM READY -> normal VM execution.

## Native path and fallback

The startup logic checks for both the compiled SLeeLa C ABI and required C++ runtime/orchestration. If both exist, that implementation path is used. Otherwise a compatible .sleela VM module may be loaded when supported.

This does not change source authority: SLeeLa VM definitions remain authoritative.

## Bootstrap

Initial services are capability-scoped: memory, process/thread, I/O, time, scheduler, resolver, cryptography, and system status. Calls that require the complete VM are held until readiness.

## VM READY

READY requires the core VM, memory, scheduler, module graph, execution dispatch, and System Harness to be ready.

## Inspect

- SleelaVMStartup.sleela
- SleelaVMModule.sleela
- SLVMModuleLoader.sleela
- SLVMNativeBinding.sleela

Startup is a staged transition, not unrestricted OS access.
