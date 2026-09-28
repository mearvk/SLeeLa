# Native Execution Plan

Version: 0.8.0

## Linux
Implement software/hardware breakpoints, ptrace registers/memory/threads, signal and fork/exec events, module discovery, stack unwinding and DWARF source mapping.

## macOS
Complete LLDB launch/attach, execution control, threads, frames, registers, memory, breakpoints, exceptions, modules, symbols and source mapping.

## Windows
Complete Windows Debug API process/thread events, CONTEXT registers, memory, breakpoints, exceptions, DLL events, symbols and PDB/source mapping.

Every native capability must feed the same DebugEngine, DebugSession and evidence model and must pass the Conformance Suite before being advertised as tested.
