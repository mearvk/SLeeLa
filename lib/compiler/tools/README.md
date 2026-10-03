# Compiler Tools

The executable compiler remains authoritative under `/impl/frontend`; package tooling orchestrates inventory, ISA coverage, build provenance, and artifact compilation without creating a second compiler language.

The repository-wide driver is:

```sh
python3 tools/sleela-build.py inventory-lib
python3 tools/sleela-build.py compile SOURCE OUTPUT
```

All `.sleela` files below `/lib` participate in the source inventory.
