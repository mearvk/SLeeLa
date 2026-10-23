#include "sleela_compression.h"

#include <ctype.h>
#include <string.h>

#define C(id, name, mime, ext, state, comp, decomp) \
    { id, name, mime, ext, state, comp, decomp }

static const sleela_compressor_handler handlers[SLEELA_COMPRESSION_COUNT] = {
    C(SLEELA_COMPRESSION_STORE, "Store (raw)", "application/octet-stream", ".bin", SLEELA_COMPRESSION_NATIVE, 1, 1),
    C(SLEELA_COMPRESSION_RLE, "Run-Length Encoding", "application/x-rle", ".rle", SLEELA_COMPRESSION_NATIVE, 1, 1),
    C(SLEELA_COMPRESSION_HUFFMAN, "Canonical Huffman", "application/x-huffman", ".huff", SLEELA_COMPRESSION_NATIVE, 1, 1),
    C(SLEELA_COMPRESSION_LZW, "Lempel-Ziv-Welch", "application/x-lzw", ".lzw,.Z", SLEELA_COMPRESSION_NATIVE, 1, 1),
    C(SLEELA_COMPRESSION_DEFLATE, "DEFLATE (RFC 1951)", "application/x-deflate", ".deflate", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_ZLIB, "zlib (RFC 1950)", "application/zlib", ".zz", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_GZIP, "gzip (RFC 1952)", "application/gzip", ".gz", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_BZIP2, "bzip2", "application/x-bzip2", ".bz2", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_LZMA, "LZMA", "application/x-lzma", ".lzma", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_XZ, "xz (LZMA2)", "application/x-xz", ".xz", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_ZSTD, "Zstandard", "application/zstd", ".zst", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_LZ4, "LZ4", "application/x-lz4", ".lz4", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_BROTLI, "Brotli", "application/x-brotli", ".br", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_SNAPPY, "Snappy", "application/x-snappy-framed", ".sz", SLEELA_COMPRESSION_BACKEND, 1, 1),
    C(SLEELA_COMPRESSION_ZIP, "ZIP archive", "application/zip", ".zip", SLEELA_COMPRESSION_CONTAINER, 1, 1)
};

size_t sleela_compression_count(void) {
    return SLEELA_COMPRESSION_COUNT;
}

const sleela_compressor_handler *sleela_compression_at(size_t index) {
    return index < SLEELA_COMPRESSION_COUNT ? &handlers[index] : NULL;
}

const sleela_compressor_handler *sleela_compression_by_id(sleela_compressor_id id) {
    return id >= 0 && id < SLEELA_COMPRESSION_COUNT ? &handlers[id] : NULL;
}

static int extension_equals(const char *candidate, const char *extension) {
    while (*candidate && *extension) {
        if (tolower((unsigned char)*candidate) !=
            tolower((unsigned char)*extension)) return 0;
        ++candidate;
        ++extension;
    }
    return *candidate == '\0' && *extension == '\0';
}

const sleela_compressor_handler *sleela_compression_by_extension(const char *extension) {
    if (!extension || !*extension) return NULL;

    for (size_t i = 0; i < SLEELA_COMPRESSION_COUNT; ++i) {
        const char *p = handlers[i].extensions;
        while (*p) {
            char token[16];
            size_t n = 0;
            while (*p && *p != ',' && n + 1 < sizeof(token))
                token[n++] = *p++;
            token[n] = '\0';
            if (extension_equals(extension, token)) return &handlers[i];
            if (*p == ',') ++p;
        }
    }
    return NULL;
}
