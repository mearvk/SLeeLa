# SLeeLa toolchain (Kiro settings for this repo)

Repo-scoped note on the native toolchain and how to compile/run SLeeLa here.

## Quick-compile binary collection

For now and for some time, a prebuilt binary collection lives in-tree at
**<https://github.com/mearvk/SLeeLa/tree/master/working-binaries>**
(local path [`/working-binaries/`](../../working-binaries/)). It lets us
**quick-compile SLeeLa or Nordshrift** without rebuilding the toolchain:

```sh
./working-binaries/sleela check <file.sleela>     # validate
./working-binaries/sleela run   <file.sleela>     # compile + execute
./working-binaries/sleela compile <file.sleela> -o <out.sleela>
./working-binaries/sleela nordshrift [--emit|--roundtrip] [--target=sleela|java|c] <file.sleela>
./working-binaries/nordshrift check|build <sheet.sst>   # the .sst transpiler driver
```

These are **Linux x86-64** builds; rebuild from source for any other platform
(see "Build from source" below). They are a convenience, not a replacement for
the integrity-gated source build.

## Binaries

The toolchain is **two** native executables. The committed copies in
[`/working-binaries/`](../../working-binaries/) report:

- **`sleela`** — `SLeeLa™ Language/Runtime 1.3.2-pro`, the authoritative
  compiler/runner, **supported source syntax 1.0 – 3.1**, native C/C++ Core VM.
  (Syntax 3.1, also written `3.01`, is SLEELA.syntax sections 44 (literal,
  initializer, labels, finally), 45 (char) and 46 (array). The committed
  `working-binaries/` were rebuilt 2026-11-13 and match the source-built
  `impl/build/sleela`.)
  Subcommands: `check <f>`, `run <f>`, `compile <f...> -o <out>`,
  `validate-artifact <f>`, `nordshrift [--emit|--roundtrip] [--target=…] <f>`,
  `version`. (Older builds branded this binary "Sleelvac™" and capped syntax at
  2.9; the current build raises the ceiling to 3.1.)
- **`nordshrift`** — `Nordshrift 2.7.1-dev`, the SLeeLa `.sst` transpiler driver
  (NS-SST-0001 spec 2.2), which **drives SLeeLa source syntax up to 3.1**.
  Subcommands: `check <sheet.sst>`, `build <sheet.sst>`, `objects`,
  `relevance --target=java|sleela|c [Object]`, `version`, `design-activity …`.
  (The SLeeLa→Nordshrift→back *round-trip* facility lives inside the `sleela`
  binary as `sleela nordshrift`, not in this standalone driver.)

There is **no separate `sleelac`** binary — compilation is `sleela compile`.

The prebuilt copies (Linux x86-64) are committed for quick access. They are
platform-specific and are **not** a substitute for the integrity-gated build;
rebuild from source for any other platform or when in doubt.

## Build from source (authoritative)

```sh
make compiler          # or: ./build-linux.sh
# -> impl/build/sleela, impl/build/nordshrift
```

`impl/frontend` is the authoritative front end (lexer → parser → semantic
analysis → IR/lowering); `impl/core` is the C/C++ execution core.

## Compiling / checking a program

```sh
sleela check lib/idea/IdeaSystem.sleela     # structural + semantic validation
sleela run   lib/idea/IdeaSystem.sleela     # compile + execute
```

Convenience wrapper: [`tools/idea-check.sh`](../../tools/idea-check.sh) builds
the toolchain on demand (or uses `working-binaries/`) and runs check + run.

## Verified built-in surface (this build)

The core semantic analyzer recognizes these contextual built-ins:

- conducted methods: `conduct`, `role`, `insight`, `congruent`, `route`,
  `sysdepth` (→ 3024), `degreemax` (→ 4)
- threading: `spawn`, `join`, `lock`, `unlock`, `send`, `recv`
- `best-of*` (route selection) and `synchro*` (measurement)

The `ai*` built-ins (`aiModel`, `aiInput`, `aiOperation`, `aiExecute`,
`aiResult`, `aiClose`) documented under `api/ai/` are a **separate API layer
and are NOT wired into the core semantic analyzer in this build** — a program
using them fails with `call to unknown method 'aiModel'` (the repo's own
`api/ai/examples/ai.sleela` fails identically). Model AI objectives with the
verified built-ins above instead; the `/idea` C/C++ core carries the real
objective structures.

## Idea System(TM)

- `lib/idea/IdeaSystem.sleela` — the Sleela runnable (compiles + runs clean).
- `idea/` — the C/C++ native core (`make run` to build + demo).
