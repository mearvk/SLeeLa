# SLeeLa Recursion

This directory contains documentation, examples, and implementation notes for recursive programming in SLeeLa. The curated C/C++ source-file prototypes are collected in [`stanchion/`](stanchion/README.md).

## Scope

- Recursive function and method design
- Base cases and termination conditions
- Direct and indirect recursion
- Stack usage, depth limits, and safe failure behavior
- Tests for correctness and edge cases

## Guidance

1. Define a clear base case before implementing the recursive step.
2. Ensure each recursive step makes measurable progress toward a base case.
3. Consider iterative alternatives when recursion depth may be large or input is untrusted.
4. Test empty, minimal, typical, and boundary inputs.
5. Document expected depth and resource limits where they matter.

## Status

The `stanchion/` subdirectory provides 12 standalone C/C++ prototypes for generic, applied, scientific, and teaching use. These are reference examples, not SLeeLa-language implementations; compile and test them before treating them as verified.
