/*
 * Run-length encoding -- C++ implementation shared by the RLE plugin.
 *
 * PackBits-style control-byte RLE (see rle_detail.h). Lossless for any input;
 * worst case (no runs) expands by about 1 byte per 128 literal bytes. Both
 * entry points validate sizes and fail closed on truncated input, following the
 * codecs/ security posture (bound every write, reject malformed headers).
 */

#include "rle_detail.h"

namespace {

constexpr size_t MAX_LITERAL = 128;  /* control 0x00..0x7F -> 1..128 literals */
constexpr size_t MAX_REPEAT  = 128;  /* control 0x80..0xFF -> 2..128 repeats  */

/* Length of the maximal run of the identical byte input[i] starting at i. */
size_t run_length(const uint8_t *input, size_t input_len, size_t i) {
    size_t n = 1;
    while (i + n < input_len && input[i + n] == input[i] && n < MAX_REPEAT) ++n;
    return n;
}

} // namespace

extern "C" sleela_compression_result sleela_rle_encode(const uint8_t *input, size_t input_len,
                                                       uint8_t *out, size_t *out_len) {
    if (!out_len) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len && !input) return SLEELA_COMPRESSION_ERR_INVALID;

    const size_t cap = out ? *out_len : 0;
    size_t written = 0;
    size_t i = 0;

    /* Emit one control byte plus its payload, respecting cap when out != NULL. */
    auto put = [&](uint8_t b) -> bool {
        if (out) { if (written >= cap) return false; out[written] = b; }
        ++written;
        return true;
    };

    while (i < input_len) {
        size_t run = run_length(input, input_len, i);
        if (run >= 2) {
            /* Repeat run of `run` copies (2..128): control = 257 - run. */
            if (!put((uint8_t)(257 - run))) return SLEELA_COMPRESSION_ERR_BUFFER;
            if (!put(input[i])) return SLEELA_COMPRESSION_ERR_BUFFER;
            i += run;
        } else {
            /* Gather a literal span until a run of >=2 appears or we hit 128. */
            size_t start = i;
            size_t lit = 0;
            while (i < input_len && lit < MAX_LITERAL) {
                if (run_length(input, input_len, i) >= 2) break;
                ++i;
                ++lit;
            }
            if (!put((uint8_t)(lit - 1))) return SLEELA_COMPRESSION_ERR_BUFFER;
            for (size_t k = 0; k < lit; ++k)
                if (!put(input[start + k])) return SLEELA_COMPRESSION_ERR_BUFFER;
        }
    }

    *out_len = written;
    return SLEELA_COMPRESSION_OK;
}

extern "C" sleela_compression_result sleela_rle_decode(const uint8_t *input, size_t input_len,
                                                       uint8_t *out, size_t *out_len) {
    if (!out_len) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len && !input) return SLEELA_COMPRESSION_ERR_INVALID;

    const size_t cap = out ? *out_len : 0;
    size_t written = 0;
    size_t i = 0;

    auto put = [&](uint8_t b) -> bool {
        if (out) { if (written >= cap) return false; out[written] = b; }
        ++written;
        return true;
    };

    while (i < input_len) {
        uint8_t ctrl = input[i++];
        if (ctrl < MAX_LITERAL) {
            size_t lit = (size_t)ctrl + 1;          /* 1..128 literal bytes */
            if (i + lit > input_len) return SLEELA_COMPRESSION_ERR_FORMAT; /* truncated */
            for (size_t k = 0; k < lit; ++k)
                if (!put(input[i + k])) return SLEELA_COMPRESSION_ERR_BUFFER;
            i += lit;
        } else {
            size_t rep = (size_t)(257 - ctrl);      /* 2..128 copies */
            if (i >= input_len) return SLEELA_COMPRESSION_ERR_FORMAT;      /* truncated */
            uint8_t v = input[i++];
            for (size_t k = 0; k < rep; ++k)
                if (!put(v)) return SLEELA_COMPRESSION_ERR_BUFFER;
        }
    }

    *out_len = written;
    return SLEELA_COMPRESSION_OK;
}
