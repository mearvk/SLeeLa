# Large Language Models

Provider-neutral native APIs for SLeeLa applications that connect to local or hosted large-language-model runtimes. The directory separates data ingestion, connectors, datasets, tokenizers, model packages, training, inference, and checkpoints.

## Directory map

| Path | Responsibility |
|---|---|
| `connectors/` | Adapters for local files and application-provided database, HTTP, memory, or object-storage streams |
| `datasets/` | JSONL interchange format and dataset schema guidance |
| `tokenizers/` | Model-compatible tokenizer assets and compatibility rules |
| `models/` | Model configuration, weights, metadata, and integrity guidance |
| `training/` | End-to-end data-to-token-batch training workflow |
| `inference/` | Model loading and streamed generation guidance |
| `checkpoints/` | Backend checkpoint lifecycle and provenance |
| `examples/` | Small, human-readable input examples |

## Public files

| File family | Purpose |
|---|---|
| `large_language_models.h/.c` | C ABI for backend lifecycle, generation options, and streamed UTF-8 output |
| `large_language_models.hpp/.cpp` | C++17 RAII inference facade |
| `large_language_models_vocabulary.h/.c` | C vocabulary/word-set store, stable token IDs, lookup, and whitespace-token encoding |
| `large_language_models_vocabulary.hpp/.cpp` | C++17 RAII vocabulary wrapper with string and vector helpers |
| `large_language_models_training.h/.c` | C training-backend ABI, batch training, evaluation, metrics, and checkpoint requests |
| `large_language_models_training.hpp/.cpp` | C++17 RAII training facade |
| `large_language_models_data.h/.c` | Bounded-memory connector read interface, local-file connector, and streaming JSONL reader |

## Data ingestion and connector flow

The supported integration path is:

```text
Dataset file / database / HTTP / object storage
                    |
                Connector
                    |
          sllmd_jsonl_read(...)
                    |
        Application JSON validation
                    |
          Model-compatible tokenizer
                    |
       Vocabulary / token-ID sequences
                    |
          Training batch construction
                    |
             sllmt_trainer
                    |
            Training backend
```

The C data API accepts an `sllmd_read_fn` callback and reads bounded chunks from its source. `sllmd_file_open/read/close` provide a local-file connector; applications can implement the same callback for databases, HTTP response bodies, memory buffers, or object storage. `sllmd_jsonl_read` emits one complete, non-empty record at a time, handles CRLF, accepts a final record without a newline, ignores blank lines, and enforces a caller-selected maximum record size. It does not load the complete dataset into memory.

The reader intentionally emits raw JSONL record bytes: it is not a JSON parser. Applications must validate each record against a versioned schema, extract text fields, check data rights/provenance, and tokenize with the exact tokenizer associated with the target model. The included example is in `examples/sample-instruction.jsonl`.

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

The backend remains responsible for forward/backward computation, optimizer state, data batching policy, mixed precision, distributed synchronization, and checkpoint serialization. The caller must ensure token IDs match the model's tokenizer and vocabulary. Do not train on data without appropriate rights, consent, and privacy safeguards.

## Build

Compile each C source as C11 and each C++ facade as C++17:

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic -c large_language_models.c
cc -std=c11 -O2 -Wall -Wextra -pedantic -c large_language_models_vocabulary.c
cc -std=c11 -O2 -Wall -Wextra -pedantic -c large_language_models_training.c
cc -std=c11 -O2 -Wall -Wextra -pedantic -c large_language_models_data.c
c++ -std=c++17 -O2 -Wall -Wextra -pedantic -c large_language_models.cpp
c++ -std=c++17 -O2 -Wall -Wextra -pedantic -c large_language_models_vocabulary.cpp
c++ -std=c++17 -O2 -Wall -Wextra -pedantic -c large_language_models_training.cpp
```

Link the C++ facades with their C objects and the selected inference/training backend implementations. No model weights, production tokenizer model, JSON parser, network client, or inference/training runtime is bundled here.

## Ownership, safety, and concurrency

- Callers own backend state, model files, datasets, credentials, and network configuration.
- Never hard-code API keys or credentials in source files or examples.
- Connector user data and backend structures are borrowed and must outlive the read operation or engine/trainer that uses them.
- The JSONL reader allocates at most `max_record_bytes + 1` bytes for its current record plus a fixed input buffer.
- Validate model identifiers, token IDs, dataset provenance, JSON schemas, and resource limits inside each application/backend.
- The vocabulary object and engine/trainer lifecycle objects are not internally synchronized; use external synchronization when sharing an instance across threads.
- Backends must document their own thread-safety, cancellation, determinism, memory use, and checkpoint guarantees.
- These interfaces are integration contracts, not claims that inference or training works without a configured backend.
