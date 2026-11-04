# SLQueue

`SLQueue` is a concrete, **string-valued FIFO queue** in the SLeeLa collections
library, backed by the VM-managed growable native array. It requires no
Generics and no new VM opcodes.

It uses a head cursor so `dequeue` does not shift the whole backing array on
every removal; live values occupy `[head, tail)`. When the dead prefix grows
past half the backing length, storage is compacted down to index `0`.

## Operations

- `enqueue(String value)` / `add(String value)`: append to the tail.
- `front()`: head value without removing it; `null` when empty.
- `dequeue()`: remove and return the head value; `null` when empty.
- `remove()`: discard the head item, if present.
- `clear()`: drop every item while retaining backing-array capacity.
- `length()` / `empty()`: number of live items / emptiness.

This first version accepts **Strings only**.

## Example

See [`SLQueueBasic`](../../impl/examples/slqueue_basic.sleela).
