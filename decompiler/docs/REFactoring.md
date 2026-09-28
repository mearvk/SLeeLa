# SLeeLa Refactoring from Native Artifacts

Recovered code is evidence-derived output, not a byte-for-byte reconstruction of original source.

The refactoring pipeline is:

1. bytes -> decoded instructions;
2. instructions -> basic blocks;
3. blocks -> CFG/function candidates;
4. CFG/data flow -> SLIR;
5. SLIR -> typed pseudocode;
6. pseudocode -> reviewable C/C++;
7. tests and invariants -> validated refactoring.

Generated source must retain provenance comments or sidecar metadata so investigators can trace important constructs back to addresses and input offsets.
