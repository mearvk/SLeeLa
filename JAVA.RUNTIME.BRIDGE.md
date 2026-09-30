# SLeeLa Java Runtime Bridge

Max Rupplin - MEARVK LLC - 2026

## Purpose

SLeeLa has a dry Java-runtime detection layer. It does not replace the SLeeLa VM, embed a JVM, download software, or alter the VM process model.

It answers whether a program materially depends on Java, AWT/Swing, or JavaFX and whether a local Java VM is available.

## Detection

The probe recognizes Java source files, java.* references, the existing SLeeLa Java conformance envelope (javaType, java.construct, java.invoke, java.static), java.awt.*, javax.swing.*, and javafx.*.

The strongest framework signal is reported as JavaFX, Swing, AWT, Java SE, or native SLeeLa.

## Runtime decision

The probe checks:

1. SLEELA_JAVA
2. JAVA_HOME/bin/java (java.exe on Windows)
3. java on PATH

It never starts Java itself.

The result is:

- continue-sleela-vm
- local-java-vm
- prompt-install-java-vm

The existing VM or launcher can consume that result and branch only when Java is actually required.

## Installation policy

If no local Java VM exists, SLeeLa should prompt rather than silently install one.

The prompt should direct the user to an official distribution source and allow SLEELA_JAVA or JAVA_HOME to be configured afterward. SLeeLa should not download an arbitrary executable.

Official Java distribution sources include Oracle Java, Microsoft Build of OpenJDK, and Eclipse Temurin. Vendor and version policy remains configurable.

## JavaFX

A Java executable alone does not prove that JavaFX libraries are installed. This dry layer only detects the JavaFX dependency and Java VM availability. A later provider can perform module/classpath checks.

## Integration boundary

SLeeLa source
-> existing compiler/loader
-> Java runtime dry probe
-> existing SLeeLa VM for native programs
-> local Java VM/provider for Java-dependent programs

This is a thin decision boundary, not a second interpreter.

## Verification

Run test-suites/java-runtime-probe.sh. The test does not require Java and verifies native SLeeLa, Java, Swing/AWT, JavaFX, and the existing Java conformance envelope.
