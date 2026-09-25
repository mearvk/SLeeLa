# SLeeLa Reflection

Portable C++17 runtime type-metadata foundation for SLeeLa.

## Purpose

Reflection supplies a deterministic metadata layer for describing native types to SLeeLa tools and runtime components. It currently supports registration and lookup of type names, size, alignment, kind, fields and method signatures.

C++17 does not provide universal automatic source-level reflection. This package therefore uses explicit or generated metadata rather than pretending arbitrary C++ types can be discovered automatically.

## Metadata model

The native registry represents:

- type name;
- size and alignment;
- type kind;
- fields;
- field offsets and sizes;
- methods and signatures.

The type-kind vocabulary includes:

- Void
- Boolean
- Integer
- Unsigned
- Floating
- String
- Struct
- Class
- Array
- Pointer
- Function
- Unknown

## Intended SLeeLa-facing surface

The native package should be exposed to Sleela-Complete through classes such as:

- ReflectionRegistry
- TypeInfo
- FieldInfo
- MethodInfo
- ParameterInfo
- EnumInfo
- BaseTypeInfo
- AttributeInfo
- ConstructorInfo
- PropertyInfo
- Invocation
- Serializer

These classes provide the authoring and runtime vocabulary; the C++ registry remains the native metadata substrate.

## Registration model

The current Registry supports explicit Type registration and lookup. The describe<T> helper records a basic type descriptor using sizeof(T) and alignof(T).

For larger systems, registration should be generated or performed through controlled macros or build tooling so that metadata remains synchronized with source definitions.

## Completeness roadmap

A production-grade reflection layer should be able to represent, where supported:

1. inheritance and base types;
2. constructors and destructors;
3. visibility and access;
4. static and instance members;
5. properties;
6. enum values;
7. namespaces and modules;
8. templates and generic metadata;
9. attributes and annotations;
10. callable method and function metadata;
11. type IDs, hashes and schema versioning;
12. serialization and deserialization metadata;
13. C ABI bridging;
14. thread-safe registry updates;
15. generated registration;
16. deterministic tests for layout and metadata consistency.

Dynamic invocation should be an explicit capability with a defined ABI and lifetime contract; metadata alone must not imply that arbitrary native methods are safely callable.

## Relationship to Sleela-Complete

Reflection is especially important for the Sleela-Complete authoring model. A designer can describe a module and its classes in Sleela, while native reflection can provide the corresponding type and layout metadata needed by tooling, diagnostics, serialization and interoperability.

See also the native header and implementation in this directory.