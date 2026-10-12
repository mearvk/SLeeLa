# Listener API

This README is the navigation entry point for listener declarations. The existing API reference and implementation are authoritative for exact field names, accepted protocols, bind behavior, and validation.

## Contract boundaries

Document listener identity, bind address, port, protocol, route association, validation errors, and platform behavior only where represented in source. A declarative listener description does not by itself guarantee that a socket is opened or a service is running.

## Maintenance

- Keep this page synchronized with the API reference and implementation.
- Add positive and invalid-input tests for bind, port, and protocol validation.
- Record the actual build/test command and distinguish declaration parsing from network binding.
- Update [the API catalogue](../API-CATALOG.md) when the supported contract changes.
