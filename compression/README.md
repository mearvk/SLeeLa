<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Compression Handlers

**Package:** `compression/`
**Date:** 2026-10-23

This package defines the SLeeLa general-purpose data-compression handler
registry. It provides one place to identify major compression standards,
describe their signatures/extensions, and expose whether a handler is natively
implemented, requires an external library backend, is only recognized, or is a
container.

It is the compression counterpart to `codecs/` (audio): same plugin + manager
architecture, applied to byte streams instead of PCM.

## Handler Classes

- **Native** — SLeeLa contains the algorithm implementation.
- **Backend** — the handler contract exists, but an approved library/backend is
  required for actual compress/decompress.
- **Recognized** — the format can be identified and described, but
  compress/decompress is not claimed.
- **Container** — identifies an archive/container rather than a single
  compression algorithm.

No handler is advertised as fully implemented merely because its name appears in
the registry.

## Major Standards Covered

| Handler | Standard / Format | Typical role | State |
|---|---|---|---|
| Store | Raw / identity | Fallback, baseline | Native |
| RLE | PackBits run-length | Simple runs | Native |
| Huffman | Canonical Huffman | Entropy coding | Native |
| LZW | Lempel-Ziv-Welch | Dictionary / legacy | Native |
| DEFLATE | RFC 1951 | General purpose | Backend |
| zlib | RFC 1950 | General purpose | Backend |
| gzip | RFC 1952 | File / HTTP | Backend |
| bzip2 | BWT + Huffman | High ratio | Backend |
| LZMA | LZMA | High ratio | Backend |
| xz | LZMA2 container | High ratio | Backend |
| Zstandard | zstd | Fast, high ratio | Backend |
| LZ4 | LZ4 frame | Very fast | Backend |
| Brotli | Brotli | Web content | Backend |
| Snappy | Snappy framed | Fast, in-memory | Backend |
| ZIP | ZIP archive | Container | Container |

## Architecture: individual algorithms + a Loader/Manager

Each algorithm is a **self-contained plugin** in its own folder under
`compression/compressors/<name>/`, implementing a small common vtable
(`compression/include/sleela_compressor_plugin.h`): `describe`, `load`,
`unload`, `probe`, `compress`, `decompress`. Native algorithms ship C **and**
C++ (the vtable/glue in `.c`, the byte-level codec in `.cpp`) plus their own
`.md`; backend algorithms ship the C vtable plus a C++ signature probe.

SLeeLa never links or calls an individual algorithm directly. It calls the
**Compression Loader / Unloader / Management Controller**
(`compression/include/sleela_compression_manager.h`), which owns every plugin's
lifecycle:

```
   SLeeLa  ->  Compression Manager (loader/unloader/controller)  ->  individual plugin
                 load / unload / resolve / compress / decompress
```

- `sleela_compression_manager_create()` registers every known algorithm (none loaded).
- `sleela_compression_manager_load(mgr, id)` / `..._unload(mgr, id)` manage lifecycle.
- `..._resolve_extension` / `..._resolve_probe` pick an algorithm from a filename
  or content bytes.
- `..._compress` / `..._decompress` dispatch to the loaded plugin.
- A backend-less algorithm reports its state and returns a clean
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` instead of faking success.

The SLeeLa-facing handle to the Loader is
`lib/compression/SLCompressionLoader.sleela` (runnable demos:
`lib/compression/compression.sleela` and
`lib/compression/compression-pipeline.sleela`).

### Native algorithms (fully implemented in-package, no external library)

- **Store** (`compressors/store/`) — the identity transform; the raw boundary
  every other algorithm is measured against.
- **RLE** (`compressors/rle/`) — PackBits-style run-length encoding, lossless.
- **Huffman** (`compressors/huffman/`) — static canonical Huffman with a
  self-describing header.
- **LZW** (`compressors/lzw/`) — variable-width (9..12 bit) Lempel-Ziv-Welch
  with an explicit CLEAR code.

All four round-trip losslessly and are covered by `tests/test_manager.c`.

### Backend algorithms (contract + probe, pluggable implementation)

DEFLATE, zlib, gzip, bzip2, LZMA, xz, Zstandard, LZ4, Brotli, Snappy are
**Backend** handlers: they identify the format (a C++ signature probe) and
delegate compress/decompress to a **runtime-registered backend adapter**.

```c
/* A host wires an approved library (zlib, libzstd, ...) at startup: */
static const sleela_compression_backend gzip_adapter = { "zlib", gz_comp, gz_decomp };
sleela_compression_backend_register(SLEELA_COMPRESSION_GZIP, &gzip_adapter);
```

With a backend registered, the plugin's compress/decompress dispatch to it; with
none registered, they return `SLEELA_COMPRESSION_ERR_UNSUPPORTED` — never a false
success. (No third-party libraries are bundled in-tree.) ZIP is a **Container**
handler (probe + identification).

## C Handler Registry (metadata)

The static registry is in:

- `compression/include/sleela_compression.h`
- `compression/src/sleela_compression.c`

It provides stable algorithm identifiers, human-readable names, MIME/type and
extension metadata, handler state, compress/decompress capability flags, lookup
by identifier, lookup by file extension, and enumeration of the whole registry.

The registry is metadata and dispatch infrastructure. It does not silently
substitute a system utility or third-party library for an algorithm
implementation.

## Result codes

Shared by every plugin entry point and by the manager
(`sleela_compression_result`):

| Code | Meaning |
|---|---|
| `0` | ok |
| `-1` | unsupported (backend not present) |
| `-2` | invalid arguments |
| `-3` | input is not this algorithm's format |
| `-4` | I/O or allocation failure |
| `-5` | algorithm not loaded |
| `-6` | output buffer too small |

## Build and test

```sh
# from compression/
make          # builds build/libsleela_compression.a
make test     # builds and runs the registry + manager tests
```

## Completion Policy

A handler becomes **Native** only when the implementation, validation, tests,
and build integration are present. A **Backend** handler may be wired to a
supported library later; until then, callers can inspect its state and fail
cleanly instead of receiving a false success.

See `COMPRESSION.md` for the detailed registry and implementation order.
