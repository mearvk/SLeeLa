# Tutorial 01 — Build a Basic Complete VM

## Goal

Create a small but complete SLeeLa VM request that can be resolved into execution, memory, CPU, concurrency, I/O, security, runtime, and build plans.

## Source request

Use SleelaVMSource as the source-level entry point. The request declares architecture, options, memory, runtime, and build information.

The source request is authoritative. Do not manually invent native C/C++ fields that are absent from the SLeeLa model.

## Completeness flow

VM source -> options -> architecture -> physical limits -> resource plan -> capability checks -> build plan -> VM output.

For a first VM, use the Basic/Complete path. Basic means complete and usable, not incomplete.

## Physical limits

Set physical/resource limits before considering dynamic growth. Dynamic policies cannot exceed the resolved physical ceiling.

## Validation

SleelaVMOutput should identify the resolved architecture, resource plan, build plan, and validation state. Missing requirements should be rejected rather than silently inserted.

## Inspect

- SleelaVMSource.sleela
- SleelaVMCompiler.sleela
- SleelaVMCompilerManager.sleela
- SleelaVMPhysicalLimits.sleela
- SleelaVMOutput.sleela

Next: Tutorial 02.
