# Registry 3.0 native record engine

Files:
- sleela_registry_v3.h — public C API and portable record structure;
- sleela_registry_v3.c — record storage, validation, SLREG 2 persistence, and SLREG 1 migration;
- sleela_registry_v3.hpp — C++17 RAII facade;
- sleela-registry3.c — administrative CLI.

## Capacity and identity

A Registry instance receives a stable 32-hex registry ID and a configured record capacity. The default is 4096 records, and the implementation caps a caller-supplied capacity at 1,000,000. Each record has a stable 32-hex ID. The helper derives an ID from a seed using SHA-256 and takes the first 128 bits; collisions are rejected by insertion checks. Callers may also supply an ID.

Namespaces and record kinds are lowercase identifiers. Human-readable names and aliases can change independently of record identity. Alias uniqueness is enforced within a namespace. Relationships use typed labels and target record IDs; targets must exist when the relationship is written and when a file is loaded.

## SLREG 2 envelope

SLREG 2 is a deterministic line format. Values are percent-escaped UTF-8; record blocks are sorted by record ID; aliases and relationship pairs are sorted during serialization. The format has a registry header, repeated record blocks, and an explicit end marker.

A save writes a sibling temporary file, flushes it, synchronizes the file descriptor, and then replaces the target. Load parses into a temporary registry and swaps it into the live object only after all records and relationships validate. Invalid input does not partially replace the live registry.

## Migration

SLREG 1 remains the compatibility format used by the existing registry core and install materializer. The explicit migration command writes a separate SLREG 2 destination. It creates:
- one compatibility record for each legacy key;
- one typed setting record per legacy value;
- one provenance record for each legacy key provenance row;
- belongs_to relationships from setting/provenance records to their parent key.

Stable IDs are derived from a fixed domain-separated seed containing the legacy path and name. The old seven provenance fields are copied verbatim. The old source file is not overwritten.

The migration preserves legacy key/value/provenance content and hierarchy references. SLREG 1 strings are unescaped legacy text, not arbitrary binary data; the migration rejects inputs that exceed the new bounded field sizes or are malformed.

## Modium

The Modium registration function creates a catalog record from metadata supplied by an already-inspected Modium resource. It enforces the 256 MiB to 64 GiB bounds and requires a device seal for removable-volume records. It does not discover, mount, claim, format, resize, read, or write any storage.

## Build and tests

From the repository root:
- make registry
- make registry-test

Or use CMake:
- cmake -S registry -B build/registry-v3 -DBUILD_TESTING=ON
- cmake --build build/registry-v3 --config Release
- ctest --test-dir build/registry-v3 -C Release --output-on-failure

The CI matrix runs the C and C++ tests on Linux, macOS, and Windows. Tests cover deterministic persistence, record capacity, alias collisions, relationships, revision conflicts, lifecycle restrictions, Modium registration constraints, and a SLREG 1 migration fixture.
