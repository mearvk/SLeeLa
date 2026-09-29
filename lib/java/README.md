<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Java Library

The `/lib/java` library family defines the SLeeLa boundary for Java interoperability.

## Purpose

This library provides SLeeLa source definitions for:

- Java package availability.
- Java class references.
- Java object references.
- Java method references.
- Future Java/JVM bridge types and services.

## Compiler and Loader

The Java library is intended to be visible to the SLeeLa Compiler, Loader, and Nordshrift symbol collection.

## Source Standard

Every SLeeLa source file in this directory includes:

- A documentation header.
- An explicit Definition.
- A stable SLeeLa class declaration.
- No undocumented native behavior.

## Expansion

Future Java library work should extend this family rather than placing Java interoperability types in unrelated library directories.