#ifndef SLEELA_HTTP_NEGOTIATION_H
#define SLEELA_HTTP_NEGOTIATION_H
#ifdef __cplusplus
extern "C" {
#endif
const char *sleela_http_negotiate(const char *requested,const char *peer_accept,int allow_fallback);
#ifdef __cplusplus
}
#endif
#endif
