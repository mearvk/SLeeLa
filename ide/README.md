<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">


# SLeeLa IDE Integration

Version: 0.2.0-dev

The /ide tree is the SLeeLa integration layer for an IntelliJ Platform-based IDE. It keeps the SLeeLa language implementation authoritative in the existing compiler while defining a reusable IDE architecture for SLeeLa, C, C++, and Java.

## Goals
- provide an IntelliJ Platform plugin host for SLeeLa;
- keep language analysis tied to the real SLeeLa compiler;
- expose C, C++, and Java as interoperability and project-language examples;
- share project, build, run, diagnostics, navigation, and debugger contracts.

## Layout
```
/ide
  /sleela-intellij   IntelliJ Platform plugin project
  /language          Language and PSI contract
  /analysis          Semantic/indexing contract
  /project           Project/module model
  /build             Check/build/run/test integration
  /debugger          Debug adapter integration
  /c                 C integration example
  /cpp               C++ integration example
  /java              Java integration example
  /sleela             SLeeLa integration example
  /tests              IDE conformance fixtures
```

## Language roles
| Language | Role |
|---|---|
| SLeeLa | First-class native language integration |
| C | Native ABI and C-header interoperability example |
| C++ | Native implementation/debugging example |
| Java | JVM/tooling and IntelliJ language integration example |

The examples do not replace the authoritative implementations of those languages. They define how an SLeeLa project can recognize, navigate, build, run, and debug mixed-language source.

## IntelliJ Platform
The plugin project uses IntelliJ Platform Gradle Plugin 2.x. The SDK documentation identifies 2.x as the active line; current target combinations must use the Java runtime required by that target platform.

## First usable milestone
1. Open a SLeeLa project.
2. Recognize SLeeLa, C, C++, and Java files.
3. Load C/C++ toolchain metadata and compile_commands.json when present.
4. Parse SLeeLa through the repository compiler bridge.
5. Provide syntax highlighting and PSI-backed navigation.
6. Surface compiler diagnostics.
7. Invoke check, build, run, and test.
8. Connect debugger actions to the existing /debugger subsystem.

**Max Rupplin — MEARVK LLC — 2026**