#ifndef SKYA_ENGINE_H
#define SKYA_ENGINE_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct skya_engine skya_engine_t;
typedef enum { SKYA_CLIENT=1, SKYA_SERVER=2, SKYA_BOTH=3 } skya_role_t;
typedef struct { uint32_t max_peers; uint16_t port; uint8_t http_version; uint8_t relay; } skya_options_t;
typedef struct { uint64_t id; char address[128]; uint16_t port; uint8_t nat_type; uint8_t http_version; uint8_t audio; uint8_t video; } skya_peer_t;
skya_engine_t *skya_create(const skya_options_t*);
void skya_destroy(skya_engine_t*);
int skya_start(skya_engine_t*, skya_role_t);
void skya_stop(skya_engine_t*);
int skya_join(skya_engine_t*, const char*);
int skya_message(skya_engine_t*, const char*, const char*);
size_t skya_peer_count(const skya_engine_t*);
int skya_peer_at(const skya_engine_t*, size_t, skya_peer_t*);
const char *skya_status(const skya_engine_t*);
int skya_port_bound(const skya_engine_t*);
uint16_t skya_bound_port(const skya_engine_t*);
#ifdef __cplusplus
}
#endif
#endif
