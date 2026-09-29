# SLeeLa JDK 28 Execution Bridge

The JDK 28 bridge allows SLeeLa bindings to retain SLeeLa source syntax while executing the actual Java/JDK implementation.

The bridge resolves the Java binary class, resolves the public constructor or method signature, executes the Java implementation on the JVM, and returns the Java-produced result or exception.

This avoids maintaining a second behavioral implementation of the JDK inside SLeeLa.

The SLeeLa Compiler and Loader remain responsible for source validation, binding resolution, permissions, and lifecycle control.

See java28/spec/JDK28-SLEELA-CONFORMANCE.md.
