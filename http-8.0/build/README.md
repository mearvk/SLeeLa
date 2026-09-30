<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# HTTP 8.0 Build

This directory is the product/version-local build surface for SLeeLa HTTP 8.0.

## Ownership

The Makefile in this directory is authoritative. `negotiation.mk` is a local build fragment used by the version-specific build.

## Entry points

- `make` — default build target.
- `make clean` — remove local build outputs when supported.

See BUILD.INVENTORY.md at the repository root for the complete build and Makefile inventory.