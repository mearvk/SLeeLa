# SLeeLa Common, Included — Fundamental Responsibility API

## Status
Common Included API surface for reusable SLeeLa fundamentals.

## Rule
A fundamental class owns one coherent responsibility. Higher-level SLeeLa modules should reuse these classes rather than create parallel one-off abstractions for the same task.

## Current fundamental classes
- Resource — lifecycle ownership
- Identifier — stable identity
- Name — symbolic naming
- Version — version representation
- Status — operation state
- Error — structured failure
- Result — success/failure transport
- Option — optional value transport
- Configuration — typed runtime configuration
- Parameter — named operation input
- TypeDescriptor — runtime type metadata
- EnumDescriptor — enumerated metadata
- Schema — structural definition
- Validator — constraint checking
- Serializer — object encoding
- Deserializer — object decoding
- Buffer — bounded byte storage
- ByteStream — sequential byte transport
- Input — input abstraction
- Output — output abstraction
- Clock — time source
- Timer — deadline/timer responsibility
- Mutex — mutual exclusion
- Condition — condition synchronization
- Thread — execution-thread lifecycle

## Java responsibility correspondence
These are responsibility correspondences, not claims of identical implementation or ABI:
- Thread ↔ java.lang.Thread
- Clock/Timer ↔ Java time and scheduling facilities
- Mutex/Condition ↔ Java locking and condition facilities
- File/Path families ↔ Java filesystem responsibilities
- Buffer/ByteStream ↔ Java byte-buffer and stream responsibilities
- Result/Option ↔ Java result/optional value patterns
- Process/Environment ↔ Java process/environment responsibilities
- Serializer/Deserializer ↔ Java data/object encoding responsibilities
- Schema/Validator/TypeDescriptor ↔ Java type and validation responsibilities

## Source location
Native fundamental implementations are under `impl/fundamental/`, with one `.hpp` contract and one `.cpp` implementation per responsibility class.

## Inclusion contract
The Common Included layer is foundational. HTTP, server, GUI, database, driver, antivirus, compiler, and application modules may depend on it, but domain-specific policy remains outside this layer.

## Census
The repository currently contains 25 fundamental responsibility classes represented by 50 native source files in this directory. This document is the API index for that surface.
