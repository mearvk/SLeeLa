#ifndef SLEELA_SLVM4_BROKER_H
#define SLEELA_SLVM4_BROKER_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM4_BROKER_VERSION 1

typedef enum {
 SLVM4_BROKER_HELLO=1,
 SLVM4_BROKER_OBJECT_DECLARE,
 SLVM4_BROKER_OBJECT_CALL,
 SLVM4_BROKER_OBJECT_RETURN,
 SLVM4_BROKER_GUI_CREATE,
 SLVM4_BROKER_GUI_EVENT,
 SLVM4_BROKER_OBJECT_RELEASE,
 SLVM4_BROKER_ERROR,
 SLVM4_BROKER_CLOSE
} slvm4_broker_message_type_t;

typedef struct {
 uint32_t magic;
 uint16_t version;
 uint16_t type;
 uint64_t request_id;
 uint64_t object_id;
 uint64_t lease_id;
 uint64_t nonce;
 uint32_t flags;
 uint32_t payload_size;
} slvm4_broker_frame_t;

typedef struct {
 uint64_t object_id;
 uint64_t lease_id;
 uint64_t expires_at_ns;
 uint32_t owner;
 uint32_t rights;
} slvm4_object_lease_t;

int slvm4_broker_validate_frame(const slvm4_broker_frame_t*, size_t);
int slvm4_broker_validate_lease(const slvm4_object_lease_t*, uint64_t now_ns);
#ifdef __cplusplus
}
#endif
#endif
