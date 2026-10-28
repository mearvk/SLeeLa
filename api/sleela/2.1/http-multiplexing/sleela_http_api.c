#include "sleela_http_api.h"
#include <stdio.h>
#include <string.h>

static int valid_decimal(const char *s){ size_t n=0; if(!s||!*s)return 0; while(s[n]){if(s[n]<'0'||s[n]>'9')return 0; if(++n>48)return 0;} return n>0; }
static int dec_to_bytes(uint8_t out[20], const char *s){ size_t i,j; uint8_t b[20]={0}; if(!valid_decimal(s))return 0; for(i=0;s[i];++i){ unsigned carry=(unsigned)(s[i]-'0'); for(j=20;j-- >0;){unsigned v=(unsigned)b[j]*10u+carry; b[j]=(uint8_t)(v&255u); carry=v>>8;} if(carry)return 0;} memcpy(out,b,20); return 1; }
static int bytes_to_dec(const uint8_t in[20], char *out, size_t cap){ uint8_t d[50]={0}; size_t n=1,i,j; if(!in||!out||cap<2)return 0; for(i=0;i<20;++i){unsigned carry=in[i]; for(j=n;j-- >0;){unsigned v=(unsigned)d[j]*256u+carry; d[j]=(uint8_t)(v%10u); carry=v/10u;} while(carry){if(n>=50)return 0; memmove(d+1,d,n); d[0]=(uint8_t)(carry%10u); carry/=10u; ++n;}} while(n>1&&d[0]==0){memmove(d,d+1,--n);} if(n>48||n+1>cap)return 0; for(i=0;i<n;++i)out[i]=(char)('0'+d[i]); out[n]=0; return 1; }
static int less_than_10e48(const char *s){ size_t n=strlen(s); if(n<48)return 1; if(n>48)return 0; return strcmp(s,"100000000000000000000000000000000000000000000000")<0; }
int sleela_http_port_from_decimal(sleela_http_port_t *o,const char*s){char t[64]; if(!o||!dec_to_bytes(o->bytes,s))return 0; return bytes_to_dec(o->bytes,t,sizeof(t))&&less_than_10e48(t);}
int sleela_http_port_to_decimal(const sleela_http_port_t*p,char*o,size_t n){return p&&bytes_to_dec(p->bytes,o,n)&&less_than_10e48(o);}
int sleela_http_port_is_valid(const sleela_http_port_t*p){char t[64];return p&&bytes_to_dec(p->bytes,t,sizeof(t))&&less_than_10e48(t);}
int sleela_http_port_encode(const sleela_http_port_t*p,uint8_t o[20]){if(!p||!o||!sleela_http_port_is_valid(p))return 0;memcpy(o,p->bytes,20);return 1;}
int sleela_http_port_decode(sleela_http_port_t*o,const uint8_t i[20]){if(!o||!i)return 0;memcpy(o->bytes,i,20);return sleela_http_port_is_valid(o);}
sleela_http_multiplex_mode_t sleela_http_multiplex_mode_for(sleela_http_version_t v){switch(v){case SLEELA_HTTP_2_0:case SLEELA_HTTP_2_1:return SLEELA_HTTP_MULTIPLEX_HTTP2_STREAM;case SLEELA_HTTP_3_0:return SLEELA_HTTP_MULTIPLEX_HTTP3_STREAM;default:return SLEELA_HTTP_MULTIPLEX_REQUEST;}}
sleela_http_download_mode_t sleela_http_download_mode_for_size(uint64_t n){return n>SLEELA_HTTP_DOWNLOAD_THRESHOLD_BYTES?SLEELA_HTTP_DOWNLOAD_RESUMABLE:SLEELA_HTTP_DOWNLOAD_STANDARD;}
static void cp(char*d,size_t n,const char*s){if(!d||!n)return;if(!s)s="";snprintf(d,n,"%s",s);}
void sleela_http_download_resume_init(sleela_http_download_resume_t*r,const char*s,const char*t,const char*f,const char*n,uint64_t z){if(!r)return;memset(r,0,sizeof(*r));cp(r->session_id,sizeof(r->session_id),s);cp(r->datetime,sizeof(r->datetime),t);cp(r->file_id,sizeof(r->file_id),f);cp(r->file_name,sizeof(r->file_name),n);r->total_size=z;}
int sleela_http_download_resume_set_position(sleela_http_download_resume_t*r,uint64_t i,uint64_t o){if(!r||o>r->total_size)return 0;r->index=i;r->offset=o;return 1;}
int sleela_http_download_resume_matches(const sleela_http_download_resume_t*r,const char*f,uint64_t i,uint64_t o){return r&&f&&strcmp(r->file_id,f)==0&&r->index==i&&r->offset==o;}