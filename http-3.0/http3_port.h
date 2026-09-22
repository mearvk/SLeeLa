#ifndef SLEELA_HTTP3_PORT_H
#define SLEELA_HTTP3_PORT_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define HTTP3_PORT_BYTES 20u
#define HTTP3_PORT_BITS 160u
#define HTTP3_PORT_MAX_DECIMAL "999999999999999999999999999999999999999999999999"
#define HTTP3_PORT_NAMESPACE_DECIMAL "1000000000000000000000000000000000000000000000000"
typedef struct { uint8_t bytes[HTTP3_PORT_BYTES]; } http3_port_t;
void http3_port_zero(http3_port_t *port);
int http3_port_from_decimal(http3_port_t *port, const char *decimal);
void http3_port_from_u64(http3_port_t *port, uint64_t value);
int http3_port_to_decimal(const http3_port_t *port, char *out, size_t out_size);
int http3_port_is_valid(const http3_port_t *port);
int http3_port_compare(const http3_port_t *a, const http3_port_t *b);
int http3_port_increment(http3_port_t *port);
void http3_port_encode(const http3_port_t *port, uint8_t out[HTTP3_PORT_BYTES]);
void http3_port_decode(http3_port_t *port, const uint8_t in[HTTP3_PORT_BYTES]);
#ifdef __cplusplus
}
#endif
#endif
