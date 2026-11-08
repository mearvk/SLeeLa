// http_client.h - TLS-verified HTTP(S) client for search/review/download.
//
// A small, safe HTTPS client built on OpenSSL's BIO/SSL stack. TLS
// certificate chain verification and hostname matching are ON by default
// (TLS 1.2 minimum). Supports GET and HEAD, bounded redirects, per-request
// timeouts, and a hard response-size cap to bound memory.
//
// It is deliberately not a full HTTP/1.1 engine: no chunked upload, no
// keep-alive pooling. It is scoped to safely *obtaining* and *reviewing*
// remote resources.
#ifndef ADVANTAGE_HTTP_CLIENT_H
#define ADVANTAGE_HTTP_CLIENT_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace advantage {

struct HttpOptions {
    bool        verifyPeer = true;     // verify server certificate chain
    bool        verifyHost = true;     // match certificate to hostname
    int         maxRedirects = 5;      // 0 disables redirect following
    long        timeoutMs = 30000;     // connect + per-read inactivity budget
    size_t      maxBodyBytes = 256u * 1024u * 1024u;  // hard cap (256 MiB)
    std::string caFile;                // optional explicit CA bundle path
    std::string caPath;                // optional CA directory
    std::string userAgent = "advantage-transport/1.0";
};

struct HttpResponse {
    bool                                ok = false;     // request completed
    int                                 status = 0;     // HTTP status code
    std::string                         statusLine;
    std::map<std::string, std::string>  headers;        // lowercased keys
    std::vector<uint8_t>                body;
    std::string                         finalUrl;       // after redirects
    std::string                         error;          // populated when !ok
};

class HttpClient {
public:
    explicit HttpClient(HttpOptions opts = {}) : opts_(std::move(opts)) {}

    // Full GET with body download.
    HttpResponse get(const std::string &url);

    // HEAD only: metadata for "review" without downloading the body.
    HttpResponse head(const std::string &url);

    const HttpOptions &options() const { return opts_; }

private:
    HttpResponse request(const std::string &method, const std::string &url,
                         int redirectsLeft);
    HttpOptions opts_;
};

}  // namespace advantage

#endif  // ADVANTAGE_HTTP_CLIENT_H
