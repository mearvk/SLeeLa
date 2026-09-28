#ifndef SLEELA_HTTP_NEGOTIATION_HPP
#define SLEELA_HTTP_NEGOTIATION_HPP
#include <string>
namespace sleela { std::string negotiate_http(const std::string& requested,const std::string& peer_accept,bool allow_fallback); }
#endif
