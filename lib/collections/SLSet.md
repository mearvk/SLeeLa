# SLSet

`SLSet` is a concrete, **string-valued set** (no duplicate members) in the
SLeeLa collections library, backed by the VM-managed growable native array. It
requires no Generics and no new VM opcodes.

Membership and insertion are linear scans, which suits small-to-moderate sets;
iteration order is insertion order.

## Operations

- `add(String value)`: insert if absent; returns `true` when the set grew,
  `false` when the value was already present.
- `contains(String value)` / `indexOf(String value)`: membership / position
  (`-1` when absent; `null` is matched, so at most one `null` member is held).
- `discard(String value)`: remove a member, preserving order; returns `true`
  when a member was removed.
- `at(int index)`: member at an insertion-order index, or `null` when out of range.
- `remove()`: drop the last member, if present.
- `clear()`: drop every member while retaining backing-array capacity.
- `length()` / `empty()`: member count / emptiness.

This first version accepts **Strings only**.

## Example

See [`SLSetBasic`](../../impl/examples/slset_basic.sleela).
