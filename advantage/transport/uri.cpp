// uri.cpp - Minimal URI parser (see uri.h).
#include "uri.h"

#include <cctype>
#include <cstdlib>

namespace advantage {

uint16_t Uri::effectivePort() const {
    if (port != 0) return port;
    if (scheme == "https") return 443;
    if (scheme == "http")  return 80;
    if (scheme == "sftp" || scheme == "ssh") return 22;
    return 0;  // file:// or unknown
}

Uri parseUri(const std::string &text) {
    Uri u;
    auto sep = text.find("://");
    if (sep == std::string::npos) return u;

    u.scheme = text.substr(0, sep);
    for (char &c : u.scheme) c = static_cast<char>(std::tolower((unsigned char)c));
    if (u.scheme.empty()) return u;

    std::string rest = text.substr(sep + 3);

    if (u.scheme == "file") {
        // file:///abs/path  -> host empty, path = /abs/path
        u.path = rest.empty() ? "/" : (rest[0] == '/' ? rest : "/" + rest);
        u.valid = true;
        return u;
    }

    // Split authority from path at the first '/'.
    std::string authority;
    auto slash = rest.find('/');
    if (slash == std::string::npos) {
        authority = rest;
        u.path = "/";
    } else {
        authority = rest.substr(0, slash);
        u.path = rest.substr(slash);
    }

    // userinfo@host:port
    auto at = authority.find('@');
    std::string hostport = authority;
    if (at != std::string::npos) {
        std::string userinfo = authority.substr(0, at);
        hostport = authority.substr(at + 1);
        auto colon = userinfo.find(':');
        if (colon == std::string::npos) {
            u.user = userinfo;
        } else {
            u.user = userinfo.substr(0, colon);
            u.password = userinfo.substr(colon + 1);
        }
    }

    auto colon = hostport.rfind(':');
    if (colon != std::string::npos) {
        u.host = hostport.substr(0, colon);
        std::string ps = hostport.substr(colon + 1);
        bool digits = !ps.empty();
        for (char c : ps) if (!std::isdigit((unsigned char)c)) digits = false;
        if (digits) {
            long p = std::strtol(ps.c_str(), nullptr, 10);
            if (p > 0 && p <= 65535) u.port = static_cast<uint16_t>(p);
            else return u;  // invalid port
        } else {
            return u;  // malformed
        }
    } else {
        u.host = hostport;
    }

    if (u.host.empty()) return u;
    u.valid = true;
    return u;
}

}  // namespace advantage
