# Router API

This folder is the API documentation entry point for deterministic route matching. Consult the existing router API reference and implementation for the exact route representation and supported methods.

## Scope

The public contract should state path normalization, method matching, precedence, duplicate-route behavior, parameter handling (if implemented), and invalid-route errors. Do not claim dynamic dispatch, middleware, or parameter extraction unless implemented and tested.

## Maintenance

- Keep route declaration and route execution clearly distinguished.
- Add exact-match, method mismatch, duplicate, and malformed-path tests as applicable.
- Document the module's actual build/test command rather than assuming a standalone build exists.
- Update [the API catalogue](../API-CATALOG.md) when the supported contract changes.
