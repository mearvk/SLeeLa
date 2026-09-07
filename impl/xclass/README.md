# Sleela ← SecureJDK 28 `.xclass` input

Sleela can take **SecureJDK 28 `.xclass`** files (XML class files, the SecureJDK
primary class format — see `userland/openjdk/jvm-config.xml`) as **input**. The
loader parses the `.xclass` structure emitted by the `xmc` compiler and
reconstructs an equivalent, runnable Sleela program, closing the loop:

```
Sleela source --(xmc / SecureJDK 28)--> .xclass --(this loader)--> Sleela
```

The normative mapping is in [`XCLASS.model`](XCLASS.model) (XCI-0001).

## Use

```sh
cd impl && make

# Ingest a .xclass and run the reconstructed program:
./build/sleela run xclass/samples/Person.xclass

# Ingest several into one program:
./build/sleela run xclass/samples/Greeter.xclass xclass/samples/Person.xclass

# Print the reconstructed Sleela source (does not run) — the human-readable view:
./build/sleela xclass --emit xclass/samples/Person.xclass

# Print identity / security / provenance:
./build/sleela xclass --info xclass/samples/Person.xclass
```

`sleela run <file.xclass>` auto-detects the extension; `sleela <file.xclass>`
works too.

## What is (and isn't) reconstructed

An `.xclass` carries a class's **structure**, not executable bytecode: each
method records its name, return type, parameter *count*, and modifiers plus
`lines`/`complexity` counts — there is no instruction stream. So the loader:

- maps each `<field>` to a Sleela field (with the xmc type mapped to
  `int`/`double`/`boolean`/`String`),
- maps each `<method>` to a **typed skeleton** — the exact signature, with a
  synthesized default-return body (`{}`, `return 0;`, `return "";`, ...),
- synthesizes an `XClassMain.main()` entry that prints a structural summary when
  no ingested class defines `main`,
- surfaces the `<security>` grades and `<provenance>` signature (advisory; not
  enforced in v1).

The reconstruction is faithful at the level the format actually carries — the
type structure and metadata — and it round-trips: `--emit` output re-parses and
runs as ordinary Sleela.

## Samples

`samples/Greeter.xclass`, `samples/Person.xclass`, `samples/Role.xclass` are the
reference files emitted by `xmc` (from the Ubuntu.Determinant `tools/xmc/` tree).
