# lib/large-language-models

SLeeLa standard-library mirror of the `artificial-intelligence/large-language-models`
C/C++/Python subsystem. These classes expose the same contracts in SLeeLa; the
native sources remain authoritative for the actual kernels, schemas, and weights
handling. Nothing here ships a model or weights.

| SLeeLa class | Mirrors (native prefix) | Purpose |
|---|---|---|
| `SLPackage` | — | Package facade for the compiler, Loader, and Nordshrift. |
| `SLLlmEngine` | `sllm_` | Thin engine over a caller-supplied backend adapter: load / unload / generate lifecycle and status codes. |
| `SLLlmGenerationOptions` | `sllm_generation_options` | Decoding knobs: max new tokens, temperature, top-p, seed. |
| `SLLlmVocabulary` | `sllmv_` | Token-id store: stable insertion-order ids, add / find / token lookup. |
| `SLLlmBaseVocabulary` | `sllmbv_` | Seed vocabulary loader (`sleela-base-en-1.0`): ten specials at fixed low ids, fail-closed id verification. |
| `SLLlmReasoning` | `sllmr_` | Reasoning Language Model (RLM): subjects, step kinds, verdicts, and the UNKNOWN-never-promoted commit policy. |
| `SLLlmCapacity` | `sllmc_` | Model-capacity profile (2–16 GB envelope) and derived reasoning budget per tier. |
| `SLLlmCodeModel` | `sllmcx_` | Procedural code-model profile: languages, task flags, verification check mode. |
| `SLLlmSleelaVerifier` | `sllmsl_` | Static SLeeLa-rule checker (reserved words, `close()` collision, comment forms, 64-field cap, metadata) that acts as the reasoning layer's symbolic half. |
| `SLLlmParametry` | `sllmp_` | Model binding: known-by-name (registry) or local-by-path, fail-closed. |
| `SLLlmLoader` | `sllme_` | Loader/handler idiom: fixed-width encode with OK / SKIP / ERROR, careful by construction. |

A runnable demo lives in `examples/LlmReasoningDemo.sleela`.

Every class carries the standard Metadata Class record (see
`.kiro/steering/metadata-class-standard.md` and `SLEELA.syntax` §15).
