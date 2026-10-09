# UTF-4088 (Experimental)

UTF-4088 is a hypothetical character-encoding and symbol-generation experiment. It is **not an existing Unicode encoding** and is not compatible with UTF-8, UTF-16, or UTF-32.

## Representation boundary

The current C++ implementation uses `std::uint64_t` for both `InputState` and `CodePoint`. Its accepted experimental identifier interval is **0x110000 through 0x1FFFFFFFF inclusive** (excluding the Unicode scalar range). This is a 33-bit experimental address space, not a literal 4088-bit integer or a 4088-bit physical encoding. A future wider representation requires a separately specified storage and serialization format.

All public APIs must be deterministic and must not treat raw electrical voltage as character data. Hardware adapters must validate and normalize signals into documented digital values before calling the library.

## Registry contract

The front-end registry contains exactly 16,606 deterministic records, IDs 0–16,605. `generate_frontend_registry()` constructs the records using `UTF4088-RG-1`; `frontend_registry()` exposes a cached instance of the same generated registry. IDs and hashes are registry slots, not proof of a glyph's cultural or linguistic meaning.

The registry generation contract is recorded in `registry_manifest.json`. Every glyph bitmap is 8×12 (96 bits), stored as twelve rows of eight pixels. Generated seed glyphs are placeholders for reproducible experiments. Human-readable meanings and historical claims require explicit, traceable annotation and source provenance.

## Build and test

From this directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

A C++20 compiler and CMake 3.20 or newer are required. The test suite covers code-point boundaries, deterministic registry generation, stable glyph signatures, invalid floating-point samples, fixed-width packing, and directed-edge coalescing. GitHub Actions runs the same build and test commands on Linux, Windows, and macOS.

## Experimental scope

The semantic graph and language-family labels are project-specific experimental metadata. They are not scientific rankings of people, cultures, or languages, and generated bitmaps do not acquire meaning merely through geometry. UTF-4088 has no standards-body approval and must not be advertised as Unicode-compatible.
