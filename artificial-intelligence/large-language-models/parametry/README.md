# Parametry — binding SLeeLa models to known or local sources

**Parametry** is the arrangement that binds a model — a **SLeeLa LLM** or a
**SLeeLa neural net** — to a concrete parameter set, resolving **either** a
known/supported model (by name, from a registry) **or** a user's local model
(by path). It is the glue that lets the rest of the subsystem size itself to a
model without SLeeLa shipping any weights.

## The either/or

A binding (`sllmp_binding`) names a `kind` (`llm` or `neural-net`) and gives a
source:

| Source | How it resolves |
|---|---|
| **known** (`name`) | looked up in the built-in registry; parameters come from that entry (caller may override) |
| **local** (`local_path`) | the caller's declared parameters are used as-is (user-owned model) |

Resolution (`sllmp_resolve`) fails closed: an unknown name with **no** local
fallback -> `UNKNOWN_MODEL`; neither name nor path -> `NO_SOURCE`; a name whose
registry kind differs from the binding -> `KIND_MISMATCH`; a local neural net
with no layer shape -> `INVALID_ARGUMENT`. An unknown name **with** a local path
falls back to the local model.

## Known models (registry)

The built-in registry lists supported model **names** and their declared
parameters only — **no weights**. Binding a known name still means the user
supplies that family's weights. Mirror file: `known-models.json`.

| Name | Kind | Parameters |
|---|---|---|
| `sleela-code-compact`  | llm | 2 GB, ctx 2048 |
| `sleela-code-standard` | llm | 4 GB, ctx 4096 |
| `sleela-code-capable`  | llm | 8 GB, ctx 8192 |
| `sleela-code-large`    | llm | 16 GB, ctx 16384 |
| `sleela-nn-xor`        | neural-net | shape 2-4-1 |
| `sleela-nn-small-mlp`  | neural-net | shape 16-32-16-4 |

## What each binding feeds

- **LLM** -> a `CapacityProfile` (2–16 GB envelope) that sizes the reasoning
  budget (context/steps/backtracks), and pairs with the SLeeLa Code LLM
  (`sleela/`) and `SLLMR_SUBJECT_CODE`.
- **Neural net** -> an `slnn_network_config` (the real `neural-nets/` API): the
  resolved layer shape builds an actual fully-connected net you can `predict`
  and `train_sample` on.

## Usage (C++)

```cpp
using namespace sleela::ai;

// A SLeeLa LLM bound to a KNOWN model by name:
Parametry a = Parametry::known(SLLMP_KIND_LLM, "sleela-code-capable");
CapacityProfile cap = a.capacity();            // 8 GB -> capable tier

// ... or to a LOCAL model by path:
Parametry b = Parametry::local_llm("/models/my-sleela-coder.gguf",
                                   6ULL*1024*1024*1024, 0, "gguf-q4_k_m");

// A SLeeLa neural net bound to a KNOWN shape, or a LOCAL one:
Parametry c = Parametry::known(SLLMP_KIND_NEURAL_NET, "sleela-nn-xor");
Parametry d = Parametry::local_neural_net("/models/my-net.slnn", {8, 12, 3});
std::vector<std::size_t> shape = d.layer_sizes();   // -> slnn_network_config
```

## Files

| File | Role |
|---|---|
| `parametry.h/.c` | C11 registry + resolver (`sllmp_*`, ABI v1) |
| `parametry.hpp` | C++17 `Parametry` (known/local; -> CapacityProfile or layer shape) |
| `known-models.json` | Declarative registry mirror |
| `binding.schema.json` + `bindings.example.jsonl` | User binding records + examples |
| `parametry_selftest.c` | Resolution, overrides, fail-closed |
| `parametry_demo.c` | Binds an LLM and a neural net (known + local); builds a real `slnn_*` net |

## What is NOT bundled

No weights, no model files. Parametry ships the registry of supported **names**
and their declared parameters, and the binding logic. The user supplies the
actual model — a known family's weights, or a local model by path.
