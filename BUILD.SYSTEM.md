# SLeeLa Build System

## Goal

A SLeeLa project should be able to move from source to a reproducible application artifact without requiring the user to manually assemble compiler, runtime, native libraries and resources.

## Build lifecycle

SOURCE → CHECK → DEPENDENCIES → COMPILE → LINK/ASSEMBLE → PACKAGE → SIGN → TEST → INSTALL/RUN

## Project inputs

A project manifest should declare:

- project name and version;
- language version;
- target platforms/architectures;
- source roots;
- resource roots;
- dependencies;
- native libraries;
- permissions/capabilities;
- output type;
- signing policy;
- test suites.

## Outputs

Supported artifact classes are intended to include:

- executable;
- library;
- service;
- plugin;
- package;
- source bundle;
- debug bundle;
- signed release bundle.

## Reproducibility

Builds should record compiler version, language version, dependency versions, target triple, build inputs and integrity digests.

## Cross compilation

The build system must distinguish host, target OS and target architecture. A host build must never silently emit an artifact for a different target.

## Security gates

Dependencies and generated artifacts are verified before execution or packaging when integrity manifests are enabled. Privilege escalation is never implicit.

## CLI direction

The application builder should provide commands equivalent to:

`sleela init`, `check`, `build`, `test`, `run`, `package`, `install`, `clean`, `doctor`, `version`.

## Current status

The existing `impl/Makefile`, scripts and CI are the current build foundation. This document defines the larger application-build contract that future tooling should implement.

**Max Rupplin — MEARVK LLC — 2026**
