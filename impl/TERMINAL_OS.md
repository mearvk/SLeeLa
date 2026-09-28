# SLeeLa Terminal / PTY

## Purpose

The terminal layer provides a predictable interactive runtime boundary for SLeeLa on Linux and Windows. It keeps terminal-specific behavior out of the VM core while exposing the same operations to the runtime.

## Backends

| Platform | Backend | Interactive primitive |
| --- | --- | --- |
| Linux | `linux-pty` | POSIX pseudo-terminal (`posix_openpt`, `fork`, session/controlling-terminal setup) |
| Windows | `windows-conpty` | Windows Pseudo Console (ConPTY) |

The public interface is `impl/core/sleela_terminal.h` and the implementation is `impl/core/sleela_terminal.c`.

## Runtime operations

- Detect the active terminal backend.
- Spawn a shell or command inside a real PTY/ConPTY.
- Read terminal output.
- Write interactive input.
- Resize the terminal window.
- Close the terminal and reap the child process.
- Report platform-specific errors without exposing host descriptors as VM handles.

`SLTerminalHandle` is an opaque integer-sized runtime handle. Callers should use the abstraction rather than assuming Linux file descriptors or Windows `HANDLE` values.

## Windows interactive runtime

Windows uses ConPTY so applications receive a terminal-like session rather than a pair of ordinary redirected pipes. The backend creates the pseudo console, associates it with a child process through `PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE`, and maintains parent-side input/output handles.

ConPTY is intended for modern Windows releases that provide the Windows Pseudo Console API. A Windows build should therefore treat unavailable ConPTY support as a platform capability failure rather than silently falling back to a non-interactive pipe mode.

## Linux interactive runtime

Linux uses a POSIX master PTY, establishes a child session and controlling terminal, attaches standard input/output/error to the slave, and executes `/bin/sh`. A command can be supplied for non-interactive smoke tests or automation.

## Security boundary

Terminal creation is an execution capability. The VM/runtime should expose it only through an explicit policy-controlled operation. Commands supplied by untrusted input must not be interpolated into a shell command without an appropriate policy and escaping layer.

## Verification

The `terminal_platform_smoke` target starts a command, verifies terminal output, exercises a terminal resize, and closes the session. The Makefile keeps the repository SHA-256 verification gate ahead of this target and its object build, consistent with the existing verified-build policy.
