# Large Language Models

Provider-neutral native API for connecting SLeeLa applications to local or hosted large-language-model runtimes.

## Public files

| File | Purpose |
|---|---|
| `large_language_models.h` | C ABI, backend vtable, generation options, streaming callback |
| `large_language_models.c` | Backend lifecycle, validation, and callback dispatch |
| `large_language_models.hpp` | C++17 RAII interface |
| `large_language_models.cpp` | C++ facade and `std::string_view` streaming adapter |

## Design

The API deliberately separates the public SLeeLa interface from model-provider details. A backend adapter supplies three operations:

1. `load` — initialize a named model or deployment.
2. `unload` — release backend resources.
3. `generate` — produce UTF-8 chunks and report them through the supplied callback.

This directory does not bundle model weights, a tokenizer, or an inference engine. A provider or local-runtime adapter must be implemented and supplied by the host application.

## Lifecycle

Create an engine with a fully populated `sllm_backend`, load a model identifier, initialize generation options, and call generate. Return zero from the chunk callback to request cancellation. Destroying an engine unloads a model that is still loaded.

The backend structure and its `user_data` are borrowed, not copied deeply. Keep the backend name and adapter state alive for the entire engine lifetime.

## Build

Compile the C implementation as C11 and the C++ facade as C++17:

```sh
cc -std=c11 -O2 -c large_language_models.c -o large_language_models.o
c++ -std=c++17 -O2 -c large_language_models.cpp -o large_language_models_cpp.o
```

Link the C++ facade with the C object and the provider adapter implementation.

## Contract and security

- The caller owns backend state, model files, credentials, and network configuration.
- Never hard-code API keys or credentials in source files or examples.
- Validate model identifiers and enforce resource limits inside each backend.
- The API passes UTF-8 text; adapters should document tokenization, context-window, and output-truncation behavior.
- Engine operations are not internally synchronized. A backend must document its thread-safety and cancellation behavior.
- The API is an integration boundary, not a claim that inference works without a configured backend.
