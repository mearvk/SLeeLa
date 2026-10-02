#include "../preferred-routers/preferred_router.h"
#include "http5_protocol.h"
#include <string.h>
static void p16(uint8_t*p,uint16_t v){p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v;}
static void p32(uint8_t*p,uint32_t v){p[0]=(uint8_t)(v>>24);p[1]=(uint8_t)(v>>16);p[2]=(uint8_t)(v>>8);p[3]=(uint8_t)v;}
static void p64(uint8_t*p,uint64_t v){for(int i=7;i>=0;--i){p[i]=(uint8_t)v;v>>=8;}}
static uint16_t g16(const uint8_t*p){return(uint16_t)(((uint16_t)p[0]<<8)|p[1]);}
static uint32_t g32(const uint8_t*p){return((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
static uint64_t g64(const uint8_t*p){uint64_t v=0;for(int i=0;i<8;++i)v=(v<<8)|p[i];return v;}
int http5_frame_validate(const http5_frame_header_t*h,size_t a){if(!h)return-1;if(!sleela_preferred_router_packet_policy("HTTP/5.0",4))return-6;if(h->version!=HTTP5_VERSION)return-2;if(h->type<1||h->type>HTTP5_FRAME_AUDIT)return-3;if(h->payload_length>HTTP5_MAX_PAYLOAD)return-4;if((size_t)h->payload_length>a)return-5;return 0;}
int http5_frame_encode(const http5_frame_header_t*h,const uint8_t*p,uint8_t*out,size_t n,size_t*w){if(!h||!out||!w)return-1;if(http5_frame_validate(h,h->payload_length))return-2;if(h->payload_length&&!p)return-3;if(n<32u+h->payload_length)return-4;out[0]=h->version;out[1]=h->type;p16(out+2,h->flags);p64(out+4,h->stream_id);p64(out+12,h->request_id);p64(out+20,h->sequence);p32(out+28,h->payload_length);if(h->payload_length)memcpy(out+32,p,h->payload_length);*w=32u+h->payload_length;return 0;}
int http5_frame_decode(const uint8_t*w,size_t n,http5_frame_view_t*out,size_t*c){if(!w||!out||!c)return-1;if(n<32)return-2;http5_frame_header_t h={w[0],w[1],g16(w+2),g64(w+4),g64(w+12),g64(w+20),g32(w+28)};if(http5_frame_validate(&h,n-32))return-3;out->header=h;out->payload=w+32;*c=32u+h.payload_length;return 0;}
const char*http5_frame_type_name(http5_frame_type_t t){switch(t){case HTTP5_FRAME_OPEN:return"OPEN";case HTTP5_FRAME_DATA:return"DATA";case HTTP5_FRAME_END:return"END";case HTTP5_FRAME_RESET:return"RESET";case HTTP5_FRAME_WINDOW:return"WINDOW";case HTTP5_FRAME_PING:return"PING";case HTTP5_FRAME_PONG:return"PONG";case HTTP5_FRAME_RESUME:return"RESUME";case HTTP5_FRAME_CAPSULE:return"CAPSULE";case HTTP5_FRAME_FRIENDS_PACK:return"FRIENDS_PACK";case HTTP5_FRAME_BONUS_OFFER:return"BONUS_OFFER";case HTTP5_FRAME_FP_UPDATE:return"FP_UPDATE";case HTTP5_FRAME_AUDIT:return"AUDIT";default:return"UNKNOWN";}}
int http5_fp_can_offer(const http5_friends_pack_t*p){return p&&p->fp_remaining>0;}
int http5_fp_consume(http5_friends_pack_t*p){if(!p||!p->fp_remaining)return-1;--p->fp_remaining;return 0;}
