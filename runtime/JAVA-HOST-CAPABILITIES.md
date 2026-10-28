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

## Launching existing Java applications

The `sleela run` command supports the following external-JVM forms:

```sh
# Runnable JAR (the JAR manifest supplies Main-Class)
sleela run build/my-app.jar [application arguments...]

# Compiled classes in a package-preserving output directory
sleela run --class-path build/classes com.example.Main [application arguments...]

# Class files in the current directory
sleela run --class com.example.Main [application arguments...]
```

The class path may contain multiple directories/JARs using the host platform's Java class-path separator (colon on Linux/macOS, semicolon on Windows). Package names must match the class-file directory structure, for example `build/classes/com/example/Main.class` is launched as `com.example.Main`. SLeeLa forwards application arguments as separate arguments and does not require source files when class files or a runnable JAR already exist. The JAR form requires a valid `Main-Class` manifest entry.

Java source launch remains available with `sleela run Program.java [arguments...]` on Java versions that support source-file mode. All these modes start an external Java process; they do not load bytecode into the SLeeLa VM.

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
