# Data Structure Correspondence

The SLeeLa data-structure layer provides a language-level counterpart to the native C/C++ contracts in this directory. The concrete SLeeLa declarations are split into one source file per structure so the Compiler/Loader can discover each object independently.

## 1. Value

Java-like values are represented as a discriminated value in the native layers.

- C: enum tag + union payload.
- C++: enum class + `std::variant`.
- SLeeLa: `DSValue` record with an explicit kind and primitive payload fields.
- SLeeLa source: `DSValue.sleela`.

## 2. Ordered sequence

The sequence contract is:

- zero or more elements;
- stable insertion order;
- indexed access;
- append;
- size/capacity distinction.

C uses manually managed contiguous storage. C++ uses `std::vector`. SLeeLa exposes the semantic object and operation names so a later compiler/runtime implementation can lower them to the native representation.

- SLeeLa source: `DSList.sleela`.

## 3. Stack

The stack contract is LIFO:

- push;
- pop;
- peek;
- size;
- empty.

This directly corresponds to the VM operand-stack concept.

- SLeeLa source: `DSStack.sleela`.

## 4. Queue

The queue contract is FIFO:

- enqueue;
- dequeue;
- front;
- size;
- empty.

The C implementation uses a circular buffer. C++ uses `std::deque`.

- SLeeLa source: `DSQueue.sleela`.

## 5. Map

The map contract associates a string key with a value:

- put/update;
- get;
- contains;
- size.

The reference implementations intentionally use straightforward storage rather than hiding the semantics behind a large framework.

- SLeeLa source: `DSMap.sleela`.

## 6. Object record

An object record contains:

- a type/name;
- named fields;
- field lookup;
- field replacement.

This is the native counterpart of a Java-like object whose state is a collection of named fields.

- SLeeLa source: `DSObject.sleela`.

## 7. SLeeLa operation and test surface

The SLeeLa module now separates the structure declarations from the aggregate operation facade and its source-level contract test:

| SLeeLa source | Responsibility |
|---|---|
| `DSValue.sleela` | Tagged value representation and constructors |
| `DSList.sleela` | Ordered sequence state and operations |
| `DSStack.sleela` | LIFO state and operations |
| `DSQueue.sleela` | FIFO state and operations |
| `DSMap.sleela` | Key/value map state and operations |
| `DSObject.sleela` | Named object-record state and operations |
| `DataStructures.sleela` | Aggregate cross-structure facade |
| `DataStructuresTest.sleela` | SLeeLa source-level contract exercise |

Every one of these files is present in both `/data-structures` and `/lib/data-structures`. The duplicate library path intentionally exposes the same semantic surface through the SLeeLa standard-library front end while retaining the module source organization.

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

The data-structures directory is therefore a **correspondence layer**, not a second VM. It gives those concepts stable data-structure names and cross-language reference implementations.

## Source-of-truth rule

The individual `.sleela` files are the authoritative SLeeLa source for the structures. The existing `data_structures.sleela` file remains as the legacy aggregate module source; new per-structure files provide the explicit Compiler/Loader discovery surface.

**SLeeLa — MEARVK LLC — 2026**
