<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Java Library

The `/lib/java` library family defines the SLeeLa boundary for Java interoperability.

## JDK 28 Framework Bindings

SLeeLa provides controlled source-level bindings for selected JDK 28 framework types. JDK 28 currently has a draft/ad-hoc API specification, so these files describe the framework contract without copying or reimplementing Java's internal implementation. citeturn0search0turn0search5

The initial binding families include:

- `java.lang`
- `java.util`
- `java.io`
- `java.nio`
- `java.nio.file`
- `java.net`
- `java.time`
- `java.math`

Additional JDK framework families will be added as the SLeeLa Java bridge matures, including reflection, concurrency, security, HTTP, class-file APIs, and compiler APIs.

## SLeeLa Control

A Java framework binding does **not** make SLeeLa subordinate to Java. The SLeeLa Compiler and Loader remain responsible for resolving the SLeeLa source model and deciding when a Java framework operation is permitted.

The intended model is:

`SLeeLa source → SLeeLa Compiler → controlled Java binding → JVM/JDK service`

This permits a user to employ the Java framework while retaining SLeeLa syntax, source definitions, loading rules, and runtime control.

## Compiler and Loader

The Java library is intended to be visible to the SLeeLa Compiler, Loader, and Nordshrift symbol collection.

## Source Standard

Every SLeeLa source file in this directory includes:

- A documentation header.
- An explicit Definition.
- A stable SLeeLa class declaration.
- A declared Java framework type.
- No copied Java implementation code.
- No undocumented native behavior.

## Expansion

Future Java library work should extend this family rather than placing Java interoperability types in unrelated library directories.
