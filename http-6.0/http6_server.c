/* http6_server.c — SLeeLa HTTP 6.0 portable server
 * Platforms: Linux, macOS, Windows 10+
 * 20 frame types incl. Consolidated Friends' Bet + Teamster Debate.
 */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif
#include "http6_server.h"
#include "http_negotiation.h"
#include "../impl/core/sleela_net.h"
#include "../preferred-routers/preferred_router.h"
#include "../resolver/resolver.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <poll.h>
#include <unistd.h>
#endif

static volatile int g_stop=0;
void h6s_server_stop(void){g_stop=1;}
#ifndef _WIN32
static void h6s_sig(int s){(void)s;g_stop=1;}
#endif

static void str_lower(char *s){for(;*s;++s)*s=(char)tolower((unsigned char)*s);}
static const char *str_ltrim(const char *s){while(*s==' '||*s=='\t')++s;return s;}
static const char *find_crlf(const char *p, const char *end){
    if(!p||!end||p>end)return NULL;
    while((size_t)(end-p)>=2){if(p[0]=='\r'&&p[1]=='\n')return p;++p;}
    return NULL;
}
static int is_token_char(unsigned char c){
    if((c>='0'&&c<='9')||(c>='A'&&c<='Z')||(c>='a'&&c<='z'))return 1;
    /* RFC token punctuation, expressed numerically to keep quoting unambiguous. */
    switch(c){case 33:case 35:case 36:case 37:case 38:case 39:case 42:
    case 43:case 45:case 46:case 94:case 95:case 96:case 124:case 126:return 1;
    default:return 0;}
}
static int valid_header_value(const char *s){
    for(;*s;++s){unsigned char c=(unsigned char)*s;if((c<0x20&&c!='\t')||c==0x7f)return 0;}
    return 1;
}

static int header_name_equal_ci(const char*a,const char*b){
    if(!a||!b)return 0;
    while(*a&&*b){if(tolower((unsigned char)*a)!=tolower((unsigned char)*b))return 0;++a;++b;}
    return *a=='\0'&&*b=='\0';
}
static int response_header_reserved(const char*n){
    return header_name_equal_ci(n,"content-length")||
           header_name_equal_ci(n,"transfer-encoding")||
           header_name_equal_ci(n,"connection")||
           header_name_equal_ci(n,"server")||
           header_name_equal_ci(n,"content-type")||
           header_name_equal_ci(n,"x-sleela-version")||
           header_name_equal_ci(n,"x-sleela-accept");
}

static const char *reason_phrase(int c){
    switch(c){case 200:return "OK";case 400:return "Bad Request";case 404:return "Not Found";
    case 413:return "Content Too Large";case 500:return "Internal Server Error";default:return "Error";}
}

static int recv_headers(SLNetHandle fd,char *buf,size_t cap){
    size_t t=0;while(t<cap){SLNetCount n=slnet_read(fd,buf+t,cap-t);
        if(n<=0)return t>0?(int)t:-1;t+=(size_t)n;
        if(t>=4){const char*lim=buf+(t>H6S_MAX_HEADER?H6S_MAX_HEADER:t);
            for(const char*p=buf+(t-(size_t)n>3?t-(size_t)n-3:0);(size_t)(lim-p)>=4;++p)
                if(p[0]=='\r'&&p[1]=='\n'&&p[2]=='\r'&&p[3]=='\n')return(int)t;}}
    return -1;
}

static const char*get_hdr(const h6s_request*r,const char*n){
    for(int i=0;i<r->header_count;++i)if(strcmp(r->headers[i].name,n)==0)return r->headers[i].value;return NULL;}
static int count_hdr(const h6s_request*r,const char*n){
    int count=0;for(int i=0;i<r->header_count;++i)if(strcmp(r->headers[i].name,n)==0)++count;return count;}
static int parse_content_length(const char*s,size_t*out){
    size_t n=0;if(!s||!*s||!out)return -1;
    for(const unsigned char*p=(const unsigned char*)s;*p;++p){
        if(*p<'0'||*p>'9')return -1;
        unsigned digit=(unsigned)(*p-'0');
        if(n>(SIZE_MAX-digit)/10)return -1;
        n=n*10+digit;
    }
    *out=n;return 0;}

static int parse_request(const char*raw,size_t raw_len,h6s_request*req,const char**bs,size_t*hb){
    const char*p=raw,*end=raw+raw_len,*le;memset(req,0,sizeof(*req));
    le=find_crlf(p,end);if(!le)return -1;
    {const char*sp=memchr(p,' ',(size_t)(le-p));if(!sp||sp==p||(size_t)(sp-p)>=sizeof(req->method))return -1;
     for(const char*q=p;q<sp;++q)if(!is_token_char((unsigned char)*q))return -1;
     memcpy(req->method,p,(size_t)(sp-p));req->method[sp-p]='\0';p=sp+1;}
    {const char*sp=memchr(p,' ',(size_t)(le-p));if(!sp||sp==p||(size_t)(sp-p)>=sizeof(req->uri))return -1;
     for(const char*q=p;q<sp;++q)if((unsigned char)*q<=0x20||(unsigned char)*q==0x7f)return -1;
     memcpy(req->uri,p,(size_t)(sp-p));req->uri[sp-p]='\0';p=sp+1;}
    {size_t vl=(size_t)(le-p);if(vl>=sizeof(req->version))return -1;
     if(!((vl==8&&memcmp(p,"HTTP/1.0",8)==0)||(vl==8&&memcmp(p,"HTTP/1.1",8)==0)))return -1;
     memcpy(req->version,p,vl);req->version[vl]='\0';}
    p=le+2;
    while((size_t)(end-p)>=2&&!(p[0]=='\r'&&p[1]=='\n')){le=find_crlf(p,end);if(!le)return -1;
        const char*c=memchr(p,':',(size_t)(le-p));if(!c||c==p||req->header_count>=H6S_MAX_HEADERS)return -1;
        h6s_header*h=&req->headers[req->header_count];size_t nl=(size_t)(c-p);
        if(nl>=sizeof(h->name))return -1;
        for(const char*q=p;q<c;++q)if(!is_token_char((unsigned char)*q))return -1;
        memcpy(h->name,p,nl);h->name[nl]='\0';str_lower(h->name);
        const char*v=str_ltrim(c+1);size_t vl=(size_t)(le-v);if(vl>=sizeof(h->value))return -1;
        for(const char*q=v;q<le;++q){unsigned char ch=(unsigned char)*q;if((ch<0x20&&ch!='\t')||ch==0x7f)return -1;}
        memcpy(h->value,v,vl);h->value[vl]='\0';if(!valid_header_value(h->value))return -1;
        ++req->header_count;p=le+2;}
    if((size_t)(end-p)<2||p[0]!='\r'||p[1]!='\n')return -1;
    p+=2;
    *hb=(size_t)(p-raw);*bs=p;
    if((strcmp(req->version,"HTTP/1.1")==0 &&
        (count_hdr(req,"host")!=1 || !get_hdr(req,"host") || !get_hdr(req,"host")[0])) ||
       count_hdr(req,"host")>1 ||
       count_hdr(req,"x-sleela-version")>1 || count_hdr(req,"x-sleela-accept")>1 ||
       count_hdr(req,"x-sleela-port")>1 || count_hdr(req,"x-sleela-service-id")>1 ||
       count_hdr(req,"x-sleela-op-id")>1)return -1;
    {const char*v;
     v=get_hdr(req,"x-sleela-version");if(v)snprintf(req->sleela_version,sizeof(req->sleela_version),"%s",v);
     v=get_hdr(req,"x-sleela-accept");if(v)snprintf(req->sleela_accept,sizeof(req->sleela_accept),"%s",v);
     v=get_hdr(req,"x-sleela-port");if(v)snprintf(req->sleela_port,sizeof(req->sleela_port),"%s",v);
     v=get_hdr(req,"x-sleela-service-id");if(v)snprintf(req->service_id,sizeof(req->service_id),"%s",v);
     v=get_hdr(req,"x-sleela-op-id");if(v)snprintf(req->op_id,sizeof(req->op_id),"%s",v);}
    return 0;
}

int h6s_response_add_header(h6s_response*r,const char*n,const char*v){
    if(!r||!n||!v||r->header_count<0||r->header_count>=H6S_MAX_HEADERS||response_header_reserved(n))return -1;
    size_t nl=strlen(n),vl=strlen(v);
    if(nl==0||nl>=H6S_MAX_HNAME||vl>=H6S_MAX_HVALUE||!valid_header_value(v))return -1;
    for(size_t i=0;i<nl;++i)if(!is_token_char((unsigned char)n[i]))return -1;
    h6s_header*h=&r->headers[r->header_count];
    memcpy(h->name,n,nl+1);memcpy(h->value,v,vl+1);++r->header_count;return 0;
}

static int send_all(SLNetHandle fd,const char*d,size_t l){
    while(l>0){SLNetCount n=slnet_write(fd,d,l);if(n<=0)return -1;d+=n;l-=(size_t)n;}return 0;}

static int appendf(char *buf,size_t cap,size_t *used,const char *fmt,...){
    va_list ap;int n;
    if(!buf||!used||*used>=cap)return -1;
    va_start(ap,fmt);n=vsnprintf(buf+*used,cap-*used,fmt,ap);va_end(ap);
    if(n<0||(size_t)n>=cap-*used)return -1;
    *used+=(size_t)n;return 0;
}

static int send_response(SLNetHandle fd,const h6s_request*req,const h6s_response*resp,const char*sn){
    char buf[H6S_MAX_HEADER];size_t used=0;
    const char*version=(req&&req->version[0])?req->version:"HTTP/1.0";
    const char*reason;
    if(!req||!resp||resp->status_code<100||resp->status_code>599||
       resp->header_count<0||resp->header_count>H6S_MAX_HEADERS||
       resp->body_length>H6S_MAX_BODY||(!resp->body&&resp->body_length>0))return -1;
    if(strcmp(version,"HTTP/1.0")!=0&&strcmp(version,"HTTP/1.1")!=0)return -1;
    reason=resp->reason?resp->reason:reason_phrase(resp->status_code);
    if(!sn||!valid_header_value(sn)||!valid_header_value(reason)||
       (resp->content_type&&!valid_header_value(resp->content_type)))return -1;
    if(appendf(buf,sizeof(buf),&used,"%s %d %s\r\n",version,resp->status_code,reason)!=0||
       appendf(buf,sizeof(buf),&used,"Server: %s\r\n",sn)!=0)return -1;
    if(resp->content_type&&appendf(buf,sizeof(buf),&used,"Content-Type: %s\r\n",resp->content_type)!=0)return -1;
    if(appendf(buf,sizeof(buf),&used,"Content-Length: %zu\r\n",resp->body_length)!=0)return -1;
    for(int i=0;i<resp->header_count;++i){
        const h6s_header*h=&resp->headers[i];
        if(!memchr(h->name,'\0',sizeof(h->name))||!memchr(h->value,'\0',sizeof(h->value))||
           !h->name[0]||response_header_reserved(h->name)||!valid_header_value(h->value))return -1;
        for(const unsigned char*p=(const unsigned char*)h->name;*p;++p)if(!is_token_char(*p))return -1;
        if(appendf(buf,sizeof(buf),&used,"%s: %s\r\n",h->name,h->value)!=0)return -1;
    }
    if(appendf(buf,sizeof(buf),&used,"X-SLeeLa-Version: 6.0\r\nX-SLeeLa-Accept: 6.0, 5.0, 4.0, 3.0, 2.1, 1.1, 1.0\r\nConnection: close\r\n\r\n")!=0)return -1;
    if(send_all(fd,buf,used)!=0)return -1;
    if(resp->body_length>0)return send_all(fd,resp->body,resp->body_length);
    return 0;
}

int h6s_download_should_resume(uint64_t cl){return cl>H6S_DOWNLOAD_THRESHOLD;}

static int download_field_ok(const char*s,size_t cap,int nonempty,size_t*length){
    const char*z;
    if(!s||!cap)return 0;
    z=(const char*)memchr(s,'\0',cap);
    if(!z)return 0;
    size_t n=(size_t)(z-s);
    if(nonempty&&n==0)return 0;
    for(size_t i=0;i<n;++i){
        unsigned char ch=(unsigned char)s[i];
        if(ch=='|'||ch=='\r'||ch=='\n'||ch<0x20||ch==0x7f)return 0;
    }
    if(length)*length=n;
    return 1;
}
static int parse_u64_field(const char*s,uint64_t*out){
    char*end=NULL;unsigned long long value;
    if(!s||!*s||!out||*s=='-'||*s=='+')return -1;
    errno=0;value=strtoull(s,&end,10);
    if(errno==ERANGE||end==s||!end||*end!='\0')return -1;
    *out=(uint64_t)value;return 0;
}
int h6s_download_serialise(const h6s_download_state*st,char*buf,size_t cap){
    int n;
    if(!st||!buf||cap<512||st->offset>st->total_size||
       !download_field_ok(st->session_id,sizeof(st->session_id),1,NULL)||
       !download_field_ok(st->datetime,sizeof(st->datetime),1,NULL)||
       !download_field_ok(st->file_id,sizeof(st->file_id),1,NULL)||
       !download_field_ok(st->file_name,sizeof(st->file_name),1,NULL))return -1;
    n=snprintf(buf,cap,"%s | %s | %s | %s | %llu | %llu | %llu",
        st->session_id,st->datetime,st->file_id,st->file_name,
        (unsigned long long)st->index,(unsigned long long)st->offset,
        (unsigned long long)st->total_size);
    return(n>0&&(size_t)n<cap)?0:-1;
}
int h6s_download_parse(const char*line,h6s_download_state*out){
    char copy[2048];char*fields[7];char*p;size_t len;uint64_t index,offset,total;
    if(!line||!out)return -1;
    len=strlen(line);if(len==0||len>=sizeof(copy))return -1;
    memcpy(copy,line,len+1);fields[0]=copy;p=copy;
    for(int i=1;i<7;++i){
        char*sep=strchr(p,'|');if(!sep)return -1;
        *sep='\0';p=sep+1;fields[i]=p;
    }
    if(strchr(p,'|'))return -1;
    for(int i=0;i<7;++i){
        while(*fields[i]==' '||*fields[i]=='\t')++fields[i];
        size_t field_len=strlen(fields[i]);
        while(field_len>0&&(fields[i][field_len-1]==' '||fields[i][field_len-1]=='\t'))
            fields[i][--field_len]='\0';
        if(!*fields[i])return -1;
        for(const unsigned char*q=(const unsigned char*)fields[i];*q;++q)
            if(*q<0x20||*q==0x7f)return -1;
    }
    if(!download_field_ok(fields[0],sizeof(out->session_id),1,NULL)||
       !download_field_ok(fields[1],sizeof(out->datetime),1,NULL)||
       !download_field_ok(fields[2],sizeof(out->file_id),1,NULL)||
       !download_field_ok(fields[3],sizeof(out->file_name),1,NULL)||
       parse_u64_field(fields[4],&index)!=0||parse_u64_field(fields[5],&offset)!=0||
       parse_u64_field(fields[6],&total)!=0||offset>total)return -1;
    memset(out,0,sizeof(*out));
    if(snprintf(out->session_id,sizeof(out->session_id),"%s",fields[0])>=(int)sizeof(out->session_id)||
       snprintf(out->datetime,sizeof(out->datetime),"%s",fields[1])>=(int)sizeof(out->datetime)||
       snprintf(out->file_id,sizeof(out->file_id),"%s",fields[2])>=(int)sizeof(out->file_id)||
       snprintf(out->file_name,sizeof(out->file_name),"%s",fields[3])>=(int)sizeof(out->file_name))return -1;
    out->index=index;out->offset=offset;out->total_size=total;return 0;
}

/* ── built-in routes ────────────────────────────────────────────────── */

static int builtin_health(h6s_response*r){
    static const char b[]="SLeeLa HTTP/6.0 OK\n";
    r->status_code=200;r->content_type="text/plain; charset=utf-8";r->body=b;r->body_length=sizeof(b)-1;return 0;}

static int builtin_status(h6s_response*r){
    static char b[1024];
    int n=snprintf(b,sizeof(b),
        "{\"server\":\"SLeeLa\",\"http_generation\":\"6.0\","
         "\"vm_generation\":6,\"vm_name\":\"Director\",\"vm_path\":\"/6\","
         "\"platform\":\"%s\",\"frame_version\":%u,"
         "\"frame_types\":[\"OPEN\",\"DATA\",\"END\",\"RESET\",\"WINDOW\","
           "\"PING\",\"PONG\",\"RESUME\",\"CAPSULE\","
           "\"FRIENDS_PACK\",\"BONUS_OFFER\",\"FP_UPDATE\",\"AUDIT\","
           "\"CONSOLIDATED_FRIENDS_BET\",\"TEAMSTER_DEBATE\","
           "\"CONSOLIDATE_IQ\",\"TEAM_AREA\",\"DEBATE_TOPIC\","
           "\"DEBATE_POSITION\",\"RECIPIENT_LABEL\"],"
         "\"consolidated_friends_bet\":true,"
         "\"teamster_debate\":true,"
         "\"lower_http_compatibility\":true}\n",
        slnet_platform_name(),HTTP6_VERSION);
    r->status_code=200;r->content_type="application/json";r->body=b;r->body_length=(size_t)n;return 0;}

static int builtin_negotiate(const h6s_request*req,h6s_response*r,int fb){
    const char*neg=sleela_http_negotiate(req->sleela_version[0]?req->sleela_version:NULL,
        req->sleela_accept[0]?req->sleela_accept:NULL,fb);
    static char b[256];int n;
    if(neg){n=snprintf(b,sizeof(b),"{\"negotiated\":\"%s\",\"ok\":true}\n",neg);r->status_code=200;}
    else{n=snprintf(b,sizeof(b),"{\"negotiated\":null,\"ok\":false}\n");r->status_code=400;}
    r->content_type="application/json";r->body=b;r->body_length=(size_t)n;return 0;}

static int builtin_h6_frame(const h6s_request*req,h6s_response*resp,h6s_frame_handler fh,void*ud){
    if(!req->body||req->body_length<HTTP6_FRAME_HEADER_SIZE){
        resp->status_code=400;resp->content_type="text/plain";resp->body="Frame body required\n";resp->body_length=20;return 0;}
    http6_frame_view_t frame;size_t consumed;
    if(http6_frame_decode((const uint8_t*)req->body,req->body_length,&frame,&consumed)!=0 ||
       consumed!=req->body_length){
        resp->status_code=400;resp->content_type="text/plain";resp->body="Invalid H6 frame\n";resp->body_length=17;return 0;}
    if(fh){
        static uint8_t rp[4096];size_t rl=0;
        if(frame.header.sequence==UINT64_MAX){resp->status_code=400;resp->content_type="text/plain";
            resp->body="Invalid sequence\n";resp->body_length=17;return 0;}
        if(fh(&frame,rp,sizeof(rp),&rl,ud)!=0 || rl>sizeof(rp)){resp->status_code=500;resp->content_type="text/plain";
            resp->body="Frame handler error\n";resp->body_length=20;return 0;}
        http6_frame_header_t rh={0};rh.version=HTTP6_VERSION;
        rh.type=(frame.header.type==HTTP6_FRAME_PING)?HTTP6_FRAME_PONG:HTTP6_FRAME_DATA;
        rh.flags=HTTP6_FLAG_FIN;rh.stream_id=frame.header.stream_id;
        rh.request_id=frame.header.request_id;rh.sequence=frame.header.sequence+1;
        rh.payload_length=(uint32_t)rl;
        static uint8_t wire[4096+HTTP6_FRAME_HEADER_SIZE];size_t written;
        if(http6_frame_encode(&rh,rp,wire,sizeof(wire),&written)!=0){
            resp->status_code=500;resp->content_type="text/plain";resp->body="Encode error\n";resp->body_length=13;return 0;}
        resp->status_code=200;resp->content_type="application/x-sleela-h6";
        resp->body=(const char*)wire;resp->body_length=written;return 0;}
    static char result[4096];int n;
    n=snprintf(result,sizeof(result),"{\"frame_type\":\"%s\",\"stream_id\":%llu,\"request_id\":%llu,"
        "\"sequence\":%llu,\"flags\":%u,\"payload_length\":%u}\n",
        http6_frame_type_name((http6_frame_type_t)frame.header.type),
        (unsigned long long)frame.header.stream_id,(unsigned long long)frame.header.request_id,
        (unsigned long long)frame.header.sequence,(unsigned)frame.header.flags,(unsigned)frame.header.payload_length);
    resp->status_code=200;resp->content_type="application/json";resp->body=result;resp->body_length=(size_t)n;return 0;}

/* ── connection handler ─────────────────────────────────────────────── */

static void handle_connection(SLNetHandle fd,const h6s_server_config*cfg){
    size_t rc=H6S_MAX_HEADER+4096;char*raw=malloc(rc);if(!raw){slnet_close(fd);return;}
    int got=recv_headers(fd,raw,rc);if(got<=0){free(raw);slnet_close(fd);return;}
    h6s_request req;h6s_response resp;const char*bs=NULL;size_t hb=0;
    memset(&resp,0,sizeof(resp));resp.status_code=200;
    if(parse_request(raw,(size_t)got,&req,&bs,&hb)!=0){
        resp.status_code=400;resp.content_type="text/plain";resp.body="Bad Request\n";resp.body_length=12;
        h6s_request bad;memset(&bad,0,sizeof(bad));memcpy(bad.version,"HTTP/1.0",9);
        send_response(fd,&bad,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
    {
        const char*cl=get_hdr(&req,"content-length");
        const char*te=get_hdr(&req,"transfer-encoding");
        size_t clen=0;
        if(count_hdr(&req,"transfer-encoding")>0 ||
           (count_hdr(&req,"content-length")>1) ||
           (cl&&parse_content_length(cl,&clen)!=0)){
            resp.status_code=400;resp.content_type="text/plain";resp.body="Invalid message framing\n";resp.body_length=24;
            send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;
        }
        if(te){resp.status_code=400;resp.content_type="text/plain";resp.body="Transfer-Encoding unsupported\n";resp.body_length=30;
            send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
        if(cl){
            if(clen>H6S_MAX_BODY){resp.status_code=413;resp.content_type="text/plain";
                resp.body="Content Too Large\n";resp.body_length=18;
                send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
            size_t already=(size_t)got-hb;
            size_t filled=already<clen?already:clen;
            req.body=(char*)malloc(clen+1);
            if(!req.body){resp.status_code=500;resp.content_type="text/plain";
                resp.body="Internal Server Error\n";resp.body_length=22;
                send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
            if(filled)memcpy(req.body,bs,filled);
            while(filled<clen){SLNetCount n=slnet_read(fd,req.body+filled,clen-filled);
                if(n<=0)break;filled+=(size_t)n;}
            if(filled!=clen){resp.status_code=400;resp.content_type="text/plain";
                resp.body="Incomplete request body\n";resp.body_length=24;
                send_response(fd,&req,&resp,cfg->server_name);free(req.body);free(raw);slnet_close(fd);return;}
            req.body_length=clen;req.body[clen]='\0';
        }
    }
    free(raw);raw=NULL;
    int handled=0;
    if(strcmp(req.uri,"/__sleela/health")==0){builtin_health(&resp);handled=1;}
    else if(strcmp(req.uri,"/__sleela/status")==0){builtin_status(&resp);handled=1;}
    else if(strcmp(req.uri,"/__sleela/negotiate")==0){builtin_negotiate(&req,&resp,cfg->allow_negotiation_fallback);handled=1;}
    else if(strcmp(req.uri,"/__sleela/h6")==0&&strcmp(req.method,"POST")==0){builtin_h6_frame(&req,&resp,cfg->frame_handler,cfg->user_data);handled=1;}
    if(!handled&&cfg->handler){if(cfg->handler(&req,&resp,cfg->user_data)!=0){
        resp.status_code=500;resp.content_type="text/plain";resp.body="Internal Server Error\n";resp.body_length=22;}}
    else if(!handled){resp.status_code=404;resp.content_type="text/plain";resp.body="Not Found\n";resp.body_length=10;}
    send_response(fd,&req,&resp,cfg->server_name);free(req.body);slnet_close(fd);}

static int portable_poll_readable(SLNetHandle fd,int ms){
#ifdef _WIN32
    WSAPOLLFD pfd;pfd.fd=(SOCKET)fd;pfd.events=POLLIN;pfd.revents=0;return WSAPoll(&pfd,1,ms)>0&&(pfd.revents&POLLIN);
#else
    struct pollfd pfd;pfd.fd=(int)fd;pfd.events=POLLIN;pfd.revents=0;return poll(&pfd,1,ms)>0&&(pfd.revents&POLLIN);
#endif
}

void h6s_server_config_init(h6s_server_config*cfg){
    if(!cfg)return;memset(cfg,0,sizeof(*cfg));cfg->port=8085;cfg->backlog=128;
    cfg->bind_address="127.0.0.1";cfg->server_name="SLeeLa-HTTP/6.0";cfg->allow_negotiation_fallback=0;}

int h6s_server_run(const h6s_server_config*cfg){
    if(!cfg)return -1;g_stop=0;
    if(slnet_startup()!=0){fprintf(stderr,"h6s: slnet_startup failed\n");return -1;}
    sleela_preferred_router_startup(NULL,"HTTP/6.0");
    const char*bind_host=(cfg->bind_address&&cfg->bind_address[0])?cfg->bind_address:"127.0.0.1";
    SLNetHandle listener=slnet_listen_on(bind_host,cfg->port,cfg->backlog);
    if(!slnet_valid(listener)){fprintf(stderr,"h6s: listen failed on port %u\n",(unsigned)cfg->port);slnet_shutdown();return -1;}
    fprintf(stderr,"SLeeLa HTTP/6.0 listening on %s:%u  [%s]\n",bind_host,(unsigned)cfg->port,slnet_platform_name());
#ifndef _WIN32
    signal(SIGINT,h6s_sig);signal(SIGTERM,h6s_sig);
#endif
    while(!g_stop){if(!portable_poll_readable(listener,500))continue;
        SLNetHandle c=slnet_accept(listener);if(!slnet_valid(c))continue;handle_connection(c,cfg);}
    slnet_close(listener);slnet_shutdown();fprintf(stderr,"SLeeLa HTTP/6.0 stopped.\n");return 0;}

/* ── self-test ──────────────────────────────────────────────────────── */

int h6s_self_test(void){
    int f=0;
    /* 1. HTTP parse */
    {const char raw[]="GET /__sleela/health HTTP/1.1\r\nHost: localhost\r\nX-SLeeLa-Version: 6.0\r\n\r\n";
     h6s_request req;const char*bs;size_t hb;
     if(parse_request(raw,sizeof(raw)-1,&req,&bs,&hb)!=0||strcmp(req.sleela_version,"6.0")!=0){
         fprintf(stderr,"FAIL: HTTP parse\n");++f;}}
    /* 1a. Reject malformed HTTP headers and ambiguous message framing. */
    {
        const char duplicate_cl[]="POST /__sleela/h6 HTTP/1.1\r\nHost: localhost\r\nContent-Length: 0\r\nContent-Length: 0\r\n\r\n";
        h6s_request req;const char*bs;size_t hb;
        if(parse_request(duplicate_cl,sizeof(duplicate_cl)-1,&req,&bs,&hb)!=0 ||
           count_hdr(&req,"content-length")!=2){
            fprintf(stderr,"FAIL: duplicate Content-Length fixture parse\\n");++f;
        }
        size_t length_value=0;
        if(parse_content_length("12x",&length_value)==0 ||
           parse_content_length("-1",&length_value)==0 ||
           parse_content_length("184467440737095516160000",&length_value)==0 ||
           parse_content_length("12",&length_value)!=0 || length_value!=12){
            fprintf(stderr,"FAIL: strict Content-Length parser\\n");++f;
        }
        const char invalid_value[]="GET / HTTP/1.1\r\nX-Test: bad\001value\r\n\r\n";
        if(parse_request(invalid_value,sizeof(invalid_value)-1,&req,&bs,&hb)==0){
            fprintf(stderr,"FAIL: control character in header accepted\\n");++f;
        }
    }
    /* 2. Frame encode/decode — CONSOLIDATED_FRIENDS_BET */
    {http6_frame_header_t h={0};h.version=HTTP6_VERSION;h.type=HTTP6_FRAME_CONSOLIDATED_FRIENDS_BET;
     h.flags=HTTP6_FLAG_USER_AUTHORED;h.stream_id=1;h.request_id=42;h.sequence=1;h.payload_length=7;
     const uint8_t payload[]="bet-one";uint8_t wire[128];size_t written;
     if(http6_frame_encode(&h,payload,wire,sizeof(wire),&written)!=0){fprintf(stderr,"FAIL: encode\n");++f;}
     else{http6_frame_view_t v;size_t consumed;
         if(http6_frame_decode(wire,written,&v,&consumed)!=0){fprintf(stderr,"FAIL: decode\n");++f;}
         else if(v.header.type!=HTTP6_FRAME_CONSOLIDATED_FRIENDS_BET||v.header.request_id!=42||
                 v.header.payload_length!=7||memcmp(v.payload,"bet-one",7)!=0){
             fprintf(stderr,"FAIL: round-trip\n");++f;}}}
    /* 3. TEAMSTER_DEBATE frame */
    {http6_frame_header_t h={0};h.version=HTTP6_VERSION;h.type=HTTP6_FRAME_TEAMSTER_DEBATE;
     h.flags=0;h.stream_id=2;h.request_id=99;h.sequence=3;h.payload_length=6;
     const uint8_t payload[]="debate";uint8_t wire[128];size_t written;
     if(http6_frame_encode(&h,payload,wire,sizeof(wire),&written)!=0){fprintf(stderr,"FAIL: debate encode\n");++f;}
     else{http6_frame_view_t v;size_t consumed;
         if(http6_frame_decode(wire,written,&v,&consumed)!=0||v.header.type!=HTTP6_FRAME_TEAMSTER_DEBATE){
             fprintf(stderr,"FAIL: debate decode\n");++f;}}}
    /* 4. All 20 frame type names */
    {for(int t=1;t<=20;++t){const char*n=http6_frame_type_name((http6_frame_type_t)t);
         if(!n||strcmp(n,"UNKNOWN")==0){fprintf(stderr,"FAIL: type %d name\n",t);++f;}}}
    /* 5. Reserved frame flags and truncated payload must fail closed. */
    {http6_frame_header_t h={0};h.version=HTTP6_VERSION;h.type=HTTP6_FRAME_DATA;
     h.flags=0x80;h.payload_length=0;uint8_t wire[64];size_t written=0;
     if(http6_frame_encode(&h,NULL,wire,sizeof(wire),&written)==0){
         fprintf(stderr,"FAIL: reserved flag accepted\\n");++f;}
     h.flags=0;h.payload_length=1;const uint8_t payload[]={'x'};
     if(http6_frame_encode(&h,payload,wire,sizeof(wire),&written)!=0){
         fprintf(stderr,"FAIL: truncated fixture encode\\n");++f;}
     else{http6_frame_view_t v;size_t consumed;
         if(http6_frame_decode(wire,written-1,&v,&consumed)==0){
             fprintf(stderr,"FAIL: truncated payload accepted\\n");++f;}}}
    /* 6. Download round-trip */
    {h6s_download_state st={0};snprintf(st.session_id,sizeof(st.session_id),"h6-s");
     snprintf(st.datetime,sizeof(st.datetime),"2026-10-15");
     snprintf(st.file_id,sizeof(st.file_id),"f6");snprintf(st.file_name,sizeof(st.file_name),"big.dat");
     st.index=4;st.offset=200000000ULL;st.total_size=600000000ULL;
     char buf[512];if(h6s_download_serialise(&st,buf,sizeof(buf))!=0){fprintf(stderr,"FAIL: dl ser\n");++f;}
     else{h6s_download_state p;if(h6s_download_parse(buf,&p)!=0||p.index!=4){fprintf(stderr,"FAIL: dl parse\n");++f;}}}
    /* 7. Download parser rejects malformed numbers, delimiters, and offsets. */
    {h6s_download_state st;
     if(h6s_download_parse("session | date | file | name | 4x | 2 | 10",&st)==0 ||
        h6s_download_parse("session | date | file | name | 4 | 11 | 10",&st)==0 ||
        h6s_download_parse("session | date | file | name | 4 | 2 | 10 | extra",&st)==0 ||
        h6s_download_parse("session | date | bad|file | name | 4 | 2 | 10",&st)==0){
         fprintf(stderr,"FAIL: malformed download state accepted\\n");++f;}}
    /* 8. Negotiation */
    {const char*r=sleela_http_negotiate("6.0","6.0, 5.0, 1.1",1);
     if(!r||strcmp(r,"6.0")!=0){fprintf(stderr,"FAIL: negotiate\n");++f;}
     r=sleela_http_negotiate("7.0","6.0, 1.1",1);
     if(!r||strcmp(r,"1.1")!=0){fprintf(stderr,"FAIL: fallback\n");++f;}
     r=sleela_http_negotiate("6.0x","6.0x",0);
     if(r){fprintf(stderr,"FAIL: unsupported version accepted\n");++f;}}

    if(f==0)fprintf(stderr,"h6s_self_test: all tests passed\n");
    else fprintf(stderr,"h6s_self_test: %d test(s) FAILED\n",f);
    return f;}
