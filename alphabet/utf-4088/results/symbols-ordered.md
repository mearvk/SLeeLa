# UTF-4088 Ordered Candidate Symbols (Inputs 1–5)

**Canonical sequence:** ascending `input_id`, exactly matching the five input JSON definitions. It is not reordered by match score.
**Status:** experimental generated candidates, not Unicode characters or semantically validated symbols.

| Order | Input | Symbol ID | Candidate key | English wording | Latin wording | Nearest term | Composite score |
|---:|---:|---|---|---|---|---|---:|
| 1 | 1 | UTF4088-SYMBOL-000001 | 0xAFA5577333931820 | spatial distance | distantia spatialis | space | 0.866672 |
| 2 | 2 | UTF4088-SYMBOL-000002 | 0x3A8AF7C9593A783F | temporal relation | relatio temporalis | time | 0.866057 |
| 3 | 3 | UTF4088-SYMBOL-000003 | 0xFD6497A9A475933E | causal definability | definibilitas causalis | cause | 0.831129 |
| 4 | 4 | UTF4088-SYMBOL-000004 | 0x120C1C41A2380D33 | merit and importance | meritum et momentum | merit | 0.835553 |
| 5 | 5 | UTF4088-SYMBOL-000005 | 0xABE9098D80D3704E | eccentric distance | distantia eccentrica | eccentricity | 0.796653 |

## 1. UTF4088-SYMBOL-000001
Input 1: Input 1 — layer zero, top-left
Candidate key: 0xAFA5577333931820

```text
#.#.####
#.#..#.#
.#.#.###
.###..##
..##..##
#..#..##
...##...
..#.....
#..####.
..##.###
.####..#
#.##...#
```

## 2. UTF4088-SYMBOL-000002
Input 2: Input 2 — layer zero, move right
Candidate key: 0x3A8AF7C9593A783F

```text
..###.#.
#...#.#.
####.###
##..#..#
.#.##..#
..###.#.
.####...
..######
..####..
.##.###.
####..##
.##...#.
```

## 3. UTF4088-SYMBOL-000003
Input 3: Input 3 — layer zero, next row
Candidate key: 0xFD6497A9A475933E

```text
######.#
.##..#..
#..#.###
#.#.#..#
#.#..#..
.###.#.#
#..#..##
..#####.
##.##.#.
#.#..##.
.##.##.#
...#..##
```

## 4. UTF4088-SYMBOL-000004
Input 4: Input 4 — layer zero, continue right
Candidate key: 0x120C1C41A2380D33

```text
...#..#.
....##..
...###..
.#.....#
#.#...#.
..###...
....##.#
..##..##
.####...
##.###.#
###..##.
##...#..
```

## 5. UTF4088-SYMBOL-000005
Input 5: Input 5 — next Z-layer, reset to top-left
Candidate key: 0xABE9098D80D3704E

```text
#.#.#.##
###.#..#
....#..#
#...##.#
#.......
##.#..##
.###....
.#..###.
...#.###
...#.#.#
.##.....
.###.#.#
```

## Uniqueness and encoding

Each 8×12 bitmap stores the candidate key in its first 64 bits and a reversible 32-bit ordinal suffix in its final 32 bits. This makes the bitmaps unique for the 32-bit ordinal range. The glyph is an internal project representation, not a Unicode character.

Regenerate with `python3 alphabet/utf-4088/run_inputs.py` from the repository root.
