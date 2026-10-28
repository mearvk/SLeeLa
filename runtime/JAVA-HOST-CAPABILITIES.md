# SLeeLa Java Host Capabilities

Author: Max Rupplin - MEARVK LLC - 2026

## Execution modes

SLeeLa has a native execution engine and an optional Java interoperability path. These are separate capabilities.

| Capability | Meaning | Default |
|---|---|---|
| Native SLeeLa VM | Executes SLeeLa programs using the SLeeLa runtime | Enabled |
| External Java host | Launches a local Java executable as a child process | Optional; no JDK build dependency |
| AWT / Swing | Java GUI classes used by the external Java process | Depends on installed Java runtime and display |
| JavaFX | External Java process plus JavaFX modules and native libraries | Requires separate JavaFX availability |
| Embedded JVM | SLeeLa process creates a JVM with JNI Invocation API | Not implemented by this default configuration |

VM generations 1–11 are SLeeLa VM generations, not Java releases. Selecting generation 10 or 11 does not provide Java bytecode execution or an embedded JVM.

## Discovery order

The runtime probe searches in this order:

1. `SLEELA_JAVA` (an explicit executable path)
2. `JAVA_HOME/bin/java` or `JAVA_HOME/bin/java.exe`
3. `java` on `PATH` (Windows may resolve `java.exe`)

No installer is run automatically. If Java is required but unavailable, the launcher should fail with an actionable diagnostic.

## JavaFX launch

JavaFX is not bundled with most modern JDK distributions. When the source imports JavaFX, configure the module path with `SLEELA_JAVAFX_MODULE_PATH` and module list with `SLEELA_JAVAFX_MODULES`, or use a JDK distribution that includes the JavaFX modules. The default module list is `javafx.controls,javafx.graphics`. The launcher passes `--module-path` and `--add-modules` only when a module path is configured.

A successful Java executable lookup does not prove JavaFX is installed. If the module path is missing, the launcher emits a clear diagnostic rather than claiming that JavaFX is supported on the host. Graphical launch can still fail on headless hosts or where native GUI libraries are unavailable.

## Security and portability

- Native SLeeLa programs must not require Java.
- Do not construct a shell command from untrusted Java source contents.
- Quote each process argument using the existing platform launcher boundary.
- Do not silently install a JDK or change system configuration.
- Report the actual Java process exit status when available.
- Embedded mode must remain disabled until a separately configured JNI build has lifecycle, shutdown, threading, and GUI tests on Linux, Windows, and macOS.

## Validation

Run `test-suites/java-runtime-probe.sh` and `test-suites/java-runtime-bridge.sh` from the repository root. These tests validate detection and handoff planning without requiring a JDK or graphical display. Full Swing/JavaFX smoke tests should be conditional on a configured JDK/display and should report skipped, not passed, when those prerequisites are absent.
