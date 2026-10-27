# SLList

`SLList` is a concrete, **string-valued** list in the SLeeLa collections
library. It deliberately uses existing SLeeLa syntax and VM-managed arrays;
it does not require Generics or new VM opcodes.

## Operations

- `add(String value)`: append a value.
- `get(int index)`: return the value, or `null` for an invalid index.
- `set(int index, String value)`: replace a value; returns `false` for an invalid index.
- `removeAt(int index)`: remove an item and shift later values left; returns `false` for an invalid index.
- `remove()`: remove the last item when non-empty.
- `clear()`: set the logical length to zero while retaining backing-array capacity.
- `length()` / `empty()`: query the logical list state.

Indices are zero-based. Invalid mutations do not change the list. The backing
array may retain unused trailing slots after removal; `size` is the logical
length and only indices below it are visible through the public API.

This first version intentionally accepts **Strings only**. It is not a
heterogeneous `SLValue` collection and is not a generic `List<T>`. The source/VM audit and proposed type-safe design are documented in
[`SLList-Heterogeneous-Design.md`](SLList-Heterogeneous-Design.md). That design
requires an explicit dynamic value type and checked type conversion; it is not
safe to emulate with unchecked mixed-type array writes.

## Example

See [`SLListBasic`](../../impl/examples/sllist_basic.sleela) for a small
exercise of append, read, replace, remove, bounds behavior, clear, and reuse.
