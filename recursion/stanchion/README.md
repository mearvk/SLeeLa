# SLeeLa Recursion Stanchion

The stanchion is a curated set of source-file-level recursion prototypes. Each file is a standalone C or C++ example with a focused responsibility, a visible base case, and a documented build command.

## Pattern analysis

The original examples in `/recursion` fall into three input-handling families:

- **Sequential:** arrays, strings, and numeric values. These generally use indices or decreasing values, validate boundaries, and normalize or check data before recursion.
- **Structural:** linked nodes, trees, and graphs. These follow the shape of data; null handling is essential, and graph traversal additionally needs cycle/visited-state tracking.
- **Search and mathematical:** backtracking, nested recursion, and numerical methods. These must constrain branching, recursion depth, overflow, or convergence.

A recursion prototype should make four things explicit: its input contract, base case, progress measure, and failure/resource policy. Recursion is not automatically safer or faster than iteration; deep or untrusted input may require an explicit depth bound or iterative implementation.

## Prototype catalogue

### C
- `c_recursive_accumulate.c` — reusable recursive fold over an integer array, with length and depth checks.
- `c_recursive_gcd.c` — applied numeric recursion using Euclid's algorithm and checked command-line parsing.
- `c_recursive_binary_search.c` — divide-and-conquer search over a sorted array, with explicit half-open bounds.
- `c_recursive_palindrome.c` — string input normalization for ASCII alphanumeric palindrome checking.
- `c_scientific_bisection.c` — scientific root-finding using a bracketed interval and tolerance/iteration limits.
- `c_teaching_trace.c` — small recursive factorial that prints call/return structure for instruction.

### C++
- `cpp_recursive_fold.cpp` — generic iterator-based recursive fold with caller-supplied operation.
- `cpp_tree_walk.cpp` — recursive preorder/inorder/postorder traversal of a binary tree.
- `cpp_graph_dfs.cpp` — applied graph DFS with visited-state protection against cycles.
- `cpp_merge_sort.cpp` — divide-and-conquer sorting over a vector.
- `cpp_subset_sum.cpp` — constraint-based backtracking over non-negative values.
- `cpp_adaptive_simpson.cpp` — scientific adaptive Simpson integration with tolerance and depth limits.

## Build examples

Run commands from this directory:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic c_recursive_accumulate.c -o c_recursive_accumulate
c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_recursive_fold.cpp -o cpp_recursive_fold
```

Each source file includes its own build command. These are portable C/C++ reference prototypes, not SLeeLa-language implementations. Compile and test them on each target platform before treating them as release-verified.

## Coverage

The 12 prototypes cover generic reuse, applied algorithms, scientific numerical work, and teaching/traceability. Together with the original eight examples, the directory has 20 focused source examples; the stanchion catalogue is the curated starting point when choosing a pattern for a new implementation.

## Primitive-specialized recursion prototypes

The primitive suite adds a C typed module and a C++ typed class for each primitive contract listed in `lib/CORE.BASE.md`: **Boolean, Byte, Character, Integer, Long, Float, and Double**. Java-compatibility primitives (such as Java's `short`) are not silently treated as native SLeeLa primitive contracts.

Each type module demonstrates three applicable recursion patterns: linear sequence traversal, divide-and-conquer range traversal, and structural binary-tree traversal. C uses a typed alias/node structure and functions; C++ uses a dedicated type-specific class and managed tree nodes. These are teaching/reference prototypes, not VM ABI definitions.

| Primitive | C source | C++ source/class | Important note |
|---|---|---|---|
| boolean | `c_primitive_boolean_recursion.c` | `cpp_primitive_boolean_recursion.cpp` / `SLStanchionBooleanRecursor` | Boolean values support predicate/counting recursion, not numeric accumulation. |
| byte | `c_primitive_byte_recursion.c` | `cpp_primitive_byte_recursion.cpp` / `SLStanchionByteRecursor` | Byte is treated as unsigned 8-bit; do not assume arithmetic wraps unless SLeeLa specifies it. |
| character | `c_primitive_character_recursion.c` | `cpp_primitive_character_recursion.cpp` / `SLStanchionCharacterRecursor` | Character is modeled as a Unicode scalar/code point, not an arbitrary UTF-8 byte. |
| integer | `c_primitive_integer_recursion.c` | `cpp_primitive_integer_recursion.cpp` / `SLStanchionIntegerRecursor` | This prototype uses signed 32-bit values; confirm the canonical SLInteger width. |
| long | `c_primitive_long_recursion.c` | `cpp_primitive_long_recursion.cpp` / `SLStanchionLongRecursor` | This prototype uses signed 64-bit values; confirm canonical SLLong width and overflow rules. |
| float | `c_primitive_float_recursion.c` | `cpp_primitive_float_recursion.cpp` / `SLStanchionFloatRecursor` | Exact equality is demonstration-only; scientific algorithms should use an explicit tolerance. |
| double | `c_primitive_double_recursion.c` | `cpp_primitive_double_recursion.cpp` / `SLStanchionDoubleRecursor` | Exact equality is demonstration-only; scientific algorithms should use an explicit tolerance. |

### Pattern applicability

- **Linear / tail-style progress, divide-and-conquer, and structural tree recursion** are demonstrated by every primitive-specific module.
- **Backtracking** remains represented by `cpp_subset_sum.cpp`, where numeric addition has defined meaning; it is not artificially applied to every primitive.
- **Mutual and nested recursion** remain represented by `mutual_even_odd.cpp` and `nested_ackermann.cpp`; these demonstrate control-flow patterns rather than operations meaningful for every type.
- **Scientific recursion** remains represented by bisection and adaptive Simpson integration for appropriate numeric types. Floating-point equality in the primitive examples is demonstration-only; scientific algorithms need explicit tolerances and convergence limits.
- **Graph recursion** remains represented by `cpp_graph_dfs.cpp`, with visited-state tracking. A primitive value alone does not define graph structure.

The C Character prototype uses `uint32_t`, while C++ uses `char32_t`; confirm the canonical SLeeLa character representation before integration. Confirm Byte, Integer, and Long widths against the runtime contracts. These prototypes have not yet been compiled or run as part of this update.
