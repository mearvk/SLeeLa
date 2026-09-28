# HTTP Server Logging

## General rule

HTTP server logs are operational records, not copies of request bodies.
The server implementations should avoid logging request bodies, credentials,
private keys, or complete query strings as routine operational data.

## Current logging boundary

| Grade | Default logging destination | Notes |
|---|---|---|
| 1 | configured log destination / standard error according to server options | HTTP/1 operational logging |
| 2 | standard error | Native HTTP/2 stream-aware diagnostics |
| 3 | standard error | SLeeLa adapter and QUIC backend diagnostics |

Grade 3 currently inherits the `wsslserver` backend's standard error stream.
The adapter does not create a second hidden HTTP/3 log file.

A deployment may redirect or supervise standard error using its service
manager. For example, systemd, a container runtime, or another process
supervisor can collect the stream without changing the protocol implementation.

## What should be logged

Recommended operational fields include:

- startup and shutdown;
- listener address and port;
- protocol grade;
- accepted/rejected connections;
- HTTP/2 stream or HTTP/3 request lifecycle identifiers where available;
- status code;
- byte counts;
- bounded error information;
- configuration validation failures;
- backend startup/exit failures.

Do not routinely log:

- request bodies;
- authorization credentials;
- TLS private-key contents;
- cookies or bearer tokens;
- complete sensitive query strings;
- arbitrary packet payloads.

## Log rotation and retention

Log rotation belongs to the deployment environment unless a specific server
option explicitly implements it. Supervisors should bound disk consumption
and retain enough records for operational diagnosis without turning ordinary
HTTP logs into an unrestricted traffic archive.

## HTTP/3 backend

The Grade 3 adapter reports its selected backend and startup parameters on
standard error, then replaces its process image with the backend. Backend
diagnostics therefore remain part of the same service log stream.
