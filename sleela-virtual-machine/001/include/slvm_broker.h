#ifndef SLEELA_SLVM_BROKER_H
#define SLEELA_SLVM_BROKER_H
#include <stddef.h>
#include <stdint.h>
#include "slvm_broker_security.h"
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM_BROKER_MAGIC 0x534C4A56u
#define SLVM_BROKER_VERSION 1u
typedef enum { SLVM_BROKER_HELLO=1, SLVM_BROKER_OBJECT_DECLARE=2, SLVM_BROKER_OBJECT_CALL=3, SLVM_BROKER_OBJECT_RETURN=4, SLVM_BROKER_GUI_CREATE=5, SLVM_BROKER_GUI_EVENT=6, SLVM_BROKER_GUI_CLOSE=7, SLVM_BROKER_OBJECT_RELEASE=8, SLVM_BROKER_ERROR=9, SLVM_BROKER_CLOSE=10 } slvm_broker_message_type_t;
typedef enum { SLVM_BROKER_OBJECT_SCALAR=1, SLVM_BROKER_OBJECT_STRING=2, SLVM_BROKER_OBJECT_BYTES=3, SLVM_BROKER_OBJECT_LIST=4, SLVM_BROKER_OBJECT_MAP=5, SLVM_BROKER_OBJECT_CLASS=6, SLVM_BROKER_OBJECT_GUI=7, SLVM_BROKER_OBJECT_STREAM=8 } slvm_broker_object_kind_t;
typedef struct { uint32_t magic; uint16_t version; uint16_t type; uint32_t flags; uint64_t request_id; uint64_t object_id; uint64_t payload_size; } slvm_broker_frame_t;
typedef int (*slvm_broker_send_fn)(const void *,size_t,void *);
typedef int (*slvm_broker_receive_fn)(void *,size_t,size_t *,void *);
typedef struct { slvm_broker_send_fn send; slvm_broker_receive_fn receive; void *context; uint8_t authenticated; uint8_t encrypted; uint8_t remote_gui;
    slvm_broker_security_t security; } slvm_broker_t;
void slvm_broker_init(slvm_broker_t *);
int slvm_broker_attach_transport(slvm_broker_t *,slvm_broker_send_fn,slvm_broker_receive_fn,void *);
int slvm_broker_send_frame(slvm_broker_t *,const slvm_broker_frame_t *,const void *,size_t);
#ifdef __cplusplus
}
#endif
#endif