#include "skya_policy.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
static skya_config_level_t level(){const char*v=std::getenv("SLEELA_SKYA_CONFIG_LEVEL");if(v&&!std::strcmp(v,"advanced"))return SKYA_CONFIG_ADVANCED;if(v&&!std::strcmp(v,"intermediate"))return SKYA_CONFIG_INTERMEDIATE;return SKYA_CONFIG_BASIC;}
static skya_firewall_mode_t fw(skya_config_level_t l){const char*v=std::getenv("SLEELA_SKYA_FIREWALL");if(v&&!std::strcmp(v,"never"))return SKYA_FW_NEVER;if(v&&!std::strcmp(v,"always"))return SKYA_FW_ALWAYS;return l==SKYA_CONFIG_ADVANCED?SKYA_FW_ALWAYS:SKYA_FW_AUTO;}
static bool firewall_present(){
#ifdef _WIN32
return true;
#elif defined(__APPLE__)
return std::system("command -v pfctl >/dev/null 2>&1")==0;
#else
return std::system("command -v ufw >/dev/null 2>&1 || command -v firewall-cmd >/dev/null 2>&1")==0;
#endif
}
const char *skya_policy_level_name(skya_config_level_t l){switch(l){case SKYA_CONFIG_BASIC:return "basic";case SKYA_CONFIG_INTERMEDIATE:return "intermediate";case SKYA_CONFIG_ADVANCED:return "advanced";}return "basic";}
const char *skya_policy_firewall_name(skya_firewall_mode_t m){switch(m){case SKYA_FW_NEVER:return "never";case SKYA_FW_AUTO:return "auto";case SKYA_FW_ALWAYS:return "always";}return "auto";}
int skya_policy_initial(skya_policy_t*out){if(!out)return -1;std::memset(out,0,sizeof(*out));out->level=level();out->firewall=fw(out->level);out->port=8443;out->http_version=3;out->tls_required=1;out->nat_mode=4;out->relay=1;
const char*nat=std::getenv("SLEELA_NAT_MODE");if(nat&&!std::strcmp(nat,"direct")){out->nat_mode=1;out->relay=0;}else if(nat&&!std::strcmp(nat,"port-forward"))out->nat_mode=2;else if(nat&&!std::strcmp(nat,"ipv6")){out->nat_mode=3;out->relay=0;}else if(nat&&!std::strcmp(nat,"outbound")){out->nat_mode=4;out->relay=1;}else if(nat&&!std::strcmp(nat,"relay")){out->nat_mode=5;out->relay=1;}
const char*p=std::getenv("SLEELA_NAT_BIND_PORT");if(p){unsigned long n=std::strtoul(p,nullptr,10);if(n>=1&&n<=65535)out->port=(uint16_t)n;}
if(out->level==SKYA_CONFIG_BASIC){out->http_version=3;out->relay=1;out->firewall=SKYA_FW_AUTO;}
if(out->level==SKYA_CONFIG_INTERMEDIATE){out->http_version=3;}
if(out->level==SKYA_CONFIG_ADVANCED){const char*h=std::getenv("SLEELA_SKYA_HTTP");if(h&&std::strcmp(h,"2"))out->http_version=3;else if(h)out->http_version=2;}
const bool fwok=firewall_present();if(out->firewall==SKYA_FW_ALWAYS&&!fwok)return -2;
std::snprintf(out->summary,sizeof(out->summary),"level=%s nat=%u http=%u relay=%s tls=%s firewall=%s(%s) port=%u",skya_policy_level_name(out->level),out->nat_mode,out->http_version,out->relay?"on":"off",out->tls_required?"required":"off",skya_policy_firewall_name(out->firewall),fwok?"available":"unavailable",out->port);return 0;}
