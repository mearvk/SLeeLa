# SLeeLa Core / Base Class Assessment

**Revision:** 0.1  
**Date:** 2026-09-28

This document separates the SLeeLa core/base object vocabulary from the larger standard library.

## Base layer

The base layer should answer what nearly every SLeeLa program needs: values, objects, null, primitive values, strings, numbers, arrays, tuples, pairs, typing, comparison, conversion, functions, exceptions, modules, namespaces, symbols, annotations, metadata, and iteration.

The foundational C++ layer provides runtime services such as buffers, results, status, configuration, scheduling, files, processes, transports, and type descriptors. The /lib/core layer provides the language-facing semantic vocabulary above those services.

## Existing core contracts

Existing core contracts include SLObject, SLBoolean, SLByte, SLCharacter, SLInteger, SLLong, SLFloat, and SLDouble.

## Added base contracts

| Contract | Purpose |
|---|---|
| SLAny | universal value/reference contract |
| SLNull | explicit absence/null value |
| SLString | text value contract |
| SLNumber | common numeric abstraction |
| SLArray | indexed collection |
| SLTuple | fixed-arity value grouping |
| SLPair | two-value association |
| SLIterator | traversal state |
| SLIterable | iterable collection contract |
| SLCallable | callable value contract |
| SLFunction | function value contract |
| SLException | recoverable exceptional condition |
| SLThrowable | common throwable contract |
| SLModule | executable/library module |
| SLPackage | module grouping and namespace boundary |
| SLNamespace | symbol namespace |
| SLSymbol | language-level symbolic identifier |
| SLType | runtime type identity |
| SLGenericType | parameterized type identity |
| SLCast | explicit value conversion |
| SLEquality | equality semantics |
| SLComparable | ordering/comparison semantics |
| SLCloneable | explicit duplication contract |
| SLDisposable | deterministic resource release contract |
| SLAnnotation | declaration metadata |
| SLMetadata | extensible type/declaration metadata |

These are language contracts, not claims that every implementation must be a native C++ class. Runtime-sensitive operations should cross an explicit VM/OS bridge.

## Core/base boundary

1. Language base: values, objects, types, functions, exceptions, collections, and symbols.
2. Runtime foundation: memory, scheduling, threads, processes, I/O, networking, and serialization.
3. Standard library: mathematics, text, databases, HTTP, security, UI, compiler/debugger services, and other domains.
4. Application modules: domain-specific SLeeLa programs.

The core boundary should remain small enough that the language can bootstrap itself without importing high-level domain libraries.

**SLeeLa — MEARVK LLC — 2026**
