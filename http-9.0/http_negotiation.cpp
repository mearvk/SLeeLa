#include "../preferred-routers/preferred_router.h"
#include "../resolver/resolver.h"
#include "http_negotiation.hpp"
#include "http_negotiation.h"
#include <string>
namespace sleela { std::string negotiate_http(const std::string& requested,const std::string& peer_accept,bool allow_fallback){if (!sleela_resolver_event_policy("HTTP/9.0", 0)) return std::string();if (!sleela_preferred_router_packet_policy("HTTP/9.0", 4)) return std::string(); const char*v=sleela_http_negotiate(requested.c_str(),peer_accept.c_str(),allow_fallback?1:0);return v?std::string(v):std::string();} }
