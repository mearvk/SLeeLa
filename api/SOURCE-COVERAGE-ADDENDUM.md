# API Source-Coverage Addendum

The HTML [source-coverage snapshot](SOURCE-COVERAGE.html) was generated from an earlier Git tree and no repository generator for that report was found during this audit. Its embedded source-file counts and commit identifier therefore must **not** be read as counts for the current branch head.

This addendum records the API documentation and test files added during the language/runtime API audit. It supplements the older snapshot; it does not pretend to be a regenerated exhaustive inventory.

## Language and runtime documentation

- [Language API README](language/README.md)
- [Syntax version matrix](language/SYNTAX-VERSION-MATRIX.md)
- [System namespace API](language/system-namespace/API.md)
- [Verified system references](language/system-namespace/REFERENCES.md)
- [Runtime GC README](runtime/gc/README.md)
- [GC-hint API](runtime/gc/GC-HINT-API.md)
- [GC semantics](runtime/gc/SEMANTICS.md)
- [GC test matrix](runtime/gc/TEST-MATRIX.md)

## Module navigation and completeness

- [Module completeness matrix](MODULE-COMPLETENESS.md)
- [Posting API README](posting/README.md)
- [Listener API README](listener/README.md)
- [Router API README](router/README.md)
- [Nordshrift API README](nordshrift/README.md)

## Executable API contract test

- [Posting/listener/router contract tests](posting/tests/test_api_contracts.c)
- Build owner: [aggregate posting Makefile](posting/Makefile)
- Run from `api/posting/`: `make test`

## Regeneration requirement

When a trustworthy generator is added or located, regenerate `SOURCE-COVERAGE.html` from the exact target branch tree and update its commit identifier, file counts, sizes, and links together. Do not update only the embedded commit hash or count by hand.
