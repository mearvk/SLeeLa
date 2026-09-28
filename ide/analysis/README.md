# IDE Analysis

Analysis connects PSI to the SLeeLa compiler semantic model.

Services:
- declaration indexing;
- reference resolution;
- type lookup;
- module/import lookup;
- standard-library indexing;
- compiler diagnostics;
- quick-fix metadata;
- documentation lookup.

The initial implementation should prefer compiler-produced facts over duplicated semantic inference.
