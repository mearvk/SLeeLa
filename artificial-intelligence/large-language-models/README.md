# Large Language Models

Provider-neutral native APIs for SLeeLa applications that connect to local or hosted large-language-model runtimes. The directory now separates inference, vocabulary management, and model training so each can evolve independently.

## Public files

| File family | Purpose |
|---|---|
| `large_language_models.h/.c` | C ABI for backend lifecycle, generation options, and streamed UTF-8 output |
| `large_language_models.hpp/.cpp` | C++17 RAII inference facade |
| `large_language_models_vocabulary.h/.c` | C vocabulary/word-set store, stable token IDs, lookup, and whitespace-token encoding |
| `large_language_models_training.h/.c` | C training-backend ABI, batch training, evaluation, metrics, and checkpoint requests |
| `large_language_models_training.hpp/.cpp` | C++17 RAII training facade |

## Vocabulary and word sets

The vocabulary API stores exact UTF-8 token strings and assigns stable, insertion-order `uint32_t` IDs. It uses an open-addressed hash table for average constant-time token lookup and grows its storage as the vocabulary expands. Duplicate additions return the existing ID. The encode helper maps whitespace-separated tokens to IDs and returns an owned ID array that the caller must release with `sllmv_vocabulary_free_ids`.

This is deliberately a vocabulary primitive, not a claim to implement every tokenizer. It does not currently perform BPE, WordPiece, unigram segmentation, byte fallback, Unicode normalization, or punctuation splitting. Punctuation remains attached to its whitespace-delimited token. Applications that need a model's exact tokenizer should provide that tokenizer in their model backend and keep its vocabulary/model metadata aligned. Input strings must be valid UTF-8 when the application treats them as UTF-8.

## Inference

The inference backend supplies three operations:

1. `load` — initialize a named model or deployment.
2. `unload` — release backend resources.
3. `generate` — produce UTF-8 chunks and report them through the supplied callback.

Return zero from the chunk callback to request cancellation. The backend structure, backend name, and `user_data` are borrowed; keep them alive for the entire engine lifetime.

## Training API

The training API defines a backend contract rather than embedding a particular transformer architecture or accelerator runtime. A backend supplies initialization, shutdown, batch-training, evaluation, and checkpoint operations. Training batches are represented as input and target token-ID arrays; metrics report loss, processed tokens, and optimizer steps. Options expose learning rate, gradient clipping, micro-batch size, gradient-accumulation steps, and a deterministic seed.

This is a far-reaching integration surface for CPU, GPU, distributed, or hosted training implementations, while avoiding a misleading built-in claim: actual forward/backward passes, optimizer state, data loading, mixed precision, distributed synchronization, and checkpoint format remain the responsibility of the selected backend. The caller must ensure token IDs match the model's tokenizer and vocabulary. Do not train on data without appropriate rights, consent, and privacy safeguards.

## Build

Compile each C source as C11 and each C++ facade as C++17:

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic -c large_language_models.c
cc -std=c11 -O2 -Wall -Wextra -pedantic -c large_language_models_vocabulary.c
cc -std=c11 -O2 -Wall -Wextra -pedantic -c large_language_models_training.c
c++ -std=c++17 -O2 -Wall -Wextra -pedantic -c large_language_models.cpp
c++ -std=c++17 -O2 -Wall -Wextra -pedantic -c large_language_models_training.cpp
```

Link the C++ facades with their C objects and the selected inference/training backend implementations. No model weights, tokenizer model, or inference/training runtime is bundled in this directory.

## Ownership, safety, and concurrency

- Callers own backend state, model files, datasets, credentials, and network configuration.
- Never hard-code API keys or credentials in source files or examples.
- Backend structures and their `user_data` pointers are borrowed and must outlive their engine/trainer.
- Validate model identifiers, token IDs, dataset provenance, and resource limits inside each backend.
- The vocabulary object and engine/trainer lifecycle objects are not internally synchronized; use external synchronization when sharing an instance across threads.
- Backends must document their own thread-safety, cancellation, determinism, memory use, and checkpoint guarantees.
- The interfaces are integration contracts, not claims that inference or training works without a configured backend.
