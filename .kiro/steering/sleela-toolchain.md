# SLeeLa toolchain (Kiro settings for this repo)

Repo-scoped note on the native toolchain and how to compile/run SLeeLa here.

## Binaries

The toolchain is **two** native executables:

- **`sleela`** — the authoritative compiler/runner (Sleelvac™). Subcommands:
  `check <f>`, `run <f>`, `compile <f...> -o <out>`, `validate-artifact <f>`,
  `nordshrift <f>`, `version`.
- **`nordshrift`** — the Nordshrift round-trip tool.

There is **no separate `sleelac`** binary — compilation is `sleela compile`.

Prebuilt copies (Linux x86-64) are committed under
[`/working-binaries/`](../../working-binaries/) for quick access. They are
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
