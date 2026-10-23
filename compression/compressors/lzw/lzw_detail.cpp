/*
 * Lempel-Ziv-Welch -- C++ implementation (see lzw_detail.h).
 *
 * Variable-width LZW, 9..12 bits, with an explicit CLEAR code (256) and a
 * dictionary cap of 4096 entries; the dictionary resets when full, keeping
 * memory and code width bounded. A self-describing header carries the original
 * length so decode can size its output exactly. Reads are bounded and the
 * header is validated, matching the codecs/ security posture.
 */

#include "lzw_detail.h"

#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr size_t HDR = 4 + 8;
const uint8_t MAGIC[4] = { 'S', 'L', 'Z', 'W' };
constexpr int    MIN_BITS   = 9;
constexpr int    MAX_BITS   = 12;
constexpr int    CLEAR_CODE = 256;
constexpr int    FIRST_CODE = 257;        /* first dictionary entry after CLEAR */
constexpr size_t MAX_ENTRIES = 1u << MAX_BITS;

void put_u64le(uint8_t *p, uint64_t v) { for (int i = 0; i < 8; ++i) p[i] = (uint8_t)((v >> (8 * i)) & 0xFF); }
uint64_t get_u64le(const uint8_t *p) { uint64_t v = 0; for (int i = 0; i < 8; ++i) v |= (uint64_t)p[i] << (8 * i); return v; }

int width_for(size_t next_code) {
    int bits = MIN_BITS;
    while (((size_t)1 << bits) < next_code + 1 && bits < MAX_BITS) ++bits;
    return bits;
}

struct BitWriter {
    uint8_t *out; size_t cap; size_t written; uint32_t acc; int nbits; bool overflow;
    BitWriter(uint8_t *o, size_t c) : out(o), cap(c), written(0), acc(0), nbits(0), overflow(false) {}
    void put(uint32_t code, int w) {
        acc = (acc << w) | (code & ((1u << w) - 1));
        nbits += w;
        while (nbits >= 8) {
            nbits -= 8;
            byte((uint8_t)((acc >> nbits) & 0xFF));
        }
    }
    void flush() { if (nbits) { byte((uint8_t)((acc << (8 - nbits)) & 0xFF)); nbits = 0; } }
    void byte(uint8_t v) { if (out) { if (written >= cap) { overflow = true; return; } out[written] = v; } ++written; }
};

struct BitReader {
    const uint8_t *in; size_t len; size_t pos; uint32_t acc; int nbits;
    BitReader(const uint8_t *i, size_t l) : in(i), len(l), pos(0), acc(0), nbits(0) {}
    bool get(int w, uint32_t &out_code) {
        while (nbits < w) {
            if (pos >= len) return false;
            acc = (acc << 8) | in[pos++];
            nbits += 8;
        }
        nbits -= w;
        out_code = (acc >> nbits) & ((1u << w) - 1);
        return true;
    }
};

} // namespace

extern "C" sleela_compression_result sleela_lzw_encode(const uint8_t *input, size_t input_len,
                                                       uint8_t *out, size_t *out_len) {
    if (!out_len) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len && !input) return SLEELA_COMPRESSION_ERR_INVALID;

    /* Two passes: measure by encoding to a scratch vector, then (if out) copy. */
    std::vector<uint8_t> scratch;
    scratch.reserve(input_len / 2 + HDR + 8);
    scratch.resize(HDR);
    std::memcpy(scratch.data(), MAGIC, 4);
    put_u64le(scratch.data() + 4, (uint64_t)input_len);

    BitWriter bw(nullptr, 0);
    /* Redirect BitWriter to the vector via a local lambda sink. */
    std::vector<uint8_t> body;
    {
        std::unordered_map<std::string, int> dict;
        auto reset = [&]() { dict.clear(); for (int i = 0; i < 256; ++i) dict[std::string(1, (char)i)] = i; };
        reset();
        size_t next = FIRST_CODE;

        /* Local bit sink over `body`. */
        uint32_t acc = 0; int nb = 0;
        auto emit = [&](uint32_t code, int w) {
            acc = (acc << w) | (code & ((1u << w) - 1));
            nb += w;
            while (nb >= 8) { nb -= 8; body.push_back((uint8_t)((acc >> nb) & 0xFF)); }
        };

        emit(CLEAR_CODE, width_for(next));
        std::string cur;
        for (size_t i = 0; i < input_len; ++i) {
            std::string nxt = cur + (char)input[i];
            if (dict.count(nxt)) {
                cur = nxt;
            } else {
                emit(dict[cur], width_for(next));
                if (next < MAX_ENTRIES) {
                    dict[nxt] = (int)next++;
                } else {
                    emit(CLEAR_CODE, width_for(next));
                    reset();
                    next = FIRST_CODE;
                }
                cur = std::string(1, (char)input[i]);
            }
        }
        if (!cur.empty()) emit(dict[cur], width_for(next));
        if (nb) body.push_back((uint8_t)((acc << (8 - nb)) & 0xFF));
    }
    (void)bw;

    size_t total = HDR + body.size();
    if (!out) { *out_len = total; return SLEELA_COMPRESSION_OK; }
    if (*out_len < total) { *out_len = total; return SLEELA_COMPRESSION_ERR_BUFFER; }

    std::memcpy(out, scratch.data(), HDR);
    if (!body.empty()) std::memcpy(out + HDR, body.data(), body.size());
    *out_len = total;
    return SLEELA_COMPRESSION_OK;
}

extern "C" sleela_compression_result sleela_lzw_decode(const uint8_t *input, size_t input_len,
                                                       uint8_t *out, size_t *out_len) {
    if (!out_len) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len && !input) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len < HDR || std::memcmp(input, MAGIC, 4) != 0)
        return SLEELA_COMPRESSION_ERR_FORMAT;

    uint64_t orig = get_u64le(input + 4);

    /* Decode to a scratch vector first so we can honour the sizing contract and
     * validate the whole stream before touching the caller's buffer. */
    std::vector<uint8_t> result;
    result.reserve((size_t)orig);

    BitReader br(input + HDR, input_len - HDR);
    std::vector<std::string> dict;
    auto reset = [&]() { dict.clear(); dict.resize(FIRST_CODE); for (int i = 0; i < 256; ++i) dict[i] = std::string(1, (char)i); };
    reset();
    size_t next = FIRST_CODE;

    std::string prev;
    bool have_prev = false;
    while (result.size() < orig) {
        int w = width_for(next);
        uint32_t code;
        if (!br.get(w, code)) return SLEELA_COMPRESSION_ERR_FORMAT; /* truncated */

        if ((int)code == CLEAR_CODE) { reset(); next = FIRST_CODE; have_prev = false; continue; }

        std::string entry;
        if (code < dict.size() && !dict[code].empty()) {
            entry = dict[code];
        } else if (code == next && have_prev) {
            entry = prev + prev[0];                 /* KwKwK special case */
        } else {
            return SLEELA_COMPRESSION_ERR_FORMAT;    /* invalid code */
        }

        result.insert(result.end(), entry.begin(), entry.end());

        if (have_prev && next < MAX_ENTRIES) {
            if (dict.size() <= next) dict.resize(next + 1);
            dict[next++] = prev + entry[0];
        }
        prev = entry;
        have_prev = true;
    }

    if (!out) { *out_len = result.size(); return SLEELA_COMPRESSION_OK; }
    if (*out_len < result.size()) { *out_len = result.size(); return SLEELA_COMPRESSION_ERR_BUFFER; }
    if (!result.empty()) std::memcpy(out, result.data(), result.size());
    *out_len = result.size();
    return SLEELA_COMPRESSION_OK;
}
