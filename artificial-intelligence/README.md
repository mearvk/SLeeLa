# Artificial Intelligence

This directory contains SLeeLa's native artificial-intelligence APIs, integrations, examples, and supporting documentation.

## Categories

| Category | Purpose | Native API |
|---|---|---|
| [Neural Networks](neural-nets/) | CPU feed-forward networks, inference, and training foundations | C11 and C++17 |
| [Large Language Models](large-language-models/) | Provider-neutral model lifecycle and streaming generation interface | C11 and C++17 |

## Source conventions

Each category provides a public C header and implementation (`.h`, `.c`), plus a C++ interface and implementation (`.hpp`, `.cpp`). Public interfaces use explicit ABI versions, opaque handles where appropriate, documented ownership, and status/error reporting.

Implementations should remain portable across supported SLeeLa platforms. Optional accelerators and provider SDKs belong behind category APIs rather than becoming mandatory dependencies of the core interface.

## Security and compatibility

Do not commit credentials, private model weights, or user data. Document model licenses, dependencies, resource requirements, network behavior, and platform constraints. Keep C ABI changes versioned and preserve clear C++ ownership semantics.
