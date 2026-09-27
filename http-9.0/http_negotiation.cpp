#include "http_negotiation.hpp"
#include "http_negotiation.h"
#include <string>
namespace sleela { std::string negotiate_http(const std::string&r,const std::string&a,bool f){const char*v=sleela_http_negotiate(r.c_str(),a.c_str(),f?1:0);return v?std::string(v):std::string();} }
