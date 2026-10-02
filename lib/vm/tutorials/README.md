# SLeeLa VM Tutorials

Practical learning path for /lib/vm. SLeeLa source definitions are authoritative; native C/C++ is implementation support.

## Learning path

1. 01-build-a-basic-vm.md — construct a minimal complete VM source request.
2. 02-startup-module-and-system-harness.md — connect a Startup Module through the System Harness.
3. 03-dynamic-memory-guard.md — configure HARD, SLOW_CAREFUL, or AGGRESSIVE memory growth.
4. examples/basic-vm.sleela — compact VM construction example.
5. examples/startup-and-memory-guard.sleela — startup and guarded-growth example.

## Authority and boundaries

SLeeLa source -> VM Compiler/Compiler Manager -> resource/capability validation -> native support -> SLVM/SLJVM runtime.

Tutorial examples do not grant unrestricted operating-system access. Physical limits, capability policy, resolver policy, memory/security management, and VM readiness remain authoritative.

## Build

make vm

or

make -C lib/vm

These are source/design examples; their presence does not imply that every example is a standalone executable.

## Related tutorials

- /lib/compiler/tutorials teaches source-to-VM compilation.
- /lib/decompiler/tutorials teaches artifact-to-SLeeLa reconstruction and evidence handling.
