/*
 * In-tree FLAC decode backend (dependency-free). See flac_native.h.
 *
 * Decodes the normative core of a FLAC stream to interleaved signed 16-bit PCM:
 * the STREAMINFO metadata block, then one or more audio frames, each carrying
 * CONSTANT / VERBATIM / FIXED subframes with Rice-coded residuals. The decoder
 * is bit-exact for the subframe types it covers and reports a clean
 * SLEELA_CODEC_ERR_UNSUPPORTED / _FORMAT for anything outside that coverage
 * rather than emitting wrong samples.
 *
 * References: the FLAC format specification (Xiph.Org). All structures below
 * (stream marker "fLaC", metadata block header, frame header with its UTF-8
 * coded sample/frame number, subframe header, Rice partitioned residual, and
 * the fixed predictor restoration filters) follow that normative description.
 */

#include "flac_native.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------------------
 * Big-endian bit reader over a byte buffer.
 * -------------------------------------------------------------------------- */
typedef struct bitreader {
    const uint8_t *data;
    size_t         len;      /* total bytes                     */
    size_t         byte;     /* next byte index                 */
    unsigned       bit;      /* bits already consumed in data[byte] (0..7) */
    int            error;    /* set on read past end            */
} bitreader;

static void br_init(bitreader *b, const uint8_t *data, size_t len) {
    b->data = data; b->len = len; b->byte = 0; b->bit = 0; b->error = 0;
}

/* Read a single bit (MSB first). */
static unsigned br_bit(bitreader *b) {
    if (b->byte >= b->len) { b->error = 1; return 0; }
    unsigned v = (b->data[b->byte] >> (7u - b->bit)) & 1u;
    if (++b->bit == 8u) { b->bit = 0; b->byte++; }
    return v;
}

/* Read n bits (n <= 32) as an unsigned big-endian value. */
static uint32_t br_bits(bitreader *b, unsigned n) {
    uint32_t v = 0;
    for (unsigned i = 0; i < n; ++i) v = (v << 1) | br_bit(b);
    return v;
}

/* Read n bits as a two's-complement signed value (n in 1..32). */
static int32_t br_sbits(bitreader *b, unsigned n) {
    uint32_t v = br_bits(b, n);
    if (n == 0) return 0;
    if (v & (1u << (n - 1))) {                 /* sign-extend */
        uint32_t mask = (n >= 32) ? 0xFFFFFFFFu : ~((1u << n) - 1u);
        v |= mask;
    }
    return (int32_t)v;
}

/* Align to the next byte boundary. */
static void br_align(bitreader *b) {
    if (b->bit != 0) { b->bit = 0; b->byte++; }
}

/* Unary count of leading zeros terminated by a 1 bit (for Rice quotient). */
static uint32_t br_unary(bitreader *b) {
    uint32_t q = 0;
    while (!b->error && br_bit(b) == 0) q++;
    return q;
}

/* ----------------------------------------------------------------------------
 * Fixed predictor restoration (orders 0..4), FLAC's normative fixed filters.
 * The residual array is overwritten in place with the restored samples.
 * -------------------------------------------------------------------------- */
static void fixed_restore(int32_t *s, size_t n, unsigned order) {
    switch (order) {
        case 0:
            break;                                   /* residual == samples */
        case 1:
            for (size_t i = 1; i < n; ++i) s[i] += s[i - 1];
            break;
        case 2:
            for (size_t i = 2; i < n; ++i) s[i] += 2 * s[i - 1] - s[i - 2];
            break;
        case 3:
            for (size_t i = 3; i < n; ++i)
                s[i] += 3 * s[i - 1] - 3 * s[i - 2] + s[i - 3];
            break;
        case 4:
            for (size_t i = 4; i < n; ++i)
                s[i] += 4 * s[i - 1] - 6 * s[i - 2] + 4 * s[i - 3] - s[i - 4];
            break;
        default:
            break;
    }
}

/* ----------------------------------------------------------------------------
 * Rice-coded residual (coding methods 0 = 4-bit params, 1 = 5-bit params).
 * Fills residual[pred_order .. block_size) following FLAC partitioning.
 * Returns 0 on success, non-zero on an unsupported/escape case.
 * -------------------------------------------------------------------------- */
static int read_residual(bitreader *b, int32_t *out, unsigned block_size,
                         unsigned pred_order) {
    unsigned method = br_bits(b, 2);
    if (method > 1) return 1;                        /* reserved coding method */
    unsigned param_bits = (method == 0) ? 4u : 5u;
    unsigned escape     = (method == 0) ? 0x0Fu : 0x1Fu;

    unsigned partition_order = br_bits(b, 4);
    unsigned partitions = 1u << partition_order;
    if (block_size % partitions) return 1;           /* must divide evenly */
    unsigned per_part = block_size >> partition_order;
    if (per_part <= pred_order && partition_order == 0) {
        /* first partition must still have room for the warmup samples */
    }

    size_t idx = pred_order;
    for (unsigned p = 0; p < partitions; ++p) {
        unsigned count = (p == 0) ? (per_part - pred_order) : per_part;
        unsigned rice = br_bits(b, param_bits);
        if (rice == escape) {
            /* Escape: `n`-bit raw residuals, n given by the next 5 bits. */
            unsigned nbits = br_bits(b, 5);
            for (unsigned i = 0; i < count; ++i) {
                if (idx >= block_size) return 1;
                out[idx++] = nbits ? br_sbits(b, nbits) : 0;
            }
        } else {
            for (unsigned i = 0; i < count; ++i) {
                if (idx >= block_size) return 1;
                uint32_t q = br_unary(b);
                uint32_t r = br_bits(b, rice);
                uint32_t u = (q << rice) | r;         /* zig-zag folded value */
                int32_t v = (int32_t)(u >> 1);
                if (u & 1u) v = -v - 1;                /* unfold to signed */
                out[idx++] = v;
                if (b->error) return 1;
            }
        }
    }
    return b->error ? 1 : 0;
}

/* ----------------------------------------------------------------------------
 * One subframe for a single channel. `bps` is the effective bit depth for this
 * subframe (may be one more than STREAMINFO for a channel-decorrelated side).
 * Writes `block_size` samples into `out`. Returns 0 on success.
 * -------------------------------------------------------------------------- */
static int read_subframe(bitreader *b, int32_t *out, unsigned block_size,
                         unsigned bps) {
    if (br_bit(b) != 0) return 1;                    /* zero padding bit */
    unsigned type = br_bits(b, 6);

    unsigned wasted = 0;
    if (br_bit(b)) {                                  /* wasted-bits flag */
        wasted = 1 + br_unary(b);
    }
    if (wasted >= bps) return 1;
    unsigned ebps = bps - wasted;

    if (type == 0) {
        /* CONSTANT */
        int32_t v = br_sbits(b, ebps);
        for (unsigned i = 0; i < block_size; ++i) out[i] = v;
    } else if (type == 1) {
        /* VERBATIM */
        for (unsigned i = 0; i < block_size; ++i) out[i] = br_sbits(b, ebps);
    } else if (type >= 8 && type <= 12) {
        /* FIXED predictor, order = type - 8 (0..4). */
        unsigned order = type - 8;
        if (order > block_size) return 1;
        for (unsigned i = 0; i < order; ++i) out[i] = br_sbits(b, ebps); /* warmup */
        if (read_residual(b, out, block_size, order)) return 1;
        fixed_restore(out, block_size, order);
    } else if (type >= 32) {
        /* LPC (type 32..63): adaptive quantized linear prediction. Detected and
         * reported as unsupported rather than mis-decoded. */
        return 2;
    } else {
        return 1;                                     /* reserved subframe type */
    }

    if (wasted) for (unsigned i = 0; i < block_size; ++i) out[i] <<= wasted;
    return 0;
}

/* ----------------------------------------------------------------------------
 * Skip a frame header's UTF-8 coded sample/frame number (1..7 bytes).
 * -------------------------------------------------------------------------- */
static void skip_utf8_coded(bitreader *b, unsigned first) {
    unsigned extra = 0;
    if (first < 0x80u) extra = 0;
    else if ((first & 0xE0u) == 0xC0u) extra = 1;
    else if ((first & 0xF0u) == 0xE0u) extra = 2;
    else if ((first & 0xF8u) == 0xF0u) extra = 3;
    else if ((first & 0xFCu) == 0xF8u) extra = 4;
    else if ((first & 0xFEu) == 0xFCu) extra = 5;
    else extra = 6;
    for (unsigned i = 0; i < extra; ++i) (void)br_bits(b, 8);
}

/* STREAMINFO fields we need. */
typedef struct streaminfo {
    uint32_t sample_rate;
    uint16_t channels;
    uint16_t bits_per_sample;
    uint64_t total_samples;
} streaminfo;

/* ----------------------------------------------------------------------------
 * Decode a full FLAC stream to interleaved 16-bit PCM.
 * -------------------------------------------------------------------------- */
static sleela_codec_result flac_decode(const uint8_t *input, size_t input_len,
                                       sleela_pcm_buffer *out) {
    if (!input || !out) return SLEELA_CODEC_ERR_INVALID;
    if (input_len < 4 || memcmp(input, "fLaC", 4) != 0) return SLEELA_CODEC_ERR_FORMAT;

    bitreader b;
    br_init(&b, input, input_len);
    b.byte = 4;                                       /* past "fLaC" */

    /* --- Metadata blocks: find STREAMINFO (type 0), skip the rest. --- */
    streaminfo si; memset(&si, 0, sizeof(si));
    int have_streaminfo = 0;
    for (;;) {
        unsigned last = br_bit(&b);
        unsigned btype = br_bits(&b, 7);
        uint32_t blen = br_bits(&b, 24);
        if (b.error) return SLEELA_CODEC_ERR_FORMAT;
        if (btype == 0) {                             /* STREAMINFO */
            if (blen < 34) return SLEELA_CODEC_ERR_FORMAT;
            (void)br_bits(&b, 16);                    /* min block size */
            (void)br_bits(&b, 16);                    /* max block size */
            (void)br_bits(&b, 24);                    /* min frame size */
            (void)br_bits(&b, 24);                    /* max frame size */
            si.sample_rate     = br_bits(&b, 20);
            si.channels        = (uint16_t)(br_bits(&b, 3) + 1);
            si.bits_per_sample = (uint16_t)(br_bits(&b, 5) + 1);
            uint32_t hi = br_bits(&b, 4);
            uint32_t lo = br_bits(&b, 32);
            si.total_samples = ((uint64_t)hi << 32) | lo;
            for (unsigned i = 0; i < 16; ++i) (void)br_bits(&b, 8); /* MD5 */
            have_streaminfo = 1;
        } else {
            for (uint32_t i = 0; i < blen; ++i) (void)br_bits(&b, 8);
        }
        if (b.error) return SLEELA_CODEC_ERR_FORMAT;
        if (last) break;
    }
    if (!have_streaminfo) return SLEELA_CODEC_ERR_FORMAT;
    if (si.bits_per_sample != 16) return SLEELA_CODEC_ERR_UNSUPPORTED; /* 16-bit PCM boundary */
    if (si.channels < 1 || si.channels > 8) return SLEELA_CODEC_ERR_UNSUPPORTED;

    /* --- Audio frames. Accumulate interleaved 16-bit samples. --- */
    size_t cap = (si.total_samples ? (size_t)si.total_samples : 4096) * si.channels;
    if (cap == 0) cap = 4096;
    int16_t *pcm = (int16_t *)malloc(cap * sizeof(int16_t));
    if (!pcm) return SLEELA_CODEC_ERR_IO;
    size_t pcm_count = 0;

    int32_t *ch[8];
    for (int i = 0; i < 8; ++i) ch[i] = NULL;
    sleela_codec_result rc = SLEELA_CODEC_OK;

    while (b.byte < b.len) {
        br_align(&b);
        /* Frame sync: 14 bits = 0x3FFE. */
        uint32_t sync = br_bits(&b, 14);
        if (b.error) break;                           /* clean end of stream */
        if (sync != 0x3FFEu) { rc = SLEELA_CODEC_ERR_FORMAT; break; }
        (void)br_bit(&b);                             /* reserved */
        unsigned blocking = br_bit(&b);               /* 0 fixed, 1 variable */
        unsigned bs_code = br_bits(&b, 4);
        unsigned sr_code = br_bits(&b, 4);
        unsigned ch_assign = br_bits(&b, 4);
        (void)br_bits(&b, 3);                         /* sample size code */
        (void)br_bit(&b);                             /* reserved */

        unsigned first = br_bits(&b, 8);              /* UTF-8 coded number */
        skip_utf8_coded(&b, first);
        (void)blocking;

        /* Block size. */
        unsigned block_size;
        if (bs_code == 1) block_size = 192;
        else if (bs_code >= 2 && bs_code <= 5) block_size = 576u << (bs_code - 2);
        else if (bs_code == 6) block_size = br_bits(&b, 8) + 1;
        else if (bs_code == 7) block_size = br_bits(&b, 16) + 1;
        else if (bs_code >= 8 && bs_code <= 15) block_size = 256u << (bs_code - 8);
        else { rc = SLEELA_CODEC_ERR_FORMAT; break; } /* bs_code 0 reserved */

        /* Sample-rate code: consume the "get from header" trailers if present. */
        if (sr_code == 12) (void)br_bits(&b, 8);
        else if (sr_code == 13 || sr_code == 14) (void)br_bits(&b, 16);

        (void)br_bits(&b, 8);                         /* frame header CRC-8 */
        if (b.error) { rc = SLEELA_CODEC_ERR_FORMAT; break; }

        /* Channel assignment: 0..7 = independent channels (count = code+1);
         * 8 = left/side, 9 = right/side, 10 = mid/side (all stereo). */
        unsigned nch, stereo_mode = 0;
        if (ch_assign <= 7) { nch = ch_assign + 1; }
        else if (ch_assign >= 8 && ch_assign <= 10) { nch = 2; stereo_mode = ch_assign; }
        else { rc = SLEELA_CODEC_ERR_FORMAT; break; }
        if (nch != si.channels) { rc = SLEELA_CODEC_ERR_FORMAT; break; }

        for (unsigned c = 0; c < nch; ++c) {
            ch[c] = (int32_t *)malloc(block_size * sizeof(int32_t));
            if (!ch[c]) { rc = SLEELA_CODEC_ERR_IO; break; }
            /* Side channels in a decorrelated stereo frame carry one extra bit. */
            unsigned bps = si.bits_per_sample;
            if ((stereo_mode == 8 && c == 1) ||       /* left/side -> side is ch 1 */
                (stereo_mode == 9 && c == 0) ||       /* right/side -> side is ch 0 */
                (stereo_mode == 10 && c == 1))        /* mid/side -> side is ch 1 */
                bps += 1;
            int sr = read_subframe(&b, ch[c], block_size, bps);
            if (sr != 0) { rc = (sr == 2) ? SLEELA_CODEC_ERR_UNSUPPORTED : SLEELA_CODEC_ERR_FORMAT; break; }
        }
        if (rc != SLEELA_CODEC_OK) { for (unsigned c = 0; c < nch; ++c) { free(ch[c]); ch[c] = NULL; } break; }

        br_align(&b);
        (void)br_bits(&b, 16);                        /* frame footer CRC-16 */

        /* Undo stereo decorrelation into independent L/R. */
        if (stereo_mode == 8) {                        /* left/side */
            for (unsigned i = 0; i < block_size; ++i) ch[1][i] = ch[0][i] - ch[1][i];
        } else if (stereo_mode == 9) {                 /* right/side */
            for (unsigned i = 0; i < block_size; ++i) ch[0][i] = ch[0][i] + ch[1][i];
        } else if (stereo_mode == 10) {                /* mid/side */
            for (unsigned i = 0; i < block_size; ++i) {
                int32_t mid = ch[0][i], side = ch[1][i];
                mid = (mid << 1) | (side & 1);
                ch[0][i] = (mid + side) >> 1;
                ch[1][i] = (mid - side) >> 1;
            }
        }

        /* Grow the interleaved buffer if needed, then append this block. */
        size_t need = pcm_count + (size_t)block_size * nch;
        if (need > cap) {
            size_t ncap = cap ? cap : 4096;
            while (ncap < need) ncap *= 2;
            int16_t *np = (int16_t *)realloc(pcm, ncap * sizeof(int16_t));
            if (!np) { rc = SLEELA_CODEC_ERR_IO; for (unsigned c = 0; c < nch; ++c) free(ch[c]); break; }
            pcm = np; cap = ncap;
        }
        for (unsigned i = 0; i < block_size; ++i)
            for (unsigned c = 0; c < nch; ++c) {
                int32_t v = ch[c][i];
                if (v > 32767) v = 32767; else if (v < -32768) v = -32768;
                pcm[pcm_count++] = (int16_t)v;
            }
        for (unsigned c = 0; c < nch; ++c) { free(ch[c]); ch[c] = NULL; }
    }

    if (rc != SLEELA_CODEC_OK) { free(pcm); return rc; }
    if (pcm_count == 0) { free(pcm); return SLEELA_CODEC_ERR_FORMAT; }

    out->samples = pcm;
    out->channels = si.channels;
    out->sample_rate = si.sample_rate ? si.sample_rate : 44100;
    out->frame_count = pcm_count / si.channels;
    return SLEELA_CODEC_OK;
}

static sleela_codec_result flac_encode(const sleela_pcm_buffer *pcm,
                                       uint8_t *output, size_t *inout_len) {
    (void)pcm; (void)output; (void)inout_len;
    return SLEELA_CODEC_ERR_UNSUPPORTED;               /* decode-only backend */
}

static const sleela_codec_backend k_backend = {
    "flac-native (in-tree, CONSTANT/VERBATIM/FIXED)",
    flac_decode,
    flac_encode
};

const sleela_codec_backend *sleela_flac_native_backend(void) { return &k_backend; }

sleela_codec_result sleela_flac_native_register(void) {
    return sleela_codec_backend_register(SLEELA_CODEC_FLAC, &k_backend);
}

sleela_codec_result sleela_flac_native_unregister(void) {
    return sleela_codec_backend_register(SLEELA_CODEC_FLAC, NULL);
}
