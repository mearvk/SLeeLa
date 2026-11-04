# spatial

The `spatial` family provides memory-efficient structures for storing and
**scanning 3D space**, built directly on the SLeeLa VM-managed native array
primitive (no Generics, no new VM opcodes). The guiding idea is *flatten and
prune, do not nest*: a nested `int[][][]` is an array of handles pointing to
arrays of handles, so every scan chases pointers and thrashes the cache. The
structures here keep storage contiguous (dense case) or skip empty space
entirely (sparse case).

Choose by how occupied your space is:

| Space is… | Use | Why |
|---|---|---|
| **Dense** (most cells used) | [`SLVoxelGrid`](SLVoxelGrid.sleela) | one flat contiguous array indexed `x + y*W + z*W*H`; cache-linear scans, no per-axis pointer chasing |
| **Sparse, "what is near P"** | [`SLSpatialHash`](SLSpatialHash.sleela) | cells quantized and hashed into buckets; near-O(1) point lookup, neighbourhood scan visits only the 3×3×3 block |
| **Sparse, region / ray / tree scans** | [`SLOctree`](SLOctree.sleela) | recursive 8-way subdivision; whole empty or non-overlapping subtrees are pruned from every scan |

## A note on the VM storage model

A SLeeLa array stores each element as a tagged `SLValue` (a type tag plus an
8-byte value), so an `int[]` is contiguous but **not** a packed C `int[]`:
every cell carries a tag. Flattening to 1D still removes per-axis pointer
chasing and gives a single contiguous allocation — the big practical win — but
true tag-free, SIMD-grade density would require a native packed-buffer
primitive in the VM, which these library classes intentionally do not assume.

## Coordinates

All three accept integer coordinates and handle negative coordinates
correctly (`SLSpatialHash` floors to the containing cell; `SLVoxelGrid` and
`SLOctree` work over a non-negative cube and reject out-of-range points).

## Examples

- [`SLVoxelGridBasic`](../../impl/examples/slvoxelgrid_basic.sleela)
- [`SLSpatialHashBasic`](../../impl/examples/slspatialhash_basic.sleela)
- [`SLOctreeBasic`](../../impl/examples/sloctree_basic.sleela)
