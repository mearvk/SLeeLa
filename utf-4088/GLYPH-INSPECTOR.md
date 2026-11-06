# UTF-4088 glyph inspection utility

Build with CMake from this directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Examples:

```sh
./build/utf4088-inspect --list-concepts
./build/utf4088-inspect --concept standard
./build/utf4088-inspect --id 0x200000
```

The concept command displays the provisional concept seed glyph. The identifier
command decodes the V1 input-state identifier and prints a deterministic
procedural preview. That preview uses the C++ seed-glyph routine; it is not
claimed to be byte-identical to the SHA-256 glyph records in the Python
million-entry binary catalogue. The inspector is a diagnostic tool, not a
font renderer or semantic validator.
