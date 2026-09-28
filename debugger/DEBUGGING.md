# SLeeLa Debugging Workflow

## 1. Reproduce

Start with the smallest failing test or executable from the existing test suite.

## 2. Record

Record:

- exact executable
- exact arguments
- build revision
- operating system
- compiler
- debugger backend
- relevant test identifier

## 3. Stop

Use a breakpoint, watchpoint, assertion event, sanitizer event, or exception event to stop at the first useful failure boundary.

## 4. Inspect

Capture the current frame, backtrace, thread identity, source location, registers and memory when the selected backend supports them.

## 5. Correlate

Attach test, function-coverage, sanitizer, regression, HTTP/server, and compiler fields to the same DebugEvent stream.

## 6. Report

Produce a deterministic report suitable for CI artifacts and human review.

## 7. Fix and reproduce

A successful debugging session ends with a regression test or fixture that reproduces the original failure and verifies the correction.

The debugger is an investigative instrument; the test suite remains the authority for pass/fail behavior.
