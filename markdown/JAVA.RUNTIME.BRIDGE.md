# SLeeLa Java Runtime Bridge

Max Rupplin - MEARVK LLC - 2026

## Main execution route

Java-dependent SLeeLa programs use a thin runtime boundary instead of a second Java interpreter inside the SLeeLa VM.

SLeeLa source
-> existing compiler / loader
-> Java runtime probe
-> Java provider
-> local Java VM
-> Java program
-> program input / output
-> SLeeLa provider boundary

Native SLeeLa programs continue through the existing SLeeLa VM.

## Runtime decision

- Native SLeeLa: continue in the SLeeLa VM.
- Java-dependent + local VM present: hand off to the configured local Java VM.
- Java-dependent + no VM present: prompt the user to install/configure an approved Java distribution, then retry the probe.

The probe checks SLEELA_JAVA, JAVA_HOME/bin/java (java.exe on Windows), then java on PATH. It does not silently install software.

## Main dispatch boundary

The main Java path is `sleela_java_runtime_dispatch_source()`. It first performs the probe. Native SLeeLa returns to the existing VM; Java-dependent code with a discoverable local Java VM receives a normal JVM handoff plan; Java-dependent code without a VM reports the installation-prompt action. The SLeeLa launcher now consumes this dispatch for Java source files using AWT, Swing, or JavaFX, routing those programs to the local Java VM through the existing OS-aware native launcher. No second interpreter is introduced.

## JVM handoff

runtime/java_runtime_bridge.c prepares the normal JVM request:

For compiled Java classes:

java -cp <classpath> <main-class> [arguments]

For Java source files:

java <source-file> [arguments]

The bridge records the sample input/output contract but does not invoke a shell or launch a process itself. Platform-native process creation remains the responsibility of the existing launcher/provider.

Example:
- Program: example.Hello
- Input: hello
- JVM request: java -cp build/classes example.Hello
- Output: the Java program's normal stdout/result returned through the provider.

The launcher applies the source-file handoff to Java AWT, Swing, and JavaFX programs. JavaFX still requires its JavaFX classes/modules and native platform components to be present.

## Installation prompt

If no Java VM is available, the user-facing layer should present a clear choice to install/configure a Java distribution from an approved source. The probe remains non-interactive.

Examples of established distribution sources are Oracle Java, Microsoft Build of OpenJDK, and Eclipse Temurin. Vendor/version policy remains configurable.

## Separation of responsibilities

- Compiler/loader: determines the program.
- Java runtime probe: determines whether Java is required and whether a VM is discoverable.
- Java provider/launcher: performs the JVM process handoff and returns program I/O.
- JavaFX/AWT/Swing provider: verifies framework-specific runtime requirements.
- SLeeLa VM: owns native SLeeLa execution.

## Verification

Run test-suites/java-runtime-probe.sh and test-suites/java-runtime-bridge.sh.
