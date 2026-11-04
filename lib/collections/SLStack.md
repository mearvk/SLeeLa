# SLStack

`SLStack` is a concrete, **string-valued LIFO stack** in the SLeeLa collections
library, backed by the VM-managed growable native array. It requires no
Generics and no new VM opcodes.

> Naming note: a different class named `SLStack` also exists in the `cpu`
> family — that one is the hardware call/data stack (stack pointer, base/limit,
> overflow/underflow faults). The spec assigns the bare name `SLStack` to the
> collections family. When a program could reach both, add `import collections;`
> (or `import cpu;`) to select the intended one; a plain single-segment package
> import is honored as a preferred resolution root.

## Operations

- `push(String value)` / `add(String value)`: push onto the top.
- `peek()`: top value without removing it; `null` when empty.
- `pop()`: remove and return the top value; `null` when empty.
- `remove()`: discard the top item, if present.
- `clear()`: drop every item while retaining backing-array capacity.
- `length()` / `empty()`: query stack height / emptiness.

Backing-array capacity freed by `pop`/`remove` is reused by later `push`
calls. This first version accepts **Strings only**.

## Example

See [`SLStackBasic`](../../impl/examples/slstack_basic.sleela).
