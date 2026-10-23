/*
 * Compression Manager test: load -> use -> unload, across native and backend
 * algorithms. Exercises the Loader/Unloader/Management Controller that SLeeLa
 * calls, round-trips every native algorithm losslessly, and confirms a backend
 * algorithm is present/resolvable yet fails cleanly until a backend is wired.
 */

#include "sleela_compression_manager.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Round-trip `input` through the native algorithm `id` and assert the output
 * equals the input exactly (lossless). */
static void roundtrip(sleela_compression_manager *m, sleela_compressor_id id,
                      const uint8_t *input, size_t len) {
    assert(sleela_compression_manager_load(m, id) == SLEELA_COMPRESSION_OK);

    sleela_byte_buffer packed;
    assert(sleela_compression_manager_compress(m, id, input, len, &packed) == SLEELA_COMPRESSION_OK);

    /* Copy the compressed bytes out before decompress reuses the instance buffer. */
    uint8_t *tmp = (uint8_t *)malloc(packed.len ? packed.len : 1);
    memcpy(tmp, packed.data, packed.len);
    size_t plen = packed.len;

    sleela_byte_buffer out;
    assert(sleela_compression_manager_decompress(m, id, tmp, plen, &out) == SLEELA_COMPRESSION_OK);
    assert(out.len == len);
    assert(memcmp(out.data, input, len) == 0);   /* lossless */

    free(tmp);
}

/* A stand-in backend adapter: the shape a real zlib/libzstd adapter takes. */
static int g_fake_gzip_calls = 0;
static sleela_compression_result fake_gzip_compress(const uint8_t *in, size_t len,
                                                    sleela_byte_buffer *out) {
    (void)in; (void)len; (void)out;
    g_fake_gzip_calls++;
    return SLEELA_COMPRESSION_OK;
}
static const sleela_compression_backend g_fake_gzip = { "fake-gzip", fake_gzip_compress, NULL };

int main(void) {
    sleela_compression_manager *m = sleela_compression_manager_create();
    assert(m != NULL);
    assert(sleela_compression_manager_count(m) == SLEELA_COMPRESSION_COUNT);

    /* Nothing is loaded until asked. */
    assert(sleela_compression_manager_is_loaded(m, SLEELA_COMPRESSION_RLE) == 0);

    /* --- Idempotent load. */
    assert(sleela_compression_manager_load(m, SLEELA_COMPRESSION_STORE) == SLEELA_COMPRESSION_OK);
    assert(sleela_compression_manager_is_loaded(m, SLEELA_COMPRESSION_STORE) == 1);
    assert(sleela_compression_manager_load(m, SLEELA_COMPRESSION_STORE) == SLEELA_COMPRESSION_OK);

    /* --- Lossless round-trips through every native algorithm. */
    const uint8_t runs[] = { 'a','a','a','a','a','b','c','c','c','c','d','d' };
    const uint8_t mixed[] = {
        'h','e','l','l','o',' ','w','o','r','l','d',' ',
        'h','e','l','l','o',' ','w','o','r','l','d','!',0,0,0,0,0,1,2,3
    };
    const uint8_t empty[1] = { 0 };

    roundtrip(m, SLEELA_COMPRESSION_STORE,   runs,  sizeof(runs));
    roundtrip(m, SLEELA_COMPRESSION_RLE,     runs,  sizeof(runs));
    roundtrip(m, SLEELA_COMPRESSION_HUFFMAN, runs,  sizeof(runs));
    roundtrip(m, SLEELA_COMPRESSION_LZW,     runs,  sizeof(runs));

    roundtrip(m, SLEELA_COMPRESSION_STORE,   mixed, sizeof(mixed));
    roundtrip(m, SLEELA_COMPRESSION_RLE,     mixed, sizeof(mixed));
    roundtrip(m, SLEELA_COMPRESSION_HUFFMAN, mixed, sizeof(mixed));
    roundtrip(m, SLEELA_COMPRESSION_LZW,     mixed, sizeof(mixed));

    /* Empty input is handled by every native algorithm. */
    roundtrip(m, SLEELA_COMPRESSION_STORE,   empty, 0);
    roundtrip(m, SLEELA_COMPRESSION_RLE,     empty, 0);
    roundtrip(m, SLEELA_COMPRESSION_HUFFMAN, empty, 0);
    roundtrip(m, SLEELA_COMPRESSION_LZW,     empty, 0);

    /* RLE actually shrinks a long run. */
    {
        uint8_t big[1000];
        memset(big, 'Z', sizeof(big));
        assert(sleela_compression_manager_load(m, SLEELA_COMPRESSION_RLE) == SLEELA_COMPRESSION_OK);
        sleela_byte_buffer packed;
        assert(sleela_compression_manager_compress(m, SLEELA_COMPRESSION_RLE, big, sizeof(big), &packed) == SLEELA_COMPRESSION_OK);
        assert(packed.len < sizeof(big));   /* compression happened */
    }

    /* Huffman magic probes back to the Huffman algorithm. */
    {
        assert(sleela_compression_manager_load(m, SLEELA_COMPRESSION_HUFFMAN) == SLEELA_COMPRESSION_OK);
        sleela_byte_buffer packed;
        assert(sleela_compression_manager_compress(m, SLEELA_COMPRESSION_HUFFMAN, mixed, sizeof(mixed), &packed) == SLEELA_COMPRESSION_OK);
        uint8_t head[4]; memcpy(head, packed.data, 4);
        assert(sleela_compression_manager_resolve_probe(m, head, 4) == SLEELA_COMPRESSION_HUFFMAN);
    }

    /* --- A backend algorithm: present and resolvable, compress fails cleanly. */
    const sleela_compressor_handler *gz = sleela_compression_manager_describe(m, SLEELA_COMPRESSION_GZIP);
    assert(gz && gz->state == SLEELA_COMPRESSION_BACKEND);
    assert(sleela_compression_manager_resolve_extension(m, ".gz") == SLEELA_COMPRESSION_GZIP);
    assert(sleela_compression_manager_load(m, SLEELA_COMPRESSION_GZIP) == SLEELA_COMPRESSION_OK);

    sleela_byte_buffer dummy;
    const uint8_t sample[3] = { 0x1F, 0x8B, 0x08 };
    assert(sleela_compression_manager_compress(m, SLEELA_COMPRESSION_GZIP, sample, 3, &dummy) == SLEELA_COMPRESSION_ERR_UNSUPPORTED);

    /* Register a backend adapter and confirm the manager dispatches to it. */
    assert(sleela_compression_backend_register(SLEELA_COMPRESSION_GZIP, &g_fake_gzip) == SLEELA_COMPRESSION_OK);
    assert(sleela_compression_manager_compress(m, SLEELA_COMPRESSION_GZIP, sample, 3, &dummy) == SLEELA_COMPRESSION_OK);
    assert(g_fake_gzip_calls == 1);
    assert(sleela_compression_backend_register(SLEELA_COMPRESSION_GZIP, NULL) == SLEELA_COMPRESSION_OK);
    assert(sleela_compression_manager_compress(m, SLEELA_COMPRESSION_GZIP, sample, 3, &dummy) == SLEELA_COMPRESSION_ERR_UNSUPPORTED);

    /* The gzip signature probes back to gzip once loaded. */
    assert(sleela_compression_manager_resolve_probe(m, sample, 3) == SLEELA_COMPRESSION_GZIP);

    /* Using a NOT-loaded algorithm returns ERR_STATE, not a crash. */
    assert(sleela_compression_manager_is_loaded(m, SLEELA_COMPRESSION_ZSTD) == 0);
    assert(sleela_compression_manager_compress(m, SLEELA_COMPRESSION_ZSTD, sample, 3, &dummy) == SLEELA_COMPRESSION_ERR_STATE);

    /* --- Unload and confirm. */
    assert(sleela_compression_manager_unload(m, SLEELA_COMPRESSION_STORE) == SLEELA_COMPRESSION_OK);
    assert(sleela_compression_manager_is_loaded(m, SLEELA_COMPRESSION_STORE) == 0);
    assert(sleela_compression_manager_unload(m, SLEELA_COMPRESSION_STORE) == SLEELA_COMPRESSION_OK);

    sleela_compression_manager_unload_all(m);
    assert(sleela_compression_manager_is_loaded(m, SLEELA_COMPRESSION_RLE) == 0);

    sleela_compression_manager_destroy(m);
    return 0;
}
