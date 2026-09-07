// ===========================================================================
// stp_wire.c -- STP-0001 framing + flat-map TLV codec (C side).
// ===========================================================================
#include "stp_wire.h"

#include <stdlib.h>
#include <string.h>

// ---- growable buffer -------------------------------------------------------

void stp_buf_init(stp_buf* b) { b->data = NULL; b->len = 0; b->cap = 0; }
void stp_buf_free(stp_buf* b) { free(b->data); b->data = NULL; b->len = b->cap = 0; }

int stp_buf_append(stp_buf* b, const void* p, size_t n) {
    if (b->len + n > b->cap) {
        size_t nc = b->cap ? b->cap * 2 : 64;
        while (nc < b->len + n) nc *= 2;
        uint8_t* nd = (uint8_t*) realloc(b->data, nc);
        if (!nd) return -1;
        b->data = nd; b->cap = nc;
    }
    memcpy(b->data + b->len, p, n);
    b->len += n;
    return 0;
}

// ---- little-endian ---------------------------------------------------------

void stp_put_u16le(uint8_t* a, uint16_t v) { a[0] = (uint8_t) v; a[1] = (uint8_t)(v >> 8); }
void stp_put_u32le(uint8_t* a, uint32_t v) {
    a[0] = (uint8_t) v; a[1] = (uint8_t)(v >> 8); a[2] = (uint8_t)(v >> 16); a[3] = (uint8_t)(v >> 24);
}
uint16_t stp_get_u16le(const uint8_t* a) { return (uint16_t)(a[0] | (a[1] << 8)); }
uint32_t stp_get_u32le(const uint8_t* a) {
    return (uint32_t) a[0] | ((uint32_t) a[1] << 8) | ((uint32_t) a[2] << 16) | ((uint32_t) a[3] << 24);
}
void stp_put_u64le(uint8_t out[8], uint64_t v) {
    for (int i = 0; i < 8; i++) out[i] = (uint8_t)(v >> (8 * i));
}
uint64_t stp_get_u64le(const uint8_t* a) {
    uint64_t v = 0; for (int i = 0; i < 8; i++) v |= ((uint64_t) a[i]) << (8 * i); return v;
}

// ---- header ----------------------------------------------------------------

void stp_header(uint8_t hdr[STP_HEADER_LEN], int type, int flags, uint32_t body_len) {
    hdr[0] = (uint8_t) STP_VERSION;
    hdr[1] = (uint8_t) type;
    stp_put_u16le(hdr + 2, (uint16_t) flags);
    stp_put_u32le(hdr + 4, body_len);
}
void stp_parse_header(const uint8_t hdr[STP_HEADER_LEN],
                      int* version, int* type, int* flags, uint32_t* body_len) {
    if (version)  *version  = hdr[0];
    if (type)     *type     = hdr[1];
    if (flags)    *flags    = stp_get_u16le(hdr + 2);
    if (body_len) *body_len = stp_get_u32le(hdr + 4);
}

// ---- map builder -----------------------------------------------------------

void stp_map_init(stp_map* m) { stp_buf_init(&m->buf); m->count = 0; }
void stp_map_free(stp_map* m) { stp_buf_free(&m->buf); m->count = 0; }

static int put_key(stp_map* m, const char* key) {
    size_t kl = strlen(key);
    if (kl > 255) return -1;
    uint8_t b = (uint8_t) kl;
    if (stp_buf_append(&m->buf, &b, 1) < 0) return -1;
    return stp_buf_append(&m->buf, key, kl);
}

int stp_map_put_bytes(stp_map* m, const char* key, const uint8_t* v, uint32_t n) {
    if (put_key(m, key) < 0) return -1;
    uint8_t vt = STP_VT_BYTES; uint8_t lenb[4];
    if (stp_buf_append(&m->buf, &vt, 1) < 0) return -1;
    stp_put_u32le(lenb, n);
    if (stp_buf_append(&m->buf, lenb, 4) < 0) return -1;
    if (n && stp_buf_append(&m->buf, v, n) < 0) return -1;
    m->count++; return 0;
}
int stp_map_put_text(stp_map* m, const char* key, const char* v) {
    uint32_t n = (uint32_t) strlen(v);
    if (put_key(m, key) < 0) return -1;
    uint8_t vt = STP_VT_TEXT; uint8_t lenb[4];
    if (stp_buf_append(&m->buf, &vt, 1) < 0) return -1;
    stp_put_u32le(lenb, n);
    if (stp_buf_append(&m->buf, lenb, 4) < 0) return -1;
    if (n && stp_buf_append(&m->buf, v, n) < 0) return -1;
    m->count++; return 0;
}
int stp_map_put_u64(stp_map* m, const char* key, uint64_t v) {
    if (put_key(m, key) < 0) return -1;
    uint8_t vt = STP_VT_U64; uint8_t vb[8];
    if (stp_buf_append(&m->buf, &vt, 1) < 0) return -1;
    stp_put_u64le(vb, v);
    if (stp_buf_append(&m->buf, vb, 8) < 0) return -1;
    m->count++; return 0;
}
int stp_map_put_bool(stp_map* m, const char* key, int v) {
    if (put_key(m, key) < 0) return -1;
    uint8_t vt = STP_VT_BOOL; uint8_t vb = v ? 1 : 0;
    if (stp_buf_append(&m->buf, &vt, 1) < 0) return -1;
    if (stp_buf_append(&m->buf, &vb, 1) < 0) return -1;
    m->count++; return 0;
}
int stp_map_encode(const stp_map* m, stp_buf* out) {
    uint8_t cnt[2]; stp_put_u16le(cnt, m->count);
    if (stp_buf_append(out, cnt, 2) < 0) return -1;
    return stp_buf_append(out, m->buf.data, m->buf.len);
}

// ---- map decoder -----------------------------------------------------------

void stp_view_init(stp_view* v, const uint8_t* body, size_t len) { v->body = body; v->len = len; }

int stp_view_find(const stp_view* v, const char* key,
                  int* vt_out, const uint8_t** val, uint32_t* vlen) {
    if (v->len < 2) return 0;
    size_t p = 0;
    uint16_t n = stp_get_u16le(v->body); p += 2;
    size_t klen_want = strlen(key);
    for (uint16_t i = 0; i < n; i++) {
        if (p >= v->len) return 0;
        uint8_t kl = v->body[p++];
        if (p + kl > v->len) return 0;
        const uint8_t* k = v->body + p; p += kl;
        if (p >= v->len) return 0;
        int vt = v->body[p++];
        const uint8_t* value; uint32_t vln;
        switch (vt) {
            case STP_VT_BYTES:
            case STP_VT_TEXT: {
                if (p + 4 > v->len) return 0;
                vln = stp_get_u32le(v->body + p); p += 4;
                if (p + vln > v->len) return 0;
                value = v->body + p; p += vln;
                break;
            }
            case STP_VT_U64: {
                if (p + 8 > v->len) return 0;
                value = v->body + p; vln = 8; p += 8;
                break;
            }
            case STP_VT_BOOL: {
                if (p + 1 > v->len) return 0;
                value = v->body + p; vln = 1; p += 1;
                break;
            }
            default: return 0;
        }
        if (kl == klen_want && memcmp(k, key, kl) == 0) {
            if (vt_out) *vt_out = vt;
            if (val)    *val = value;
            if (vlen)   *vlen = vln;
            return 1;
        }
    }
    return 0;
}

int stp_view_bytes(const stp_view* v, const char* key, const uint8_t** out, uint32_t* n) {
    int vt; return stp_view_find(v, key, &vt, out, n) && (vt == STP_VT_BYTES || vt == STP_VT_TEXT);
}
int stp_view_text(const stp_view* v, const char* key, char* out, size_t out_sz) {
    int vt; const uint8_t* val; uint32_t n;
    if (!stp_view_find(v, key, &vt, &val, &n) || vt != STP_VT_TEXT) return 0;
    size_t c = n < out_sz - 1 ? n : out_sz - 1;
    memcpy(out, val, c); out[c] = '\0';
    return 1;
}
int stp_view_u64(const stp_view* v, const char* key, uint64_t* out) {
    int vt; const uint8_t* val; uint32_t n;
    if (!stp_view_find(v, key, &vt, &val, &n) || vt != STP_VT_U64) return 0;
    *out = stp_get_u64le(val);
    return 1;
}
int stp_view_bool(const stp_view* v, const char* key, int* out) {
    int vt; const uint8_t* val; uint32_t n;
    if (!stp_view_find(v, key, &vt, &val, &n) || vt != STP_VT_BOOL) return 0;
    *out = val[0] != 0;
    return 1;
}
