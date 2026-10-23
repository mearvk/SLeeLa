#ifndef SLEELA_COMPRESSION_H
#define SLEELA_COMPRESSION_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum sleela_compressor_state {
    SLEELA_COMPRESSION_NATIVE = 0,      /* algorithm implemented in-package            */
    SLEELA_COMPRESSION_BACKEND = 1,     /* contract present; needs an approved library  */
    SLEELA_COMPRESSION_RECOGNIZED = 2,  /* format can be identified, not (de)compressed */
    SLEELA_COMPRESSION_CONTAINER = 3    /* archive/container, not a raw codec           */
} sleela_compressor_state;

typedef enum sleela_compressor_id {
    SLEELA_COMPRESSION_STORE = 0,       /* raw/store: identity (no compression)         */
    SLEELA_COMPRESSION_RLE,             /* run-length encoding                          */
    SLEELA_COMPRESSION_HUFFMAN,         /* canonical Huffman                            */
    SLEELA_COMPRESSION_LZW,             /* Lempel-Ziv-Welch                             */
    SLEELA_COMPRESSION_DEFLATE,         /* RFC 1951 raw DEFLATE                          */
    SLEELA_COMPRESSION_ZLIB,            /* RFC 1950 zlib wrapper                         */
    SLEELA_COMPRESSION_GZIP,            /* RFC 1952 gzip wrapper                         */
    SLEELA_COMPRESSION_BZIP2,           /* Burrows-Wheeler + Huffman                     */
    SLEELA_COMPRESSION_LZMA,            /* LZMA                                          */
    SLEELA_COMPRESSION_XZ,              /* .xz container over LZMA2                       */
    SLEELA_COMPRESSION_ZSTD,            /* Zstandard                                     */
    SLEELA_COMPRESSION_LZ4,             /* LZ4                                           */
    SLEELA_COMPRESSION_BROTLI,          /* Brotli                                        */
    SLEELA_COMPRESSION_SNAPPY,          /* Snappy                                        */
    SLEELA_COMPRESSION_ZIP,             /* ZIP archive container                         */
    SLEELA_COMPRESSION_COUNT
} sleela_compressor_id;

typedef struct sleela_compressor_handler {
    sleela_compressor_id id;
    const char *name;
    const char *mime;
    const char *extensions;
    sleela_compressor_state state;
    int can_compress;
    int can_decompress;
} sleela_compressor_handler;

size_t sleela_compression_count(void);
const sleela_compressor_handler *sleela_compression_at(size_t index);
const sleela_compressor_handler *sleela_compression_by_id(sleela_compressor_id id);
const sleela_compressor_handler *sleela_compression_by_extension(const char *extension);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_COMPRESSION_H */
