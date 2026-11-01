/*
 * Test the in-tree FLAC decode backend end to end.
 *
 * Builds real FLAC streams byte by byte (a bit writer mirroring the format
 * spec), registers the backend through the manager's backend registry, decodes
 * through the public manager path, and asserts the PCM is bit-exact. Covers:
 *   1. a VERBATIM stereo subframe  -> exact 16-bit samples, stereo interleave;
 *   2. a FIXED order-0 mono subframe with a Rice-coded residual (coding method
 *      0) -> exercises the unary+binary Rice decoder and the fixed filter;
 *   3. an LPC subframe              -> reported UNSUPPORTED, never mis-decoded;
 *   4. honest dispatch: with the backend cleared, decode is UNSUPPORTED again.
 */

#include "sleela_codec_manager.h"
#include "sleela_codec_plugin.h"
#include "flac_native.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Minimal big-endian bit writer ---- */
typedef struct bw { uint8_t *buf; size_t cap, byte; unsigned bit; } bw;
static void bw_init(bw *w) { w->cap = 256; w->buf = malloc(w->cap); w->byte = 0; w->bit = 0; w->buf[0] = 0; }
static void bw_bit(bw *w, unsigned v) {
    if (w->byte + 1 >= w->cap) { w->cap *= 2; w->buf = realloc(w->buf, w->cap); }
    if (v & 1u) w->buf[w->byte] |= (uint8_t)(1u << (7u - w->bit));
    if (++w->bit == 8u) { w->bit = 0; w->buf[++w->byte] = 0; }
}
static void bw_bits(bw *w, uint32_t v, unsigned n) { for (unsigned i = 0; i < n; ++i) bw_bit(w, (v >> (n - 1 - i)) & 1u); }
static void bw_sbits(bw *w, int32_t v, unsigned n) { bw_bits(w, (uint32_t)v & ((n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1u)), n); }
static void bw_align(bw *w) { while (w->bit != 0) bw_bit(w, 0); }
static size_t bw_len(bw *w) { return w->byte + (w->bit ? 1 : 0); }

/* Rice-encode one signed value with parameter k (zig-zag + unary quotient). */
static void bw_rice(bw *w, int32_t v, unsigned k) {
    uint32_t u = (v >= 0) ? ((uint32_t)v << 1) : (((uint32_t)(-v) << 1) - 1u);
    uint32_t q = u >> k;
    for (uint32_t i = 0; i < q; ++i) bw_bit(w, 0);
    bw_bit(w, 1);
    bw_bits(w, u & ((1u << k) - 1u), k);
}

/* Write "fLaC" + a STREAMINFO metadata block (last block) for the given shape. */
static void write_header(bw *w, uint32_t rate, unsigned channels, unsigned bps, uint64_t total) {
    for (int i = 0; i < 4; ++i) bw_bits(w, (uint32_t)"fLaC"[i], 8);
    bw_bit(w, 1);                 /* last metadata block */
    bw_bits(w, 0, 7);             /* type 0 = STREAMINFO */
    bw_bits(w, 34, 24);           /* block length */
    bw_bits(w, 16, 16);           /* min block size */
    bw_bits(w, 65535, 16);        /* max block size */
    bw_bits(w, 0, 24);            /* min frame size */
    bw_bits(w, 0, 24);            /* max frame size */
    bw_bits(w, rate, 20);
    bw_bits(w, channels - 1, 3);
    bw_bits(w, bps - 1, 5);
    bw_bits(w, (uint32_t)(total >> 32), 4);
    bw_bits(w, (uint32_t)(total & 0xFFFFFFFFu), 32);
    for (int i = 0; i < 16; ++i) bw_bits(w, 0, 8); /* MD5 */
}

/* Common frame header for a fixed-blocksize frame, through the UTF-8 coded
 * number. The block-size / sample-rate trailers (when bs_code/sr_code demand
 * them) and the CRC-8 are written by the caller AFTER this, in that order, so
 * the byte layout matches the decoder's read order. */
static void write_frame_header(bw *w, unsigned bs_code, unsigned sr_code, unsigned ch_assign, unsigned frame_num) {
    bw_bits(w, 0x3FFE, 14);       /* sync */
    bw_bit(w, 0);                 /* reserved */
    bw_bit(w, 0);                 /* fixed block size */
    bw_bits(w, bs_code, 4);
    bw_bits(w, sr_code, 4);
    bw_bits(w, ch_assign, 4);
    bw_bits(w, 4, 3);             /* sample size code (4 = 16 bit; decoder ignores) */
    bw_bit(w, 0);                 /* reserved */
    bw_bits(w, frame_num, 8);     /* UTF-8 coded number (<0x80 -> 1 byte) */
}

int main(void) {
    sleela_codec_manager *m = sleela_codec_manager_create();
    assert(m);
    assert(sleela_codec_manager_load(m, SLEELA_CODEC_FLAC) == SLEELA_CODEC_OK);

    /* Backend absent -> clean UNSUPPORTED. */
    sleela_pcm_buffer pcm;
    const uint8_t marker[4] = { 'f','L','a','C' };
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, marker, 4, &pcm) == SLEELA_CODEC_ERR_UNSUPPORTED);

    /* Register the real in-tree backend. */
    assert(sleela_flac_native_register() == SLEELA_CODEC_OK);

    /* ---------- (1) VERBATIM stereo, 4 frames of samples ---------- */
    {
        const int16_t L[4] = { 1000, -2000, 32767, -32768 };
        const int16_t R[4] = { -1,   2,     -3,    4 };
        bw w; bw_init(&w);
        write_header(&w, 44100, 2, 16, 4);
        /* block size code 7 -> next 16 bits = blocksize-1; use 4 samples. */
        write_frame_header(&w, 7, 9 /*44.1k*/, 1 /*2 independent ch*/, 0);
        bw_bits(&w, 4 - 1, 16);                 /* blocksize-1 for bs_code 7 */
        bw_bits(&w, 0, 8);                       /* CRC-8 (decoder consumes, does not verify) */
        /* subframe L: header 0 + VERBATIM(1) + no wasted, then 4 x 16-bit. */
        bw_bit(&w, 0); bw_bits(&w, 1, 6); bw_bit(&w, 0);
        for (int i = 0; i < 4; ++i) bw_sbits(&w, L[i], 16);
        /* subframe R */
        bw_bit(&w, 0); bw_bits(&w, 1, 6); bw_bit(&w, 0);
        for (int i = 0; i < 4; ++i) bw_sbits(&w, R[i], 16);
        bw_align(&w);
        bw_bits(&w, 0, 16);                      /* frame CRC-16 */

        assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, w.buf, bw_len(&w), &pcm) == SLEELA_CODEC_OK);
        assert(pcm.channels == 2);
        assert(pcm.sample_rate == 44100);
        assert(pcm.frame_count == 4);
        for (int i = 0; i < 4; ++i) {
            assert(pcm.samples[i * 2 + 0] == L[i]);   /* interleaved L */
            assert(pcm.samples[i * 2 + 1] == R[i]);   /* interleaved R */
        }
        printf("  verbatim-stereo: 4 frames, bit-exact L/R OK\n");
        free(w.buf);
    }

    /* ---------- (2) FIXED order-0 mono, Rice residual (method 0, k=2) ---------- */
    {
        const int16_t S[6] = { 5, -3, 7, -1, 0, 2 };    /* order 0: residual == samples */
        bw w; bw_init(&w);
        write_header(&w, 48000, 1, 16, 6);
        write_frame_header(&w, 7, 10 /*48k*/, 0 /*1 ch*/, 0);
        bw_bits(&w, 6 - 1, 16);                 /* blocksize 6 */
        bw_bits(&w, 0, 8);                       /* CRC-8 */
        /* subframe: header 0 + FIXED order 0 (type 8) + no wasted. */
        bw_bit(&w, 0); bw_bits(&w, 8, 6); bw_bit(&w, 0);
        /* residual: coding method 0 (2 bits), partition order 0 (4 bits),
         * rice param k=2 (4 bits), then 6 Rice codes. */
        bw_bits(&w, 0, 2);
        bw_bits(&w, 0, 4);
        bw_bits(&w, 2, 4);
        for (int i = 0; i < 6; ++i) bw_rice(&w, S[i], 2);
        bw_align(&w);
        bw_bits(&w, 0, 16);

        assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, w.buf, bw_len(&w), &pcm) == SLEELA_CODEC_OK);
        assert(pcm.channels == 1 && pcm.sample_rate == 48000 && pcm.frame_count == 6);
        for (int i = 0; i < 6; ++i) assert(pcm.samples[i] == S[i]);
        printf("  fixed-order0-rice: 6 mono samples, bit-exact OK\n");
        free(w.buf);
    }

    /* ---------- (3) LPC subframe -> UNSUPPORTED, not mis-decoded ---------- */
    {
        bw w; bw_init(&w);
        write_header(&w, 44100, 1, 16, 1);
        write_frame_header(&w, 7, 9, 0, 0);
        bw_bits(&w, 1 - 1, 16);
        bw_bits(&w, 0, 8);                       /* CRC-8 */
        bw_bit(&w, 0); bw_bits(&w, 32, 6); bw_bit(&w, 0);  /* type 32 = LPC order 1 */
        bw_align(&w);
        assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, w.buf, bw_len(&w), &pcm) == SLEELA_CODEC_ERR_UNSUPPORTED);
        printf("  lpc-subframe: reported unsupported (honest) OK\n");
        free(w.buf);
    }

    /* ---------- (4) Clearing the backend restores clean UNSUPPORTED ---------- */
    assert(sleela_flac_native_unregister() == SLEELA_CODEC_OK);
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, marker, 4, &pcm) == SLEELA_CODEC_ERR_UNSUPPORTED);

    sleela_codec_manager_destroy(m);
    printf("flac-native: all tests passed\n");
    return 0;
}
