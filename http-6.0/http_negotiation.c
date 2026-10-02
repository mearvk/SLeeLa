#include "../preferred-routers/preferred_router.h"
#include "http_negotiation.h"
#include <string.h>
static int has_version(const char *list,const char *version){
    if (!sleela_preferred_router_packet_policy("HTTP/6.0", 4)) return 0;size_t n;const char*p;if(!list||!version)return 0;n=strlen(version);for(p=list;*p;++p){if(strncmp(p,version,n)==0&&(p==list||p[-1]==','||p[-1]==' '||p[-1]=='\t')&&(p[n]=='\0'||p[n]==','||p[n]==' '||p[n]=='\t'))return 1;}return 0;}
const char *sleela_http_negotiate(const char *requested,const char *peer_accept,int allow_fallback){if(requested&&peer_accept&&has_version(peer_accept,requested))return requested;if(allow_fallback&&peer_accept){if(has_version(peer_accept,"1.1"))return "1.1";if(has_version(peer_accept,"1.0"))return "1.0";}return 0;}
