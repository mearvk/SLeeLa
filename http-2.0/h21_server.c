/* h21_server.c — SLeeLa HTTP 2.0/2.1 portable server implementation
 *
 * Platforms: Linux, macOS, Windows 10+
 *
 * Transport: HTTP/1.x over sleela_net portable sockets.
 * Application: SLeeLa 2.1 envelope/naming/pipeline core rides inside the
 * HTTP request body at /__sleela/h21 (POST).  Standard HTTP endpoints
 * (health, status, negotiate) are served alongside.
 */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "h21_server.h"
#include "http_negotiation.h"
#include "../impl/core/sleela_net.h"
#include "../preferred-routers/preferred_router.h"
#include "../resolver/resolver.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <poll.h>
#include <unistd.h>
#endif

/* ── signal-safe stop flag ──────────────────────────────────────────── */

static volatile int g_stop = 0;

void h21_server_stop(void) { g_stop = 1; }

#ifndef _WIN32
static void h21_signal_handler(int sig) { (void)sig; g_stop = 1; }
#endif

/* ── helpers ────────────────────────────────────────────────────────── */

static void str_lower(char *s)
{
    for (; *s; ++s)
        *s = (char)tolower((unsigned char)*s);
}

static const char *str_trim_left(const char *s)
{
    while (*s == ' ' || *s == '\t') ++s;
    return s;
}

static const char *reason_phrase(int code)
{
    switch (code) {
    case 200: return "OK";
    case 204: return "No Content";
    case 400: return "Bad Request";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 413: return "Content Too Large";
    case 500: return "Internal Server Error";
    default:  return "Error";
    }
}

static int portable_poll_readable(SLNetHandle fd, int timeout_ms);

/* ── request parsing ────────────────────────────────────────────────── */

static int recv_headers(SLNetHandle fd, char *buf, size_t cap)
{
    size_t total = 0;
    while (total < cap) {
        if (!portable_poll_readable(fd, 5000)) return -1;
        SLNetCount n = slnet_read(fd, buf + total, cap - total);
        if (n <= 0) return total > 0 ? (int)total : -1;
        total += (size_t)n;
        if (total >= 4) {
            const char *end = buf + (total >= H21_SERVER_MAX_HEADER
                                     ? H21_SERVER_MAX_HEADER : total);
            for (const char *p = buf + (total - n > 3 ? total - n - 3 : 0);
                 p + 3 < end; ++p) {
                if (p[0] == '\r' && p[1] == '\n' &&
                    p[2] == '\r' && p[3] == '\n')
                    return (int)total;
            }
        }
    }
    return -1;
}

static int find_header(const h21_http_request *req, const char *name)
{
    for (int i = 0; i < req->header_count; ++i)
        if (strcmp(req->headers[i].name, name) == 0)
            return i;
    return -1;
}

static const char *get_header(const h21_http_request *req, const char *name)
{
    int i = find_header(req, name);
    return i >= 0 ? req->headers[i].value : NULL;
}

static int is_token_char(unsigned char c)
{
    if(isalnum(c))return 1;
    return strchr("!#$%&'*+-.^_`|~", (int)c) != NULL;
}

static int parse_request(const char *raw,size_t raw_len,h21_http_request *req,
                         const char **body_start,size_t *header_bytes)
{
    const char *p=raw,*end=raw+raw_len,*line_end=NULL,*sp1,*sp2;int cl_seen=0;
    if(!raw||!req||!body_start||!header_bytes)return -1;
    memset(req,0,sizeof(*req));
    for(const char *q=raw;q+1<end;++q)if(q[0]=='\r'&&q[1]=='\n'){line_end=q;break;}
    if(!line_end||line_end==p)return -1;
    sp1=memchr(p,' ',(size_t)(line_end-p));if(!sp1||sp1==p)return -1;
    sp2=memchr(sp1+1,' ',(size_t)(line_end-sp1-1));
    if(!sp2||sp2==sp1+1||memchr(sp2+1,' ',(size_t)(line_end-sp2-1)))return -1;
    if((size_t)(sp1-p)>=sizeof(req->method)||(size_t)(sp2-sp1-1)>=sizeof(req->uri)||
       (size_t)(line_end-sp2-1)>=sizeof(req->version))return -1;
    for(const unsigned char *q=(const unsigned char *)p;q<(const unsigned char *)sp1;++q)
        if(!is_token_char(*q))return -1;
    memcpy(req->method,p,(size_t)(sp1-p));memcpy(req->uri,sp1+1,(size_t)(sp2-sp1-1));
    memcpy(req->version,sp2+1,(size_t)(line_end-sp2-1));
    if(strcmp(req->version,"HTTP/1.0")&&strcmp(req->version,"HTTP/1.1"))return -1;
    for(const unsigned char *q=(const unsigned char *)req->uri;*q;++q)if(*q<=0x20||*q==0x7f)return -1;
    p=line_end+2;
    while(p+1<end&&!(p[0]=='\r'&&p[1]=='\n')){
        const char *e=NULL;for(const char *q=p;q+1<end;++q)if(q[0]=='\r'&&q[1]=='\n'){e=q;break;}
        if(!e||e==p||*p==' '||*p=='\t'||req->header_count>=H21_SERVER_MAX_HEADERS)return -1;
        const char *line_next=e+2;
        const char *colon=memchr(p,':',(size_t)(e-p));if(!colon||colon==p)return -1;
        for(const unsigned char *q=(const unsigned char *)p;q<(const unsigned char *)colon;++q)
            if(!is_token_char(*q))return -1;
        h21_http_header *h=&req->headers[req->header_count];size_t nl=(size_t)(colon-p);
        if(nl>=sizeof(h->name))return -1;
        memcpy(h->name,p,nl);h->name[nl]='\0';str_lower(h->name);
        const char *v=str_trim_left(colon+1);const char *value_end=e;while(value_end>v&&(value_end[-1]==' '||value_end[-1]=='\t'))--value_end;
        size_t vl=(size_t)(value_end-v);if(vl>=sizeof(h->value))return -1;
        for(const unsigned char *q=(const unsigned char *)v;q<(const unsigned char *)value_end;++q)
            if((*q<0x20&&*q!='\t')||*q==0x7f)return -1;
        memcpy(h->value,v,vl);h->value[vl]='\0';
        if(!strcmp(h->name,"content-length")){
            if(cl_seen++||!vl)return -1;
            for(size_t k=0;k<vl;++k)if(h->value[k]<'0'||h->value[k]>'9')return -1;
        }
        if(!strcmp(h->name,"transfer-encoding"))return -1;
        ++req->header_count;p=line_next;
    }
    if(p+1>=end||p[0]!='\r'||p[1]!='\n')return -1;
    p+=2;*header_bytes=(size_t)(p-raw);*body_start=p;
    const char *v;
    v=get_header(req,"x-sleela-version");if(v)snprintf(req->sleela_version,sizeof(req->sleela_version),"%s",v);
    v=get_header(req,"x-sleela-accept");if(v)snprintf(req->sleela_accept,sizeof(req->sleela_accept),"%s",v);
    v=get_header(req,"x-sleela-port");if(v)snprintf(req->sleela_port,sizeof(req->sleela_port),"%s",v);
    v=get_header(req,"x-sleela-service-id");if(v)snprintf(req->service_id,sizeof(req->service_id),"%s",v);
    v=get_header(req,"x-sleela-op-id");if(v)snprintf(req->op_id,sizeof(req->op_id),"%s",v);
    return 0;
}

/* ── response serialisation ─────────────────────────────────────────── */

int h21_http_response_add_header(h21_http_response *resp,const char *name,const char *value)
{
    if(!resp||!name||!value||resp->header_count<0||resp->header_count>=H21_SERVER_MAX_HEADERS)return -1;
    size_t nl=strlen(name),vl=strlen(value);if(!nl||nl>=H21_SERVER_MAX_HNAME||vl>=H21_SERVER_MAX_HVALUE)return -1;
    for(size_t i=0;i<nl;++i)if(!is_token_char((unsigned char)name[i]))return -1;
    for(size_t i=0;i<vl;++i){unsigned char c=(unsigned char)value[i];if((c<0x20&&c!='\t')||c==0x7f)return -1;}
    h21_http_header *h=&resp->headers[resp->header_count];memcpy(h->name,name,nl+1);memcpy(h->value,value,vl+1);
    ++resp->header_count;return 0;
}

static int send_all(SLNetHandle fd, const char *data, size_t len)
{
    while (len > 0) {
        SLNetCount n = slnet_write(fd, data, len);
        if (n <= 0) return -1;
        data += n;
        len  -= (size_t)n;
    }
    return 0;
}

static int valid_header_text(const char *s,int name)
{
    if(!s||!*s)return 0;
    for(const unsigned char *p=(const unsigned char *)s;*p;++p)
        if(name?!is_token_char(*p):((*p<0x20&&*p!='\t')||*p==0x7f))return 0;
    return 1;
}
static int send_response(SLNetHandle fd,const h21_http_request *req,const h21_http_response *resp,const char *server_name)
{
    char buf[H21_SERVER_MAX_HEADER];size_t off=0;
    const char *reason=resp->reason?resp->reason:reason_phrase(resp->status_code);
#define APPEND_FMT(...) do{if(off>=sizeof(buf))return -1;int z=snprintf(buf+off,sizeof(buf)-off,__VA_ARGS__);if(z<0||(size_t)z>=sizeof(buf)-off)return -1;off+=(size_t)z;}while(0)
    if(!req||!resp||resp->status_code<100||resp->status_code>599||!valid_header_text(reason,0)||
       !valid_header_text(server_name,0)||(resp->body_length&&!resp->body)||resp->body_length>H21_SERVER_MAX_BODY||
       resp->header_count<0||resp->header_count>H21_SERVER_MAX_HEADERS||
       (resp->content_type&&!valid_header_text(resp->content_type,0)))return -1;
    APPEND_FMT("%s %d %s\r\n",req->version[0]?req->version:"HTTP/1.0",resp->status_code,reason);
    APPEND_FMT("Server: %s\r\n",server_name);
    if(resp->content_type)APPEND_FMT("Content-Type: %s\r\n",resp->content_type);
    APPEND_FMT("Content-Length: %zu\r\n",resp->body_length);
    for(int i=0;i<resp->header_count;++i){
        if(!valid_header_text(resp->headers[i].name,1)||!valid_header_text(resp->headers[i].value,0))return -1;
        if(!strcmp(resp->headers[i].name,"Content-Length")||!strcmp(resp->headers[i].name,"Transfer-Encoding")||!strcmp(resp->headers[i].name,"Connection"))return -1;
        APPEND_FMT("%s: %s\r\n",resp->headers[i].name,resp->headers[i].value);
    }
    APPEND_FMT("X-SLeeLa-Version: 2.1\r\nX-SLeeLa-Accept: 2.1, 2.0, 1.1, 1.0\r\nConnection: close\r\n\r\n");
#undef APPEND_FMT
    if(send_all(fd,buf,off))return -1;
    if(resp->body_length)return send_all(fd,resp->body,resp->body_length);
    return 0;
}

/* ── download helpers ───────────────────────────────────────────────── */

int h21_download_should_resume(uint64_t content_length)
{
    return content_length > H21_DOWNLOAD_THRESHOLD ? 1 : 0;
}

int h21_download_serialise(const h21_download_state *st,char *buf,size_t cap)
{
    if(!st||!buf||cap<512)return -1;
    const char *v[]={st->session_id,st->datetime,st->file_id,st->file_name};
    for(size_t i=0;i<4;++i){if(!v[i][0]||strchr(v[i],'|')||strchr(v[i],'/')||strchr(v[i],'\\')||strstr(v[i],".."))return -1;
        for(const unsigned char *p=(const unsigned char *)v[i];*p;++p)if(*p<0x20||*p==0x7f)return -1;}
    if(st->offset>st->total_size)return -1;
    int n=snprintf(buf,cap,"%s | %s | %s | %s | %llu | %llu | %llu",v[0],v[1],v[2],v[3],
       (unsigned long long)st->index,(unsigned long long)st->offset,(unsigned long long)st->total_size);
    return n>0&&(size_t)n<cap?0:-1;
}
static int parse_u64_strict(const char *s,uint64_t *out)
{
    char *end=NULL;unsigned long long n;if(!s||!*s||!out)return -1;
    for(const unsigned char *p=(const unsigned char *)s;*p;++p)if(*p<'0'||*p>'9')return -1;
    errno=0;n=strtoull(s,&end,10);if(errno==ERANGE||!end||*end)return -1;*out=(uint64_t)n;return 0;
}
int h21_download_parse(const char *line,h21_download_state *out)
{
    if(!line||!out||strlen(line)>=2048)return -1;
    char copy[2048];memcpy(copy,line,strlen(line)+1);char *fields[7];size_t count=0;char *p=copy;
    while(count<7){char *sep=strchr(p,'|');if(sep)*sep='\0';while(*p==' '||*p=='\t')++p;
        size_t n=strlen(p);while(n&&(p[n-1]==' '||p[n-1]=='\t'))p[--n]='\0';if(!n)return -1;
        fields[count++]=p;if(!sep)break;if(count==7)return -1;p=sep+1;}
    if(count!=7)return -1;
    memset(out,0,sizeof(*out));
    if(strlen(fields[0])>=sizeof(out->session_id)||strlen(fields[1])>=sizeof(out->datetime)||
       strlen(fields[2])>=sizeof(out->file_id)||strlen(fields[3])>=sizeof(out->file_name))return -1;
    const char *v[]={fields[0],fields[1],fields[2],fields[3]};
    for(size_t i=0;i<4;++i){if(strchr(v[i],'/')||strchr(v[i],'\\')||strchr(v[i],'|')||strstr(v[i],".."))return -1;
        for(const unsigned char *q=(const unsigned char *)v[i];*q;++q)if(*q<0x20||*q==0x7f)return -1;}
    if(parse_u64_strict(fields[4],&out->index)||parse_u64_strict(fields[5],&out->offset)||
       parse_u64_strict(fields[6],&out->total_size)||out->offset>out->total_size)return -1;
    memcpy(out->session_id,fields[0],strlen(fields[0])+1);memcpy(out->datetime,fields[1],strlen(fields[1])+1);
    memcpy(out->file_id,fields[2],strlen(fields[2])+1);memcpy(out->file_name,fields[3],strlen(fields[3])+1);
    return 0;
}

/* ── built-in route handlers ────────────────────────────────────────── */

static int builtin_health(const h21_http_request *req, h21_http_response *resp)
{
    (void)req;
    static const char body[] = "SLeeLa HTTP/2.1 OK\n";
    resp->status_code  = 200;
    resp->content_type = "text/plain; charset=utf-8";
    resp->body         = body;
    resp->body_length  = sizeof(body) - 1;
    return 0;
}

static int builtin_status(const h21_http_request *req, h21_http_response *resp)
{
    (void)req;
    static char buf[512];
    int n = snprintf(buf, sizeof(buf),
        "{\"server\":\"SLeeLa\","
         "\"http_generation\":\"2.1\","
         "\"vm_generation\":2,"
         "\"vm_name\":\"Operator\","
         "\"vm_path\":\"/2\","
         "\"platform\":\"%s\","
         "\"route_data_schema\":\"1.0\","
         "\"envelope_version\":%u,"
         "\"naming\":true,"
         "\"pipeline\":true,"
         "\"integrity_gate\":false,"
         "\"lower_http_compatibility\":true}\n",
        slnet_platform_name(), H21_ENVELOPE_VERSION);
    resp->status_code  = 200;
    resp->content_type = "application/json";
    resp->body         = buf;
    resp->body_length  = (size_t)n;
    return 0;
}

static int builtin_negotiate(const h21_http_request *req,
                             h21_http_response *resp, int allow_fallback)
{
    const char *negotiated = sleela_http_negotiate(
        req->sleela_version[0] ? req->sleela_version : NULL,
        req->sleela_accept[0]  ? req->sleela_accept  : NULL,
        allow_fallback);

    static char buf[256];
    int n;
    if (negotiated) {
        n = snprintf(buf, sizeof(buf),
                     "{\"negotiated\":\"%s\",\"ok\":true}\n", negotiated);
        resp->status_code = 200;
    } else {
        n = snprintf(buf, sizeof(buf),
                     "{\"negotiated\":null,\"ok\":false}\n");
        resp->status_code = 400;
    }
    resp->content_type = "application/json";
    resp->body         = buf;
    resp->body_length  = (size_t)n;
    return 0;
}

/* ── H2.1 envelope endpoint ─────────────────────────────────────────── */

static int builtin_h21_envelope(const h21_http_request *req,
                                h21_http_response      *resp,
                                h21_pipeline_t         *pipeline)
{
    if (!req->body || req->body_length == 0) {
        resp->status_code  = 400;
        resp->content_type = "text/plain";
        resp->body         = "Envelope body required\n";
        resp->body_length  = 22;
        return 0;
    }

    static char result[H21_ENVELOPE_MAX_PAYLOAD + 256];
    size_t written = 0;

    int rc = h21_pipeline_handle_wire(pipeline,
                                      (const uint8_t *)req->body,
                                      req->body_length,
                                      result, sizeof(result), &written);
    if (rc != 0) {
        resp->status_code  = 500;
        resp->content_type = "text/plain";
        resp->body         = "Pipeline error\n";
        resp->body_length  = 15;
        return 0;
    }

    resp->status_code  = 200;
    resp->content_type = "application/x-sleela-h21";
    resp->body         = result;
    resp->body_length  = written;
    return 0;
}

/* ── connection handler ─────────────────────────────────────────────── */

static void handle_connection(SLNetHandle fd, const h21_server_config *cfg)
{
    size_t raw_cap = H21_SERVER_MAX_HEADER + 4096;
    char  *raw = malloc(raw_cap);
    if (!raw) { slnet_close(fd); return; }

    int got = recv_headers(fd, raw, raw_cap);
    if (got <= 0) { free(raw); slnet_close(fd); return; }

    h21_http_request  req;
    h21_http_response resp;
    const char       *body_start = NULL;
    size_t            header_bytes = 0;

    memset(&resp, 0, sizeof(resp));
    resp.status_code = 200;

    if (parse_request(raw, (size_t)got, &req, &body_start, &header_bytes) != 0) {
        resp.status_code  = 400;
        resp.content_type = "text/plain";
        resp.body         = "Bad Request\n";
        resp.body_length  = 12;
        h21_http_request bad;
        memset(&bad, 0, sizeof(bad));
        memcpy(bad.version, "HTTP/1.0", 9);
        send_response(fd, &bad, &resp, cfg->server_name);
        free(raw); slnet_close(fd); return;
    }

    /* Read body if Content-Length present. */
    {
        const char *cl = get_header(&req, "content-length");
        if (cl) {
            uint64_t parsed_length = 0;
            if (parse_u64_strict(cl, &parsed_length) != 0 ||
                parsed_length > H21_SERVER_MAX_BODY || parsed_length > (uint64_t)SIZE_MAX) {
                resp.status_code = 400;
                resp.content_type = "text/plain";
                resp.body = "Bad Content-Length\n";
                resp.body_length = strlen(resp.body);
                send_response(fd, &req, &resp, cfg->server_name);
                free(raw); slnet_close(fd); return;
            }
            size_t content_len = (size_t)parsed_length;
            if (content_len > H21_SERVER_MAX_BODY) {
                resp.status_code  = 413;
                resp.content_type = "text/plain";
                resp.body         = "Content Too Large\n";
                resp.body_length  = 18;
                send_response(fd, &req, &resp, cfg->server_name);
                free(raw); slnet_close(fd); return;
            }
            size_t already = (size_t)got - header_bytes;
            if (already > content_len) {
                resp.status_code = 400; resp.content_type = "text/plain";
                resp.body = "Unexpected bytes after request body\n"; resp.body_length = strlen(resp.body);
                send_response(fd, &req, &resp, cfg->server_name);
                free(raw); slnet_close(fd); return;
            }
            req.body = malloc(content_len + 1);
            if (!req.body) {
                resp.status_code  = 500;
                resp.content_type = "text/plain";
                resp.body         = "Internal Server Error\n";
                resp.body_length  = 22;
                send_response(fd, &req, &resp, cfg->server_name);
                free(raw); slnet_close(fd); return;
            }
            if (already > 0 && already <= content_len)
                memcpy(req.body, body_start, already);
            size_t remaining = already < content_len ? content_len - already : 0;
            size_t filled    = already < content_len ? already : content_len;
            while (remaining > 0) {
                if (!portable_poll_readable(fd, 5000)) break;
                SLNetCount n = slnet_read(fd, req.body + filled, remaining);
                if (n <= 0) break;
                filled    += (size_t)n;
                remaining -= (size_t)n;
            }
            if (remaining != 0) {
                free(req.body); req.body = NULL;
                resp.status_code = 400; resp.content_type = "text/plain";
                resp.body = "Incomplete request body\n"; resp.body_length = strlen(resp.body);
                send_response(fd, &req, &resp, cfg->server_name);
                free(raw); slnet_close(fd); return;
            }
            req.body_length = filled;
            req.body[filled] = '\0';
        }
    }

    free(raw);
    raw = NULL;

    /* ---- built-in routes ---- */
    int handled = 0;

    if (strcmp(req.uri, "/__sleela/health") == 0) {
        builtin_health(&req, &resp);
        handled = 1;
    } else if (strcmp(req.uri, "/__sleela/status") == 0) {
        builtin_status(&req, &resp);
        handled = 1;
    } else if (strcmp(req.uri, "/__sleela/negotiate") == 0) {
        builtin_negotiate(&req, &resp, cfg->allow_negotiation_fallback);
        handled = 1;
    } else if (strcmp(req.uri, "/__sleela/h21") == 0 &&
               strcmp(req.method, "POST") == 0 && cfg->pipeline) {
        builtin_h21_envelope(&req, &resp, cfg->pipeline);
        handled = 1;
    }

    /* ---- application handler ---- */
    if (!handled && cfg->handler) {
        int rc = cfg->handler(&req, &resp, cfg->user_data);
        if (rc != 0) {
            resp.status_code  = 500;
            resp.content_type = "text/plain";
            resp.body         = "Internal Server Error\n";
            resp.body_length  = 22;
        }
    } else if (!handled) {
        resp.status_code  = 404;
        resp.content_type = "text/plain";
        resp.body         = "Not Found\n";
        resp.body_length  = 10;
    }

    send_response(fd, &req, &resp, cfg->server_name);
    free(req.body);
    slnet_close(fd);
}

/* ── portable poll ──────────────────────────────────────────────────── */

static int portable_poll_readable(SLNetHandle fd, int timeout_ms)
{
#ifdef _WIN32
    WSAPOLLFD pfd;
    pfd.fd = (SOCKET)fd; pfd.events = POLLIN; pfd.revents = 0;
    return WSAPoll(&pfd, 1, timeout_ms) > 0 && (pfd.revents & POLLIN);
#else
    struct pollfd pfd;
    pfd.fd = (int)fd; pfd.events = POLLIN; pfd.revents = 0;
    return poll(&pfd, 1, timeout_ms) > 0 && (pfd.revents & POLLIN);
#endif
}

/* ── server entry point ─────────────────────────────────────────────── */

void h21_server_config_init(h21_server_config *cfg)
{
    if (!cfg) return;
    memset(cfg, 0, sizeof(*cfg));
    cfg->port        = 8081;
    cfg->backlog     = 128;
    cfg->server_name = "SLeeLa-HTTP/2.1";
    cfg->allow_negotiation_fallback = 1;
}

int h21_server_run(const h21_server_config *cfg)
{
    if (!cfg) return -1;
    g_stop = 0;

    if (slnet_startup() != 0) {
        fprintf(stderr, "h21: slnet_startup failed\n");
        return -1;
    }

    if (!sleela_preferred_router_packet_policy("HTTP/2.0", 1))
        fprintf(stderr, "h21: preferred-router policy unavailable, "
                        "continuing with built-in defaults\n");
    if (!sleela_resolver_event_policy("HTTP/2.0", 0))
        fprintf(stderr, "h21: resolver event policy check returned false, "
                        "continuing\n");

    sleela_preferred_router_startup(NULL, "HTTP/2.0");

    SLNetHandle listener = slnet_listen(cfg->port, cfg->backlog);
    if (!slnet_valid(listener)) {
        fprintf(stderr, "h21: failed to listen on port %u\n",
                (unsigned)cfg->port);
        slnet_shutdown();
        return -1;
    }

    fprintf(stderr, "SLeeLa HTTP/2.1 listening on port %u  [%s]\n",
            (unsigned)cfg->port, slnet_platform_name());

#ifndef _WIN32
    signal(SIGINT,  h21_signal_handler);
    signal(SIGTERM, h21_signal_handler);
#endif

    while (!g_stop) {
        if (!portable_poll_readable(listener, 500))
            continue;
        SLNetHandle client = slnet_accept(listener);
        if (!slnet_valid(client))
            continue;
        handle_connection(client, cfg);
    }

    slnet_close(listener);
    slnet_shutdown();
    fprintf(stderr, "SLeeLa HTTP/2.1 stopped.\n");
    return 0;
}

/* ── test echo handler (used by self-test) ──────────────────────────── */

static h21_status_t test_echo_handler(const uint8_t *payload,
                                      size_t         payload_len,
                                      uint8_t       *result,
                                      size_t         result_cap,
                                      size_t        *result_len,
                                      void          *ctx)
{
    (void)ctx;
    size_t n = payload_len < result_cap ? payload_len : result_cap;
    memcpy(result, payload, n);
    *result_len = n;
    return H21_STATUS_OK;
}

/* ── self-test ──────────────────────────────────────────────────────── */

int h21_self_test(void)
{
    int failures = 0;

    /* 1. HTTP request parse round-trip */
    {
        const char raw[] =
            "POST /__sleela/h21 HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 0\r\n"
            "X-SLeeLa-Version: 2.1\r\n"
            "X-SLeeLa-Accept: 2.1, 2.0, 1.1, 1.0\r\n"
            "X-SLeeLa-Port: 443\r\n"
            "X-SLeeLa-Service-ID: test-service\r\n"
            "X-SLeeLa-Op-ID: echo\r\n"
            "\r\n";

        h21_http_request req;
        const char *body_start;
        size_t header_bytes;

        if (parse_request(raw, sizeof(raw) - 1, &req,
                          &body_start, &header_bytes) != 0) {
            fprintf(stderr, "FAIL: HTTP parse_request\n");
            ++failures;
        } else {
            if (strcmp(req.method, "POST") != 0) {
                fprintf(stderr, "FAIL: method = '%s'\n", req.method);
                ++failures;
            }
            if (strcmp(req.sleela_version, "2.1") != 0) {
                fprintf(stderr, "FAIL: sleela_version = '%s'\n",
                        req.sleela_version);
                ++failures;
            }
            if (strcmp(req.service_id, "test-service") != 0) {
                fprintf(stderr, "FAIL: service_id = '%s'\n", req.service_id);
                ++failures;
            }
        }
    }

    /* 2. Envelope textual round-trip */
    {
        h21_envelope_t env, env2;
        const uint8_t payload[] = "test-payload";
        h21_envelope_init(&env, 42, 7, 12345ULL, H21_FLAG_NONE,
                          payload, sizeof(payload) - 1);

        char buf[512];
        size_t written;
        if (h21_envelope_pack_text(&env, buf, sizeof(buf), &written) != 0) {
            fprintf(stderr, "FAIL: envelope pack_text\n");
            ++failures;
        } else if (h21_envelope_unpack_text(buf, written, &env2) != 0) {
            fprintf(stderr, "FAIL: envelope unpack_text\n");
            ++failures;
        } else {
            if (env2.service_id != 42 || env2.op_id != 7 ||
                env2.request_id != 12345ULL ||
                env2.payload_len != sizeof(payload) - 1 ||
                memcmp(env2.payload, payload, sizeof(payload) - 1) != 0) {
                fprintf(stderr, "FAIL: envelope text round-trip mismatch\n");
                ++failures;
            }
        }
    }

    /* 3. Envelope binary round-trip */
    {
        h21_envelope_t env, env2;
        const uint8_t payload[] = "binary-test";
        h21_envelope_init(&env, 100, 3, 999ULL,
                          H21_FLAG_BINARY, payload, sizeof(payload) - 1);

        uint8_t buf[256];
        size_t written;
        if (h21_envelope_pack_binary(&env, buf, sizeof(buf), &written) != 0) {
            fprintf(stderr, "FAIL: envelope pack_binary\n");
            ++failures;
        } else if (h21_envelope_unpack_binary(buf, written, &env2) != 0) {
            fprintf(stderr, "FAIL: envelope unpack_binary\n");
            ++failures;
        } else {
            if (env2.service_id != 100 || env2.op_id != 3 ||
                env2.request_id != 999ULL) {
                fprintf(stderr, "FAIL: envelope binary round-trip mismatch\n");
                ++failures;
            }
        }
    }

    /* 4. Naming dictionary */
    {
        h21_naming_t dict;
        h21_naming_init(&dict);

        uint32_t svc = h21_naming_intern_service(&dict, "math");
        if (svc == 0) {
            fprintf(stderr, "FAIL: naming intern_service returned 0\n");
            ++failures;
        }

        uint32_t svc2 = h21_naming_intern_service(&dict, "math");
        if (svc2 != svc) {
            fprintf(stderr, "FAIL: naming second intern returned %u != %u\n",
                    svc2, svc);
            ++failures;
        }

        uint32_t op = h21_naming_intern_op(&dict, "math", "add");
        if (op == 0) {
            fprintf(stderr, "FAIL: naming intern_op returned 0\n");
            ++failures;
        }

        uint32_t lookup = h21_naming_lookup_service(&dict, "math");
        if (lookup != svc) {
            fprintf(stderr, "FAIL: naming lookup = %u\n", lookup);
            ++failures;
        }

        const char *name = h21_naming_service_name(&dict, svc);
        if (!name || strcmp(name, "math") != 0) {
            fprintf(stderr, "FAIL: naming reverse = '%s'\n",
                    name ? name : "(null)");
            ++failures;
        }

        if (h21_naming_lookup_service(&dict, "nonexistent") != 0) {
            fprintf(stderr, "FAIL: naming unknown should be 0\n");
            ++failures;
        }
    }

    /* 5. Pipeline register + dispatch */
    {
        h21_pipeline_t pipe;
        h21_pipeline_init(&pipe);

        uint32_t svc_id = h21_pipeline_register_service(&pipe, "echo-svc", NULL);
        uint32_t op_id  = h21_pipeline_register_op(&pipe, "echo-svc", "echo",
                                                   H21_RETRY_READ,
                                                   test_echo_handler);

        if (svc_id == 0 || op_id == 0) {
            fprintf(stderr, "FAIL: pipeline register svc=%u op=%u\n",
                    svc_id, op_id);
            ++failures;
        } else {
            h21_envelope_t env;
            const uint8_t payload[] = "ping";
            h21_envelope_init(&env, svc_id, op_id, 42ULL, H21_FLAG_NONE,
                              payload, sizeof(payload) - 1);

            h21_response_t resp;
            if (h21_pipeline_dispatch(&pipe, &env, &resp) != 0) {
                fprintf(stderr, "FAIL: pipeline dispatch returned error\n");
                ++failures;
            } else {
                if (resp.status != H21_STATUS_OK) {
                    fprintf(stderr, "FAIL: dispatch status = %d\n",
                            (int)resp.status);
                    ++failures;
                }
                if (resp.request_id != 42ULL) {
                    fprintf(stderr, "FAIL: resp request_id = %llu\n",
                            (unsigned long long)resp.request_id);
                    ++failures;
                }
                if (resp.result_len != 4 ||
                    memcmp(resp.result, "ping", 4) != 0) {
                    fprintf(stderr, "FAIL: dispatch result mismatch\n");
                    ++failures;
                }
            }

            /* Wire round-trip: textual envelope → pipeline → textual response. */
            char wire[512];
            size_t wire_len;
            h21_envelope_pack_text(&env, wire, sizeof(wire), &wire_len);

            char out[512];
            size_t out_len;
            if (h21_pipeline_handle_wire(&pipe, (const uint8_t *)wire, wire_len,
                                         out, sizeof(out), &out_len) != 0) {
                fprintf(stderr, "FAIL: handle_wire returned error\n");
                ++failures;
            } else {
                h21_response_t parsed_resp;
                if (h21_response_unpack_text(out, out_len, &parsed_resp) != 0) {
                    fprintf(stderr, "FAIL: response unpack_text\n");
                    ++failures;
                } else if (parsed_resp.status != H21_STATUS_OK ||
                           parsed_resp.request_id != 42ULL) {
                    fprintf(stderr, "FAIL: wire round-trip mismatch\n");
                    ++failures;
                }
            }
        }

        /* Retry class query. */
        if (h21_pipeline_retry_class(&pipe, svc_id, op_id) != H21_RETRY_READ) {
            fprintf(stderr, "FAIL: retry_class = %d\n",
                    h21_pipeline_retry_class(&pipe, svc_id, op_id));
            ++failures;
        }

        /* Unknown service dispatch. */
        {
            h21_envelope_t bad_env;
            h21_envelope_init(&bad_env, 999, 1, 1ULL, 0, NULL, 0);
            h21_response_t bad_resp;
            h21_pipeline_dispatch(&pipe, &bad_env, &bad_resp);
            if (bad_resp.status != H21_STATUS_UNKNOWN_SVC) {
                fprintf(stderr, "FAIL: unknown svc status = %d\n",
                        (int)bad_resp.status);
                ++failures;
            }
        }
    }

    /* 6. Download state round-trip */
    {
        h21_download_state st = {0};
        snprintf(st.session_id, sizeof(st.session_id), "h21-sess-001");
        snprintf(st.datetime,   sizeof(st.datetime),   "2026-10-15T00:00:00Z");
        snprintf(st.file_id,    sizeof(st.file_id),    "file-xyz");
        snprintf(st.file_name,  sizeof(st.file_name),  "largefile.dat");
        st.index = 5; st.offset = 262144000ULL; st.total_size = 524288000ULL;

        char buf[512];
        if (h21_download_serialise(&st, buf, sizeof(buf)) != 0) {
            fprintf(stderr, "FAIL: download serialise\n");
            ++failures;
        } else {
            h21_download_state parsed;
            if (h21_download_parse(buf, &parsed) != 0) {
                fprintf(stderr, "FAIL: download parse\n");
                ++failures;
            } else if (parsed.index != 5 || parsed.offset != 262144000ULL) {
                fprintf(stderr, "FAIL: download round-trip mismatch\n");
                ++failures;
            }
        }
    }

    /* 7. Negotiation */
    {
        const char *r = sleela_http_negotiate("2.0", "2.0, 1.1, 1.0", 1);
        if (!r || strcmp(r, "2.0") != 0) {
            fprintf(stderr, "FAIL: negotiate 2.0 = '%s'\n", r ? r : "(null)");
            ++failures;
        }
        r = sleela_http_negotiate("3.0", "2.0, 1.1, 1.0", 1);
        if (!r || strcmp(r, "1.1") != 0) {
            fprintf(stderr, "FAIL: negotiate fallback = '%s'\n",
                    r ? r : "(null)");
            ++failures;
        }
    }

    if (failures == 0)
        fprintf(stderr, "h21_self_test: all tests passed\n");
    else
        fprintf(stderr, "h21_self_test: %d test(s) FAILED\n", failures);

    return failures;
}
