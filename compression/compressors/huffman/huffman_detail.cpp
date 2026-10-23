/*
 * Canonical static Huffman -- C++ implementation (see huffman_detail.h).
 *
 * Length-limited canonical Huffman. The encoder counts symbol frequencies,
 * builds a Huffman tree, extracts per-symbol code lengths (capped at 32),
 * assigns canonical codes from the length table, and writes an MSB-first
 * bitstream behind a self-describing header. The decoder rebuilds the identical
 * canonical codes from the stored length table alone. Every read is bounded and
 * malformed headers are rejected, matching the codecs/ security posture.
 */

#include "huffman_detail.h"

#include <algorithm>
#include <cstring>
#include <queue>
#include <vector>

namespace {

constexpr size_t HDR = 4 + 8 + 256;   /* magic + orig_len + length table */
const uint8_t MAGIC[4] = { 'S', 'H', 'F', '1' };

void put_u64le(uint8_t *p, uint64_t v) {
    for (int i = 0; i < 8; ++i) p[i] = (uint8_t)((v >> (8 * i)) & 0xFF);
}
uint64_t get_u64le(const uint8_t *p) {
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i) v |= (uint64_t)p[i] << (8 * i);
    return v;
}

/* Derive code lengths (<=32) for each present symbol from its frequency. */
void build_lengths(const uint64_t freq[256], uint8_t lengths[256]) {
    std::memset(lengths, 0, 256);

    struct Node { uint64_t w; int sym; int left; int right; };
    std::vector<Node> nodes;
    auto cmp = [&](int a, int b) { return nodes[a].w > nodes[b].w; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    int present = 0;
    for (int s = 0; s < 256; ++s) {
        if (freq[s]) {
            nodes.push_back({ freq[s], s, -1, -1 });
            pq.push((int)nodes.size() - 1);
            ++present;
        }
    }
    if (present == 0) return;
    if (present == 1) { lengths[nodes[pq.top()].sym] = 1; return; }  /* single symbol */

    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        nodes.push_back({ nodes[a].w + nodes[b].w, -1, a, b });
        pq.push((int)nodes.size() - 1);
    }

    /* Walk the tree to assign depths (code lengths). */
    std::vector<std::pair<int,int>> stack; /* (node, depth) */
    stack.push_back({ pq.top(), 0 });
    while (!stack.empty()) {
        auto [n, d] = stack.back();
        stack.pop_back();
        if (nodes[n].sym >= 0) {
            int len = d ? d : 1;
            lengths[nodes[n].sym] = (uint8_t)std::min(len, 32);
        } else {
            stack.push_back({ nodes[n].left, d + 1 });
            stack.push_back({ nodes[n].right, d + 1 });
        }
    }
}

/* Assign canonical codes from a length table. codes[s] holds the right-aligned
 * code; use lengths[s] bits of it, MSB first. */
void canonical_codes(const uint8_t lengths[256], uint32_t codes[256]) {
    uint32_t count[33] = { 0 };
    for (int s = 0; s < 256; ++s) if (lengths[s]) ++count[lengths[s]];

    uint32_t next[33] = { 0 };
    uint32_t code = 0;
    for (int bits = 1; bits <= 32; ++bits) {
        code = (code + count[bits - 1]) << 1;
        next[bits] = code;
    }
    for (int s = 0; s < 256; ++s)
        if (lengths[s]) codes[s] = next[lengths[s]]++;
}

struct BitWriter {
    uint8_t *out; size_t cap; size_t written; uint8_t acc; int nbits; bool overflow;
    BitWriter(uint8_t *o, size_t c) : out(o), cap(c), written(0), acc(0), nbits(0), overflow(false) {}
    void bit(int b) {
        acc = (uint8_t)((acc << 1) | (b & 1));
        if (++nbits == 8) { byte(acc); acc = 0; nbits = 0; }
    }
    void put_code(uint32_t code, int len) { for (int i = len - 1; i >= 0; --i) bit((code >> i) & 1); }
    void flush() { if (nbits) { acc = (uint8_t)(acc << (8 - nbits)); byte(acc); acc = 0; nbits = 0; } }
    void byte(uint8_t v) {
        if (out) { if (written >= cap) { overflow = true; return; } out[written] = v; }
        ++written;
    }
};

} // namespace

extern "C" sleela_compression_result sleela_huffman_encode(const uint8_t *input, size_t input_len,
                                                           uint8_t *out, size_t *out_len) {
    if (!out_len) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len && !input) return SLEELA_COMPRESSION_ERR_INVALID;

    uint64_t freq[256] = { 0 };
    for (size_t i = 0; i < input_len; ++i) ++freq[input[i]];

    uint8_t lengths[256];
    build_lengths(freq, lengths);
    uint32_t codes[256] = { 0 };
    canonical_codes(lengths, codes);

    /* Compute payload bit count to size the output precisely. */
    uint64_t bits = 0;
    for (size_t i = 0; i < input_len; ++i) bits += lengths[input[i]];
    size_t payload = (size_t)((bits + 7) / 8);
    size_t total = HDR + payload;

    if (!out) { *out_len = total; return SLEELA_COMPRESSION_OK; }
    if (*out_len < total) { *out_len = total; return SLEELA_COMPRESSION_ERR_BUFFER; }

    std::memcpy(out, MAGIC, 4);
    put_u64le(out + 4, (uint64_t)input_len);
    std::memcpy(out + 12, lengths, 256);

    BitWriter bw(out + HDR, payload);
    for (size_t i = 0; i < input_len; ++i) bw.put_code(codes[input[i]], lengths[input[i]]);
    bw.flush();
    if (bw.overflow) return SLEELA_COMPRESSION_ERR_BUFFER;

    *out_len = total;
    return SLEELA_COMPRESSION_OK;
}

extern "C" sleela_compression_result sleela_huffman_decode(const uint8_t *input, size_t input_len,
                                                           uint8_t *out, size_t *out_len) {
    if (!out_len) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len && !input) return SLEELA_COMPRESSION_ERR_INVALID;
    if (input_len < HDR || std::memcmp(input, MAGIC, 4) != 0)
        return SLEELA_COMPRESSION_ERR_FORMAT;

    uint64_t orig = get_u64le(input + 4);
    const uint8_t *lengths = input + 12;

    if (!out) { *out_len = (size_t)orig; return SLEELA_COMPRESSION_OK; }
    if (*out_len < (size_t)orig) { *out_len = (size_t)orig; return SLEELA_COMPRESSION_ERR_BUFFER; }

    if (orig == 0) { *out_len = 0; return SLEELA_COMPRESSION_OK; }

    /* Rebuild canonical codes and a (code,len)->symbol lookup. */
    uint32_t codes[256] = { 0 };
    canonical_codes(lengths, codes);

    const uint8_t *payload = input + HDR;
    size_t payload_len = input_len - HDR;

    size_t out_pos = 0;
    size_t byte_pos = 0;
    int bit_pos = 0;          /* 0 = MSB of current byte */
    uint32_t cur = 0;
    int cur_len = 0;

    while (out_pos < orig) {
        if (byte_pos >= payload_len) return SLEELA_COMPRESSION_ERR_FORMAT; /* truncated */
        int b = (payload[byte_pos] >> (7 - bit_pos)) & 1;
        if (++bit_pos == 8) { bit_pos = 0; ++byte_pos; }

        cur = (cur << 1) | (uint32_t)b;
        ++cur_len;
        if (cur_len > 32) return SLEELA_COMPRESSION_ERR_FORMAT;

        for (int s = 0; s < 256; ++s) {
            if (lengths[s] == cur_len && codes[s] == cur) {
                out[out_pos++] = (uint8_t)s;
                cur = 0;
                cur_len = 0;
                break;
            }
        }
    }

    *out_len = (size_t)orig;
    return SLEELA_COMPRESSION_OK;
}
