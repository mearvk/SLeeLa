# Canonical Huffman

**Algorithm id:** `SLEELA_COMPRESSION_HUFFMAN` · **State:** Native · **Extensions:** `.huff` · **MIME:** `application/x-huffman`

Static canonical Huffman coding. The encoder counts symbol frequencies, builds a
Huffman tree, caps code lengths at 32 bits, and emits an MSB-first bitstream
behind a self-describing header:

```
magic    "SHF1"        (4 bytes)
orig_len u64 LE        (8 bytes)
lengths  256 bytes     (code length per symbol; 0 = absent)
payload  canonical-Huffman bitstream of orig_len symbols
```

The decoder rebuilds the identical canonical codes from the stored length table
alone, so the codes themselves are never transmitted.

## Files

| File | Language | Role |
|------|----------|------|
| `huffman.c` | C | Plugin vtable (`sleela_compressor_plugin_huffman`), buffer ownership. |
| `huffman_detail.cpp` | C++ | Tree build, canonical coding, bit I/O (`sleela_huffman_encode` / `..._decode`). |
| `huffman_detail.h` | — | C-linkage declaration. |

## Capabilities

- **compress / decompress** — fully in-package, lossless, two-phase sized.
  Decode validates the magic and bounds every bitstream read, rejecting
  malformed/truncated input with `SLEELA_COMPRESSION_ERR_FORMAT`.
- **probe** — recognizes the `"SHF1"` magic.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
