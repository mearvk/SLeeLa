#ifndef SLEELA_NATIVE_IPC_H
#define SLEELA_NATIVE_IPC_H
/* Portable SLeeLa IPC v1 framing. Wire fields are little-endian; never send
 * native structs, pointers, SLValue objects, or VM-owned handles. */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../../os-level-support/loader/native_loader.h"
#define SL_IPC_HEADER_SIZE 24u
#define SL_IPC_MAX_PAYLOAD (16u*1024u*1024u)
#define SL_IPC_VERSION 1u
typedef enum { SL_IPC_REQUEST=1, SL_IPC_RESPONSE=2, SL_IPC_CANCEL=3,
               SL_IPC_HELLO=4, SL_IPC_HELLO_ACK=5 } SLIPCMessageKind;
typedef struct { uint16_t version,kind; uint64_t request_id;
                 uint32_t payload_length; int32_t status; } SLIPCHeader;
typedef struct { const uint8_t *data; size_t length,offset; } SLIPCCursor;
static inline void sl_ipc_put16(uint8_t*p,uint16_t v){p[0]=(uint8_t)v;p[1]=(uint8_t)(v>>8);}
static inline void sl_ipc_put32(uint8_t*p,uint32_t v){unsigned i;for(i=0;i<4;i++)p[i]=(uint8_t)(v>>(8*i));}
static inline void sl_ipc_put64(uint8_t*p,uint64_t v){unsigned i;for(i=0;i<8;i++)p[i]=(uint8_t)(v>>(8*i));}
static inline uint16_t sl_ipc_get16(const uint8_t*p){return(uint16_t)(p[0]|((uint16_t)p[1]<<8));}
static inline uint32_t sl_ipc_get32(const uint8_t*p){return(uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static inline uint64_t sl_ipc_get64(const uint8_t*p){uint64_t v=0;unsigned i;for(i=0;i<8;i++)v|=((uint64_t)p[i])<<(8*i);return v;}
static inline int sl_ipc_encode_header(const SLIPCHeader*h,uint8_t*out,size_t cap){
 if(!h||!out||cap<24||h->version!=1||h->kind<1||h->kind>5||h->payload_length>SL_IPC_MAX_PAYLOAD)return-1;
 out[0]='S';out[1]='L';out[2]='I';out[3]='P';sl_ipc_put16(out+4,h->version);sl_ipc_put16(out+6,h->kind);
 sl_ipc_put64(out+8,h->request_id);sl_ipc_put32(out+16,h->payload_length);sl_ipc_put32(out+20,(uint32_t)h->status);return 24;
}
static inline int sl_ipc_decode_header(const uint8_t*in,size_t n,SLIPCHeader*h){
 if(!in||!h||n<24||memcmp(in,"SLIP",4))return-1;
 h->version=sl_ipc_get16(in+4);h->kind=sl_ipc_get16(in+6);h->request_id=sl_ipc_get64(in+8);
 h->payload_length=sl_ipc_get32(in+16);h->status=(int32_t)sl_ipc_get32(in+20);
 return(h->version==1&&h->kind>=1&&h->kind<=5&&h->payload_length<=SL_IPC_MAX_PAYLOAD)?24:-1;
}
/* Values reuse SLNativeValue tags. UTF-8 decode results borrow the input
 * buffer; copy before reuse and validate UTF-8 before VM String creation. */
static inline int sl_ipc_encode_value(const SLNativeValue*v,uint8_t*out,size_t cap,size_t*used){
 size_t n;uint64_t bits=0;if(!v||!out||!used)return-1;
 switch(v->tag){case SL_NATIVE_VALUE_NULL:n=1;break;
 case SL_NATIVE_VALUE_BOOL:n=2;if(v->as.boolean!=0&&v->as.boolean!=1)return-1;break;
 case SL_NATIVE_VALUE_I64:n=9;break;case SL_NATIVE_VALUE_F64:n=9;memcpy(&bits,&v->as.f64,8);break;
 case SL_NATIVE_VALUE_UTF8:if(v->as.utf8.length>SL_IPC_MAX_PAYLOAD||(!v->as.utf8.data&&v->as.utf8.length))return-1;n=5u+v->as.utf8.length;break;
 default:return-1;}if(n>cap||n>SL_IPC_MAX_PAYLOAD)return-1;out[0]=(uint8_t)v->tag;
 if(v->tag==SL_NATIVE_VALUE_BOOL)out[1]=(uint8_t)v->as.boolean;
 else if(v->tag==SL_NATIVE_VALUE_I64)sl_ipc_put64(out+1,(uint64_t)v->as.i64);
 else if(v->tag==SL_NATIVE_VALUE_F64)sl_ipc_put64(out+1,bits);
 else if(v->tag==SL_NATIVE_VALUE_UTF8){sl_ipc_put32(out+1,v->as.utf8.length);if(v->as.utf8.length)memcpy(out+5,v->as.utf8.data,v->as.utf8.length);}
 *used=n;return 0;
}
static inline int sl_ipc_decode_value(SLIPCCursor*c,SLNativeValue*v){
 size_t n;uint64_t bits;if(!c||!v||!c->data||c->offset>=c->length)return-1;
 memset(v,0,sizeof(*v));v->tag=c->data[c->offset];
 switch(v->tag){case SL_NATIVE_VALUE_NULL:n=1;break;
 case SL_NATIVE_VALUE_BOOL:n=2;if(c->length-c->offset<n||c->data[c->offset+1]>1)return-1;v->as.boolean=c->data[c->offset+1];break;
 case SL_NATIVE_VALUE_I64:n=9;if(c->length-c->offset<n)return-1;v->as.i64=(int64_t)sl_ipc_get64(c->data+c->offset+1);break;
 case SL_NATIVE_VALUE_F64:n=9;if(c->length-c->offset<n)return-1;bits=sl_ipc_get64(c->data+c->offset+1);memcpy(&v->as.f64,&bits,8);break;
 case SL_NATIVE_VALUE_UTF8:if(c->length-c->offset<5)return-1;v->as.utf8.length=sl_ipc_get32(c->data+c->offset+1);n=5u+v->as.utf8.length;if(v->as.utf8.length>SL_IPC_MAX_PAYLOAD||n>c->length-c->offset)return-1;v->as.utf8.data=c->data+c->offset+5;break;
 default:return-1;}if(n>c->length-c->offset)return-1;c->offset+=n;return 0;
}
#endif
