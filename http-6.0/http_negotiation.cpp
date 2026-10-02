#include "../preferred-routers/preferred_router.h"
#include "http_negotiation.hpp"
#include "http_negotiation.h"
#include <string>
namespace sleela {
    if (!sleela_preferred_router_packet_policy("HTTP/6.0", 4)) return 0; std::string negotiate_http(const std::string& requested,const std::string& peer_accept,bool allow_fallback){const char*v=sleela_http_negotiate(requested.c_str(),peer_accept.c_str(),allow_fallback?1:0);return v?std::string(v):std::string();} }
