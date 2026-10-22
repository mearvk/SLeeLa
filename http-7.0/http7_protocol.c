/* SLeeLa HTTP 7.0 frame and Reality Assertion implementation. */
#include "http7_protocol.h"
#include "../preferred-routers/preferred_router.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

static void p16(uint8_t*p,uint16_t v){p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v;}
static void p32(uint8_t*p,uint32_t v){p[0]=(uint8_t)(v>>24);p[1]=(uint8_t)(v>>16);p[2]=(uint8_t)(v>>8);p[3]=(uint8_t)v;}
static void p64(uint8_t*p,uint64_t v){for(int i=7;i>=0;--i){p[i]=(uint8_t)v;v>>=8;}}
static uint16_t g16(const uint8_t*p){return (uint16_t)(((uint16_t)p[0]<<8)|p[1]);}
static uint32_t g32(const uint8_t*p){return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
static uint64_t g64(const uint8_t*p){uint64_t v=0;for(int i=0;i<8;++i)v=(v<<8)|p[i];return v;}

int http7_frame_validate(const http7_frame_header_t*h,size_t available){
 if(!h)return -1;
 if(!sleela_preferred_router_packet_policy("HTTP/7.0",4))return -6;
 if(h->version!=HTTP7_VERSION)return -2;
 if(h->type<HTTP7_FRAME_OPEN||h->type>HTTP7_FRAME_ASSERTION_AUTHOR)return -3;
 if((h->flags&0xff00u)!=0)return -7;
 if(h->payload_length>HTTP7_MAX_PAYLOAD)return -4;
 if((size_t)h->payload_length>available)return -5;
 return 0;
}
int http7_frame_encode(const http7_frame_header_t*h,const uint8_t*payload,uint8_t*out,size_t cap,size_t*written){
 if(!h||!out||!written)return -1;
 *written=0;
 if(http7_frame_validate(h,h->payload_length)!=0)return -2;
 if(h->payload_length&&!payload)return -3;
 if(cap<HTTP7_FRAME_HEADER_SIZE+(size_t)h->payload_length)return -4;
 out[0]=h->version;out[1]=h->type;p16(out+2,h->flags);p64(out+4,h->stream_id);
 p64(out+12,h->request_id);p64(out+20,h->sequence);p32(out+28,h->payload_length);
 if(h->payload_length)memcpy(out+HTTP7_FRAME_HEADER_SIZE,payload,h->payload_length);
 *written=HTTP7_FRAME_HEADER_SIZE+(size_t)h->payload_length;return 0;
}
int http7_frame_decode(const uint8_t*wire,size_t len,http7_frame_view_t*out,size_t*consumed){
 if(!wire||!out||!consumed)return -1;
 *consumed=0;
 if(len<HTTP7_FRAME_HEADER_SIZE)return -2;
 http7_frame_header_t h;
 h.version=wire[0];h.type=wire[1];h.flags=g16(wire+2);h.stream_id=g64(wire+4);
 h.request_id=g64(wire+12);h.sequence=g64(wire+20);h.payload_length=g32(wire+28);
 if(http7_frame_validate(&h,len-HTTP7_FRAME_HEADER_SIZE)!=0)return -3;
 out->header=h;out->payload=wire+HTTP7_FRAME_HEADER_SIZE;
 *consumed=HTTP7_FRAME_HEADER_SIZE+(size_t)h.payload_length;return 0;
}
const char*http7_frame_type_name(http7_frame_type_t t){
 switch(t){
 case 1:return"OPEN";case 2:return"DATA";case 3:return"END";case 4:return"RESET";case 5:return"WINDOW";
 case 6:return"PING";case 7:return"PONG";case 8:return"RESUME";case 9:return"CAPSULE";
 case 10:return"FRIENDS_PACK";case 11:return"BONUS_OFFER";case 12:return"FP_UPDATE";case 13:return"AUDIT";
 case 14:return"CONSOLIDATED_FRIENDS_BET";case 15:return"TEAMSTER_DEBATE";case 16:return"CONSOLIDATE_IQ";
 case 17:return"TEAM_AREA";case 18:return"DEBATE_TOPIC";case 19:return"DEBATE_POSITION";case 20:return"RECIPIENT_LABEL";
 case 21:return"ASSERTION";case 22:return"ASSERTION_SOURCE";case 23:return"ASSERTION_STATUS";case 24:return"ASSERTION_AUTHOR";
 default:return"UNKNOWN";}
}
const char*http7_assertion_status_name(http7_assertion_status_t s){
 switch(s){case HTTP7_ASSERT_FACTUAL_DOCUMENTED:return"FACTUAL_DOCUMENTED";case HTTP7_ASSERT_USER_AUTHORED:return"USER_AUTHORED";
 case HTTP7_ASSERT_FICTIONAL:return"FICTIONAL";case HTTP7_ASSERT_COUNTERFACTUAL:return"COUNTERFACTUAL";
 case HTTP7_ASSERT_DISPUTED:return"DISPUTED";case HTTP7_ASSERT_UNVERIFIED:return"UNVERIFIED";default:return"UNKNOWN";}
}
static int valid_utf8(const unsigned char *s, size_t n) {
 size_t i = 0;
 while (i < n) {
  unsigned char c = s[i++];
  if (c <= 0x7f) continue;
  uint32_t cp;
  size_t need;
  if (c >= 0xc2 && c <= 0xdf) { cp = c & 0x1fu; need = 1; }
  else if (c >= 0xe0 && c <= 0xef) { cp = c & 0x0fu; need = 2; }
  else if (c >= 0xf0 && c <= 0xf4) { cp = c & 0x07u; need = 3; }
  else return 0;
  if (need > n - i) return 0;
  for (size_t j = 0; j < need; ++j) {
   unsigned char t = s[i++];
   if ((t & 0xc0u) != 0x80u) return 0;
   cp = (cp << 6) | (uint32_t)(t & 0x3fu);
  }
  if ((need == 1 && cp < 0x80u) || (need == 2 && cp < 0x800u) ||
      (need == 3 && cp < 0x10000u) || cp > 0x10ffffu ||
      (cp >= 0xd800u && cp <= 0xdfffu)) return 0;
 }
 return 1;
}
static int valid_field(const char*s,size_t cap,int required){
 if (!s) return 0;
 size_t n = 0;
 while (n < cap && s[n] != '\0') ++n;
 if (n == cap || (required && n == 0)) return 0;
 for (size_t i = 0; i < n; ++i) {
  unsigned char c = (unsigned char)s[i];
  if (c == '\t' || c == '\r' || c == '\n' || c == 0) return 0;
 }
 return valid_utf8((const unsigned char*)s, n);
}
int http7_assertion_serialise(const http7_assertion_t*a,char*buf,size_t cap){
 if(!a||!buf||cap==0)return -1;
 if(strcmp(http7_assertion_status_name(a->status),"UNKNOWN")==0)return -2;
 if(!valid_field(a->statement,sizeof(a->statement),1)||
    !valid_field(a->source,sizeof(a->source),a->status==HTTP7_ASSERT_FACTUAL_DOCUMENTED)||
    !valid_field(a->source_date,sizeof(a->source_date),0)||
    !valid_field(a->author,sizeof(a->author),a->status==HTTP7_ASSERT_USER_AUTHORED)||
    !valid_field(a->document_reference,sizeof(a->document_reference),0))return -3;
 int n=snprintf(buf,cap,"%s\t%s\t%s\t%s\t%s\t%s\n",http7_assertion_status_name(a->status),
 a->statement,a->source,a->source_date,a->author,a->document_reference);
 return n>=0&&(size_t)n<cap?n:-4;
}
int http7_assertion_parse(const char*line,size_t len,http7_assertion_t*out){
 if(!line||!out||len==0||len>=HTTP7_MAX_FIELD*5u+512u)return -1;
 if(memchr(line,'\0',len)!=NULL)return -2;
 for(size_t i=0;i<len;++i){unsigned char ch=(unsigned char)line[i];if((ch<0x20&&ch!='\t'&&!(ch=='\n'&&i==len-1))||ch==0x7f)return -3;}
 char copy[HTTP7_MAX_FIELD*5u+513u];memcpy(copy,line,len);copy[len]='\0';
 if(len&&copy[len-1]=='\n')copy[--len]='\0';
 if(len&&copy[len-1]=='\r')return -3;
 char*fields[6];size_t lengths[6];size_t start=0;int count=0;
 for(size_t i=0;i<=len;++i){
  if(i<len&&copy[i]!='\t')continue;
  if(count>=6)return -4;
  fields[count]=copy+start;lengths[count]=i-start;
  if(lengths[count]==0&&count==1)return -5;
  copy[i]='\0';++count;start=i+1;
 }
 if(count!=6)return -6;
 http7_assertion_status_t st=HTTP7_ASSERT_UNVERIFIED;int found=0;
 for(int i=0;i<=5;++i){const char*n=http7_assertion_status_name((http7_assertion_status_t)i);
  if(strlen(n)==lengths[0]&&memcmp(fields[0],n,lengths[0])==0){st=(http7_assertion_status_t)i;found=1;break;}}
 if(!found)return -7;
 memset(out,0,sizeof(*out));out->status=st;
 char*dst[5]={out->statement,out->source,out->source_date,out->author,out->document_reference};
 size_t caps[5]={sizeof(out->statement),sizeof(out->source),sizeof(out->source_date),sizeof(out->author),sizeof(out->document_reference)};
 for(int i=0;i<5;++i){
  size_t n=lengths[i+1];if(n>=caps[i])return -8;
  memcpy(dst[i],fields[i+1],n);dst[i][n]='\0';
  int required=(i==0)||(i==1&&st==HTTP7_ASSERT_FACTUAL_DOCUMENTED)||(i==3&&st==HTTP7_ASSERT_USER_AUTHORED);
  if(!valid_field(dst[i],caps[i],required))return -9;
 }
 return 0;
}
