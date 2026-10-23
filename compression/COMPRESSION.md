# SLeeLa Compression Standards Registry

**Date:** 2026-10-23

## 1. Purpose

This document establishes the general-purpose data-compression coverage
expected by SLeeLa and separates algorithm identification from actual
compress/decompress implementation. It is the compression counterpart to the
audio-codec registry in `codecs/CODECS.md`.

## 2. Registry

| ID | Standard | Extensions | Primary use | Current handler |
|---|---|---|---|---|
| STORE | Raw / store (identity) | .bin | No compression (fallback) | Native |
| RLE | Run-length encoding (PackBits) | .rle | Simple runs / images | Native |
| HUFFMAN | Canonical Huffman | .huff | Entropy coding | Native |
| LZW | Lempel-Ziv-Welch | .lzw, .Z | Dictionary / legacy | Native |
| DEFLATE | RFC 1951 raw DEFLATE | .deflate | General purpose | Backend |
| ZLIB | RFC 1950 zlib wrapper | .zz | General purpose | Backend |
| GZIP | RFC 1952 gzip wrapper | .gz | File / HTTP transport | Backend |
| BZIP2 | Burrows-Wheeler + Huffman | .bz2 | High-ratio files | Backend |
| LZMA | LZMA | .lzma | High-ratio archives | Backend |
| XZ | .xz container over LZMA2 | .xz | High-ratio archives | Backend |
| ZSTD | Zstandard | .zst | Fast, high-ratio | Backend |
| LZ4 | LZ4 | .lz4 | Very fast | Backend |
| BROTLI | Brotli | .br | Web content | Backend |
| SNAPPY | Snappy (framed) | .sz | Fast, in-memory | Backend |
| ZIP | ZIP archive | .zip | Archive container | Container |

## 3. Capability Semantics

The registry distinguishes:

- **Native** — compress/decompress exists inside the SLeeLa package.
- **Backend** — SLeeLa has a handler contract but needs an approved library
  implementation (zlib, libzstd, liblz4, libbz2, liblzma, brotli, snappy, ...).
- **Recognized** — SLeeLa can identify the format but does not claim
  compress/decompress.
- **Container** — the format packages one or more compressed members but is not
  itself a single compression algorithm.

This distinction is required for reliable capability reporting.

## 4. Implementation Order

1. Establish the common plugin ABI, registry, and the Loader/Unloader/Manager.
2. Ship the native baseline: Store, RLE, Huffman, LZW (lossless, in-package).
3. Add the general-purpose DEFLATE family (DEFLATE, zlib, gzip) via backends.
4. Add the high-ratio family (bzip2, LZMA, xz) via backends.
5. Add the modern fast family (zstd, lz4, brotli, snappy) via backends.
6. Add the ZIP container adapter.
7. Add conformance fixtures for every algorithm actually implemented.

## 5. Security and Correctness

Every compressor/decompressor must:

- validate input sizes before allocation;
- reject malformed or truncated headers;
- bound output sizes and dictionary/window growth;
- detect integer overflow;
- avoid unbounded recursion and decompression bombs;
- fail closed on truncated input;
- report unsupported formats explicitly;
- keep parsing separate from the caller's buffers (two-phase sizing).

The native algorithms honour this: RLE/Huffman/LZW validate magic and bound
every read/write, and decode rejects malformed streams with
`SLEELA_COMPRESSION_ERR_FORMAT`.

## 6. Licensing

Compression support must not imply that a patent, proprietary SDK, or
third-party implementation is included in SLeeLa. Each external backend must be
reviewed for its license and distribution terms before it becomes a bundled
dependency. No third-party libraries are bundled in-tree; backend handlers
dispatch to a host-registered adapter.

## 7. Status

The registry establishes coverage and dispatch contracts, and four native
algorithms are fully implemented in-package with passing tests. Backend handlers
identify their format and fail cleanly until an approved backend adapter is
registered — they never fake a successful (de)compress.
