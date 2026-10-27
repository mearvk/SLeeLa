# Native Collector Integration Contract

The SLVM Core includes `runtime/garbage_collector.h` and initializes a collector in the VM state. The Core also defines root marking for globals, the last result, mailboxes, and the current worker's value stack. Struct and array instances are registered as external GC objects.

## Root and recursion requirements

At every safe collection point, all values reachable from globals, active operand stacks, frame locals, closure captures, exception/unwind records, mailboxes, and native handles must be traced. Current Core frames use a bounded stack and frame table; verify that all live locals remain within the scanned stack prefix and add explicit frame-root tests before changing that representation.

Collection is currently deferred while worker threads are active, which avoids scanning concurrently mutating stacks but can delay reclamation. Any future concurrent collector must stop or coordinate every mutator and enumerate every active thread stack.

## Allocation and ownership

- `gc_allocate` owns its allocated payload.
- `gc_allocate_external` owns the collector wrapper, not the external payload; a destroy callback may update or release external state.
- Each external allocation must be paired with an appropriate lifecycle callback.
- A VM handle must be invalidated when its GC object is reclaimed.

## Barriers and generations

For every reference-field update, pass the actual old and new object references, including both struct and array values. The generational remembered set must remain correct across consecutive young collections. If bookkeeping allocation fails, the collector must conservatively fall back to a full collection rather than risk reclaiming a reachable object.

## Failure behavior

The collector must not sweep after incomplete marking. Allocation accounting and dynamic-array capacity arithmetic must reject overflow. A failed trace must leave all potentially reachable objects allocated and allow a later safe retry.

## Memory budget

The collector's collection threshold is a trigger for work, not a hard heap limit. The VM's configured managed-memory ceiling must be enforced at allocation boundaries separately. The intended default is a lazy 512 MiB ceiling, not an eagerly reserved heap.

## Verification

Run `make -C runtime test` for the native runtime regression target. Then run the repository's supported Core/SLVM test suite, including recursive object graphs and compiled-artifact execution. Do not mark the integration complete solely because the standalone collector tests pass.
