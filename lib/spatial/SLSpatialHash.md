# SLSpatialHash

`SLSpatialHash` is a **sparse 3D spatial hash grid**. Continuous integer points
are quantized to cells of a fixed `cellSize`, and each occupied cell maps to a
`String` payload (an id or label). Cells are distributed across a fixed number
of hash buckets with per-bucket chains, so insert and point lookup are near
O(1), and a "what is near P" scan only visits the cells of the neighbourhood
around P rather than the whole populated set. Suited to sparse worlds, point
clouds, and broad-phase collision.

Negative coordinates are handled with floor division (point `-1` with
`cellSize 10` is in cell `-1`, not `0`) and a non-negative bucket index.

## Operations

- `configure(int cellSize, int bucketCount)`: set quantization and bucket count
  (each clamped to a minimum of 1); clears all entries.
- `getCellSize()` / `getBucketCount()` / `length()` / `empty()`.
- `insert(int x, int y, int z, String value)`: insert or overwrite the payload
  for the cell containing the point; returns `true` when a new cell was created.
- `queryPoint(int x, int y, int z)`: payload for the cell containing the point,
  or `null` when unoccupied (near-O(1)).
- `contains(int x, int y, int z)`: whether that cell is occupied.
- `queryNeighbourhood(int x, int y, int z, int radius, SLList out)`: append every
  payload whose cell lies in the `(2*radius+1)³` block centred on the point
  (`radius 1` → 27 cells); returns the number appended. Visits only neighbour
  cells.
- `cellX(int x)` / `cellY(int y)` / `cellZ(int z)`: the integer cell of a coordinate.

A cell holds at most one payload; inserting into an occupied cell overwrites it.

## Example

See [`SLSpatialHashBasic`](../../impl/examples/slspatialhash_basic.sleela).
