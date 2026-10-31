# SLeeLa API documentation

The `/api` tree contains versioned API documentation and native API references.

## SLeeLa language API

- [SLeeLa 2.9 — standard-library API](sleela/2.9/README.md): generated HTML reference for the `/lib` tree, including package, class, field, and method documentation.
- [SLeeLa API editions](sleela/README.md): version index.

## Native API

Native C and C++ API references are maintained under [`/api/native`](native/).

## Rebuilding the 2.9 HTML reference

From the repository root, run:

```sh
python3 tools/generate-sleeladoc-api.py --out api/sleela/2.9
```

The generator reads the current `/lib` source tree and extracts the SleelaDoc file headers and comments. To build the latest working documentation site instead of the versioned snapshot, run:

```sh
python3 tools/generate-sleeladoc-api.py
```

The versioned snapshot is committed so it can be browsed directly from the repository without running a generator.
