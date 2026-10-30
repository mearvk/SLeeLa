# LLM Basis: Exemplar AI Cases

Ten terminal-started examples of LLM-driven application behavior, each implemented in C11 and C++17. Every source file has its own `main()`; startup arguments select a model, generation settings, and task input.

## Cases

| Case | C11 | C++17 |
|---|---|---|
| Conversational assistant | `01-chat.c` | `01-chat.cpp` |
| Document summarization | `02-summarize.c` | `02-summarize.cpp` |
| Question answering | `03-question-answer.c` | `03-question-answer.cpp` |
| Text classification | `04-classify.c` | `04-classify.cpp` |
| Structured information extraction | `05-structured-extraction.c` | `05-structured-extraction.cpp` |
| Translation | `06-translate.c` | `06-translate.cpp` |
| Code review assistant | `07-code-review.c` | `07-code-review.cpp` |
| Context-grounded response | `08-context-grounded.c` | `08-context-grounded.cpp` |
| Sentiment and tone analysis | `09-sentiment.c` | `09-sentiment.cpp` |
| Step-by-step tutor | `10-tutor.c` | `10-tutor.cpp` |

## Runtime contract

- C programs call `sllm_engine_create/load/generate/destroy`.
- C++ programs use the `sleela::ai::LanguageModel` RAII wrapper.
- Both stream UTF-8 output and accept the same command-line arguments.
- `basis_backend.h` declares the adapter entry point `sllm_basis_backend()`.

A provider adapter must implement that function and return a valid `sllm_backend` connected to an actual local or hosted model runtime. The adapter descriptor, callbacks, and user data must remain alive for the engine's lifetime. No provider SDK, API key, network client, model weights, or fabricated model response is included. Generation requires a real adapter.

## Build integration

Compile the parent C implementation as C11 and its C++ facade as C++17, then link the selected provider adapter and its documented dependencies. Example link shape, assuming the build has produced these illustrative library names:

```sh
cc -std=c11 -I.. -I. 01-chat.c -L.. -lsleela_llm -lsleela_llm_provider -o 01-chat
c++ -std=c++17 -I.. -I. 01-chat.cpp -L.. -lsleela_llm_cpp -lsleela_llm -lsleela_llm_provider -o 01-chat-cpp
```

Use the actual library names and paths from the selected SLeeLa build and provider adapter. The provider adapter is a required link dependency.

## Terminal usage

All cases use this argument shape:

```sh
./01-chat --model local-assistant --max-tokens 300 --temperature 0.4 -- "Explain recursion with a small example."
```

Pass the task input after `--`; quote option values containing spaces. Shared options are `--model`, `--system`, `--context`, `--max-tokens`, `--temperature`, `--top-p`, and `--seed`. `--help` prints usage. The parser bounds input and context buffers and rejects unknown switches or invalid numeric values.

Use `--context "..." ` for reference material in grounded tasks. Context is explicitly labelled as untrusted in the prompt; production applications still need independent data validation and prompt-injection controls.

## Notes

- These are starting points, not production safety policies.
- The model identifier is interpreted by the provider adapter, not assumed to be a filesystem path.
- `--seed` is backend-defined and does not guarantee deterministic output across providers.
- Never place credentials in source code, shell history, or committed configuration.
- The examples use fixed-size introductory buffers; production applications should use appropriate dynamic or streaming input paths.
