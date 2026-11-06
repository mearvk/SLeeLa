# SLeeLa 3.0 API — session additions

A focused index of the API-visible additions made in this working session,
documented against the SLeeLa 3.0 edition. The generated HTML surfaces
(`sleeladoc-api/`, `docs/api/`, `api/sleela/3.0/version-014/`) were regenerated
from the live `/lib` tree and now include every new source unit below; this page
also records the **language-level** additions that are specified in
`SLEELA.syntax` rather than as a `/lib` class, which the generator cannot infer.

## 1. Loop-timing annotations and the `sleep` built-in (language)

`@loopDelay` / `@loopBudget` pace a `for` / `while` / `do` loop, and `sleep(ms)`
blocks the current thread anywhere.

- Full reference: [`LOOP_TIMING_API.md`](LOOP_TIMING_API.md)
- Specification: [`SLEELA.syntax` §32](../../../SLEELA.syntax)
- Opcode: `OP_SLEEP` (ISA index 157); source ISA record `SLISASleep` in
  [`lib/vm/InstructionSet.sleela`](../../../lib/vm/InstructionSet.sleela),
  rendered on the generated [`vm` package page](version-014/package/vm.html).
- Example: [`examples/loop-timing-annotation-demo.sleela`](../../../examples/loop-timing-annotation-demo.sleela)

## 2. Native big-integer multiply (`OP_BIGMUL` / `SLBigInteger.multiply`)

A native base-1e9 limb-array multiply bridged to the VM as `OP_BIGMUL`
(ISA index 156), with the larger `SL_MAX_ARRAY_LEN` ceiling (2^24) that lets a
multi-million-limb number fit in a single array.

- Source ISA record `SLISABigMul` in
  [`lib/vm/InstructionSet.sleela`](../../../lib/vm/InstructionSet.sleela)
  (generated [`vm` package page](version-014/package/vm.html)).
- Library surface: `SLBigInteger.multiply` in
  [`lib/math/SLBigInteger.sleela`](../../../lib/math/SLBigInteger.sleela)
  (generated [`math` package page](version-014/package/math.html)).

## 3. Standalone tools (repository top level, outside `/lib`)

These are complete programs/folders, not `/lib` library packages, so they are
documented by their own READMEs rather than the SleelaDoc package pages:

- **Local Generator** — scaffolds a local SLeeLa project from a blueprint:
  [`local-generator/README.md`](../../../local-generator/README.md).
- **Pi Generator** — streams the digits of Pi to a file via a spigot:
  [`pi-generator/README.md`](../../../pi-generator/README.md).

## Generated API surfaces

All three HTML surfaces are produced from the same `/lib` inventory by
`tools/generate-sleeladoc-api.py` (`make api-docs`) and were rebuilt this
session:

| Surface | Entry point |
|---|---|
| Live SleelaDoc site | [`sleeladoc-api/index.html`](../../../sleeladoc-api/index.html) |
| Contributor site | [`docs/api/index.html`](../../../docs/api/index.html) |
| Immutable 3.0 snapshot | [`api/sleela/3.0/version-014/index.html`](version-014/index.html) |
