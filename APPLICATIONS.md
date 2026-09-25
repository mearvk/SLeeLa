# SLeeLa Application Model

## Native SLeeLa application

A SLeeLa-native application is built from SLeeLa source through the SLeeLa compiler/runtime/application toolchain. It may use explicitly declared native libraries or platform facilities without ceasing to be a SLeeLa application.

## Lifecycle

IDEA → PROJECT → SOURCE → CHECK → BUILD → TEST → PACKAGE → INSTALL → RUN → VERIFY

## Application profiles

- CLI;
- desktop GUI;
- terminal/Phraign™;
- server/service;
- HTTP client/server;
- scientific;
- data processing;
- database;
- email;
- telephony/VoIP;
- driver-integrated;
- distributed;
- compiler/tooling;
- protocol implementation.

## Application manifest

An application manifest should identify language version, dependencies, capabilities, entry point, resources, target platform, artifact type, integrity policy and release metadata.

## Entirely in SLeeLa

The intended application-builder contract is that an application can be authored, checked, built, tested, packaged and launched through the SLeeLa toolchain without requiring the user to write a second application language. Native components remain explicit dependencies when required by the target platform.

## Completion criteria

A complete application is not just source code. It includes source, manifest, dependencies, tests, build recipe, generated artifact, integrity metadata and installation/run instructions.

**Max Rupplin — MEARVK LLC — 2026**
