# Tutorial 02 — Startup Module and System Harness

SLeeLa VM startup selects the configured VM Startup Module, connects the System Harness, selects the compiled SLeeLa C/C++ path when required interfaces exist or a compatible .sleela VM module otherwise, performs bounded capability-scoped bootstrap calls, and reaches VM READY only when the core VM, memory, scheduler, module graph, execution dispatch, and harness are ready.

Inspect SleelaVMStartup.sleela, SleelaVMModule.sleela, SLVMModuleLoader.sleela, and SLVMNativeBinding.sleela.

Startup is a staged transition and does not grant unrestricted operating-system authority. Calls requiring the complete VM remain queued until readiness.
