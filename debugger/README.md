<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Debugger

The SLeeLa Debugger is a development-time diagnostic layer for the SLeeLa software lifecycle.

Build -> Launch -> Break -> Inspect -> Trace -> Diagnose -> Test -> Reproduce -> Report

It provides a portable core for breakpoints, watchpoints, source locations, stack frames, runtime events, assertions, exceptions, test correlation, coverage correlation, sanitizer correlation, regression correlation, and deterministic reports.

It is backend-neutral. Future process adapters may use ptrace, Mach task APIs, Windows debugging APIs, LLDB, or another supported facility.

Build:
    make -C debugger

Run:
    ./debugger/sleela-debugger --self-test
    ./debugger/sleela-debugger --report debugger/example-report.txt

The debugger records and analyzes events; it does not execute arbitrary payloads or bypass authorization.