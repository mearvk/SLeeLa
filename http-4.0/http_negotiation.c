#include "http_negotiation.h"
#include <string.h>
static int has_version(const char *l,const char *v){size_t n;const char*p;if(!l||!v)return 0;n=strlen(v);for(p=l;*p;++p)if(strncmp(p,v,n)==0&&(p==l||p[-1]==','||p[-1]==' '||p[-1]=='\t')&&(p[n]=='\0'||p[n]==','||p[n]==' '||p[n]=='\t'))return 1;return 0;}
const char *sleela_http_negotiate(const char *r,const char *a,int f){if(r&&a&&has_version(a,r))return r;if(f&&a){if(has_version(a,"1.1"))return "1.1";if(has_version(a,"1.0"))return "1.0";}return 0;}
