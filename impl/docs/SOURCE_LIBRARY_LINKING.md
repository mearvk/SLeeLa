# Automatic SLeeLa Source-Library Linking

## Behavior

The single-file and multi-file `run`, `check`, and `compile` paths discover referenced source units under `/lib` and merge those declarations into the program before semantic analysis or artifact generation. Required source declarations are therefore part of the resulting runnable artifact; execution does not depend on the compiler reopening the original library source tree.

## Library root

By default, the compiler discovers `lib` relative to the working directory, its parent, or the SLeeLa repository root. Set `SLEELA_LIB=/absolute/path/to/lib` to use a different library root. The configured directory must contain `.sleela` source units.

## Resolution and failure behavior

- Only capitalized unit names referenced by the application or a previously loaded library unit are considered for automatic source loading.
- Dependency discovery is iterative and capped at 64 passes to avoid unbounded expansion.
- If multiple indexed units have the same referenced filename stem, resolution fails with an ambiguity diagnostic rather than choosing an arbitrary package.
- Unreadable or unparsable dependencies fail the operation.
- The SHA-256 execution/build verification gate remains required.

## Verification checklist

From the SLeeLa repository root:

```sh
python3 tools/verify-before-execution.py --manifest security/sha256-manifest.json --root .
make -C impl rebuild-verified
SLEELA_SHEET=./SHEET.sheet ./impl/build/sleela check example/gui-military-command-console.sleela
```

Then run the existing frontend, class-dispatch, artifact-ABI, and GUI/military-console tests. Confirm missing dependencies and ambiguous same-name units fail clearly, and confirm the produced artifact runs without needing the source library directory at runtime.

## Current implementation note

Automatic source discovery is implemented in the compiler driver. Full compiler/build acceptance must still be established by the repository CI and local regression suite before treating the change as release-verified.
