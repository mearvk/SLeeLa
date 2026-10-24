# Lesson 7 — Native Bridge and Platform Limits

The SLeeLa facade is not itself an operating-system display driver. The native core lives under the repository's `ballum/` tree, while `impl/core/ballum_vm_bridge.*` provides the VM-facing bridge.

## Trace a request

1. A SLeeLa program checks display state and capability.
2. It calls a `uiBallum*` function.
3. The VM bridge validates and forwards to the native manager.
4. The platform backend performs the OS operation if supported and permitted.
5. A precise status is returned to the caller.

The package README and source comments are the authority for current platform coverage. Do not infer support merely because a profile class exists: profile metadata, compiled backend support, permissions, and runtime success are separate things.

## Review checklist

- Is the backend actually compiled and selected on this OS?
- Does the display advertise the requested capability?
- Are bounds and value ranges checked before dispatch?
- Are native errors preserved as status codes?
- Are platform-specific permissions documented?
- Does a headless run fail cleanly without pretending an operation succeeded?

For current architecture and API details, return to [the package README](../README.md).
