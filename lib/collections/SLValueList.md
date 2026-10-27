# SLValueList

`SLValueList` is the first non-generic heterogeneous collection API. It stores VM tagged values through `Any[]`, including primitives, strings, null, arrays, and class/struct references. Stored references are handles, so the intended behavior is identity-preserving rather than object copying.

## API

- `add(Any value)`
- `Any get(int index)`
- `boolean set(int index, Any value)`
- `boolean removeAt(int index)`
- `void remove()` removes the final logical item
- `void clear()`
- `int length()`
- `boolean empty()`

Indices are zero-based. Invalid reads return null; invalid writes/removals return false. Removed entries are nulled so the list does not keep otherwise-unused object references alive; allocated array capacity is retained for reuse.

## Current boundary

The intended contract allows mixed values to be stored as `Any`, including class/struct references represented by VM handles. An `Any` reference does not expose arbitrary concrete members directly. The expected access pattern is to test the runtime type (for example, with `instanceof`) and then use a checked cast such as `(Customer) item`. A mismatched non-null cast must produce a controlled runtime error; casting null to a reference type should preserve null.

**Verification status:** source and runtime support for `Any`, type tests, and checked casts are present in the repository, but this document does not claim that the complete heterogeneous-list path has passed end-to-end tests. Confirm the actual compiler/runtime behavior before relying on the example semantics. Ordinary typed arrays have semantic checks where the declared array type is known; runtime array instances do not yet carry a complete element-type contract for every native or unknown-typed boundary.

For recursion, a stored reference must retain its identity as it is read from the list and passed through nested method calls. A leaf mutation should be visible through the caller's alias. The object must also remain reachable if the list is its only root during garbage collection. These are explicit regression requirements, not guarantees inferred merely from the `Any` type. See `SLList-Heterogeneous-Design.md`.
