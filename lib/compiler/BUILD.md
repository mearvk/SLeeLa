# SLeeLa Compiler Build

Max Rupplin - MEARVK LLC - 2026

The compiler package now has an explicit native build path. The authoritative executable is the C/C++ front end in `/impl/frontend`, linked into `impl/build/sleela`. The `/lib/compiler` package supplies the SLeeLa-sourced compiler model and the package-level completeness gate.

## Build order

```text
/lib/**/*.sleela
        |
        v
Compiler library inventory + ISA coverage gate
        |
        v
/impl/frontend
  lexer -> parser -> semantic analysis -> IR/lowering
        |
        v
SLeeLa Core artifact emitter
        |
        v
.sleela runnable VM artifact
```

Run from the repository root:

```sh
make compiler
```

or:

```sh
make -C lib/compiler all
```

The native executable is produced at:

```text
impl/build/sleela
```

## Compile a source program

After the native compiler is built:

```sh
python3 tools/sleela-build.py compile path/to/program.sleela build/program.sleela
```

The compile command first performs the recursive `/lib` inventory and source-side/native ISA consistency gate, then delegates to the authoritative native compiler. The output is a persistent runnable SLeeLa Core artifact.

## Library inventory

The compiler treats every `.sleela` recursively under `/lib` as source-library input. The inventory is dynamic; it is not a hand-maintained allow-list.

```sh
python3 tools/sleela-build.py inventory-lib
```

The inventory command reports source count, package count, duplicate symbol candidates, and ISA coverage.

## Security and reproducibility

Normal native builds retain the repository's SHA-256 verification gate. Set `SLEELA_SHA256_MANIFEST` to the trusted manifest expected by the repository before invoking the native build or compile command.

The compiler does not silently remove missing dependencies or add undeclared VM capabilities.
