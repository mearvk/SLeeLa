# Artificial Intelligence

This directory contains SLeeLa's native artificial-intelligence APIs, integrations, examples, and supporting documentation.

## Categories

| Category | Purpose | Native API |
|---|---|---|
| [Neural Networks](neural-nets/) | CPU feed-forward networks, inference, and training foundations | C11 and C++17 |\n| [Character Recognition](character-recognition/) | Clean-glyph recognition and reviewable document transcription for European scripts | C11 |
| [Large Language Models](large-language-models/) | Provider-neutral model lifecycle, data ingestion, vocabulary, training, and streaming generation | C11 and C++17 |

## Source conventions

Each category provides public C interfaces and implementations, plus C++ interfaces and implementations where appropriate. Public interfaces use explicit ABI versions, opaque handles where appropriate, documented ownership, and status/error reporting.

Implementations should remain portable across supported SLeeLa platforms. Optional accelerators and provider SDKs belong behind category APIs rather than becoming mandatory dependencies of the core interface.

## Security and compatibility

Do not commit credentials, private model weights, or user data. Document model licenses, dependencies, resource requirements, network behavior, and platform constraints. Keep C ABI changes versioned and preserve clear C++ ownership semantics.
