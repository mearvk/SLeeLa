# Synchro Status

## 1. Implemented

- Common 16-byte SYNC packet contract.
- C packet/statistics implementation.
- C language integration ABI.
- C++17 RAII integration wrapper.
- Java integration wrapper and SLA evaluation.
- Existing native VM OP_SYN_* operations.
- Native Makefile targets and deterministic C smoke coverage.

## 2. Tested

- C packet/integration statistics and timeout behavior.
- C++17 integration runtime behavior.
- Java integration construction and SLA-result path.

## 3. VM Integration Boundary

The VM already exposes Synchro through OP_SYN_OPEN, OP_SYN_DISPATCH,
OP_SYN_STAT, OP_SYN_REPORT, and OP_SYN_CLOSE, backed by
impl/core/sleela_synchro.*. The reusable synchro/c ABI remains below that VM
surface. The next adapter work is VM-to-integration translation so the VM
preserves the same packet/statistics semantics without duplicating the wire
contract.

## 4. Remaining Verification

- Malformed magic, short packet, wrong sequence, duplicate ACK, timestamp
  rollback, and statistics-window tests.
- Linux, macOS, and Windows builds.
- Repository-native Java build/test target.
- End-to-end VM -> Synchro -> UDP -> statistics -> SLA test.
- CI jobs for supported platforms.

## Completion rule

Synchro is complete when sections 1-3 are implemented, protocol/platform tests
pass, and the VM end-to-end path is verified. SLA measurements remain
observations, not delivery guarantees.
