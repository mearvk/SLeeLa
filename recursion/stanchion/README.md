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
