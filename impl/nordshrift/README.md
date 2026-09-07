# Nordshrift

**Nordshrift** is the transpiler driver for Sleela. It reads a **`.sst` control
sheet** (the format specified by **NS-SST-0001** — see `/SST.model`) and drives
the transpilation of the Sleela sources the sheet names into the target the
sheet selects — the **triplet**: **Java**, **Sleela** (executed on the C core),
or **C**.

The `.sst` file is the *control surface*, not the program. The program is the
set of `.sleela` files named by the sheet's `source:` section.

## Build & run

```sh
cd impl
make                                          # builds build/sleela and build/nordshrift

nordshrift check <sheet.sst>                  # validate a sheet; print NSS-* diagnostics
nordshrift build <sheet.sst>                  # transpile the sheet's sources to its target
```

### Self-contained demo

```sh
./build/nordshrift check nordshrift/examples/demo/build.sst
./build/nordshrift build nordshrift/examples/demo/build.sst    # target-language sleela -> runs on the core
```

The demo sheet sets `target-language sleela`, so `build` transpiles
`examples/demo/src/Demo.sleela` and runs it on the Sleela core:

```
square(1) = 1
square(2) = 4
...
done
```

Flip `target-language` to `java` or `c` in the sheet to retarget the same source
— the emitted Java compiles with `javac` and the emitted C compiles with `gcc`,
producing identical output.

## The `.sst` sheet (NS-SST-0001)

Indentation-significant, pragma-first:

```sst
#nordshrift 1.0
#sleela     1.0

sheet demo:
  version      1.0.0
  description  "Transpiles the demo Sleela sources."

source:
  root  "src"
  glob  "**/*.sleela"

target:
  root            "out"
  layout          mirror-source
  java-version    21
  target-language sleela      // java | sleela | c   (Nordshrift triplet superset)
```

The parser handles all the spec's sections (`sheet`, `import`, `source`,
`target`, `pipeline`, `rules`, `effects`, `derive`, `guards`, `interop`,
`profile`, inline `rule` blocks) and emits the structured **NSS-*** diagnostics.
`source`/`target`/`pipeline` are honored by the build; the analysis-oriented
sections (`rules`/`effects`/`derive`/`guards`/`interop`/`profile`) are parsed and
validated now and applied to emission in a later stage.

## Conformance notes

- The spec's `target` section is Java-only; Nordshrift adds `target-language`
  (default `java`) to drive the triplet — a conformant superset.
- The spec references a companion `SL-META-0001` (Sleela meta-model). It is not
  in the repo; the meta-model concepts are stubbed for the deferred sections.

See [`NORDSHRIFT.md`](NORDSHRIFT.md) for the architecture, the section table, and
the implemented diagnostic codes; see `/SST.model` for the normative grammar.
