#ifndef SLEELA_NATIVE_IPC_H
#define SLEELA_NATIVE_IPC_H

/* Portable SLeeLa IPC v1 framing and bounded value codecs.
 * Wire data is explicitly little-endian; never send native structs,
 * pointers, SLValue objects, or VM-owned handles. */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../../os-level-support/loader/native_loader.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(double) == 8, "IPC v1 requires 8-byte binary64 storage");
#endif

#define SL_IPC_HEADER_SIZE 24u
#define SL_IPC_MAX_PAYLOAD (16u * 1024u * 1024u)
#define SL_IPC_VERSION 1u
#define SL_IPC_LIMB_BASE UINT32_C(1000000000)
#define SL_IPC_MAX_LIMBS ((SL_IPC_MAX_PAYLOAD - 9u) / 4u)

typedef enum {
    SL_IPC_REQUEST = 1, SL_IPC_RESPONSE = 2, SL_IPC_CANCEL = 3,
    SL_IPC_HELLO = 4, SL_IPC_HELLO_ACK = 5
} SLIPCMessageKind;

typedef struct {
    uint16_t version, kind;
    uint64_t request_id;
    uint32_t payload_length;
    int32_t status;
} SLIPCHeader;

typedef struct {
    const uint8_t *data;
    size_t length, offset;
} SLIPCCursor;

typedef struct {
    const uint8_t *method;
    uint16_t method_length, argument_count;
    SLIPCCursor arguments;
} SLIPCRequestView;

/* Canonical base-1e9 limbs, least-significant first. Sign is +1 or -1;
 * zero is represented by exactly one zero limb with sign +1. */
typedef struct {
    int8_t sign;
    uint32_t count;
    const uint32_t *limbs;
} SLIPCLimbs;

static inline void sl_ipc_put16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8);
}
static inline void sl_ipc_put32(uint8_t *p, uint32_t v) {
    unsigned i; for (i = 0; i < 4; ++i) p[i] = (uint8_t)(v >> (8u * i));
}
static inline void sl_ipc_put64(uint8_t *p, uint64_t v) {
    unsigned i; for (i = 0; i < 8; ++i) p[i] = (uint8_t)(v >> (8u * i));
}
static inline uint16_t sl_ipc_get16(const uint8_t *p) {
    return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}
static inline uint32_t sl_ipc_get32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
static inline uint64_t sl_ipc_get64(const uint8_t *p) {
    uint64_t v = 0; unsigned i;
    for (i = 0; i < 8; ++i) v |= ((uint64_t)p[i]) << (8u * i);
    return v;
}

static inline int sl_ipc_encode_header(const SLIPCHeader *h, uint8_t *out, size_t cap) {
    if (!h || !out || cap < SL_IPC_HEADER_SIZE || h->version != SL_IPC_VERSION ||
        h->kind < SL_IPC_REQUEST || h->kind > SL_IPC_HELLO_ACK ||
        h->payload_length > SL_IPC_MAX_PAYLOAD) return -1;
    out[0] = 'S'; out[1] = 'L'; out[2] = 'I'; out[3] = 'P';
    sl_ipc_put16(out + 4, h->version); sl_ipc_put16(out + 6, h->kind);
    sl_ipc_put64(out + 8, h->request_id); sl_ipc_put32(out + 16, h->payload_length);
    sl_ipc_put32(out + 20, (uint32_t)h->status);
    return (int)SL_IPC_HEADER_SIZE;
}
static inline int sl_ipc_decode_header(const uint8_t *in, size_t n, SLIPCHeader *h) {
    if (!in || !h || n < SL_IPC_HEADER_SIZE || memcmp(in, "SLIP", 4) != 0) return -1;
    h->version = sl_ipc_get16(in + 4); h->kind = sl_ipc_get16(in + 6);
    h->request_id = sl_ipc_get64(in + 8); h->payload_length = sl_ipc_get32(in + 16);
    h->status = (int32_t)sl_ipc_get32(in + 20);
    return (h->version == SL_IPC_VERSION && h->kind >= SL_IPC_REQUEST &&
            h->kind <= SL_IPC_HELLO_ACK && h->payload_length <= SL_IPC_MAX_PAYLOAD)
           ? (int)SL_IPC_HEADER_SIZE : -1;
}

/* Scalar values reuse the existing typed ABI tags. Decoded UTF-8 is a borrowed
 * byte span; callers must validate UTF-8 and copy before reusing the frame. */
static inline int sl_ipc_encode_value(const SLNativeValue *v, uint8_t *out,
                                      size_t cap, size_t *used) {
    size_t n; uint64_t bits = 0;
    if (!v || !out || !used) return -1;
    switch (v->tag) {
    case SL_NATIVE_VALUE_NULL: n = 1; break;
    case SL_NATIVE_VALUE_BOOL:
        if (v->as.boolean != 0 && v->as.boolean != 1) return -1;
        n = 2; break;
    case SL_NATIVE_VALUE_I64: n = 9; break;
    case SL_NATIVE_VALUE_F64: n = 9; memcpy(&bits, &v->as.f64, 8); break;
    case SL_NATIVE_VALUE_UTF8:
        if (v->as.utf8.length > SL_IPC_MAX_PAYLOAD ||
            (!v->as.utf8.data && v->as.utf8.length)) return -1;
        n = 5u + (size_t)v->as.utf8.length; break;
    default: return -1;
    }
    if (n > cap || n > SL_IPC_MAX_PAYLOAD) return -1;
    out[0] = (uint8_t)v->tag;
    if (v->tag == SL_NATIVE_VALUE_BOOL) out[1] = (uint8_t)v->as.boolean;
    else if (v->tag == SL_NATIVE_VALUE_I64) sl_ipc_put64(out + 1, (uint64_t)v->as.i64);
    else if (v->tag == SL_NATIVE_VALUE_F64) sl_ipc_put64(out + 1, bits);
    else if (v->tag == SL_NATIVE_VALUE_UTF8) {
        sl_ipc_put32(out + 1, v->as.utf8.length);
        if (v->as.utf8.length) memcpy(out + 5, v->as.utf8.data, v->as.utf8.length);
    }
    *used = n;
    return 0;
}

static inline int sl_ipc_decode_value(SLIPCCursor *c, SLNativeValue *v) {
    size_t n; uint64_t bits;
    if (!c || !v || !c->data || c->offset > c->length || c->offset >= c->length) return -1;
    memset(v, 0, sizeof(*v)); v->tag = c->data[c->offset];
    switch (v->tag) {
    case SL_NATIVE_VALUE_NULL: n = 1; break;
    case SL_NATIVE_VALUE_BOOL:
        n = 2;
        if (c->length - c->offset < n || c->data[c->offset + 1] > 1) return -1;
        v->as.boolean = c->data[c->offset + 1]; break;
    case SL_NATIVE_VALUE_I64:
        n = 9; if (c->length - c->offset < n) return -1;
        v->as.i64 = (int64_t)sl_ipc_get64(c->data + c->offset + 1); break;
    case SL_NATIVE_VALUE_F64:
        n = 9; if (c->length - c->offset < n) return -1;
        bits = sl_ipc_get64(c->data + c->offset + 1); memcpy(&v->as.f64, &bits, 8); break;
    case SL_NATIVE_VALUE_UTF8:
        if (c->length - c->offset < 5) return -1;
        v->as.utf8.length = sl_ipc_get32(c->data + c->offset + 1);
        if (v->as.utf8.length > SL_IPC_MAX_PAYLOAD) return -1;
        n = 5u + (size_t)v->as.utf8.length;
        if (n > c->length - c->offset) return -1;
        v->as.utf8.data = c->data + c->offset + 5; break;
    default: return -1;
    }
    if (n > c->length - c->offset) return -1;
    c->offset += n;
    return 0;
}

/* Request payload: u16 method length, method bytes, u16 argc, scalar values.
 * The language boundary performs UTF-8 validation of the method identifier. */
static inline int sl_ipc_encode_request(const uint8_t *method, uint16_t method_len,
                                       const SLNativeValue *argv, uint16_t argc,
                                       uint8_t *out, size_t cap, size_t *written) {
    size_t off = 0, n; uint16_t i;
    if (!method || !method_len || method_len > 1024 || (!argv && argc) ||
        !out || !written || cap > SL_IPC_MAX_PAYLOAD) return -1;
    if (cap < 4u + (size_t)method_len) return -1;
    for (i = 0; i < method_len; ++i) if (method[i] == 0) return -1;
    sl_ipc_put16(out, method_len); off = 2;
    memcpy(out + off, method, method_len); off += method_len;
    sl_ipc_put16(out + off, argc); off += 2;
    for (i = 0; i < argc; ++i) {
        if (off > cap || sl_ipc_encode_value(&argv[i], out + off, cap - off, &n)) return -1;
        off += n;
    }
    if (off > SL_IPC_MAX_PAYLOAD) return -1;
    *written = off;
    return 0;
}
static inline int sl_ipc_decode_request(const uint8_t *payload, size_t length,
                                        SLIPCRequestView *out) {
    uint16_t ml, argc, i; size_t start; SLIPCCursor scan; SLNativeValue value;
    if (!payload || !out || length < 4 || length > SL_IPC_MAX_PAYLOAD) return -1;
    ml = sl_ipc_get16(payload);
    if (!ml || ml > 1024 || (size_t)ml > length - 4) return -1;
    for (i = 0; i < ml; ++i) if (payload[2 + i] == 0) return -1;
    argc = sl_ipc_get16(payload + 2 + ml); start = 4u + (size_t)ml;
    scan.data = payload; scan.length = length; scan.offset = start;
    for (i = 0; i < argc; ++i) if (sl_ipc_decode_value(&scan, &value)) return -1;
    if (scan.offset != length) return -1;
    out->method = payload + 2; out->method_length = ml; out->argument_count = argc;
    out->arguments.data = payload; out->arguments.length = length; out->arguments.offset = start;
    return 0;
}

/* Limb payload: sign byte (1 positive, 2 negative), u32 count, then count
 * u32 base-1e9 limbs, least-significant first. */
static inline int sl_ipc_encode_limbs(const SLIPCLimbs *v, uint8_t *out,
                                      size_t cap, size_t *written) {
    uint32_t i;
    size_t need;
    if (!v || !out || !written || !v->limbs || !v->count ||
        v->count > SL_IPC_MAX_LIMBS || (v->sign != 1 && v->sign != -1)) return -1;
    need = 5u + (size_t)v->count * 4u;
    if (need > cap || need > SL_IPC_MAX_PAYLOAD) return -1;
    for (i = 0; i < v->count; ++i) if (v->limbs[i] >= SL_IPC_LIMB_BASE) return -1;
    if (v->count > 1 && v->limbs[v->count - 1] == 0) return -1;
    if (v->count == 1 && v->limbs[0] == 0 && v->sign != 1) return -1;
    out[0] = (uint8_t)(v->sign == 1 ? 1 : 2);
    sl_ipc_put32(out + 1, v->count);
    for (i = 0; i < v->count; ++i) sl_ipc_put32(out + 5u + (size_t)i * 4u, v->limbs[i]);
    *written = need;
    return 0;
}
static inline int sl_ipc_decode_limbs(const uint8_t *data, size_t length,
                                      SLIPCLimbs *out, uint32_t *storage,
                                      uint32_t storage_capacity) {
    uint32_t count, i; size_t need;
    if (!data || !out || !storage || length < 9 || length > SL_IPC_MAX_PAYLOAD) return -1;
    count = sl_ipc_get32(data + 1);
    if (!count || count > SL_IPC_MAX_LIMBS || count > storage_capacity) return -1;
    need = 5u + (size_t)count * 4u;
    if (need != length || (data[0] != 1 && data[0] != 2)) return -1;
    for (i = 0; i < count; ++i) {
        storage[i] = sl_ipc_get32(data + 5u + (size_t)i * 4u);
        if (storage[i] >= SL_IPC_LIMB_BASE) return -1;
    }
    if ((count > 1 && storage[count - 1] == 0) ||
        (count == 1 && storage[0] == 0 && data[0] != 1)) return -1;
    out->sign = (int8_t)(data[0] == 1 ? 1 : -1);
    out->count = count; out->limbs = (const uint8_t *)storage == data ? storage : storage;
    return 0;
}
#endif
