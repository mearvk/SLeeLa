# SLeeLa Language

## Purpose

This document defines the practical language surface of SLeeLa: source structure, types, expressions, declarations, control flow, contracts, effects, modules, diagnostics, and interoperability.

## Source unit

A `.sleela` file is a Wrapper™. A source unit may begin with its language-version declaration and is compiled by the SLeeLa C++ front end.

## Syntax edition

The current normative edition is **SLeeLa Syntax 3.0**. The shared Sleela/Sleelvac frontend accepts `#sleela` declarations from **1.0 through 3.0**; an unversioned source defaults to 3.0 with a warning. Syntax 3.0 consolidates the established 1.x/2.x surface without erasing historical feature gates. A feature remains unavailable below its own minimum syntax version, even when a newer compiler accepts the file. See [`SLEELA.syntax`](../SLEELA.syntax) for normative grammar and [`api/sleela/2.1/language/SYNTAX-VERSION-MATRIX.md`](../api/sleela/2.1/language/SYNTAX-VERSION-MATRIX.md) for the feature history.

## Core constructs

- classes, fields, methods, constructors and access control;
- values, variables, bindings and expressions;
- conditionals, loops, calls and returns;
- structured `struct` values and member access;
- dynamic `T[]` **arrays** (`new T[n]`, `a[i]` read/write, `arrayLength`/`arrayGet`/`arraySet`/`arrayPush`);
- references and controlled managed storage;
- static/protected semantics;
- contracts, flows, effects, contexts, rules and projections;
- Maybe-style explicit optional values;
- compile-time conducted methods;
- concurrency operations exposed by the runtime;
- syntax-version compatibility.

## Semantic model

SLeeLa supports object-oriented, imperative, functional/expression-oriented, declarative, concurrent, event-driven, systems, numerical, model-based and transformation-oriented programming. These are methods available to one language rather than separate languages.

## Types

The design includes ordinary scalar/value types plus structured types, classes, references, contracts, variance, sealed hierarchies, row-polymorphic structures, Hindley-Milner-style inference, structural contract subtyping and effect typing.

## Native boundary

The language may reach C/C++ facilities through the documented native ABI and OS abstraction. Native execution is a controlled boundary; it is not an excuse for the language type system to silently assume native behavior.

## Diagnostics

Compiler diagnostics must identify source location, rule/version involved, and actionable cause. Version incompatibility is detected before ordinary parsing where the metadocument requires it.

## Compatibility

The authoritative formal rules remain in `src/Sleela.manifest`, `src/Sleela.formal-core`, `SLEELA.syntax`, and implementation documentation. This document is the user-facing language map.

**Max Rupplin — MEARVK LLC — 2026**
