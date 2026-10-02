#include "slvm_broker.h"
#include <string.h>
void slvm_broker_init(slvm_broker_t *b){if(b)memset(b,0,sizeof(*b));}
int slvm_broker_attach_transport(slvm_broker_t *b,slvm_broker_send_fn s,slvm_broker_receive_fn r,void *c){if(!b||!s||!r)return 0;b->send=s;b->receive=r;b->context=c;return 1;}
int slvm_broker_send_frame(slvm_broker_t *b,const slvm_broker_frame_t *f,const void *p,size_t n){if(!b||!b->send||!f||n!=f->payload_size)return 0;if(f->magic!=SLVM_BROKER_MAGIC||f->version!=SLVM_BROKER_VERSION)return 0;if(b->send(f,sizeof(*f),b->context)<=0)return 0;return n?b->send(p,n,b->context)>0:1;}