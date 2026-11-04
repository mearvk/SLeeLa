# SLVoxelGrid

`SLVoxelGrid` is a **dense 3D grid of `int` cells** stored in a single flat,
contiguous native array indexed by `i = x + y*width + z*width*height` (row-major,
`x` fastest). It is the integrated alternative to a nested `int[][][]`: one
allocation, cache-linear scans, and no per-axis pointer chasing.

Cells carry a caller-chosen **empty sentinel** (default `0`) so a scan can skip
unset space. Note the VM default-initialises `new int[n]` to `null`, so
`allocate` explicitly fills every cell with the sentinel.

## Operations

- `allocate(int w, int h, int d, int emptyValue)`: allocate `w*h*d` cells, all
  set to `emptyValue`; negative dimensions clamp to `0`.
- `getWidth()` / `getHeight()` / `getDepth()` / `getEmptyValue()` / `size()`.
- `inBounds(int x, int y, int z)` / `indexOf(int x, int y, int z)` (`-1` out of range).
- `get(int x, int y, int z)`: cell value, or the empty sentinel out of range.
- `set(int x, int y, int z, int value)`: write a cell; `false` out of range.
- `getFlat(int index)` / `setFlat(int index, int value)`: direct flat-index access.
- `isEmpty(int x, int y, int z)`: whether a cell holds the empty sentinel.
- `clear()` / `fill(int value)`.
- `occupancy()`: count of non-empty cells (a cache-linear walk of the backing array).
- `nextOccupied(int startFlat)`: flat index of the next non-empty cell at or
  after `startFlat`, or `-1`; walk occupied cells without nested loops.
- `xOf(int index)` / `yOf(int index)` / `zOf(int index)`: decode a flat index.

## Example

See [`SLVoxelGridBasic`](../../impl/examples/slvoxelgrid_basic.sleela).
