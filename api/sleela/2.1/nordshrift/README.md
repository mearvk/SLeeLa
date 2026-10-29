# Nordshrift API

The [Nordshrift API reference](API.html) is the detailed entry point for the `.sst` semantic model and SLeeLa architecture input-object vocabulary. This README is a navigation and maintenance page; it does not duplicate the API reference.

## Contract boundaries

- Keep the semantic input-object schema separate from side-effecting execution.
- Document required fields, accepted values, validation failures, versioning, and serialization behavior against the actual implementation.
- Mark proposed or design-only concepts as such until backed by executable code and tests.

## Build and tests

Use the build/test commands documented by the implementation that consumes Nordshrift. Do not invent a standalone Makefile or claim independent runtime behavior if the module is a reference/schema surface.

## Maintenance

When the contract changes, update [API.html](API.html), the [full API catalogue](../API-CATALOG.md), relevant fixtures, and the source coverage inventory.
