# SLeeLa Native Reflection

The reflection package is explicit metadata, not a claim of universal C++ runtime reflection. C++17 does not provide a complete portable reflection facility, so metadata is registered explicitly or generated.

Supported metadata includes types, stable IDs, namespaces, inheritance, fields, methods, parameters, constructors, properties and attributes.

## Safety boundary

Reflection does not implicitly invoke arbitrary functions, construct objects, bypass access control or extend object lifetimes. Dynamic invocation and serialization require explicit adapters with documented ABI and lifetime contracts.

The registry is mutex-protected. A returned Type pointer is only valid while the registry is not concurrently mutated; callers that need stable snapshots should copy the Type.

## Production completion

Generated registration, enum-value metadata, serializer adapters, C ABI exposure and dynamic invocation remain separate opt-in layers and must be tested per platform/ABI.

**Max Rupplin — MEARVK LLC — 2026**
