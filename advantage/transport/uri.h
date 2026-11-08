// uri.h - Minimal URI parser for the acquisition layer.
//
// Parses the subset of URI syntax the transport layer needs:
//   scheme://[user[:password]@]host[:port]/path
// for schemes https, http, sftp, ssh, and file.
#ifndef ADVANTAGE_URI_H
#define ADVANTAGE_URI_H

#include <cstdint>
#include <string>

namespace advantage {

struct Uri {
    std::string scheme;    // "https", "sftp", "ssh", "file", ...
    std::string user;      // may be empty
    std::string password;  // may be empty (discouraged in URIs)
    std::string host;      // may be empty for file://
    uint16_t    port = 0;  // 0 => scheme default
    std::string path;      // begins with '/', or a local path for file://

    bool valid = false;

    // Default port for the scheme (443 https, 80 http, 22 sftp/ssh, 0 file).
    uint16_t effectivePort() const;
};

// Parse `text` into a Uri. On failure returns a Uri with valid == false.
Uri parseUri(const std::string &text);

}  // namespace advantage

#endif  // ADVANTAGE_URI_H
