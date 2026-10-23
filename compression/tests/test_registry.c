#include "sleela_compression.h"

#include <assert.h>

int main(void) {
    assert(sleela_compression_count() == SLEELA_COMPRESSION_COUNT);

    const sleela_compressor_handler *store = sleela_compression_by_id(SLEELA_COMPRESSION_STORE);
    assert(store != NULL);
    assert(store->state == SLEELA_COMPRESSION_NATIVE);
    assert(store->can_compress && store->can_decompress);

    const sleela_compressor_handler *rle = sleela_compression_by_id(SLEELA_COMPRESSION_RLE);
    assert(rle != NULL && rle->state == SLEELA_COMPRESSION_NATIVE);

    const sleela_compressor_handler *gz = sleela_compression_by_extension(".GZ");
    assert(gz != NULL);
    assert(gz->id == SLEELA_COMPRESSION_GZIP);
    assert(gz->state == SLEELA_COMPRESSION_BACKEND);

    const sleela_compressor_handler *zip = sleela_compression_by_extension(".zip");
    assert(zip != NULL);
    assert(zip->state == SLEELA_COMPRESSION_CONTAINER);

    /* LZW advertises two extensions; both resolve to it. */
    assert(sleela_compression_by_extension(".lzw")->id == SLEELA_COMPRESSION_LZW);
    assert(sleela_compression_by_extension(".Z")->id == SLEELA_COMPRESSION_LZW);

    assert(sleela_compression_by_extension(".not-a-codec") == NULL);
    assert(sleela_compression_at(SLEELA_COMPRESSION_COUNT) == NULL);
    return 0;
}
