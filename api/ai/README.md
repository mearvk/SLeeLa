# SLeeLa AI API

The SLeeLa AI API is a native, VM-connectable interface for data, files, audio,
and video. It deliberately separates **input representation**, **model
description**, **inference operation**, and **execution backend**.

The native C API lives in `impl/core/sleela_ai.h/.c`. It can be bound directly
to the Standard SLeeLa VM through a function boundary, or exposed through a
connector. The AI layer does not replace the SLeeLa runtime: it is a runtime
service.

## Supported input domains

- **Data** — byte-oriented or structured data supplied by the SLeeLa program.
- **Files** — named filesystem inputs subject to the existing SLeeLa file/security layer.
- **Audio** — native `SLMediaFrame` audio frames.
- **Video** — native `SLMediaFrame` video frames.

The A/V path intentionally reuses the existing timestamped media representation
rather than creating a second media model.

## Operations

The initial stable vocabulary is:

1. `inspect`
2. `classify`
3. `extract`
4. `summarize`
5. `transform`

The API describes what is requested; the selected provider/model performs the
actual inference. No AI operation implicitly executes shell commands, installs
software, changes files, or performs network activity.

## Native VM connection

There are two supported execution forms:

- **Native**: a provider is bound directly to the SLeeLa runtime.
- **Connector**: a provider is connected through `SLAIVMInvokeFn`, keeping the
  SLeeLa VM as the authoritative execution boundary.

This permits a local native model, an in-process provider, or a separately
managed connector without changing the SLeeLa program-level API.

## XML model and data descriptions

XML is a first-class declarative description format for the AI boundary. The
repository includes `api/ai/model.xml`, which describes model identity,
revision, trust state, input domains, operations, and output vocabulary.

This follows the repository's existing BODI/XML direction: XML describes a
validated operation/model contract; it is not itself arbitrary executable code.
W3C XML 1.0 remains the interoperability baseline. citeturn0search10

## Media interoperability

The native media model uses timestamped audio/video frames. This aligns with
the current media direction in which decoded audio/video are represented as
typed frames with sample rate/channel/frame metadata; W3C WebCodecs similarly
defines typed `AudioData` and `VideoFrame` representations and supports
reference-based media resources. citeturn0search0turn0search9

## Quality boundary

The AI API is intentionally a provider boundary. Model quality is not claimed
merely because an API exists. The implementation provides deterministic
validation, bounded data structures, explicit backend selection, and a
testable VM invocation seam. Higher-level model quality belongs to the selected
provider/model and its evaluation set.

## Source of truth

- Native interface: `impl/core/sleela_ai.h`
- Native implementation: `impl/core/sleela_ai.c`
- XML model: `api/ai/model.xml`
- API example: `api/ai/README.md`
- Smoke test: `impl/tests/core/ai_smoke.c`


## XML source locators

SLeeLa can describe an external XML document or XML flow source with a bounded source locator:

```xml
<source kind="audio"
        url="file:///media/input.xml"
        args="model=audio-v1&flow=inspect"/>
```

The same source vocabulary supports `data`, `file`, `audio`, `video`, `data-flow`, and `ai-flow`. The `url` identifies the XML resource and `args` carries declarative provider/model/flow parameters.

This is a **locator and model-flow declaration**, not an implicit fetch or execution command. SLeeLa resolves and retrieves the resource only through an explicitly authorized file/network provider and then validates the XML under the normal VM/security boundary. Relative XML references can follow XML Base semantics where the selected XML vocabulary permits them. citeturn0search0


## SLeeLa source examples

Source-level examples now live in `api/ai/examples/`:

- `ai.sleela` — AI model and operation flow.
- `data.sleela` — declarative data-model construction.
- `xml.sleela` — XML model/data loading and validation.
- `listener.sleela` — XML source listener declaration.
- `ai-data-listener-flow.sleela` — combined XML source → data model → AI → listener flow.

These examples define the intended SLeeLa program-level vocabulary for the native bridge. They are source documents for the API contract; until corresponding language/runtime bindings are installed, the `ai*`, `data*`, `xml*`, and `listener*` calls should not be described as already implemented compiler intrinsics.
