# SLeeLa Debug Engine Integration

DebugSession remains the authoritative session and event boundary. DebugEngine is the reusable debugger-domain state layer.

Native backend flow:
1. Perform the operating-system operation.
2. Confirm success.
3. Update DebugEngine state.
4. Emit DebugEvent through DebugSession.
5. Associate stop records and evidence where applicable.
6. Let the Test Suite mark a capability tested only after an executable test passes.

This keeps portable models separate from native capability claims and provides one stable connection for the Linux ptrace, macOS LLDB and Windows Debug API adapters.

The engine also supplies the common state needed by the existing action model, HTTP debugging, crash evidence, sanitizer integration and future terminal/DAP interfaces.