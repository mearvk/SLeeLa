# JDK 28 Java Source Envelope for SLeeLa

## Set

The Java support set is represented as SLeeLa source files with the same class names and package paths as the Java types.

Examples:
- java.lang.String -> lib/java/java/lang/String.sleela
- java.util.ArrayList -> lib/java/java/util/ArrayList.sleela
- java.io.File -> lib/java/java/io/File.sleela
- java.time.Instant -> lib/java/java/time/Instant.sleela

These are SLeeLa source files. Their declarations, names, loader visibility, and policy surface belong to SLeeLa.

## Function

The SLeeLa source envelope is the public source contract. When a supported operation requires Java behavior, the conformance boundary delegates that operation to the actual Java implementation.

Model:

SLeeLa source -> SLeeLa compiler/loader -> Java conformance boundary -> actual JDK implementation

SLeeLa does not copy the JDK implementation into another language.

## Same-name rule

A Java binary name maps to a SLeeLa source path:

java.package.Type -> lib/java/java/package/Type.sleela

The final filename is the Java class name.

## Behavioral rule

A SLeeLa Java envelope must fail closed when the requested Java type or operation is unavailable. It must not silently substitute a SLeeLa approximation.

## Conformance

JavaConform.sleela and the Java bridge define the conformance boundary. Existing Java-side differential tests remain the behavioral oracle until the compiler/VM has a native Java invocation opcode.

## Pure SLeeLa meaning

The Java class declarations and control surface are authored and loaded as SLeeLa source. Java remains an execution provider for Java-defined behavior.
