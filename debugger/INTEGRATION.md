# Debugger Integration

The debugger is the correlation layer beside SLeeLa verification.

- Test Suite events identify test IDs and failures.
- Function coverage identifies source functions and verification status.
- Sanitizer output can be recorded as sanitizer events.
- Regression fixtures can be recorded as regression events.
- Server Edition and HTTP diagnostics can be converted to log events.
- Annotation runtime failures can carry annotation, route, and Nexter Colony fields.
- Compiler/frontend failures can carry source location and phase.

The current debugger is an in-process diagnostic core. It is not yet a full OS process controller. Platform backends should be added behind the existing model rather than duplicating the test framework.
