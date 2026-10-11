#include "mqtt_connector.h"
#include <mosquitto.h>
#include <stdio.h>
#include <string.h>
static void seterr(sleela_mqtt*c,const char*s){if(c)snprintf(c->error,sizeof c->error,"%s",s?s:"MQTT error");}
static void message_cb(struct mosquitto*m,void*ctx,const struct mosquitto_message*msg){(void)m;sleela_mqtt*c=(sleela_mqtt*)ctx;if(c&&c->callback&&msg)c->callback(c->callback_context,msg->topic,msg->payload,(size_t)msg->payloadlen,msg->qos);}
int sleela_mqtt_open(sleela_mqtt*c,const char*id,const char*host,int port,int keepalive){if(!c||!id||!host)return 0;memset(c,0,sizeof *c);int r=mosquitto_lib_init();if(r!=MOSQ_ERR_SUCCESS){seterr(c,mosquitto_strerror(r));return 0;}struct mosquitto*m=mosquitto_new(id,true,c);if(!m){seterr(c,"mosquitto client allocation failed");return 0;}c->client=m;mosquitto_message_callback_set(m,message_cb);r=mosquitto_connect(m,host,port>0?port:1883,keepalive>0?keepalive:30);if(r!=MOSQ_ERR_SUCCESS){seterr(c,mosquitto_strerror(r));mosquitto_destroy(m);c->client=NULL;return 0;}return 1;}
int sleela_mqtt_set_message_callback(sleela_mqtt*c,sleela_mqtt_message_callback cb,void*ctx){if(!c||!c->client)return 0;c->callback=cb;c->callback_context=ctx;return 1;}
int sleela_mqtt_subscribe(sleela_mqtt*c,const char*t,int q){if(!c||!c->client||!t||q<0||q>2)return 0;int r=mosquitto_subscribe((struct mosquitto*)c->client,NULL,t,q);if(r!=MOSQ_ERR_SUCCESS)seterr(c,mosquitto_strerror(r));return r==MOSQ_ERR_SUCCESS;}
int sleela_mqtt_publish(sleela_mqtt*c,const char*t,const void*p,size_t n,int q,int retain){if(!c||!c->client||!t||n>2147483647U||q<0||q>2)return 0;int r=mosquitto_publish((struct mosquitto*)c->client,NULL,t,(int)n,p,q,retain);if(r!=MOSQ_ERR_SUCCESS)seterr(c,mosquitto_strerror(r));return r==MOSQ_ERR_SUCCESS;}
int sleela_mqtt_loop(sleela_mqtt*c,int ms){if(!c||!c->client)return 0;int r=mosquitto_loop((struct mosquitto*)c->client,ms>0?ms:100,1);if(r!=MOSQ_ERR_SUCCESS)seterr(c,mosquitto_strerror(r));return r==MOSQ_ERR_SUCCESS;}
void sleela_mqtt_close(sleela_mqtt*c){if(c&&c->client){mosquitto_disconnect((struct mosquitto*)c->client);mosquitto_destroy((struct mosquitto*)c->client);c->client=NULL;c->callback=NULL;c->callback_context=NULL;}}
void sleela_mqtt_global_cleanup(void){mosquitto_lib_cleanup();}
const char*sleela_mqtt_error(const sleela_mqtt*c){return c?c->error:"invalid connector";}
