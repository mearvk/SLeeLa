# SLeeLa Language Specification

## Status

This is the engineering specification companion to SL-META-0001. It separates normative intent from implementation status.

## Compilation contract

SOURCE → VERSION RESOLUTION → LEX → PARSE → AST → SEMANTIC CHECK → LOWER → BYTECODE/CORE → EXECUTE

A conforming compiler must reject a source unit that violates its declared supported language-version range.

## Lexical contract

The lexer recognizes identifiers, literals, operators, delimiters, comments, declarations, access modifiers, keywords, and the `#sleela` version pragma.

## Syntactic contract

The parser builds a typed AST for classes, fields, methods, statements, expressions, structs, calls, member access and supported control-flow forms.

## Semantic contract

Semantic analysis establishes declaration visibility, type compatibility, member resolution, method resolution, reference rules, struct layout rules, contract conformance, effects, and version constraints before lowering.

## Type contract

The language model permits explicit and inferred types. Inference must not silently cross an explicit annotation or contract boundary. Structural compatibility does not imply identity.

## Runtime contract

Lowered code executes against the stable C ABI exposed by `impl/core/sleela_core.h`. Runtime behavior must remain distinguishable from compiler behavior.

## Error contract

Errors are classified as lexical, syntax, semantic, version, lowering, runtime, resource, platform, or integration failures. A failure must not be represented as a successful result.

## ABI contract

The C ABI is versioned independently of surface syntax. A language syntax update must not silently change native calling conventions.

## Conformance

Conformance requires positive tests, negative tests, version tests, ABI tests, runtime tests, and platform tests.

**Authoritative companions:** `src/Sleela.manifest`, `src/Sleela.formal-core`, `SLEELA.syntax`, `COMPILER.md`.

**Max Rupplin — MEARVK LLC — 2026**
