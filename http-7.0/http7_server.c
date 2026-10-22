/* http7_server.c — SLeeLa HTTP 7.0 portable server
 * Platforms: Linux, macOS, Windows 10+
 * 24 frame types incl. Reality Assertion.
 */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif
#include "http7_server.h"
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
void h7s_server_stop(void){g_stop=1;}
#ifndef _WIN32
static void h7s_sig(int s){(void)s;g_stop=1;}
#endif

static void str_lower(char *s){for(;*s;++s)*s=(char)tolower((unsigned char)*s);}
static const char *str_ltrim(const char *s){while(*s==' '||*s=='\t')++s;return s;}
static const char *reason_phrase(int c){
    switch(c){case 200:return"OK";case 400:return"Bad Request";case 404:return"Not Found";
    case 413:return"Content Too Large";case 500:return"Internal Server Error";default:return"Error";}}

static int recv_headers(SLNetHandle fd,char*buf,size_t cap){
    size_t t=0;while(t<cap){SLNetCount n=slnet_read(fd,buf+t,cap-t);if(n<=0)return t>0?(int)t:-1;
    t+=(size_t)n;if(t>=4){const char*lim=buf+(t>H7S_MAX_HEADER?H7S_MAX_HEADER:t);
    for(const char*p=buf+(t-n>3?t-n-3:0);p+3<lim;++p)
        if(p[0]=='\r'&&p[1]=='\n'&&p[2]=='\r'&&p[3]=='\n')return(int)t;}}return -1;}

static const char*get_hdr(const h7s_request*r,const char*n){
    for(int i=0;i<r->header_count;++i)if(strcmp(r->headers[i].name,n)==0)return r->headers[i].value;return NULL;}

static int is_token_char(unsigned char c){
    if((c>='0'&&c<='9')||(c>='A'&&c<='Z')||(c>='a'&&c<='z'))return 1;
    return strchr("!#$%&'*+-.^_`|~",(int)c)!=NULL;
}
static int parse_request(const char*raw,size_t raw_len,h7s_request*req,const char**bs,size_t*hb){
    if(!raw||!req||!bs||!hb||raw_len>H7S_MAX_HEADER+H7S_MAX_BODY)return -1;
    const char *p=raw,*end=raw+raw_len,*le;
    memset(req,0,sizeof(*req));
    le=NULL;
    for(const char*q=p;q+1<end;++q)if(q[0]=='\r'&&q[1]=='\n'){le=q;break;}
    if(!le)return -1;
    const char*sp=memchr(p,' ',(size_t)(le-p));if(!sp||sp==p)return -1;
    size_t ml=(size_t)(sp-p);if(ml>=sizeof(req->method))return -1;
    for(size_t i=0;i<ml;++i)if(!is_token_char((unsigned char)p[i]))return -1;
    memcpy(req->method,p,ml);p=sp+1;
    sp=memchr(p,' ',(size_t)(le-p));if(!sp||sp==p)return -1;
    size_t ul=(size_t)(sp-p);if(ul>=sizeof(req->uri))return -1;
    for(size_t i=0;i<ul;++i)if((unsigned char)p[i]<=0x20||(unsigned char)p[i]==0x7f)return -1;
    memcpy(req->uri,p,ul);p=sp+1;
    size_t vl=(size_t)(le-p);if(vl>=sizeof(req->version))return -1;
    if(!((vl==8&&memcmp(p,"HTTP/1.1",8)==0)||(vl==8&&memcmp(p,"HTTP/1.0",8)==0)))return -1;
    memcpy(req->version,p,vl);p=le+2;
    int host_count=0,cl_count=0,te_count=0;
    while(p<end){
        if(p+1<end&&p[0]=='\r'&&p[1]=='\n'){p+=2;break;}
        le=NULL;for(const char*q=p;q+1<end;++q)if(q[0]=='\r'&&q[1]=='\n'){le=q;break;}
        if(!le||le==p||req->header_count>=H7S_MAX_HEADERS)return -1;
        const char*colon=memchr(p,':',(size_t)(le-p));if(!colon||colon==p)return -1;
        size_t nl=(size_t)(colon-p);if(nl>=sizeof(req->headers[0].name))return -1;
        for(size_t i=0;i<nl;++i)if(!is_token_char((unsigned char)p[i]))return -1;
        h7s_header*h=&req->headers[req->header_count];
        memcpy(h->name,p,nl);h->name[nl]='\0';str_lower(h->name);
        const char*v=colon+1;while(v<le&&(*v==' '||*v=='\t'))++v;
        const char*ve=le;while(ve>v&&(ve[-1]==' '||ve[-1]=='\t'))--ve;
        size_t vv=(size_t)(ve-v);if(vv>=sizeof(h->value))return -1;
        for(const char*q=v;q<ve;++q)if(((unsigned char)*q<0x20&&*q!='\t')||(unsigned char)*q==0x7f)return -1;
        memcpy(h->value,v,vv);h->value[vv]='\0';
        for(int i=0;i<req->header_count;++i)if(strcmp(req->headers[i].name,h->name)==0)return -1;
        if(strcmp(h->name,"host")==0){if(vv==0)return -1;++host_count;}
        if(strcmp(h->name,"content-length")==0)++cl_count;
        if(strcmp(h->name,"transfer-encoding")==0)++te_count;
        ++req->header_count;p=le+2;
    }
    if(p>end|| (strcmp(req->version,"HTTP/1.1")==0&&host_count!=1)||cl_count>1||te_count>0)return -1;
    *hb=(size_t)(p-raw);*bs=p;
    const char*v;
    v=get_hdr(req,"x-sleela-version");if(v)snprintf(req->sleela_version,sizeof(req->sleela_version),"%s",v);
    v=get_hdr(req,"x-sleela-accept");if(v)snprintf(req->sleela_accept,sizeof(req->sleela_accept),"%s",v);
    v=get_hdr(req,"x-sleela-port");if(v)snprintf(req->sleela_port,sizeof(req->sleela_port),"%s",v);
    v=get_hdr(req,"x-sleela-service-id");if(v)snprintf(req->service_id,sizeof(req->service_id),"%s",v);
    v=get_hdr(req,"x-sleela-op-id");if(v)snprintf(req->op_id,sizeof(req->op_id),"%s",v);
    return 0;
}

static int parse_content_length(const char*s,size_t*out){
    if(!s||!*s||!out)return -1;size_t n=0;
    for(const unsigned char*p=(const unsigned char*)s;*p;++p){
        if(*p<'0'||*p>'9')return -1;unsigned d=(unsigned)(*p-'0');
        if(n>(SIZE_MAX-d)/10u)return -1;n=n*10u+d;
    }
    *out=n;return 0;
}

static int send_all(SLNetHandle fd,const char*d,size_t l){
    while(l>0){SLNetCount n=slnet_write(fd,d,l);if(n<=0)return -1;d+=n;l-=(size_t)n;}return 0;}

static int send_response(SLNetHandle fd,const h7s_request*req,const h7s_response*resp,const char*sn){
    char buf[H7S_MAX_HEADER];int off=0;
    const char*reason=resp->reason?resp->reason:reason_phrase(resp->status_code);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"%s %d %s\r\n",req->version[0]?req->version:"HTTP/1.0",resp->status_code,reason);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Server: %s\r\n",sn);
    if(resp->content_type)off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Content-Type: %s\r\n",resp->content_type);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Content-Length: %zu\r\n",resp->body_length);
    for(int i=0;i<resp->header_count;++i)off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"%s: %s\r\n",resp->headers[i].name,resp->headers[i].value);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"X-SLeeLa-Version: 7.0\r\nX-SLeeLa-Accept: 7.0, 6.0, 5.0, 4.0, 3.0, 2.1, 1.1, 1.0\r\nConnection: close\r\n\r\n");
    if(off<=0||(size_t)off>=sizeof(buf))return -1;
    if(send_all(fd,buf,(size_t)off)!=0)return -1;
    if(resp->body&&resp->body_length>0)return send_all(fd,resp->body,resp->body_length);return 0;}

int h7s_download_should_resume(uint64_t cl){return cl>H7S_DOWNLOAD_THRESHOLD;}
int h7s_download_serialise(const h7s_download_state*st,char*buf,size_t cap){
    if(!st||!buf||cap<512)return -1;
    int n=snprintf(buf,cap,"%s | %s | %s | %s | %llu | %llu | %llu",st->session_id,st->datetime,st->file_id,st->file_name,
        (unsigned long long)st->index,(unsigned long long)st->offset,(unsigned long long)st->total_size);
    return(n>0&&(size_t)n<cap)?0:-1;}
int h7s_download_parse(const char*line,h7s_download_state*out){
    if(!line||!out)return -1;memset(out,0,sizeof(*out));
    char c[2048];snprintf(c,sizeof(c),"%s",line);char*f[7];int cnt=0;char*t=strtok(c,"|");
    while(t&&cnt<7){while(*t==' '||*t=='\t')++t;size_t l=strlen(t);
        while(l>0&&(t[l-1]==' '||t[l-1]=='\t'))t[--l]='\0';f[cnt++]=t;t=strtok(NULL,"|");}
    if(cnt!=7)return -1;
    snprintf(out->session_id,sizeof(out->session_id),"%s",f[0]);snprintf(out->datetime,sizeof(out->datetime),"%s",f[1]);
    snprintf(out->file_id,sizeof(out->file_id),"%s",f[2]);snprintf(out->file_name,sizeof(out->file_name),"%s",f[3]);
    out->index=(uint64_t)strtoull(f[4],NULL,10);out->offset=(uint64_t)strtoull(f[5],NULL,10);
    out->total_size=(uint64_t)strtoull(f[6],NULL,10);return 0;}

/* ── built-in routes ────────────────────────────────────────────────── */

static int builtin_health(h7s_response*r){
    static const char b[]="SLeeLa HTTP/7.0 OK\n";
    r->status_code=200;r->content_type="text/plain; charset=utf-8";r->body=b;r->body_length=sizeof(b)-1;return 0;}

static int builtin_status(h7s_response*r){
    static char b[1536];
    int n=snprintf(b,sizeof(b),
        "{\"server\":\"SLeeLa\",\"http_generation\":\"7.0\","
         "\"vm_generation\":7,\"vm_name\":\"Administrator\",\"vm_path\":\"/7\","
         "\"platform\":\"%s\",\"frame_version\":%u,"
         "\"frame_count\":24,"
         "\"reality_assertion\":true,"
         "\"assertion_statuses\":[\"FACTUAL_DOCUMENTED\",\"USER_AUTHORED\","
           "\"FICTIONAL\",\"COUNTERFACTUAL\",\"DISPUTED\",\"UNVERIFIED\"],"
         "\"lower_http_compatibility\":true}\n",
        slnet_platform_name(),HTTP7_VERSION);
    r->status_code=200;r->content_type="application/json";r->body=b;r->body_length=(size_t)n;return 0;}

static int builtin_negotiate(const h7s_request*req,h7s_response*r,int fb){
    const char*neg=sleela_http_negotiate(req->sleela_version[0]?req->sleela_version:NULL,
        req->sleela_accept[0]?req->sleela_accept:NULL,fb);
    static char b[256];int n;
    if(neg){n=snprintf(b,sizeof(b),"{\"negotiated\":\"%s\",\"ok\":true}\n",neg);r->status_code=200;}
    else{n=snprintf(b,sizeof(b),"{\"negotiated\":null,\"ok\":false}\n");r->status_code=400;}
    r->content_type="application/json";r->body=b;r->body_length=(size_t)n;return 0;}

static int builtin_h7_frame(const h7s_request*req,h7s_response*resp,h7s_frame_handler fh,void*ud){
    if(!req->body||req->body_length<HTTP7_FRAME_HEADER_SIZE){
        resp->status_code=400;resp->content_type="text/plain";resp->body="Frame body required\n";resp->body_length=20;return 0;}
    http7_frame_view_t frame;size_t consumed;
    if(http7_frame_decode((const uint8_t*)req->body,req->body_length,&frame,&consumed)!=0){
        resp->status_code=400;resp->content_type="text/plain";resp->body="Invalid H7 frame\n";resp->body_length=17;return 0;}
    if(fh){
        static uint8_t rp[4096];size_t rl=0;
        if(fh(&frame,rp,sizeof(rp),&rl,ud)!=0){resp->status_code=500;resp->content_type="text/plain";
            resp->body="Frame handler error\n";resp->body_length=20;return 0;}
        http7_frame_header_t rh={0};rh.version=HTTP7_VERSION;
        rh.type=(frame.header.type==HTTP7_FRAME_PING)?HTTP7_FRAME_PONG:HTTP7_FRAME_DATA;
        rh.flags=HTTP7_FLAG_FIN;rh.stream_id=frame.header.stream_id;
        rh.request_id=frame.header.request_id;rh.sequence=frame.header.sequence+1;
        rh.payload_length=(uint32_t)rl;
        static uint8_t wire[4096+HTTP7_FRAME_HEADER_SIZE];size_t written;
        if(http7_frame_encode(&rh,rp,wire,sizeof(wire),&written)!=0){
            resp->status_code=500;resp->content_type="text/plain";resp->body="Encode error\n";resp->body_length=13;return 0;}
        resp->status_code=200;resp->content_type="application/x-sleela-h7";
        resp->body=(const char*)wire;resp->body_length=written;return 0;}
    static char result[4096];int n;
    n=snprintf(result,sizeof(result),"{\"frame_type\":\"%s\",\"stream_id\":%llu,\"request_id\":%llu,"
        "\"sequence\":%llu,\"flags\":%u,\"payload_length\":%u}\n",
        http7_frame_type_name((http7_frame_type_t)frame.header.type),
        (unsigned long long)frame.header.stream_id,(unsigned long long)frame.header.request_id,
        (unsigned long long)frame.header.sequence,(unsigned)frame.header.flags,(unsigned)frame.header.payload_length);
    resp->status_code=200;resp->content_type="application/json";resp->body=result;resp->body_length=(size_t)n;return 0;}

/* ── connection handler ─────────────────────────────────────────────── */

static void handle_connection(SLNetHandle fd,const h7s_server_config*cfg){
    size_t rc=H7S_MAX_HEADER+4096;char*raw=malloc(rc);if(!raw){slnet_close(fd);return;}
    int got=recv_headers(fd,raw,rc);if(got<=0){free(raw);slnet_close(fd);return;}
    h7s_request req;h7s_response resp;const char*bs=NULL;size_t hb=0;
    memset(&resp,0,sizeof(resp));resp.status_code=200;
    if(parse_request(raw,(size_t)got,&req,&bs,&hb)!=0){
        resp.status_code=400;resp.content_type="text/plain";resp.body="Bad Request\n";resp.body_length=12;
        h7s_request bad;memset(&bad,0,sizeof(bad));memcpy(bad.version,"HTTP/1.0",9);
        send_response(fd,&bad,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
    {const char*cl=get_hdr(&req,"content-length");
     if(cl){size_t clen=(size_t)strtoull(cl,NULL,10);
         if(clen>H7S_MAX_BODY){resp.status_code=413;resp.content_type="text/plain";
             resp.body="Content Too Large\n";resp.body_length=18;
             send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
         size_t already=(size_t)got-hb;req.body=malloc(clen+1);
         if(!req.body){resp.status_code=500;resp.content_type="text/plain";
             resp.body="Internal Server Error\n";resp.body_length=22;
             send_response(fd,&req,&resp,cfg->server_name);free(raw);slnet_close(fd);return;}
         if(already>0&&already<=clen)memcpy(req.body,bs,already);
         size_t rem=already<clen?clen-already:0,filled=already<clen?already:clen;
         while(rem>0){SLNetCount n=slnet_read(fd,req.body+filled,rem);if(n<=0)break;filled+=(size_t)n;rem-=(size_t)n;}
         req.body_length=filled;req.body[filled]='\0';}}
    free(raw);raw=NULL;
    int handled=0;
    if(strcmp(req.uri,"/__sleela/health")==0){builtin_health(&resp);handled=1;}
    else if(strcmp(req.uri,"/__sleela/status")==0){builtin_status(&resp);handled=1;}
    else if(strcmp(req.uri,"/__sleela/negotiate")==0){builtin_negotiate(&req,&resp,cfg->allow_negotiation_fallback);handled=1;}
    else if(strcmp(req.uri,"/__sleela/h7")==0&&strcmp(req.method,"POST")==0){builtin_h7_frame(&req,&resp,cfg->frame_handler,cfg->user_data);handled=1;}
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

void h7s_server_config_init(h7s_server_config*cfg){
    if(!cfg)return;memset(cfg,0,sizeof(*cfg));cfg->port=8086;cfg->backlog=128;
    cfg->server_name="SLeeLa-HTTP/7.0";cfg->allow_negotiation_fallback=1;}

int h7s_server_run(const h7s_server_config*cfg){
    if(!cfg)return -1;g_stop=0;
    if(slnet_startup()!=0){fprintf(stderr,"h7s: slnet_startup failed\n");return -1;}
    sleela_preferred_router_startup(NULL,"HTTP/7.0");
    SLNetHandle listener=slnet_listen(cfg->port,cfg->backlog);
    if(!slnet_valid(listener)){fprintf(stderr,"h7s: listen failed on port %u\n",(unsigned)cfg->port);slnet_shutdown();return -1;}
    fprintf(stderr,"SLeeLa HTTP/7.0 listening on port %u  [%s]\n",(unsigned)cfg->port,slnet_platform_name());
#ifndef _WIN32
    signal(SIGINT,h7s_sig);signal(SIGTERM,h7s_sig);
#endif
    while(!g_stop){if(!portable_poll_readable(listener,500))continue;
        SLNetHandle c=slnet_accept(listener);if(!slnet_valid(c))continue;handle_connection(c,cfg);}
    slnet_close(listener);slnet_shutdown();fprintf(stderr,"SLeeLa HTTP/7.0 stopped.\n");return 0;}

/* ── self-test ──────────────────────────────────────────────────────── */

int h7s_self_test(void){
    int f=0;
    /* 1. HTTP parse */
    {const char raw[]="GET / HTTP/1.1\r\nX-SLeeLa-Version: 7.0\r\n\r\n";
     h7s_request req;const char*bs;size_t hb;
     if(parse_request(raw,sizeof(raw)-1,&req,&bs,&hb)!=0||strcmp(req.sleela_version,"7.0")!=0){
         fprintf(stderr,"FAIL: HTTP parse\n");++f;}}
    /* 2. ASSERTION frame encode/decode */
    {http7_frame_header_t h={0};h.version=HTTP7_VERSION;h.type=HTTP7_FRAME_ASSERTION;
     h.flags=HTTP7_FLAG_FACTUAL;h.stream_id=1;h.request_id=42;h.sequence=1;h.payload_length=11;
     const uint8_t payload[]="hello world";uint8_t wire[128];size_t written;
     if(http7_frame_encode(&h,payload,wire,sizeof(wire),&written)!=0){fprintf(stderr,"FAIL: encode\n");++f;}
     else{http7_frame_view_t v;size_t consumed;
         if(http7_frame_decode(wire,written,&v,&consumed)!=0){fprintf(stderr,"FAIL: decode\n");++f;}
         else if(v.header.type!=HTTP7_FRAME_ASSERTION||v.header.request_id!=42||
                 v.header.payload_length!=11||memcmp(v.payload,"hello world",11)!=0){
             fprintf(stderr,"FAIL: round-trip\n");++f;}}}
    /* 3. All 24 frame type names */
    {for(int t=1;t<=24;++t){const char*n=http7_frame_type_name((http7_frame_type_t)t);
         if(!n||strcmp(n,"UNKNOWN")==0){fprintf(stderr,"FAIL: type %d\n",t);++f;}}}
    /* 4. Assertion serialise/parse round-trip */
    {http7_assertion_t a={0};
     a.status=HTTP7_ASSERT_FACTUAL_DOCUMENTED;
     snprintf(a.statement,sizeof(a.statement),"Nuclear arms exist.");
     snprintf(a.source,sizeof(a.source),"public record");
     snprintf(a.source_date,sizeof(a.source_date),"2026-10-15");
     snprintf(a.author,sizeof(a.author),"test");
     snprintf(a.document_reference,sizeof(a.document_reference),"ref-001");
     char buf[1024];int n=http7_assertion_serialise(&a,buf,sizeof(buf));
     if(n<0){fprintf(stderr,"FAIL: assertion serialise\n");++f;}
     else{http7_assertion_t b;
         if(http7_assertion_parse(buf,(size_t)n,&b)!=0){fprintf(stderr,"FAIL: assertion parse\n");++f;}
         else if(b.status!=HTTP7_ASSERT_FACTUAL_DOCUMENTED||
                 strcmp(b.statement,"Nuclear arms exist.")!=0||
                 strcmp(b.author,"test")!=0){
             fprintf(stderr,"FAIL: assertion round-trip\n");++f;}}}
    /* 5. All 6 assertion status names */
    {for(int s=0;s<=5;++s){const char*n=http7_assertion_status_name((http7_assertion_status_t)s);
         if(!n||strcmp(n,"UNKNOWN")==0){fprintf(stderr,"FAIL: status %d\n",s);++f;}}}
    /* 6. Download round-trip */
    {h7s_download_state st={0};snprintf(st.session_id,sizeof(st.session_id),"h7");
     snprintf(st.datetime,sizeof(st.datetime),"2026");snprintf(st.file_id,sizeof(st.file_id),"f7");
     snprintf(st.file_name,sizeof(st.file_name),"big");st.index=5;st.offset=300000000ULL;st.total_size=700000000ULL;
     char buf[512];if(h7s_download_serialise(&st,buf,sizeof(buf))!=0){fprintf(stderr,"FAIL: dl\n");++f;}
     else{h7s_download_state p;if(h7s_download_parse(buf,&p)!=0||p.index!=5){fprintf(stderr,"FAIL: dl parse\n");++f;}}}
    /* 7. Negotiation */
    {const char*r=sleela_http_negotiate("7.0","7.0, 6.0, 1.1",1);
     if(!r||strcmp(r,"7.0")!=0){fprintf(stderr,"FAIL: negotiate\n");++f;}
     r=sleela_http_negotiate("8.0","7.0, 1.1",1);
     if(!r||strcmp(r,"1.1")!=0){fprintf(stderr,"FAIL: fallback\n");++f;}}

    if(f==0)fprintf(stderr,"h7s_self_test: all tests passed\n");
    else fprintf(stderr,"h7s_self_test: %d test(s) FAILED\n",f);
    return f;}
