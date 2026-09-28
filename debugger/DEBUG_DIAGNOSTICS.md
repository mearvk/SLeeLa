# SLeeLa Debug Diagnostics

This layer adds execution recording/replay metadata, crash-dump records, sanitizer findings, memory diagnostics, profiling, coverage, lock/deadlock evidence, persistent session artifacts, and explicit security policy.

Native crash-dump parsing, OS heap instrumentation, DWARF/PDB decoding, LLDB/DAP transport and reverse execution remain backend integrations.

## Debug Artifact Format

A persisted session should use a .sleela-debug directory containing session metadata, events, breakpoints, threads, stack, registers, modules, symbols, memory map, evidence, coverage, crash information and replay metadata.

## Security

Attach, memory writes and expression execution are denied unless explicitly authorized by policy. Read, inspect and report are safe baseline operations.

## Connections

The layer feeds DebugSession, ActionController, HTTP debugging, future DAP and terminal interfaces, and Test Suite readiness reports.