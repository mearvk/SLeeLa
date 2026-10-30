# Sleelvac example (basic): build the compiler and read its help

`sleelvac` is the standalone SLeeLa **compiler product**, built from the same
front-end sources as the `sleela` command driver. Start by building it and
reading its own help, which is the authoritative list of its options.

From the repository root:

```sh
make -C impl
./impl/build/sleelvac --help
```

Why start with `--help`? Because `sleelvac` is a *separate* product from
`sleela`. Its option set is its own. Reading its help first means you never
guess at a flag — you use the ones it actually advertises.
