#include "synchro.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
static uint32_t be32(uint32_t x){return ((x&0xffu)<<24)|((x&0xff00u)<<8)|((x>>8)&0xff00u)|((x>>24)&0xffu);}
static uint64_t be64(uint64_t x){uint64_t y=0;for(int i=0;i<8;i++)y=(y<<8)|((x>>(i*8))&0xff);return y;}
static int cmp(const void*a,const void*b){double x=*(const double*)a,y=*(const double*)b;return x<y?-1:x>y;}
int synchro_stats_init(synchro_stats*s,size_t w){if(!s||!w)return -1;memset(s,0,sizeof(*s));s->window=s->capacity=w;s->rtts_ms=calloc(w,sizeof(double));return s->rtts_ms?0:-1;}
void synchro_stats_free(synchro_stats*s){if(s){free(s->rtts_ms);memset(s,0,sizeof(*s));}}
int synchro_stats_record(synchro_stats*s,int ok,double r){if(!s)return -1;s->sent++;if(!ok){s->lost++;return 0;}s->acked++;if(s->count<s->capacity){s->rtts_ms[s->count++]=r;s->sum+=r;s->sumsq+=r*r;}else{double old=s->rtts_ms[0];memmove(s->rtts_ms,s->rtts_ms+1,(s->capacity-1)*sizeof(double));s->rtts_ms[s->capacity-1]=r;s->sum+=r-old;s->sumsq+=r*r-old*old;}return 0;}
double synchro_percentile(const synchro_stats*s,double p){if(!s||!s->count||p<0||p>100)return NAN;double*a=malloc(s->count*sizeof(double));if(!a)return NAN;memcpy(a,s->rtts_ms,s->count*sizeof(double));qsort(a,s->count,sizeof(double),cmp);size_t rank=p==0?1:(size_t)ceil(p/100.0*s->count);if(rank>s->count)rank=s->count;double v=a[rank-1];free(a);return v;}
double synchro_loss_rate(const synchro_stats*s){return s&&s->sent?(double)s->lost/s->sent:0;} double synchro_delivery_rate(const synchro_stats*s){return s&&s->sent?(double)s->acked/s->sent:0;}
int synchro_packet_encode(uint8_t*b,size_t n,uint32_t seq,uint64_t sent){if(!b||n<16)return -1;uint32_t m=be32(SYNCHRO_MAGIC),q=be32(seq);uint64_t t=be64(sent);memcpy(b,&m,4);memcpy(b+4,&q,4);memcpy(b+8,&t,8);return 16;}
int synchro_packet_decode(const uint8_t*b,size_t n,uint32_t*seq,uint64_t*sent){if(!b||n<16||!seq||!sent)return -1;uint32_t m,q;uint64_t t;memcpy(&m,b,4);memcpy(&q,b+4,4);memcpy(&t,b+8,8);if(be32(m)!=SYNCHRO_MAGIC)return -2;*seq=be32(q);*sent=be64(t);return 0;}
