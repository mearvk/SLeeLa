# SLeeLa HTTP 8.0 — Status

HTTP 8.0 is an experimental SLeeLa generation, not an IETF HTTP/8 standard.

## Present tree

- protocol C and C++ sources;
- cryptographic C++ interface;
- early security policy configuration;
- cryptographic support documentation;
- negotiation C/C++ sources;
- negotiation build check.

## Required hardening

Before deployment, validate provider availability, algorithm policy, key handling, authentication, replay behavior, secret storage, and explicit fallback policy.

## Build

A dedicated `build/Makefile` checks the complete C/C++ source set consistently with the neighboring HTTP generations.
