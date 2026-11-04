# SLOctree

`SLOctree` is a **sparse point-region octree** over an integer cube `[0, size)³`,
where `size` is rounded up to a power of two so the cube halves cleanly down to
unit cells. Each node covers an axis-aligned cube and is either a leaf (at most
one point payload) or has up to eight children, one per octant. `insert`
descends and subdivides **only along the path** to the inserted point, so empty
space costs nothing, and every scan prunes whole subtrees that are empty or do
not overlap the query. Suited to sparse volumes, point clouds, region/frustum
queries, and ray traversal.

A node is both the tree handle and a subtree root. Empty octants are `null` and
are skipped during traversal.

## Operations

- `allocate(int size)`: initialise the root cube covering `[0, size)³`, `size`
  rounded up to a power of two; clears contents.
- `getSpan()`: edge length of the root cube. `length()`: occupied unit cells.
  `empty()`. `branched()`: whether a node has subdivided.
- `insert(int x, int y, int z, String value)`: insert a point payload; returns
  `true` when a new cell became occupied, `false` on overwrite or out-of-cube.
- `query(int x, int y, int z)`: payload at a cell, or `null`.
- `contains(int x, int y, int z)`: whether the cell is occupied.
- `covers(int x, int y, int z)`: whether a point is inside this node cube.
- `collectRegion(int qx, int qy, int qz, int qw, int qh, int qd, SLList out)`:
  append every payload whose point lies in the axis-aligned box
  `[qx,qx+qw) × [qy,qy+qh) × [qz,qz+qd)`; returns the count appended.
  Non-overlapping and empty subtrees are pruned.
- `collectAll(SLList out)`: append every stored payload (still skips empty subtrees).

## Example

See [`SLOctreeBasic`](../../impl/examples/sloctree_basic.sleela).
