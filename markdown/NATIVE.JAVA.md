# SLeeLa Native Java Runtime and Libraries

Max Rupplin - MEARVK LLC - 2026

## Purpose

This document defines the secondary Java execution circuit used when a SLeeLa program depends on Java libraries or Java-based GUI frameworks such as AWT, Swing, or JavaFX.

This is a runtime interoperability path. It is not a second Java interpreter inside SLeeLa.

## Main execution circuit

Native SLeeLa remains on the normal SLeeLa VM path:

SLeeLa source -> compiler / loader -> SLeeLa VM

When Java dependency signals are detected:

SLeeLa source -> compiler / loader -> Java runtime probe -> Java provider -> local Java VM -> Java program -> provider I/O -> SLeeLa

The Java circuit is therefore a secondary execution path selected by program requirements.

## Detection

The runtime probe recognizes Java SE and framework-specific signals including:

- `import java.`
- `package java.`
- SLeeLa Java envelopes using `javaType`
- `java.construct:`
- `java.invoke:`
- `java.static:`
- `.java` source paths
- `java.awt.*`
- `javax.swing.*`
- `javafx.*`

Framework classification gives JavaFX, Swing, and AWT their specific provider category while retaining Java SE as the underlying runtime requirement.

## Native Java VM discovery

The probe searches for a usable local Java executable in this order:

1. `SLEELA_JAVA`
2. `JAVA_HOME/bin/java` or `JAVA_HOME/bin/java.exe`
3. `java` on `PATH`

The probe does not silently install a Java distribution.

If a Java-dependent program is detected and no local VM is available, the user-facing SLeeLa layer should prompt for Java installation/configuration and then retry discovery.

## Java libraries

The Java VM is the execution foundation for Java libraries. SLeeLa does not duplicate the implementation of those libraries merely because a corresponding `.sleela` definition exists.

The `/lib/java/java` definitions describe the Java types available through the SLeeLa language boundary. Their behavioral implementation is supplied by the Java runtime/provider when execution crosses this boundary.

This distinction is important:

- SLeeLa `.sleela` definition: language-visible type/class contract.
- Java provider: interoperability and invocation boundary.
- Java VM: Java bytecode/runtime execution.
- Java library/framework: actual Java implementation.
- SLeeLa VM: native SLeeLa execution.

## Swing, AWT, and JavaFX

Java GUI dependencies use the same secondary circuit.

### AWT

A program using `java.awt.*` is classified as Java/AWT and is handed to the Java runtime when the required local VM is available.

### Swing

A program using `javax.swing.*` is classified as Java/Swing. Swing runs through the Java runtime rather than through a separate SLeeLa Swing interpreter.

### JavaFX

A program using `javafx.*` is classified as JavaFX. JavaFX requires more than the presence of a `java` executable: the required JavaFX classes/modules and their native platform components must also be available.

The JavaFX provider should therefore perform a framework-specific availability check before launch.

## JVM handoff

The bridge prepares a normal JVM invocation. For compiled classes it uses:

`java -cp <classpath> <main-class> [arguments]`

For Java source files it uses Java source-file mode:

`java <source-file> [arguments]`

The bridge itself does not invoke a shell. The platform launcher/provider is responsible for safe native process creation, argument handling, input delivery, and collection of the actual program output.

Example:

- Java program: `example.Hello`
- Class path: `build/classes`
- Input: `hello`
- JVM request: `java -cp build/classes example.Hello`
- Result: the provider returns the Java program's actual output through the SLeeLa runtime boundary.

For GUI programs, the same JVM process hosts the Java GUI framework while SLeeLa remains the controlling language/runtime boundary.

## Installation and distribution policy

When no Java VM is available, SLeeLa should ask the user to install or configure a Java distribution rather than silently changing the host system.

Suitable distribution sources can include established vendors such as Oracle Java, Microsoft Build of OpenJDK, and Eclipse Temurin. The specific vendor and required Java version should remain configurable by the application, platform, or SLeeLa distribution policy.

A distribution being listed here means it is an established Java distribution source; it does not constitute a blanket security guarantee.

## Input and output

The secondary circuit preserves ordinary program semantics as far as the provider supports them:

`SLeeLa input -> Java program -> Java stdout/stderr/result -> SLeeLa provider`

For GUI applications, window events and GUI state are handled by the Java GUI framework/provider rather than converted into an independent SLeeLa GUI implementation.

## Failure states

The provider should distinguish at least these states:

- Java not required: continue with SLeeLa VM.
- Java required and VM found: prepare/launch the Java program.
- Java required but VM absent: prompt for installation/configuration.
- VM found but required framework unavailable: report the missing Java library/module/framework.
- JVM launch failure: return the native process/runtime diagnostic.
- Java program failure: return its exit status and available diagnostics.

## Architectural rule

Do not make Java/Swing/JavaFX execution a hidden replacement for the SLeeLa VM.

The rule is:

**Run SLeeLa natively when the program is SLeeLa-native. Cross the Java boundary only when the program actually requires Java.**

This keeps the Java facility lightweight while allowing SLeeLa to use the Java ecosystem normally.

## Related implementation

- `runtime/java_runtime_probe.h`
- `runtime/java_runtime_probe.c`
- `runtime/java_runtime_bridge.h`
- `runtime/java_runtime_bridge.c`
- `test-suites/java-runtime-probe.sh`
- `test-suites/java-runtime-bridge.sh`
- `JAVA.RUNTIME.BRIDGE.md`

## Verification

The probe and bridge tests are intentionally self-contained. They verify detection and JVM handoff planning without requiring a Java installation on the test host.

The integration is now wired into the SLeeLa command-line loader/launcher for Java source files that require AWT, Swing, or JavaFX. Those framework inputs are dispatched through the Java runtime bridge; native SLeeLa and ordinary Java-family inputs retain their existing execution paths. The provider invokes Java source-file mode through the existing OS-aware native launcher.