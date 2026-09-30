<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">


# http-7.0/build Build

This directory is a product/version-local build surface for SLeeLa.

## Ownership

The Makefile in this directory is authoritative for the build targets here. Do not move generated artifacts into the repository root.

## Entry points

- `make` — default build target defined by the local Makefile.
- `make clean` — remove local build outputs when supported by the Makefile.

For HTTP version builds, `negotiation.mk` is a local build fragment used by the version-specific Makefile.

See BUILD.INVENTORY.md at the repository root for the complete build and Makefile inventory.