#ifndef SKYA_DRIVER_RUNTIME_H
#define SKYA_DRIVER_RUNTIME_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SKYA_DRIVER_MESSAGE_MAX 2048u
#define SKYA_DRIVER_QUEUE_CAPACITY 256u
typedef enum { SKYA_MSG_EVENT=1, SKYA_MSG_COMMAND=2, SKYA_MSG_AUDIO=3, SKYA_MSG_CONTROL=4, SKYA_MSG_ERROR=5 } skya_message_type;
typedef struct { uint64_t sequence; uint64_t timestamp_ns; skya_message_type type; size_t length; unsigned char data[SKYA_DRIVER_MESSAGE_MAX]; } skya_driver_message;
typedef struct skya_driver_runtime skya_driver_runtime;
int skya_driver_runtime_init(skya_driver_runtime **out,size_t capacity); void skya_driver_runtime_destroy(skya_driver_runtime *rt); int skya_driver_runtime_start(skya_driver_runtime *rt); int skya_driver_runtime_stop(skya_driver_runtime *rt); int skya_driver_runtime_send(skya_driver_runtime *rt,skya_message_type type,const void *data,size_t length,uint64_t *sequence_out); int skya_driver_runtime_receive(skya_driver_runtime *rt,skya_driver_message *out,int wait_ms); int skya_driver_runtime_cancel_wait(skya_driver_runtime *rt); int skya_driver_runtime_lock(skya_driver_runtime *rt); int skya_driver_runtime_unlock(skya_driver_runtime *rt); int skya_driver_runtime_is_running(const skya_driver_runtime *rt); size_t skya_driver_runtime_depth(const skya_driver_runtime *rt);
#ifdef __cplusplus
}
#endif
#endif
