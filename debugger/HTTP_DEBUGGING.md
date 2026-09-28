# SLeeLa Debugger — HTTP and Server Debugging

Version: 0.6.0

The debugger should provide a domain layer for SLeeLa's HTTP/server stack while keeping native debugging platform-neutral.

## Correlation chain

connection -> socket -> packet -> HTTP parse -> request -> route -> handler -> response

Each stage should expose a stable diagnostic correlation identifier.

## Debug events

- connection opened/closed
- packet received/sent
- parse started/completed/failed
- malformed packet
- request created
- route selected
- handler entered/exited
- response created/sent
- timeout
- protocol violation
- server exception

## Timeline

HTTP events should be joinable with debugger events, logs, tests, sanitizer failures, and crashes.

## Safety

HTTP payloads are untrusted diagnostic data. They must remain data in the debugger model and must never be interpreted as debugger commands or shell input.

## Useful stop conditions

Examples include a selected route, parser failure, handler exception, protocol violation, timeout, or explicitly configured request/connection condition.

## Implementation boundary

This document describes the domain-debugging contract. It does not claim that native HTTP-aware breakpoints or protocol inspection are implemented until corresponding source code and tests exist.
