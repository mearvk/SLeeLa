# SLeeLa Build Output Layout

The SLeeLa build process produces a stable runtime-facing directory beneath `impl/build/SLeeLa/` in addition to compiler object files.

```text
impl/build/SLeeLa/
├── bin/
│   ├── sleela
│   └── nordshrift
├── Config/
│   └── sleela.conf
├── Options/
│   └── OPTIONS.md
├── lib/
├── docs/
│   ├── MEMORY_MANAGER.md
│   └── PURPOSE.md
└── README.md
```

## Config

`Config/` is the consolidated default configuration location for a staged build. A fresh build copies the repository's default configuration into `Config/sleela.conf`, so the resulting runtime has one clear default configuration set rather than scattered configuration files.

The current default memory setting is a **2 GiB maximum**, with an allowed configuration range of **256 MiB through 2 GiB**.

## Options

`Options/` contains the runtime/build option reference for the staged result. Options select or override behavior; defaults remain in `Config/`.

Build objects and temporary test binaries remain under `impl/build/` outside the staged runtime directory. The staged directory is therefore the basic deployable form produced by the build process.
