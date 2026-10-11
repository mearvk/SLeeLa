#ifndef SLEELA_MQTT_CONNECTOR_H
#define SLEELA_MQTT_CONNECTOR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef void (*sleela_mqtt_message_callback)(void *context,const char *topic,const void *payload,size_t size,int qos);
typedef struct sleela_mqtt { void *client; char error[256]; sleela_mqtt_message_callback callback; void *callback_context; } sleela_mqtt;
int sleela_mqtt_open(sleela_mqtt *c,const char *client_id,const char *host,int port,int keepalive);
int sleela_mqtt_set_message_callback(sleela_mqtt *c,sleela_mqtt_message_callback callback,void *context);
int sleela_mqtt_subscribe(sleela_mqtt *c,const char *topic,int qos);
int sleela_mqtt_publish(sleela_mqtt *c,const char *topic,const void *payload,size_t size,int qos,int retain);
int sleela_mqtt_loop(sleela_mqtt *c,int timeout_ms);
void sleela_mqtt_close(sleela_mqtt *c);
void sleela_mqtt_global_cleanup(void); /* call once after all clients have closed */
const char *sleela_mqtt_error(const sleela_mqtt *c);
#ifdef __cplusplus
}
#endif
#endif
