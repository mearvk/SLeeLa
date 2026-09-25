#include "http4_reassembly.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
static int bt(const uint8_t *b,uint32_t i){return(b[i/8u]&(uint8_t)(1u<<(i%8u)))!=0;}
static void bs(uint8_t *b,uint32_t i){b[i/8u]|=(uint8_t)(1u<<(i%8u));}
int http4_reassembly_init(http4_reassembly_t *s,size_t n,uint32_t count,uint64_t stream,uint64_t req,uint64_t id){
 if(!s||n>HTTP4_MAX_PAYLOAD||n>UINT32_MAX||count==0||count>HTTP4_MAX_REASSEMBLY_SEGMENTS)return-1;
 size_t bm=((size_t)count+7u)/8u,cv=(n+7u)/8u;
 if(bm>SIZE_MAX-n||cv>SIZE_MAX-n-bm)return-2;
 memset(s,0,sizeof(*s)); s->buffer=malloc(n?n:1u); s->bitmap=calloc(bm?bm:1u,1u); s->coverage=calloc(cv?cv:1u,1u);
 if(!s->buffer||!s->bitmap||!s->coverage){http4_reassembly_reset(s);return-3;}
 s->stream_id=stream;s->request_id=req;s->segment_id=id;s->total_length=(uint32_t)n;s->segment_count=count;return 0;
}
void http4_reassembly_reset(http4_reassembly_t *s){if(!s)return;free(s->buffer);free(s->bitmap);free(s->coverage);memset(s,0,sizeof(*s));}
int http4_reassembly_add(http4_reassembly_t *s,const http4_segment_view_t *v){
 if(!s||!v||!s->buffer||!s->bitmap||!s->coverage)return-1;const http4_segment_header_t*h=&v->header;
 if(h->magic!=HTTP4_SEGMENT_MAGIC||h->version!=HTTP4_SEGMENT_VERSION)return-2;
 if(v->stream_id!=s->stream_id||v->request_id!=s->request_id||h->segment_id!=s->segment_id||h->segment_count!=s->segment_count||h->total_length!=s->total_length)return-3;
 if(h->segment_index>=s->segment_count)return-4;if((size_t)h->offset>s->total_length||(size_t)h->data_length>s->total_length-h->offset)return-5;
 if(h->data_length&&!v->data)return-6;if(bt(s->bitmap,h->segment_index))return-7;
 for(uint32_t i=0;i<h->data_length;i++)if(bt(s->coverage,h->offset+i))return-8;
 if(h->data_length){memcpy(s->buffer+h->offset,v->data,h->data_length);for(uint32_t i=0;i<h->data_length;i++)bs(s->coverage,h->offset+i);}
 bs(s->bitmap,h->segment_index);s->received_segments++;s->received_bytes+=h->data_length;return 0;
}
int http4_reassembly_complete(const http4_reassembly_t*s){return s&&s->buffer&&s->bitmap&&s->coverage&&s->received_segments==s->segment_count&&s->received_bytes==s->total_length;}
const uint8_t*http4_reassembly_data(const http4_reassembly_t*s){return s&&http4_reassembly_complete(s)?s->buffer:NULL;}
size_t http4_reassembly_size(const http4_reassembly_t*s){return http4_reassembly_complete(s)?s->total_length:0;}
