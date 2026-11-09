# JDK 28 API Synchronization

This directory contains the SLeeLa equivalents of the documented Java API surface.

## Source of Truth

The synchronizer uses the official Java SE/JDK 28 API specification:

- https://download.java.net/java/early_access/jdk28/docs/api/
- All classes and interfaces index: `allclasses-index.html`

JDK 28 is an early-access specification and may change before final release.

## Non-Overwrite Rule

`tools/jdk28_sync.py` is **additive-only**:

1. It discovers documented API types from the JDK 28 API index.
2. It maps each Java package/type to `/lib/java/<package>/<Type>.sleela`.
3. If a SLeeLa file already exists, it is skipped.
4. Existing source is never replaced.
5. Missing parent directories are created automatically.

Nested Java types retain their binary/documented name in the filename, for example:
`java/util/Map.Entry.sleela`.

## Generate

From the repository root:

```bash
python3 lib/java/tools/jdk28_sync.py --dry-run
python3 lib/java/tools/jdk28_sync.py
```

The script reports the number of API types discovered, new files created, and existing files preserved.

The generated files are SLeeLa source contracts, not copied Java/OpenJDK implementation source. Runtime behavior remains behind the SLeeLa Java conformance boundary.

## Member Signature Catalog

The companion tool, tools/jdk28_signature_sync.py, records member declarations
from each official Javadoc type page inside a marked SLeeLa comment section.
It preserves generic parameters, annotations, modifiers, and throws clauses as
documentation. It does not synthesize executable methods or placeholder bodies.
A generated signature catalog is metadata, not proof that the SLeeLa compiler
can invoke the corresponding Java method.

Commands from the repository root:

    python3 lib/java/tools/jdk28_signature_sync.py --check
    python3 lib/java/tools/jdk28_signature_sync.py --limit 25

The --check mode reports drift without modifying source. The --limit option is
intended for controlled development batches. JDK 28 is an early-access API
baseline; generated metadata should be refreshed deliberately when the upstream
specification changes.

Focused offline parser and inventory regression tests:

    python3 test-suites/java-library-integrity.py

These tests cover nested Javadoc signature markup, module/package path mapping,
non-executable catalog output, idempotent section replacement, and exact
manifest-count validation. They do not require a local JDK or run repository CI.

## Preview APIs

The JDK 28 API documentation includes preview APIs. They are part of the documented API index and are therefore eligible for additive SLeeLa envelopes.

## Verification

After synchronization, the SLeeLa compiler/loader and the Java conformance tests should scan `/lib/java` and validate the generated declarations.
