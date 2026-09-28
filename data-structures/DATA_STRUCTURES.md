# Data Structure Correspondence

## 1. Value

Java-like values are represented as a discriminated value in the native layers.

- C: enum tag + union payload.
- C++: enum class + `std::variant`.
- Sleela: `DSValue` record with an explicit kind and primitive payload fields.

## 2. Ordered sequence

The sequence contract is:

- zero or more elements;
- stable insertion order;
- indexed access;
- append;
- size/capacity distinction.

C uses manually managed contiguous storage. C++ uses `std::vector`. Sleela exposes the semantic object and operation names so a later compiler/runtime implementation can lower them to the native representation.

## 3. Stack

The stack contract is LIFO:

- push;
- pop;
- peek;
- size;
- empty.

This directly corresponds to the VM operand-stack concept.

## 4. Queue

The queue contract is FIFO:

- enqueue;
- dequeue;
- front;
- size;
- empty.

The C implementation uses a circular buffer. C++ uses `std::deque`.

## 5. Map

The map contract associates a string key with a value:

- put/update;
- get;
- contains;
- size.

The reference implementations intentionally use straightforward storage rather than hiding the semantics behind a large framework.

## 6. Object record

An object record contains:

- a type/name;
- named fields;
- field lookup;
- field replacement.

This is the native counterpart of a Java-like object whose state is a collection of named fields.

## Invariants

All implementations preserve:

- size is never negative;
- indexed access is bounded;
- stack underflow and queue underflow are reported;
- map lookup distinguishes missing keys from present values;
- object field lookup distinguishes missing fields from present fields.

## Relationship to the existing VM

The current VM already has:

- `SLValue`;
- operand stack;
- constant pool;
- globals;
- call frames.

The new directory is therefore a **correspondence layer**, not a second VM. It gives those concepts stable data-structure names and cross-language reference implementations.
