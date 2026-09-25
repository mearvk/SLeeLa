# SLeeLa Package Manager

## Purpose

The package manager is the dependency and artifact layer for SLeeLa applications.

## Package identity

Every package should have:

- name;
- semantic version;
- target compatibility;
- dependency list;
- license metadata;
- source/artifact identity;
- integrity digest;
- optional signature;
- manifest;
- reproducibility metadata.

## Resolution

Resolution should be deterministic. A lock file records exact selected versions and integrity hashes.

## Dependency classes

- SLeeLa source packages;
- compiled SLeeLa libraries;
- native libraries;
- platform packages;
- development/test packages;
- application resources.

## Trust

A package digest establishes content integrity. A signature, when present, establishes authenticity according to the configured trust policy. Neither should be conflated with the other.

## Offline mode

The package manager should support a complete local cache and fail clearly when an undeclared external dependency is required.

## Platform selection

Packages are resolved by target OS, architecture, ABI and language/runtime compatibility.

## Lifecycle

DISCOVER → RESOLVE → LOCK → FETCH → VERIFY → INSTALL → BUILD → TEST

## Future CLI

`sleela pkg search|add|remove|update|lock|fetch|verify|install|publish`.

**Max Rupplin — MEARVK LLC — 2026**
