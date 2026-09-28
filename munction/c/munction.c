#include "munction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct sleela_munction {
    char name[SLEELA_MUNCTION_MAX_NAME], address[SLEELA_MUNCTION_MAX_ADDRESS];
    char interims[SLEELA_MUNCTION_MAX_INTERIMS], policy[SLEELA_MUNCTION_MAX_INTERIMS], scheme[32];
    sleela_munction_channel channel;
    sleela_munction_phase phase;
    int calls, latched, bumped;
    uint64_t sent_bytes, acknowledged_bytes, received_units;
};
uint64_t sleela_munction_digest(const uint8_t *data,size_t n){uint64_t h=1469598103934665603ULL;if(!data&&n)return 0;for(size_t i=0;i<n;i++){h^=data[i];h*=1099511628211ULL;}return h;}
static int step(sleela_munction*m){if(!m||m->phase==SLEELA_MUNCTION_CLOSED)return -1;if(++m->calls>SLEELA_MUNCTION_MAX_CALLS){m->bumped=1;return -1;}return 0;}
static void bump(sleela_munction*m){if(m)m->bumped=1;}
static int movable(const sleela_munction*m){return m&&(m->phase==SLEELA_MUNCTION_CONNECTED||m->phase==SLEELA_MUNCTION_MOVING);}
sleela_munction *sleela_munction_start(const char*n){if(!n||!*n)return NULL;sleela_munction*m=calloc(1,sizeof(*m));if(!m)return NULL;snprintf(m->name,sizeof m->name,"%s",n);m->phase=SLEELA_MUNCTION_STARTED;m->calls=1;return m;}
int sleela_munction_connect(sleela_munction*m,const sleela_munction_channel*c,const char*a){if(step(m)||!c||!c->scheme||!c->open||m->phase!=SLEELA_MUNCTION_STARTED){bump(m);return -1;}m->channel=*c;snprintf(m->scheme,sizeof m->scheme,"%s",c->scheme);snprintf(m->address,sizeof m->address,"%s",a?a:"");if(c->open(c->ctx,m->address)){bump(m);return -1;}m->phase=SLEELA_MUNCTION_CONNECTED;return 0;}
int sleela_munction_enable(sleela_munction*m,const char*p){if(step(m)||!p||m->phase!=SLEELA_MUNCTION_CONNECTED){bump(m);return -1;}snprintf(m->policy,sizeof m->policy,"%s",p);return 0;}
int64_t sleela_munction_send(sleela_munction*m,const uint8_t*d,size_t n){if(step(m)||!movable(m)||(!d&&n)||!m->channel.send){bump(m);return -1;}int64_t a=m->channel.send(m->channel.ctx,d,n);m->sent_bytes+=n;if(a>=0)m->acknowledged_bytes+=(uint64_t)a;if(a!=(int64_t)n)bump(m);m->phase=SLEELA_MUNCTION_MOVING;return a;}
int sleela_munction_thatch(sleela_munction*m,const char*s){if(step(m)||!movable(m)||!s||!m->channel.thatch){bump(m);return -1;}if(m->channel.thatch(m->channel.ctx,s)){bump(m);return -1;}snprintf(m->interims,sizeof m->interims,"%s",s);return 0;}
int64_t sleela_munction_consume(sleela_munction*m,uint8_t*out,size_t cap){if(step(m)||!movable(m)||!m->channel.consume){bump(m);return -1;}uint64_t seq=0;int64_t n=m->channel.consume(m->channel.ctx,out,cap,&seq);if(n>=0)m->received_units++;return n;}
int sleela_munction_observe(sleela_munction*m,char*out,size_t cap){if(step(m)||!movable(m)||!m->channel.observe){bump(m);if(out&&cap)*out=0;return -1;}return m->channel.observe(m->channel.ctx,out,cap);}
int sleela_munction_latch(sleela_munction*m){if(step(m)||!movable(m)||m->latched||!m->channel.latch){bump(m);return -1;}if(m->channel.latch(m->channel.ctx)){bump(m);return -1;}m->latched=1;return 0;}
static sleela_munction_outcome finish(sleela_munction*m,sleela_munction_outcome o,char*out,size_t cap){if(m->calls<SLEELA_MUNCTION_MIN_CALLS||m->calls>SLEELA_MUNCTION_MAX_CALLS||m->bumped)if(o==SLEELA_MUNCTION_REACHED)o=SLEELA_MUNCTION_CONTAINED;if(m->channel.close&&m->channel.close(m->channel.ctx)&&o==SLEELA_MUNCTION_REACHED)o=SLEELA_MUNCTION_CONTAINED;if(out&&cap){const char*r=o==SLEELA_MUNCTION_REACHED?"REACHED":o==SLEELA_MUNCTION_CONTAINED?"CONTAINED":"ABORTED";snprintf(out,cap,"receipt{name=%s scheme=%s address=%s calls=%d sent=%llu ack=%llu received=%llu interims=[%s] latched=%s coherent=%s outcome=%s}",m->name,m->scheme,m->address,m->calls,(unsigned long long)m->sent_bytes,(unsigned long long)m->acknowledged_bytes,(unsigned long long)m->received_units,m->interims,m->latched?"true":"false",sleela_munction_coherent(m)?"true":"false",r);}if(m->channel.destroy)m->channel.destroy(m->channel.ctx);free(m);return o;}
sleela_munction_outcome sleela_munction_close(sleela_munction*m,char*out,size_t cap){if(!m)return SLEELA_MUNCTION_ABORTED;if(step(m))return SLEELA_MUNCTION_CONTAINED;return finish(m,SLEELA_MUNCTION_REACHED,out,cap);}
sleela_munction_outcome sleela_munction_abort(sleela_munction*m,char*out,size_t cap){if(!m)return SLEELA_MUNCTION_ABORTED;if(step(m))return SLEELA_MUNCTION_CONTAINED;m->bumped=1;return finish(m,SLEELA_MUNCTION_ABORTED,out,cap);}
int sleela_munction_call_count(const sleela_munction*m){return m?m->calls:0;}
uint64_t sleela_munction_sent_bytes(const sleela_munction*m){return m?m->sent_bytes:0;}
uint64_t sleela_munction_acknowledged_bytes(const sleela_munction*m){return m?m->acknowledged_bytes:0;}
uint64_t sleela_munction_received_units(const sleela_munction*m){return m?m->received_units:0;}
int sleela_munction_coherent(const sleela_munction*m){return m&&!m->bumped&&m->sent_bytes==m->acknowledged_bytes;}
sleela_munction_phase sleela_munction_phase_of(const sleela_munction*m){return m?m->phase:SLEELA_MUNCTION_CLOSED;}
