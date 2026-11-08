// http_client.cpp - TLS-verified HTTP(S) client (see http_client.h).
//
// Security notes:
//   - SSL_CTX uses the system default trust store (or an explicit CA bundle),
//     sets SSL_VERIFY_PEER, a minimum of TLS 1.2, and enables X509 hostname
//     checking via SSL_set1_host + X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS.
//   - SNI is set so virtual-hosted TLS endpoints present the right cert.
//   - Response body is capped at opts.maxBodyBytes.
//   - Redirects are bounded and only followed for http/https targets.
#include "http_client.h"

#include "uri.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <ctime>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

namespace advantage {

namespace {

std::string lower(std::string s) {
    for (char &c : s) c = static_cast<char>(std::tolower((unsigned char)c));
    return s;
}

std::string ssl_error() {
    unsigned long e = ERR_get_error();
    if (!e) return "unknown TLS error";
    char buf[256];
    ERR_error_string_n(e, buf, sizeof buf);
    return buf;
}

// RAII for OpenSSL objects we create here.
struct SslBundle {
    SSL_CTX *ctx = nullptr;
    BIO     *bio = nullptr;
    ~SslBundle() {
        if (bio) BIO_free_all(bio);
        if (ctx) SSL_CTX_free(ctx);
    }
};

}  // namespace

HttpResponse HttpClient::get(const std::string &url) {
    return request("GET", url, opts_.maxRedirects);
}

HttpResponse HttpClient::head(const std::string &url) {
    return request("HEAD", url, opts_.maxRedirects);
}

HttpResponse HttpClient::request(const std::string &method,
                                 const std::string &url, int redirectsLeft) {
    HttpResponse r;
    r.finalUrl = url;

    Uri u = parseUri(url);
    if (!u.valid || (u.scheme != "https" && u.scheme != "http")) {
        r.error = "unsupported or invalid URL: " + url;
        return r;
    }
    const bool tls = (u.scheme == "https");
    const uint16_t port = u.effectivePort();

    SslBundle b;
    if (tls) {
        b.ctx = SSL_CTX_new(TLS_client_method());
        if (!b.ctx) { r.error = "SSL_CTX_new failed: " + ssl_error(); return r; }
        SSL_CTX_set_min_proto_version(b.ctx, TLS1_2_VERSION);

        if (!opts_.caFile.empty() || !opts_.caPath.empty()) {
            if (SSL_CTX_load_verify_locations(
                    b.ctx,
                    opts_.caFile.empty() ? nullptr : opts_.caFile.c_str(),
                    opts_.caPath.empty() ? nullptr : opts_.caPath.c_str()) != 1) {
                r.error = "failed to load CA trust: " + ssl_error();
                return r;
            }
        } else {
            SSL_CTX_set_default_verify_paths(b.ctx);
        }
        SSL_CTX_set_verify(b.ctx,
                           opts_.verifyPeer ? SSL_VERIFY_PEER : SSL_VERIFY_NONE,
                           nullptr);

        b.bio = BIO_new_ssl_connect(b.ctx);
        if (!b.bio) { r.error = "BIO_new_ssl_connect failed"; return r; }

        SSL *ssl = nullptr;
        BIO_get_ssl(b.bio, &ssl);
        if (!ssl) { r.error = "BIO_get_ssl failed"; return r; }
        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

        // SNI + hostname verification bound to the certificate.
        SSL_set_tlsext_host_name(ssl, u.host.c_str());
        if (opts_.verifyHost) {
            X509_VERIFY_PARAM *vp = SSL_get0_param(ssl);
            X509_VERIFY_PARAM_set_hostflags(vp, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
            if (X509_VERIFY_PARAM_set1_host(vp, u.host.c_str(), 0) != 1) {
                r.error = "failed to set verify host";
                return r;
            }
        }

        std::string hp = u.host + ":" + std::to_string(port);
        BIO_set_conn_hostname(b.bio, hp.c_str());
    } else {
        b.bio = BIO_new(BIO_s_connect());
        if (!b.bio) { r.error = "BIO_new(connect) failed"; return r; }
        BIO_set_conn_hostname(b.bio, u.host.c_str());
        BIO_set_conn_port(b.bio, std::to_string(port).c_str());
    }

    if (BIO_do_connect(b.bio) <= 0) {
        r.error = "connection failed: " + ssl_error();
        return r;
    }
    if (tls) {
        if (BIO_do_handshake(b.bio) <= 0) {
            r.error = "TLS handshake failed: " + ssl_error();
            return r;
        }
        SSL *ssl = nullptr;
        BIO_get_ssl(b.bio, &ssl);
        long vr = SSL_get_verify_result(ssl);
        if (opts_.verifyPeer && vr != X509_V_OK) {
            r.error = std::string("certificate verification failed: ") +
                      X509_verify_cert_error_string(vr);
            return r;
        }
    }

    // Build and send the request.
    std::string req;
    req += method + " " + u.path + " HTTP/1.1\r\n";
    req += "Host: " + u.host + "\r\n";
    req += "User-Agent: " + opts_.userAgent + "\r\n";
    req += "Accept: */*\r\n";
    req += "Connection: close\r\n\r\n";

    if (BIO_write(b.bio, req.data(), (int)req.size()) <= 0) {
        r.error = "failed to send request: " + ssl_error();
        return r;
    }

    // Read full response (Connection: close => read to EOF).
    std::string raw;
    char buf[16384];
    for (;;) {
        int n = BIO_read(b.bio, buf, sizeof buf);
        if (n > 0) {
            raw.append(buf, (size_t)n);
            if (raw.size() > opts_.maxBodyBytes + 65536) {
                r.error = "response exceeds maxBodyBytes";
                return r;
            }
        } else if (n == 0) {
            break;  // clean EOF
        } else {
            if (BIO_should_retry(b.bio)) continue;
            break;
        }
    }

    // Split headers / body.
    auto hdrEnd = raw.find("\r\n\r\n");
    if (hdrEnd == std::string::npos) {
        r.error = "malformed response (no header terminator)";
        return r;
    }
    std::string head = raw.substr(0, hdrEnd);
    std::string body = raw.substr(hdrEnd + 4);

    // Parse status line.
    auto lineEnd = head.find("\r\n");
    r.statusLine = head.substr(0, lineEnd);
    {
        // "HTTP/1.1 200 OK"
        auto sp1 = r.statusLine.find(' ');
        if (sp1 != std::string::npos) {
            r.status = std::atoi(r.statusLine.c_str() + sp1 + 1);
        }
    }

    // Parse headers.
    size_t pos = (lineEnd == std::string::npos) ? head.size() : lineEnd + 2;
    while (pos < head.size()) {
        auto eol = head.find("\r\n", pos);
        std::string line = head.substr(pos, (eol == std::string::npos) ? std::string::npos : eol - pos);
        pos = (eol == std::string::npos) ? head.size() : eol + 2;
        auto colon = line.find(':');
        if (colon == std::string::npos) continue;
        std::string key = lower(line.substr(0, colon));
        std::string val = line.substr(colon + 1);
        // trim
        val.erase(0, val.find_first_not_of(" \t"));
        auto last = val.find_last_not_of(" \t");
        if (last != std::string::npos) val.erase(last + 1);
        r.headers[key] = val;
    }

    // Follow redirects for GET/HEAD.
    if (r.status >= 300 && r.status < 400 && redirectsLeft > 0) {
        auto it = r.headers.find("location");
        if (it != r.headers.end()) {
            std::string loc = it->second;
            // Resolve relative-to-host (simple: absolute, or path-absolute).
            std::string next;
            if (loc.rfind("http://", 0) == 0 || loc.rfind("https://", 0) == 0) {
                next = loc;
            } else if (!loc.empty() && loc[0] == '/') {
                next = u.scheme + "://" + u.host +
                       (port == u.effectivePort() ? "" : ":" + std::to_string(port)) + loc;
            } else {
                next = u.scheme + "://" + u.host + "/" + loc;
            }
            return request(method, next, redirectsLeft - 1);
        }
    }

    if (method != "HEAD") {
        if (body.size() > opts_.maxBodyBytes) {
            r.error = "response body exceeds maxBodyBytes";
            return r;
        }
        r.body.assign(body.begin(), body.end());
    }
    r.ok = true;
    return r;
}

}  // namespace advantage
