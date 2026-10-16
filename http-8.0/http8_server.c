/* http8_server.c — SLeeLa HTTP 8.0 portable server. 28 frame types. */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif
#include "http8_server.h"
#include "http_negotiation.h"
#include "../impl/core/sleela_net.h"
#include "../preferred-routers/preferred_router.h"
#include "../resolver/resolver.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <poll.h>
#include <unistd.h>
#endif

static volatile int g_stop=0;
void h8s_server_stop(void){g_stop=1;}
#ifndef _WIN32
static void h8s_sig(int s){(void)s;g_stop=1;}
#endif

static void str_lower(char*s){for(;*s;++s)*s=(char)tolower((unsigned char)*s);}
static const char*str_ltrim(const char*s){while(*s==' '||*s=='\t')++s;return s;}
static const char*reason_phrase(int c){switch(c){case 200:return"OK";case 400:return"Bad Request";
    case 404:return"Not Found";case 413:return"Content Too Large";case 500:return"Internal Server Error";default:return"Error";}}

static int recv_headers(SLNetHandle fd,char*buf,size_t cap){
    size_t t=0;while(t<cap){SLNetCount n=slnet_read(fd,buf+t,cap-t);if(n<=0)return t>0?(int)t:-1;t+=(size_t)n;
    if(t>=4){const char*lim=buf+(t>H8S_MAX_HEADER?H8S_MAX_HEADER:t);
    for(const char*p=buf+(t-n>3?t-n-3:0);p+3<lim;++p)
        if(p[0]=='\r'&&p[1]=='\n'&&p[2]=='\r'&&p[3]=='\n')return(int)t;}}return -1;}
static const char*get_hdr(const h8s_request*r,const char*n){
    for(int i=0;i<r->header_count;++i)if(strcmp(r->headers[i].name,n)==0)return r->headers[i].value;return NULL;}

static int parse_request(const char*raw,size_t len,h8s_request*req,const char**bs,size_t*hb){
    const char*p=raw,*end=raw+len,*le;memset(req,0,sizeof(*req));
    le=strstr(p,"\r\n");if(!le)return -1;
    {const char*sp=memchr(p,' ',(size_t)(le-p));if(!sp||(size_t)(sp-p)>=sizeof(req->method))return -1;memcpy(req->method,p,(size_t)(sp-p));p=sp+1;}
    {const char*sp=memchr(p,' ',(size_t)(le-p));if(!sp||(size_t)(sp-p)>=sizeof(req->uri))return -1;memcpy(req->uri,p,(size_t)(sp-p));p=sp+1;}
    {size_t vl=(size_t)(le-p);if(vl>=sizeof(req->version))return -1;memcpy(req->version,p,vl);}
    p=le+2;
    while(p+1<end&&!(p[0]=='\r'&&p[1]=='\n')){le=strstr(p,"\r\n");if(!le)break;
        const char*c=memchr(p,':',(size_t)(le-p));if(!c||c==p||req->header_count>=H8S_MAX_HEADERS)return -1;
        h8s_header*h=&req->headers[req->header_count];size_t nl=(size_t)(c-p);
        if(nl>=sizeof(h->name))return -1;memcpy(h->name,p,nl);h->name[nl]='\0';str_lower(h->name);
        const char*v=str_ltrim(c+1);size_t vl=(size_t)(le-v);if(vl>=sizeof(h->value))return -1;
        memcpy(h->value,v,vl);h->value[vl]='\0';++req->header_count;p=le+2;}
    if(p+1<end&&p[0]=='\r'&&p[1]=='\n')p+=2;*hb=(size_t)(p-raw);*bs=p;
    {const char*v;v=get_hdr(req,"x-sleela-version");if(v)snprintf(req->sleela_version,sizeof(req->sleela_version),"%s",v);
     v=get_hdr(req,"x-sleela-accept");if(v)snprintf(req->sleela_accept,sizeof(req->sleela_accept),"%s",v);
     v=get_hdr(req,"x-sleela-port");if(v)snprintf(req->sleela_port,sizeof(req->sleela_port),"%s",v);}
    return 0;}

static int send_all(SLNetHandle fd,const char*d,size_t l){while(l>0){SLNetCount n=slnet_write(fd,d,l);if(n<=0)return -1;d+=n;l-=(size_t)n;}return 0;}
static int send_response(SLNetHandle fd,const h8s_request*req,const h8s_response*resp,const char*sn){
    char buf[H8S_MAX_HEADER];int off=0;
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"%s %d %s\r\n",req->version[0]?req->version:"HTTP/1.0",resp->status_code,resp->reason?resp->reason:reason_phrase(resp->status_code));
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Server: %s\r\n",sn);
    if(resp->content_type)off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Content-Type: %s\r\n",resp->content_type);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Content-Length: %zu\r\n",resp->body_length);
    for(int i=0;i<resp->header_count;++i)off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"%s: %s\r\n",resp->headers[i].name,resp->headers[i].value);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"X-SLeeLa-Version: 8.0\r\nX-SLeeLa-Accept: 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.1, 1.1, 1.0\r\nConnection: close\r\n\r\n");
    if(off<=0||(size_t)off>=sizeof(buf))return -1;if(send_all(fd,buf,(size_t)off))return -1;
    if(resp->body&&resp->body_length>0)return send_all(fd,resp->body,resp->body_length);return 0;}

int h8s_download_should_resume(uint64_t cl){return cl>H8S_DOWNLOAD_THRESHOLD;}
int h8s_download_serialise(const h8s_download_state*st,char*buf,size_t cap){if(!st||!buf||cap<512)return -1;
    int n=snprintf(buf,cap,"%s | %s | %s | %s | %llu | %llu | %llu",st->session_id,st->datetime,st->file_id,st->file_name,
        (unsigned long long)st->index,(unsigned long long)st->offset,(unsigned long long)st->total_size);
    return(n>0&&(size_t)n<cap)?0:-1;}
int h8s_download_parse(const char*line,h8s_download_state*out){if(!line||!out)return -1;memset(out,0,sizeof(*out));
    char c[2048];snprintf(c,sizeof(c),"%s",line);char*f[7];int cnt=0;char*t=strtok(c,"|");
    while(t&&cnt<7){while(*t==' '||*t=='\t')++t;size_t l=strlen(t);while(l>0&&(t[l-1]==' '||t[l-1]=='\t'))t[--l]='\0';f[cnt++]=t;t=strtok(NULL,"|");}
    if(cnt!=7)return -1;snprintf(out->session_id,sizeof(out->session_id),"%s",f[0]);snprintf(out->datetime,sizeof(out->datetime),"%s",f[1]);
    snprintf(out->file_id,sizeof(out->file_id),"%s",f[2]);snprintf(out->file_name,sizeof(out->file_name),"%s",f[3]);
    out->index=(uint64_t)strtoull(f[4],NULL,10);out->offset=(uint64_t)strtoull(f[5],NULL,10);out->total_size=(uint64_t)strtoull(f[6],NULL,10);return 0;}

static int builtin_health(h8s_response*r){static const char b[]="SLeeLa HTTP/8.0 OK\n";
    r->status_code=200;r->content_type="text/plain; charset=utf-8";r->body=b;r->body_length=sizeof(b)-1;return 0;}
static int builtin_status(h8s_response*r){static char b[1024];
    int n=snprintf(b,sizeof(b),"{\"server\":\"SLeeLa\",\"http_generation\":\"8.0\","
        "\"vm_generation\":8,\"vm_name\":\"Executive\",\"vm_path\":\"/8\","
        "\"platform\":\"%s\",\"frame_version\":%u,\"frame_count\":28,"
        "\"session_model\":true,\"national_signal_frequency\":true,"
        "\"binary_checker\":true,\"crypto_policy\":true,"
        "\"dark_power\":true,\"lower_http_compatibility\":true}\n",
        slnet_platform_name(),HTTP8_VERSION);
    r->status_code=200;r->content_type="application/json";r->body=b;r->body_length=(size_t)n;return 0;}
static int builtin_negotiate(const h8s_request*req,h8s_response*r,int fb){
    const char*neg=sleela_http_negotiate(req->sleela_version[0]?req->sleela_version:NULL,
        req->sleela_accept[0]?req->sleela_accept:NULL,fb);
    static char b[256];int n;
    if(neg){n=snprintf(b,sizeof(b),"{\"negotiated\":\"%s\",\"ok\":true}\n",neg);r->status_code=200;}
    else{n=snprintf(b,sizeof(b),"{\"negotiated\":null,\"ok\":false}\n");r->status_code=400;}
    r->content_type="application/json";r->body=b;r->body_length=(size_t)n;return 0;}

static int builtin_h8_frame(const h8s_request*req,h8s_response*resp,h8s_frame_handler fh,void*ud){
    if(!req->body||req->body_length<HTTP8_FRAME_HEADER_SIZE){resp->status_code=400;resp->content_type="text/plain";resp->body="Frame body required\n";resp->body_length=20;return 0;}
    http8_frame_view_t frame;size_t consumed;
    if(http8_frame_decode((const uint8_t*)req->body,req->body_length,&frame,&consumed)){resp->status_code=400;resp->content_type="text/plain";resp->body="Invalid H8 frame\n";resp->body_length=17;return 0;}
    if(fh){static uint8_t rp[4096];size_t rl=0;if(fh(&frame,rp,sizeof(rp),&rl,ud)){resp->status_code=500;resp->content_type="text/plain";resp->body="Handler error\n";resp->body_length=14;return 0;}
        http8_frame_header_t rh={0};rh.version=HTTP8_VERSION;rh.type=(frame.header.type==HTTP8_FRAME_PING)?HTTP8_FRAME_PONG:HTTP8_FRAME_DATA;
        rh.flags=HTTP8_FLAG_FIN;rh.stream_id=frame.header.stream_id;rh.request_id=frame.header.request_id;rh.sequence=frame.header.sequence+1;rh.payload_length=(uint32_t)rl;
        static uint8_t wire[4096+HTTP8_FRAME_HEADER_SIZE];size_t written;
        if(http8_frame_encode(&rh,rp,wire,sizeof(wire),&written)){resp->status_code=500;resp->content_type="text/plain";resp->body="Encode error\n";resp->body_length=13;return 0;}
        resp->status_code=200;resp->content_type="application/x-sleela-h8";resp->body=(const char*)wire;resp->body_length=written;return 0;}
    static char result[4096];int n=snprintf(result,sizeof(result),"{\"frame_type\":\"%s\",\"stream_id\":%llu,\"request_id\":%llu,\"sequence\":%llu,\"flags\":%u,\"payload_length\":%u}\n",
        http8_frame_type_name((http8_frame_type_t)frame.header.type),(unsigned long long)frame.header.stream_id,(unsigned long long)frame.header.request_id,
        (unsigned long long)frame.header.sequence,(unsigned)frame.header.flags,(unsigned)frame.header.payload_length);
    resp->status_code=200;resp->content_type="application/json";resp->body=result;resp->body_length=(size_t)n;return 0;}

static void handle_connection(SLNetHandle fd,const h8s_server_config*cfg){
    size_t rc=H8S_MAX_HEADER+4096;char*raw=malloc(rc);if(!raw){slnet_close(fd);return;}
    int got=recv_headers(fd,raw,rc);if(got<=0){free(raw);slnet_close(fd);return;}
    h8s_request req;h8s_response resp;const char*bs=NULL;size_t hb=0;memset(&resp,0,sizeof(resp));resp.status_code=200;
    if(parse_request(raw,(size_t)got,&req,&bs,&hb)){resp.status_code=400;resp.content_type="text/plain";resp.body="Bad Request\n";resp.body_length=12;
        h8s_request bad;memset(&bad,0,sizeof(bad));memcpy(bad.version,"HTTP/1.0",9);send_response(fd,&bad,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
    {const char*cl=get_hdr(&req,"content-length");if(cl){size_t clen=(size_t)strtoull(cl,NULL,10);
        if(clen>H8S_MAX_BODY){resp.status_code=413;resp.content_type="text/plain";resp.body="Content Too Large\n";resp.body_length=18;send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
        size_t already=(size_t)got-hb;req.body=malloc(clen+1);if(!req.body){resp.status_code=500;resp.content_type="text/plain";resp.body="ISE\n";resp.body_length=4;send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
        if(already>0&&already<=clen)memcpy(req.body,bs,already);size_t rem=already<clen?clen-already:0,filled=already<clen?already:clen;
        while(rem>0){SLNetCount n=slnet_read(fd,req.body+filled,rem);if(n<=0)break;filled+=(size_t)n;rem-=(size_t)n;}req.body_length=filled;req.body[filled]='\0';}}
    free(raw);raw=NULL;int handled=0;
    if(!strcmp(req.uri,"/__sleela/health")){builtin_health(&resp);handled=1;}
    else if(!strcmp(req.uri,"/__sleela/status")){builtin_status(&resp);handled=1;}
    else if(!strcmp(req.uri,"/__sleela/negotiate")){builtin_negotiate(&req,&resp,cfg->allow_negotiation_fallback);handled=1;}
    else if(!strcmp(req.uri,"/__sleela/h8")&&!strcmp(req.method,"POST")){builtin_h8_frame(&req,&resp,cfg->frame_handler,cfg->user_data);handled=1;}
    if(!handled&&cfg->handler){if(cfg->handler(&req,&resp,cfg->user_data)){resp.status_code=500;resp.content_type="text/plain";resp.body="ISE\n";resp.body_length=4;}}
    else if(!handled){resp.status_code=404;resp.content_type="text/plain";resp.body="Not Found\n";resp.body_length=10;}
    send_response(fd,&req,&resp,cfg->server_name);free(req.body);slnet_close(fd);}

static int ppoll(SLNetHandle fd,int ms){
#ifdef _WIN32
    WSAPOLLFD pfd;pfd.fd=(SOCKET)fd;pfd.events=POLLIN;pfd.revents=0;return WSAPoll(&pfd,1,ms)>0&&(pfd.revents&POLLIN);
#else
    struct pollfd pfd;pfd.fd=(int)fd;pfd.events=POLLIN;pfd.revents=0;return poll(&pfd,1,ms)>0&&(pfd.revents&POLLIN);
#endif
}

void h8s_server_config_init(h8s_server_config*cfg){if(!cfg)return;memset(cfg,0,sizeof(*cfg));
    cfg->port=8087;cfg->backlog=128;cfg->server_name="SLeeLa-HTTP/8.0";cfg->allow_negotiation_fallback=1;}

int h8s_server_run(const h8s_server_config*cfg){if(!cfg)return -1;g_stop=0;
    if(slnet_startup()){fprintf(stderr,"h8s: slnet_startup failed\n");return -1;}
    sleela_preferred_router_startup(NULL,"HTTP/8.0");
    SLNetHandle listener=slnet_listen(cfg->port,cfg->backlog);
    if(!slnet_valid(listener)){fprintf(stderr,"h8s: listen failed port %u\n",(unsigned)cfg->port);slnet_shutdown();return -1;}
    fprintf(stderr,"SLeeLa HTTP/8.0 listening on port %u  [%s]\n",(unsigned)cfg->port,slnet_platform_name());
#ifndef _WIN32
    signal(SIGINT,h8s_sig);signal(SIGTERM,h8s_sig);
#endif
    while(!g_stop){if(!ppoll(listener,500))continue;SLNetHandle c=slnet_accept(listener);if(!slnet_valid(c))continue;handle_connection(c,cfg);}
    slnet_close(listener);slnet_shutdown();fprintf(stderr,"SLeeLa HTTP/8.0 stopped.\n");return 0;}

/* ── self-test ──────────────────────────────────────────────────────── */
int h8s_self_test(void){int f=0;
    /* 1. HTTP parse */
    {const char raw[]="GET / HTTP/1.1\r\nX-SLeeLa-Version: 8.0\r\n\r\n";h8s_request req;const char*bs;size_t hb;
     if(parse_request(raw,sizeof(raw)-1,&req,&bs,&hb)||strcmp(req.sleela_version,"8.0")){fprintf(stderr,"FAIL: HTTP parse\n");++f;}}
    /* 2. SESSION_BEGIN frame */
    {http8_frame_header_t h={0};h.version=HTTP8_VERSION;h.type=HTTP8_FRAME_SESSION_BEGIN;h.flags=HTTP8_FLAG_CRYPTO_REQUIRED;
     h.stream_id=1;h.request_id=42;h.sequence=1;h.payload_length=7;const uint8_t p[]="session";
     uint8_t wire[128];size_t written;
     if(http8_frame_encode(&h,p,wire,sizeof(wire),&written)){fprintf(stderr,"FAIL: encode\n");++f;}
     else{http8_frame_view_t v;size_t consumed;if(http8_frame_decode(wire,written,&v,&consumed)||v.header.type!=HTTP8_FRAME_SESSION_BEGIN||v.header.request_id!=42){fprintf(stderr,"FAIL: decode\n");++f;}}}
    /* 3. NATIONAL_SIGNAL frame */
    {http8_frame_header_t h={0};h.version=HTTP8_VERSION;h.type=HTTP8_FRAME_NATIONAL_SIGNAL;h.stream_id=2;h.request_id=99;h.sequence=1;h.payload_length=3;
     const uint8_t p[]="nsf";uint8_t wire[128];size_t written;
     if(http8_frame_encode(&h,p,wire,sizeof(wire),&written)){fprintf(stderr,"FAIL: nsf encode\n");++f;}
     else{http8_frame_view_t v;size_t consumed;if(http8_frame_decode(wire,written,&v,&consumed)||v.header.type!=HTTP8_FRAME_NATIONAL_SIGNAL){fprintf(stderr,"FAIL: nsf decode\n");++f;}}}
    /* 4. All 28 frame type names */
    {for(int t=1;t<=28;++t){const char*n=http8_frame_type_name((http8_frame_type_t)t);if(!n||!strcmp(n,"UNKNOWN")){fprintf(stderr,"FAIL: type %d\n",t);++f;}}}
    /* 5. Session handshake */
    {http8_session s;http8_session_init(&s);s.subscription=HTTP8_SUB_ACTIVE;s.radio=HTTP8_RADIO_READY;
     snprintf(s.international.jurisdiction,sizeof(s.international.jurisdiction),"test");
     snprintf(s.international.process,sizeof(s.international.process),"test");
     snprintf(s.international.duty,sizeof(s.international.duty),"test");
     snprintf(s.nsf.emblem,sizeof(s.nsf.emblem),"e");snprintf(s.nsf.signal,sizeof(s.nsf.signal),"s");
     snprintf(s.nsf.frequency_unit,sizeof(s.nsf.frequency_unit),"MHz");snprintf(s.nsf.frequency_range,sizeof(s.nsf.frequency_range),"r");
     snprintf(s.nsf.jurisdiction,sizeof(s.nsf.jurisdiction),"j");snprintf(s.nsf.source,sizeof(s.nsf.source),"s");
     snprintf(s.nsf.timestamp,sizeof(s.nsf.timestamp),"2026");
     s.checker.binary_checker_required=HTTP8_YES;s.checker.binary_checker_passed=HTTP8_YES;
     snprintf(s.checker.metric,sizeof(s.checker.metric),"m");snprintf(s.checker.legal,sizeof(s.checker.legal),"l");
     snprintf(s.checker.social,sizeof(s.checker.social),"s");snprintf(s.checker.agreement_hash,sizeof(s.checker.agreement_hash),"h");
     snprintf(s.initiator.party_id,sizeof(s.initiator.party_id),"init");s.initiator.safety_acceptance=HTTP8_YES;
     snprintf(s.responder.party_id,sizeof(s.responder.party_id),"resp");s.responder.safety_acceptance=HTTP8_YES;
     if(http8_session_begin_handshake(&s)!=100){fprintf(stderr,"FAIL: handshake begin\n");++f;}
     if(http8_session_accept_party(&s.initiator)!=200){fprintf(stderr,"FAIL: accept init\n");++f;}
     if(http8_session_accept_party(&s.responder)!=200){fprintf(stderr,"FAIL: accept resp\n");++f;}
     if(http8_session_complete_handshake(&s)!=200){fprintf(stderr,"FAIL: complete\n");++f;}
     if(!http8_session_can_exchange(&s)){fprintf(stderr,"FAIL: can_exchange\n");++f;}}
    /* 6. Download */
    {h8s_download_state st={0};snprintf(st.session_id,sizeof(st.session_id),"h8");snprintf(st.datetime,sizeof(st.datetime),"2026");
     snprintf(st.file_id,sizeof(st.file_id),"f8");snprintf(st.file_name,sizeof(st.file_name),"big");st.index=6;st.offset=400000000ULL;st.total_size=800000000ULL;
     char buf[512];if(h8s_download_serialise(&st,buf,sizeof(buf))){fprintf(stderr,"FAIL: dl\n");++f;}
     else{h8s_download_state p2;if(h8s_download_parse(buf,&p2)||p2.index!=6){fprintf(stderr,"FAIL: dl parse\n");++f;}}}
    /* 7. Negotiation */
    {const char*r=sleela_http_negotiate("8.0","8.0, 7.0, 1.1",1);if(!r||strcmp(r,"8.0")){fprintf(stderr,"FAIL: negotiate\n");++f;}
     r=sleela_http_negotiate("9.0","8.0, 1.1",1);if(!r||strcmp(r,"1.1")){fprintf(stderr,"FAIL: fallback\n");++f;}}
    if(f==0)fprintf(stderr,"h8s_self_test: all tests passed\n");else fprintf(stderr,"h8s_self_test: %d test(s) FAILED\n",f);return f;}
