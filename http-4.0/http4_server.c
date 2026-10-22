/* http4_server.c — SLeeLa HTTP 4.0 portable server implementation
 *
 * Platforms: Linux, macOS, Windows 10+
 *
 * Transport: HTTP/1.x over sleela_net portable sockets.
 * Application: SLeeLa HTTP 4.0 frame protocol — OPEN/DATA/END/RESET/WINDOW/
 * PING/PONG/RESUME/CAPSULE — rides inside the HTTP request body at
 * /__sleela/h4 (POST).  Frames are binary (32-byte header + payload).
 */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "http4_server.h"
#include "http_negotiation.h"
#include "../impl/core/sleela_net.h"
#include "../preferred-routers/preferred_router.h"
#include "../resolver/resolver.h"

#include <ctype.h>
#include <errno.h>
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

/* ── signal-safe stop ───────────────────────────────────────────────── */

static volatile int g_stop = 0;
void h4s_server_stop(void) { g_stop = 1; }
#ifndef _WIN32
static void h4s_sig(int s) { (void)s; g_stop = 1; }
#endif

/* ── helpers ────────────────────────────────────────────────────────── */

static void str_lower(char *s)
{ for (; *s; ++s) *s = (char)tolower((unsigned char)*s); }

static const char *str_ltrim(const char *s)
{ while (*s == ' ' || *s == '\t') ++s; return s; }

static const char *reason_phrase(int c)
{
    switch (c) {
    case 200: return "OK";
    case 400: return "Bad Request";
    case 404: return "Not Found";
    case 413: return "Content Too Large";
    case 500: return "Internal Server Error";
    default:  return "Error";
    }
}

/* ── HTTP request parsing ───────────────────────────────────────────── */

static int recv_headers(SLNetHandle fd, char *buf, size_t cap)
{
    size_t total = 0;
    while (total < cap) {
        SLNetCount n = slnet_read(fd, buf + total, cap - total);
        if (n <= 0) return total > 0 ? (int)total : -1;
        total += (size_t)n;
        if (total >= 4) {
            const char *lim = buf + (total > H4S_MAX_HEADER ? H4S_MAX_HEADER : total);
            for (const char *p = buf + (total - n > 3 ? total - n - 3 : 0);
                 p + 3 < lim; ++p)
                if (p[0]=='\r'&&p[1]=='\n'&&p[2]=='\r'&&p[3]=='\n') return (int)total;
        }
    }
    return -1;
}

static const char *get_header(const h4s_request *r, const char *n)
{ for (int i = 0; i < r->header_count; ++i)
    if (strcmp(r->headers[i].name, n) == 0) return r->headers[i].value;
  return NULL; }

static int parse_request(const char *raw, size_t raw_len, h4s_request *req,
                         const char **body_start, size_t *header_bytes)
{
    const char *p = raw, *end = raw + raw_len, *le;
    memset(req, 0, sizeof(*req));

    le = strstr(p, "\r\n"); if (!le) return -1;
    { const char *sp = memchr(p,' ',(size_t)(le-p));
      if (!sp||(size_t)(sp-p)>=sizeof(req->method)) return -1;
      memcpy(req->method,p,(size_t)(sp-p)); p=sp+1; }
    { const char *sp = memchr(p,' ',(size_t)(le-p));
      if (!sp||(size_t)(sp-p)>=sizeof(req->uri)) return -1;
      memcpy(req->uri,p,(size_t)(sp-p)); p=sp+1; }
    { size_t vl=(size_t)(le-p);
      if (vl>=sizeof(req->version)) return -1;
      memcpy(req->version,p,vl); }
    p = le + 2;

    while (p+1<end && !(p[0]=='\r'&&p[1]=='\n')) {
        le = strstr(p,"\r\n"); if (!le) break;
        const char *colon = memchr(p,':',(size_t)(le-p));
        if (!colon||colon==p) return -1;
        if (req->header_count>=H4S_MAX_HEADERS) return -1;
        h4s_header *h = &req->headers[req->header_count];
        size_t nl=(size_t)(colon-p);
        if (nl>=sizeof(h->name)) return -1;
        memcpy(h->name,p,nl); h->name[nl]='\0'; str_lower(h->name);
        const char *v = str_ltrim(colon+1);
        size_t vl=(size_t)(le-v);
        while (vl > 0 && (v[vl-1] == ' ' || v[vl-1] == '\t')) --vl;
        if (vl>=sizeof(h->value)) return -1;
        memcpy(h->value,v,vl); h->value[vl]='\0';
        ++req->header_count; p=le+2;
    }
    if (p+1<end && p[0]=='\r' && p[1]=='\n') p+=2;
    *header_bytes = (size_t)(p-raw); *body_start = p;

    { const char *v;
      v=get_header(req,"x-sleela-version");
      if(v) snprintf(req->sleela_version,sizeof(req->sleela_version),"%s",v);
      v=get_header(req,"x-sleela-accept");
      if(v) snprintf(req->sleela_accept,sizeof(req->sleela_accept),"%s",v);
      v=get_header(req,"x-sleela-port");
      if(v) snprintf(req->sleela_port,sizeof(req->sleela_port),"%s",v);
      v=get_header(req,"x-sleela-service-id");
      if(v) snprintf(req->service_id,sizeof(req->service_id),"%s",v);
      v=get_header(req,"x-sleela-op-id");
      if(v) snprintf(req->op_id,sizeof(req->op_id),"%s",v);
    }
    return 0;
}

/* ── response serialisation ─────────────────────────────────────────── */

int h4s_response_add_header(h4s_response *r, const char *n, const char *v)
{
    if (!r||!n||!v||r->header_count>=H4S_MAX_HEADERS) return -1;
    h4s_header *h = &r->headers[r->header_count++];
    snprintf(h->name,sizeof(h->name),"%s",n);
    snprintf(h->value,sizeof(h->value),"%s",v);
    return 0;
}

static int send_all(SLNetHandle fd, const char *d, size_t l)
{ while(l>0){SLNetCount n=slnet_write(fd,d,l);if(n<=0)return -1;d+=n;l-=(size_t)n;}return 0; }

static int send_response(SLNetHandle fd, const h4s_request *req,
                         const h4s_response *resp, const char *sn)
{
    char buf[H4S_MAX_HEADER]; int off=0;
    const char *reason = resp->reason ? resp->reason : reason_phrase(resp->status_code);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"%s %d %s\r\n",
                  req->version[0]?req->version:"HTTP/1.0",resp->status_code,reason);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Server: %s\r\n",sn);
    if(resp->content_type)
        off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Content-Type: %s\r\n",resp->content_type);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"Content-Length: %zu\r\n",resp->body_length);
    for(int i=0;i<resp->header_count;++i)
        off+=snprintf(buf+off,sizeof(buf)-(size_t)off,"%s: %s\r\n",
                      resp->headers[i].name,resp->headers[i].value);
    off+=snprintf(buf+off,sizeof(buf)-(size_t)off,
                  "X-SLeeLa-Version: 4.0\r\n"
                  "X-SLeeLa-Accept: 4.0, 3.0, 2.1, 1.1, 1.0\r\n"
                  "Connection: close\r\n\r\n");
    if(off<=0||(size_t)off>=sizeof(buf)) return -1;
    if(send_all(fd,buf,(size_t)off)!=0) return -1;
    if(resp->body&&resp->body_length>0) return send_all(fd,resp->body,resp->body_length);
    return 0;
}

/* ── download helpers ───────────────────────────────────────────────── */

int h4s_download_should_resume(uint64_t cl){return cl>H4S_DOWNLOAD_THRESHOLD;}

static int h4s_download_fields_valid(const h4s_download_state *st)
{
    if (!st ||
        !memchr(st->session_id, '\0', sizeof(st->session_id)) ||
        !memchr(st->datetime, '\0', sizeof(st->datetime)) ||
        !memchr(st->file_id, '\0', sizeof(st->file_id)) ||
        !memchr(st->file_name, '\0', sizeof(st->file_name)) ||
        !st->session_id[0] || !st->datetime[0] ||
        !st->file_id[0] || !st->file_name[0] || st->offset > st->total_size)
        return 0;
    /* A filename is a leaf name, never a path supplied by the peer. */
    if (strchr(st->file_name, '/') || strchr(st->file_name, '\\') ||
        strstr(st->file_name, "..")) return 0;
    return 1;
}

static int h4s_parse_u64(const char *s, uint64_t *out)
{
    if (!s || !*s || !out) return -1;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p)
        if (*p < '0' || *p > '9') return -1;
    errno = 0;
    char *end = NULL;
    unsigned long long v = strtoull(s, &end, 10);
    if (errno == ERANGE || !end || *end != '\0') return -1;
    *out = (uint64_t)v;
    return 0;
}

int h4s_download_serialise(const h4s_download_state *st,char *buf,size_t cap)
{
    if (!h4s_download_fields_valid(st) || !buf || cap == 0) return -1;
    int n=snprintf(buf,cap,"%s | %s | %s | %s | %llu | %llu | %llu",
                   st->session_id,st->datetime,st->file_id,st->file_name,
                   (unsigned long long)st->index,(unsigned long long)st->offset,
                   (unsigned long long)st->total_size);
    return(n>0&&(size_t)n<cap)?0:-1;
}

int h4s_download_parse(const char *line, h4s_download_state *out)
{
    if (!line || !out) return -1;
    size_t n = strlen(line);
    if (n == 0 || n >= 2048) return -1;
    char copy[2048];
    memcpy(copy, line, n + 1);
    char *fields[7];
    char *p = copy;
    for (int i = 0; i < 7; ++i) {
        fields[i] = p;
        char *sep = strchr(p, '|');
        if (i < 6) {
            if (!sep) return -1;
            *sep = '\0';
            p = sep + 1;
        } else if (sep) {
            return -1;
        }
        while (*fields[i] == ' ' || *fields[i] == '\t') ++fields[i];
        size_t fl = strlen(fields[i]);
        while (fl > 0 && (fields[i][fl-1] == ' ' || fields[i][fl-1] == '\t'))
            fields[i][--fl] = '\0';
        if (fl == 0) return -1;
    }
    h4s_download_state tmp;
    memset(&tmp, 0, sizeof(tmp));
    if (strlen(fields[0]) >= sizeof(tmp.session_id) ||
        strlen(fields[1]) >= sizeof(tmp.datetime) ||
        strlen(fields[2]) >= sizeof(tmp.file_id) ||
        strlen(fields[3]) >= sizeof(tmp.file_name)) return -1;
    memcpy(tmp.session_id, fields[0], strlen(fields[0]) + 1);
    memcpy(tmp.datetime, fields[1], strlen(fields[1]) + 1);
    memcpy(tmp.file_id, fields[2], strlen(fields[2]) + 1);
    memcpy(tmp.file_name, fields[3], strlen(fields[3]) + 1);
    if (h4s_parse_u64(fields[4], &tmp.index) != 0 ||
        h4s_parse_u64(fields[5], &tmp.offset) != 0 ||
        h4s_parse_u64(fields[6], &tmp.total_size) != 0 ||
        !h4s_download_fields_valid(&tmp)) return -1;
    *out = tmp;
    return 0;
}

/* ── built-in routes ────────────────────────────────────────────────── */

static int builtin_health(const h4s_request *r, h4s_response *resp)
{
    (void)r;
    static const char b[]="SLeeLa HTTP/4.0 OK\n";
    resp->status_code=200; resp->content_type="text/plain; charset=utf-8";
    resp->body=b; resp->body_length=sizeof(b)-1; return 0;
}

static int builtin_status(const h4s_request *r, h4s_response *resp)
{
    (void)r;
    static char b[512];
    int n=snprintf(b,sizeof(b),
        "{\"server\":\"SLeeLa\","
         "\"http_generation\":\"4.0\","
         "\"vm_generation\":4,"
         "\"vm_name\":\"Supervisor\","
         "\"vm_path\":\"/4\","
         "\"platform\":\"%s\","
         "\"route_data_schema\":\"1.0\","
         "\"frame_version\":%u,"
         "\"frame_types\":[\"OPEN\",\"DATA\",\"END\",\"RESET\",\"WINDOW\","
           "\"PING\",\"PONG\",\"RESUME\",\"CAPSULE\"],"
         "\"segmentation\":true,"
         "\"reassembly\":true,"
         "\"packet_context\":true,"
         "\"lower_http_compatibility\":true}\n",
        slnet_platform_name(), HTTP4_VERSION);
    resp->status_code=200; resp->content_type="application/json";
    resp->body=b; resp->body_length=(size_t)n; return 0;
}

static int builtin_negotiate(const h4s_request *r, h4s_response *resp, int fb)
{
    const char *neg=sleela_http_negotiate(
        r->sleela_version[0]?r->sleela_version:NULL,
        r->sleela_accept[0]?r->sleela_accept:NULL, fb);
    static char b[256]; int n;
    if(neg){n=snprintf(b,sizeof(b),"{\"negotiated\":\"%s\",\"ok\":true}\n",neg);resp->status_code=200;}
    else{n=snprintf(b,sizeof(b),"{\"negotiated\":null,\"ok\":false}\n");resp->status_code=400;}
    resp->content_type="application/json";resp->body=b;resp->body_length=(size_t)n;return 0;
}

/* ── H4 frame endpoint ──────────────────────────────────────────────── */

static int builtin_h4_frame(const h4s_request *req, h4s_response *resp,
                            h4s_frame_handler fh, void *ud)
{
    if(!req->body||req->body_length<HTTP4_FRAME_HEADER_SIZE){
        resp->status_code=400;resp->content_type="text/plain";
        resp->body="Frame body required (min 32 bytes)\n";resp->body_length=35;return 0;
    }
    http4_frame_view_t frame; size_t consumed;
    if(http4_frame_decode((const uint8_t*)req->body,req->body_length,
                          &frame,&consumed)!=0 || consumed != req->body_length){
        resp->status_code=400;resp->content_type="text/plain";
        resp->body="Invalid H4 frame or trailing bytes\n";
        resp->body_length=strlen(resp->body);return 0;
    }
    if (frame.header.sequence == UINT64_MAX) {
        resp->status_code=400;resp->content_type="text/plain";
        resp->body="Frame sequence exhausted\n";
        resp->body_length=strlen(resp->body);return 0;
    }

    /* If no frame handler, echo frame info as JSON. */
    static char result[4096]; int n;
    if(fh){
        static uint8_t resp_payload[4096]; size_t resp_len=0;
        int rc=fh(&frame,resp_payload,sizeof(resp_payload),&resp_len,ud);
        if(rc!=0 || resp_len > sizeof(resp_payload) || resp_len > UINT32_MAX){
            resp->status_code=500;resp->content_type="text/plain";
            resp->body="Frame handler error or oversized response\n";
            resp->body_length=strlen(resp->body);return 0;
        }
        /* Encode a response frame (PONG for PING, DATA for DATA/OPEN). */
        http4_frame_header_t rh;
        memset(&rh,0,sizeof(rh));
        rh.version=HTTP4_VERSION;
        rh.type=(frame.header.type==HTTP4_FRAME_PING)?HTTP4_FRAME_PONG:HTTP4_FRAME_DATA;
        rh.flags=HTTP4_FLAG_FIN;
        rh.stream_id=frame.header.stream_id;
        rh.request_id=frame.header.request_id;
        rh.sequence=frame.header.sequence+1;
        rh.payload_length=(uint32_t)resp_len;
        static uint8_t wire[4096+HTTP4_FRAME_HEADER_SIZE]; size_t written;
        if(http4_frame_encode(&rh,resp_payload,wire,sizeof(wire),&written)!=0){
            resp->status_code=500;resp->content_type="text/plain";
            resp->body="Frame encode error\n";resp->body_length=19;return 0;
        }
        resp->status_code=200;resp->content_type="application/x-sleela-h4";
        resp->body=(const char*)wire;resp->body_length=written;return 0;
    }
    /* Default: echo frame metadata as JSON. */
    n=snprintf(result,sizeof(result),
        "{\"frame_type\":\"%s\","
         "\"stream_id\":%llu,"
         "\"request_id\":%llu,"
         "\"sequence\":%llu,"
         "\"flags\":%u,"
         "\"payload_length\":%u}\n",
        http4_frame_type_name((http4_frame_type_t)frame.header.type),
        (unsigned long long)frame.header.stream_id,
        (unsigned long long)frame.header.request_id,
        (unsigned long long)frame.header.sequence,
        (unsigned)frame.header.flags,
        (unsigned)frame.header.payload_length);
    resp->status_code=200;resp->content_type="application/json";
    resp->body=result;resp->body_length=(size_t)n;return 0;
}

/* ── connection handler ─────────────────────────────────────────────── */

static void handle_connection(SLNetHandle fd, const h4s_server_config *cfg)
{
    size_t raw_cap=H4S_MAX_HEADER+4096;
    char *raw=malloc(raw_cap + 1u);
    if(!raw){slnet_close(fd);return;}
    int got=recv_headers(fd,raw,raw_cap);
    if(got<=0){free(raw);slnet_close(fd);return;}
    raw[(size_t)got] = '\0';

    h4s_request req; h4s_response resp;
    const char *bs=NULL; size_t hb=0;
    memset(&resp,0,sizeof(resp)); resp.status_code=200;

    if(parse_request(raw,(size_t)got,&req,&bs,&hb)!=0){
        resp.status_code=400;resp.content_type="text/plain";
        resp.body="Bad Request\n";resp.body_length=12;
        h4s_request bad;memset(&bad,0,sizeof(bad));memcpy(bad.version,"HTTP/1.0",9);
        send_response(fd,&bad,&resp,cfg->server_name);free(raw);slnet_close(fd);return;
    }
    {
      const char *cl = get_header(&req, "content-length");
      int cl_count = 0, te_count = 0;
      for (int i = 0; i < req.header_count; ++i) {
          if (strcmp(req.headers[i].name, "content-length") == 0) ++cl_count;
          if (strcmp(req.headers[i].name, "transfer-encoding") == 0) ++te_count;
      }
      /* This server implements Content-Length only, never ambiguous framing. */
      if (cl_count > 1 || te_count > 0) {
          resp.status_code=400; resp.content_type="text/plain";
          resp.body="Invalid message framing\n"; resp.body_length=strlen(resp.body);
          send_response(fd,&req,&resp,cfg->server_name);
          free(raw); slnet_close(fd); return;
      }
      if (cl) {
          uint64_t clen64 = 0;
          if (h4s_parse_u64(cl, &clen64) != 0 || clen64 > H4S_MAX_BODY) {
              resp.status_code = clen64 > H4S_MAX_BODY ? 413 : 400;
              resp.content_type = "text/plain";
              resp.body = resp.status_code == 413 ? "Content Too Large\n" : "Invalid Content-Length\n";
              resp.body_length = strlen(resp.body);
              send_response(fd,&req,&resp,cfg->server_name);
              free(raw); slnet_close(fd); return;
          }
          size_t clen = (size_t)clen64;
          size_t already = (size_t)got - hb;
          if (already > clen) {
              resp.status_code=400; resp.content_type="text/plain";
              resp.body="Body exceeds Content-Length\n"; resp.body_length=strlen(resp.body);
              send_response(fd,&req,&resp,cfg->server_name);
              free(raw); slnet_close(fd); return;
          }
          req.body = malloc(clen + 1u);
          if (!req.body) {
              resp.status_code=500; resp.content_type="text/plain";
              resp.body="Internal Server Error\n"; resp.body_length=22;
              send_response(fd,&req,&resp,cfg->server_name);
              free(raw); slnet_close(fd); return;
          }
          if (already) memcpy(req.body, bs, already);
          size_t filled = already, rem = clen - already;
          while (rem > 0) {
              SLNetCount n = slnet_read(fd, req.body + filled, rem);
              if (n <= 0) break;
              filled += (size_t)n; rem -= (size_t)n;
          }
          req.body_length = filled;
          req.body[filled] = '\0';
          if (rem != 0) {
              free(req.body);
              resp.status_code=400; resp.content_type="text/plain";
              resp.body="Truncated request body\n"; resp.body_length=23;
              send_response(fd,&req,&resp,cfg->server_name);
              free(raw); slnet_close(fd); return;
          }
      } else if ((size_t)got > hb) {
          resp.status_code=400; resp.content_type="text/plain";
          resp.body="Body requires Content-Length\n"; resp.body_length=strlen(resp.body);
          send_response(fd,&req,&resp,cfg->server_name);
          free(raw); slnet_close(fd); return;
      }
    }
    free(raw);raw=NULL;

    int handled=0;
    if(strcmp(req.uri,"/__sleela/health")==0){builtin_health(&req,&resp);handled=1;}
    else if(strcmp(req.uri,"/__sleela/status")==0){builtin_status(&req,&resp);handled=1;}
    else if(strcmp(req.uri,"/__sleela/negotiate")==0){
        builtin_negotiate(&req,&resp,cfg->allow_negotiation_fallback);handled=1;}
    else if(strcmp(req.uri,"/__sleela/h4")==0&&strcmp(req.method,"POST")==0){
        builtin_h4_frame(&req,&resp,cfg->frame_handler,cfg->user_data);handled=1;}

    if(!handled&&cfg->handler){
        int rc=cfg->handler(&req,&resp,cfg->user_data);
        if(rc!=0){resp.status_code=500;resp.content_type="text/plain";
            resp.body="Internal Server Error\n";resp.body_length=22;}
    }else if(!handled){resp.status_code=404;resp.content_type="text/plain";
        resp.body="Not Found\n";resp.body_length=10;}

    send_response(fd,&req,&resp,cfg->server_name);
    free(req.body);slnet_close(fd);
}

/* ── portable poll ──────────────────────────────────────────────────── */

static int portable_poll_readable(SLNetHandle fd, int timeout_ms)
{
#ifdef _WIN32
    WSAPOLLFD pfd;pfd.fd=(SOCKET)fd;pfd.events=POLLIN;pfd.revents=0;
    return WSAPoll(&pfd,1,timeout_ms)>0&&(pfd.revents&POLLIN);
#else
    struct pollfd pfd;pfd.fd=(int)fd;pfd.events=POLLIN;pfd.revents=0;
    return poll(&pfd,1,timeout_ms)>0&&(pfd.revents&POLLIN);
#endif
}

/* ── server entry point ─────────────────────────────────────────────── */

void h4s_server_config_init(h4s_server_config *cfg)
{
    if(!cfg)return; memset(cfg,0,sizeof(*cfg));
    cfg->port=8083; cfg->backlog=128;
    cfg->server_name="SLeeLa-HTTP/4.0";
    cfg->allow_negotiation_fallback=1;
}

int h4s_server_run(const h4s_server_config *cfg)
{
    if(!cfg)return -1; g_stop=0;
    if(slnet_startup()!=0){fprintf(stderr,"h4s: slnet_startup failed\n");return -1;}
    if(!sleela_preferred_router_packet_policy("HTTP/4.0",1))
        fprintf(stderr,"h4s: preferred-router policy unavailable, continuing\n");
    sleela_preferred_router_startup(NULL,"HTTP/4.0");

    SLNetHandle listener=slnet_listen(cfg->port,cfg->backlog);
    if(!slnet_valid(listener)){fprintf(stderr,"h4s: listen failed on port %u\n",(unsigned)cfg->port);slnet_shutdown();return -1;}
    fprintf(stderr,"SLeeLa HTTP/4.0 listening on port %u  [%s]\n",(unsigned)cfg->port,slnet_platform_name());

#ifndef _WIN32
    signal(SIGINT,h4s_sig);signal(SIGTERM,h4s_sig);
#endif
    while(!g_stop){if(!portable_poll_readable(listener,500))continue;
        SLNetHandle c=slnet_accept(listener);if(!slnet_valid(c))continue;handle_connection(c,cfg);}
    slnet_close(listener);slnet_shutdown();
    fprintf(stderr,"SLeeLa HTTP/4.0 stopped.\n");return 0;
}

/* ── test echo frame handler ────────────────────────────────────────── */

static int test_echo_frame(const http4_frame_view_t *frame,
                           uint8_t *resp_payload, size_t resp_cap,
                           size_t *resp_len, void *ud)
{
    (void)ud;
    size_t n=frame->header.payload_length<resp_cap?frame->header.payload_length:resp_cap;
    if(n>0) memcpy(resp_payload,frame->payload,n);
    *resp_len=n; return 0;
}

/* ── self-test ──────────────────────────────────────────────────────── */

int h4s_self_test(void)
{
    int failures=0;

    /* 1. HTTP parse */
    { const char raw[]="POST /__sleela/h4 HTTP/1.1\r\nHost: localhost\r\n"
          "Content-Length: 0\r\nX-SLeeLa-Version: 4.0\r\n\r\n";
      h4s_request req;const char *bs;size_t hb;
      if(parse_request(raw,sizeof(raw)-1,&req,&bs,&hb)!=0){
          fprintf(stderr,"FAIL: HTTP parse\n");++failures;}
      else if(strcmp(req.sleela_version,"4.0")!=0){
          fprintf(stderr,"FAIL: version='%s'\n",req.sleela_version);++failures;}
    }

    /* 2. Frame encode/decode round-trip */
    { http4_frame_header_t h={0};
      h.version=HTTP4_VERSION; h.type=HTTP4_FRAME_DATA;
      h.flags=HTTP4_FLAG_INCREMENTAL; h.stream_id=1; h.request_id=42;
      h.sequence=7; h.payload_length=4;
      const uint8_t payload[]="ping";
      uint8_t wire[128]; size_t written;
      if(http4_frame_encode(&h,payload,wire,sizeof(wire),&written)!=0){
          fprintf(stderr,"FAIL: frame encode\n");++failures;}
      else{
          http4_frame_view_t v; size_t consumed;
          if(http4_frame_decode(wire,written,&v,&consumed)!=0){
              fprintf(stderr,"FAIL: frame decode\n");++failures;}
          else if(v.header.stream_id!=1||v.header.request_id!=42||
                  v.header.sequence!=7||v.header.payload_length!=4||
                  memcmp(v.payload,"ping",4)!=0){
              fprintf(stderr,"FAIL: frame round-trip\n");++failures;}
      }
    }

    /* 3. Frame handler round-trip */
    { http4_frame_header_t h={0};
      h.version=HTTP4_VERSION;h.type=HTTP4_FRAME_PING;h.flags=0;
      h.stream_id=5;h.request_id=99;h.sequence=1;h.payload_length=0;
      uint8_t wire[128];size_t written;
      http4_frame_encode(&h,NULL,wire,sizeof(wire),&written);
      http4_frame_view_t fv;size_t consumed;
      http4_frame_decode(wire,written,&fv,&consumed);
      uint8_t resp_payload[64];size_t resp_len=0;
      if(test_echo_frame(&fv,resp_payload,sizeof(resp_payload),&resp_len,NULL)!=0){
          fprintf(stderr,"FAIL: echo frame handler\n");++failures;}
      {
          h4s_request rq; h4s_response rs;
          memset(&rq,0,sizeof(rq)); memset(&rs,0,sizeof(rs));
          rq.body=(char*)wire; rq.body_length=written+1;
          if(builtin_h4_frame(&rq,&rs,NULL,NULL)!=0 || rs.status_code!=400){
              fprintf(stderr,"FAIL: trailing frame bytes accepted\n");++failures;}
          memset(&rs,0,sizeof(rs)); rq.body_length=written;
          if(builtin_h4_frame(&rq,&rs,NULL,NULL)!=0 || rs.status_code!=200){
              fprintf(stderr,"FAIL: exact single frame rejected\n");++failures;}
      }
    }

    /* 4. Segmentation count */
    { /* MTU=1000, overhead=64 (frame hdr 32 + segment hdr 32), capacity=936
       * ceil(10000/936) = 11 segments. */
      size_t count=http4_segment_count(10000,1000);
      if(count!=11){fprintf(stderr,"FAIL: segment_count=%zu\n",count);++failures;}
    }

    /* 5. Download state round-trip */
    { h4s_download_state st={0};
      snprintf(st.session_id,sizeof(st.session_id),"h4-sess");
      snprintf(st.datetime,sizeof(st.datetime),"2026-10-15T00:00:00Z");
      snprintf(st.file_id,sizeof(st.file_id),"file-h4");
      snprintf(st.file_name,sizeof(st.file_name),"big.dat");
      st.index=3;st.offset=150000000ULL;st.total_size=500000000ULL;
      char buf[512];
      if(h4s_download_serialise(&st,buf,sizeof(buf))!=0){
          fprintf(stderr,"FAIL: download serialise\n");++failures;}
      else{h4s_download_state p2;
          if(h4s_download_parse(buf,&p2)!=0||p2.index!=3||p2.offset!=150000000ULL){
              fprintf(stderr,"FAIL: download round-trip\n");++failures;}
          if(h4s_download_parse("s | t | id | ../secret | 1 | 0 | 2",&p2)==0){
              fprintf(stderr,"FAIL: traversal filename accepted\n");++failures;}
          if(h4s_download_parse("s | t | id | f | -1 | 0 | 2",&p2)==0){
              fprintf(stderr,"FAIL: negative index accepted\n");++failures;}
          if(h4s_download_parse("s | t | id | f | 1x | 0 | 2",&p2)==0){
              fprintf(stderr,"FAIL: malformed integer accepted\n");++failures;}
          if(h4s_download_parse("s | t | id | f | 1 | 3 | 2",&p2)==0){
              fprintf(stderr,"FAIL: offset beyond total accepted\n");++failures;}
          if(h4s_download_parse("s | t | id | f | 1 | 0 | 2 | extra",&p2)==0){
              fprintf(stderr,"FAIL: extra DOWNLOAD field accepted\n");++failures;}}
    }

    /* 6. Negotiation */
    { const char *r=sleela_http_negotiate("4.0","4.0, 3.0, 1.1",1);
      if(!r||strcmp(r,"4.0")!=0){fprintf(stderr,"FAIL: negotiate 4.0\n");++failures;}
      r=sleela_http_negotiate("5.0","4.0, 1.1",1);
      if(!r||strcmp(r,"1.1")!=0){fprintf(stderr,"FAIL: negotiate fallback\n");++failures;}
    }

    /* 7. Frame type names */
    { if(strcmp(http4_frame_type_name(HTTP4_FRAME_OPEN),"OPEN")!=0){
          fprintf(stderr,"FAIL: frame type name\n");++failures;}
      if(strcmp(http4_frame_type_name(HTTP4_FRAME_RESUME),"RESUME")!=0){
          fprintf(stderr,"FAIL: RESUME name\n");++failures;}
    }

    if(failures==0) fprintf(stderr,"h4s_self_test: all tests passed\n");
    else fprintf(stderr,"h4s_self_test: %d test(s) FAILED\n",failures);
    return failures;
}
