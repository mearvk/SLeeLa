// ===========================================================================
// stp_wire.h -- STP-0001 framing + portable flat-map TLV codec (C side).
//
// Byte-for-byte compatible with the Java Wire class in the SecureJDK 28
// supervisor (Ubuntu.Determinant/userland/openjdk/transition-supervisor):
//   * 8-byte little-endian frame header: version|type|flags(u16)|body_len(u32)
//   * body = u16 entry count, then per entry:
//       u8 key_len | key bytes | u8 value_type | value
//     value types: 0=bytes(u32 len+bytes) 1=text(u32 len+utf8) 2=u64(8B LE) 3=bool(1B)
// ===========================================================================
#ifndef SLEELA_STP_WIRE_H
#define SLEELA_STP_WIRE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- protocol constants (mirror Stp.java) ----
#define STP_PROTO           "STP-0001"
#define STP_VERSION         1
#define STP_HEADER_LEN      8

#define STP_T_HELLO_CLIENT   0x01
#define STP_T_HELLO_SERVER   0x02
#define STP_T_TRANSITION_REQ 0x10
#define STP_T_ACK            0x11
#define STP_T_DENY           0x12
#define STP_T_HEARTBEAT      0x13
#define STP_T_CLOSE          0x1E
#define STP_T_ERROR          0x1F

#define STP_F_ENCRYPTED      0x0001
#define STP_F_LAST           0x0002

#define STP_VT_BYTES 0
#define STP_VT_TEXT  1
#define STP_VT_U64   2
#define STP_VT_BOOL  3

// ---- a growable byte buffer ----
typedef struct {
    uint8_t* data;
    size_t   len;
    size_t   cap;
} stp_buf;

void stp_buf_init(stp_buf* b);
void stp_buf_free(stp_buf* b);
int  stp_buf_append(stp_buf* b, const void* p, size_t n);   // 0 ok, -1 oom

// ---- little-endian helpers ----
void     stp_put_u16le(uint8_t* a, uint16_t v);
void     stp_put_u32le(uint8_t* a, uint32_t v);
uint16_t stp_get_u16le(const uint8_t* a);
uint32_t stp_get_u32le(const uint8_t* a);
void     stp_put_u64le(uint8_t out[8], uint64_t v);
uint64_t stp_get_u64le(const uint8_t* a);

// ---- frame header ----
void stp_header(uint8_t hdr[STP_HEADER_LEN], int type, int flags, uint32_t body_len);
void stp_parse_header(const uint8_t hdr[STP_HEADER_LEN],
                      int* version, int* type, int* flags, uint32_t* body_len);

// ---- flat-map body builder (into a stp_buf) ----
typedef struct {
    stp_buf   buf;      // accumulates encoded entries (after the 2-byte count)
    uint16_t  count;
} stp_map;

void stp_map_init(stp_map* m);
void stp_map_free(stp_map* m);
int  stp_map_put_bytes(stp_map* m, const char* key, const uint8_t* v, uint32_t n);
int  stp_map_put_text (stp_map* m, const char* key, const char* v);
int  stp_map_put_u64  (stp_map* m, const char* key, uint64_t v);
int  stp_map_put_bool (stp_map* m, const char* key, int v);
// Finalize: writes count + entries into `out` (out must be stp_buf_init'd).
int  stp_map_encode(const stp_map* m, stp_buf* out);

// ---- flat-map decoder (read-only view over a decoded body) ----
typedef struct {
    const uint8_t* body;
    size_t         len;
} stp_view;

void stp_view_init(stp_view* v, const uint8_t* body, size_t len);
// Locate a key. On success sets *vt and the value slice; returns 1, else 0.
int  stp_view_find(const stp_view* v, const char* key,
                   int* vt, const uint8_t** val, uint32_t* vlen);
// Typed getters (return 1 on success/present, 0 otherwise).
int  stp_view_bytes(const stp_view* v, const char* key, const uint8_t** out, uint32_t* n);
int  stp_view_text (const stp_view* v, const char* key, char* out, size_t out_sz);
int  stp_view_u64  (const stp_view* v, const char* key, uint64_t* out);
int  stp_view_bool (const stp_view* v, const char* key, int* out);

#ifdef __cplusplus
}
#endif
#endif // SLEELA_STP_WIRE_H
