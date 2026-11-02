#include "sleela/decompiler/destination.hpp"

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace sleela::decompiler {
namespace {

std::uint16_t default_port(DestinationKind k) {
    switch (k) {
        case DestinationKind::FtpStore: return 21;
        case DestinationKind::HttpPost:
        case DestinationKind::HttpPut:  return 80;
        case DestinationKind::Sftp:     return 22;
        default:                        return 0;
    }
}

int ai_family(IpPreference p) {
    switch (p) { case IpPreference::IPv4: return AF_INET; case IpPreference::IPv6: return AF_INET6; default: return AF_UNSPEC; }
}

// Resolve host:port (DNS, IPv4, or IPv6) and connect a TCP socket. On success returns the
// fd and fills `resolved` with the printable IP actually connected to. -1 on failure.
int tcp_connect(const std::string& host, std::uint16_t port, IpPreference pref,
                int timeout_s, std::string& resolved, std::string& error) {
    addrinfo hints{};
    hints.ai_family = ai_family(pref);
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    const std::string portstr = std::to_string(port);
    addrinfo* res = nullptr;
    const int rc = ::getaddrinfo(host.c_str(), portstr.c_str(), &hints, &res);
    if (rc != 0) { error = std::string("DNS/address resolution failed: ") + gai_strerror(rc); return -1; }

    int fd = -1;
    for (addrinfo* ai = res; ai; ai = ai->ai_next) {
        fd = ::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (fd < 0) continue;
        if (timeout_s > 0) {
            timeval tv{timeout_s, 0};
            ::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            ::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
        }
        if (::connect(fd, ai->ai_addr, ai->ai_addrlen) == 0) {
            char ip[INET6_ADDRSTRLEN] = {0};
            if (ai->ai_family == AF_INET)
                ::inet_ntop(AF_INET, &reinterpret_cast<sockaddr_in*>(ai->ai_addr)->sin_addr, ip, sizeof(ip));
            else if (ai->ai_family == AF_INET6)
                ::inet_ntop(AF_INET6, &reinterpret_cast<sockaddr_in6*>(ai->ai_addr)->sin6_addr, ip, sizeof(ip));
            resolved = ip;
            break;
        }
        ::close(fd);
        fd = -1;
    }
    ::freeaddrinfo(res);
    if (fd < 0) error = "could not connect to any resolved address for " + host;
    return fd;
}

bool send_all(int fd, const char* data, std::size_t len, std::string& error) {
    std::size_t off = 0;
    while (off < len) {
        const ssize_t n = ::send(fd, data + off, len - off, 0);
        if (n <= 0) { error = std::string("send failed: ") + std::strerror(errno); return false; }
        off += static_cast<std::size_t>(n);
    }
    return true;
}

bool send_all(int fd, const std::string& s, std::string& error) { return send_all(fd, s.data(), s.size(), error); }

std::string recv_line(int fd) {
    std::string line; char c;
    while (::recv(fd, &c, 1, 0) == 1) { line.push_back(c); if (c == '\n') break; }
    return line;
}

// --- FTP (RFC 959) STOR ------------------------------------------------------------------
int ftp_read_code(int fd, std::string& last) {
    // Read one (possibly multi-line) reply; return the 3-digit code.
    std::string line = recv_line(fd);
    last = line;
    if (line.size() < 4) return 0;
    const int code = std::atoi(line.substr(0, 3).c_str());
    if (line[3] == '-') {           // multi-line: read until "NNN " terminator
        const std::string term = line.substr(0, 3) + " ";
        for (;;) {
            line = recv_line(fd);
            if (line.empty()) break;
            last = line;
            if (line.rfind(term, 0) == 0) break;
        }
    }
    return code;
}

DeliveryResult ftp_store(const Destination& d, const std::string& data) {
    DeliveryResult r;
    std::string err, last;
    const std::uint16_t port = d.port ? d.port : default_port(d.kind);
    const int ctrl = tcp_connect(d.host, port, d.ip_preference, d.timeout_seconds, r.resolved_address, err);
    if (ctrl < 0) { r.message = err; return r; }

    auto fail = [&](const std::string& m) { r.status_code = r.status_code ? r.status_code : 0; r.message = m; ::close(ctrl); return r; };
    if (ftp_read_code(ctrl, last) / 100 != 2) return fail("FTP greeting rejected: " + last);

    const std::string user = d.username.empty() ? "anonymous" : d.username;
    if (!send_all(ctrl, "USER " + user + "\r\n", err)) return fail(err);
    int code = ftp_read_code(ctrl, last);
    if (code == 331) { // password required
        if (!send_all(ctrl, "PASS " + d.password + "\r\n", err)) return fail(err);
        code = ftp_read_code(ctrl, last);
    }
    if (code / 100 != 2) return fail("FTP login failed: " + last);

    if (!send_all(ctrl, "TYPE I\r\n", err)) return fail(err);
    ftp_read_code(ctrl, last);

    if (!d.passive_ftp) return fail("active FTP not supported; use passive (PASV)");
    if (!send_all(ctrl, "PASV\r\n", err)) return fail(err);
    code = ftp_read_code(ctrl, last);
    if (code != 227) return fail("FTP PASV failed: " + last);
    // Parse "227 Entering Passive Mode (h1,h2,h3,h4,p1,p2)".
    const auto lp = last.find('('), rp = last.find(')');
    if (lp == std::string::npos || rp == std::string::npos || rp <= lp) return fail("malformed PASV reply: " + last);
    int v[6] = {0,0,0,0,0,0};
    if (std::sscanf(last.c_str() + lp + 1, "%d,%d,%d,%d,%d,%d", &v[0],&v[1],&v[2],&v[3],&v[4],&v[5]) != 6)
        return fail("cannot parse PASV address: " + last);
    const std::string dhost = std::to_string(v[0]) + "." + std::to_string(v[1]) + "." + std::to_string(v[2]) + "." + std::to_string(v[3]);
    const std::uint16_t dport = static_cast<std::uint16_t>((v[4] << 8) | v[5]);
    std::string draddr;
    const int dsock = tcp_connect(dhost, dport, IpPreference::IPv4, d.timeout_seconds, draddr, err);
    if (dsock < 0) return fail("FTP data connection failed: " + err);

    if (!send_all(ctrl, "STOR " + d.path + "\r\n", err)) { ::close(dsock); return fail(err); }
    code = ftp_read_code(ctrl, last);
    if (code != 150 && code != 125) { ::close(dsock); return fail("FTP STOR rejected: " + last); }

    if (!send_all(dsock, data, err)) { ::close(dsock); return fail("FTP data send failed: " + err); }
    ::close(dsock);
    r.bytes_sent = data.size();

    code = ftp_read_code(ctrl, last);          // transfer-complete reply
    r.status_code = code;
    send_all(ctrl, "QUIT\r\n", err);
    ::close(ctrl);
    r.ok = (code / 100 == 2);
    r.message = r.ok ? "FTP STOR complete: " + last : "FTP STOR did not complete: " + last;
    return r;
}

// --- HTTP/1.1 POST / PUT -----------------------------------------------------------------
DeliveryResult http_send(const Destination& d, const std::string& data, const char* method) {
    DeliveryResult r;
    std::string err;
    const std::uint16_t port = d.port ? d.port : default_port(d.kind);
    const int fd = tcp_connect(d.host, port, d.ip_preference, d.timeout_seconds, r.resolved_address, err);
    if (fd < 0) { r.message = err; return r; }

    const std::string target = d.path.empty() ? "/" : d.path;
    const std::string host_header = port == 80 ? d.host : d.host + ":" + std::to_string(port);
    std::ostringstream req;
    req << method << " " << target << " HTTP/1.1\r\n"
        << "Host: " << host_header << "\r\n"
        << "User-Agent: Slecompiler/1.0\r\n"
        << "Content-Type: " << d.content_type << "\r\n"
        << "Content-Length: " << data.size() << "\r\n"
        << "Connection: close\r\n";
    for (const auto& h : d.headers) req << h << "\r\n";
    req << "\r\n";

    if (!send_all(fd, req.str(), err) || !send_all(fd, data, err)) { r.message = err; ::close(fd); return r; }
    r.bytes_sent = data.size();

    const std::string status = recv_line(fd);   // "HTTP/1.1 201 Created"
    const auto sp = status.find(' ');
    if (sp != std::string::npos) r.status_code = std::atoi(status.c_str() + sp + 1);
    // Drain remaining headers/body briefly for a clean close.
    char buf[1024];
    while (::recv(fd, buf, sizeof(buf), 0) > 0) { /* discard */ }
    ::close(fd);

    r.ok = (r.status_code >= 200 && r.status_code < 300);
    r.message = (r.ok ? std::string("HTTP ") + method + " succeeded: " : std::string("HTTP ") + method + " returned: ") + status;
    return r;
}

// --- SFTP (governed external helper) -----------------------------------------------------
// We do not embed an SSH stack. If a governed `sftp` client is present, delegate to it via a
// batch command over a pipe. Password auth is not scripted here (that would require leaking a
// credential to a child); key-based/agent auth is expected. Unavailable -> honest failure.
bool have_helper(const char* name) {
    const std::string cmd = std::string("command -v ") + name + " >/dev/null 2>&1";
    return std::system(cmd.c_str()) == 0;
}

DeliveryResult sftp_put(const Destination& d, const std::string& data) {
    DeliveryResult r;
    if (!have_helper("sftp")) {
        r.message = "SFTP requires a governed 'sftp' client on PATH; none found (no SSH stack is embedded)";
        return r;
    }
    // Stage the payload to a temp file, then run a batch put. Host key checking is left to the
    // system ssh configuration; we do not weaken it here.
    char tmpl[] = "/tmp/slecompiler-sftp-XXXXXX";
    const int tfd = ::mkstemp(tmpl);
    if (tfd < 0) { r.message = "could not stage SFTP payload"; return r; }
    { std::ofstream t(tmpl, std::ios::binary); t.write(data.data(), static_cast<std::streamsize>(data.size())); }
    ::close(tfd);

    const std::uint16_t port = d.port ? d.port : default_port(d.kind);
    const std::string user = d.username.empty() ? std::string() : d.username + "@";
    std::ostringstream cmd;
    cmd << "sftp -b - -P " << port << " " << user << d.host
        << " <<'SLEOF'\nput " << tmpl << " " << d.path << "\nquit\nSLEOF\n";
    const int rc = std::system(cmd.str().c_str());
    ::unlink(tmpl);

    r.bytes_sent = data.size();
    r.ok = (rc == 0);
    r.resolved_address = d.host;   // resolution handled inside ssh
    r.message = r.ok ? "SFTP put complete" : "SFTP put failed (sftp exit code non-zero)";
    return r;
}

// --- local File / Directory --------------------------------------------------------------
DeliveryResult write_local(const std::string& path, const std::string& data) {
    DeliveryResult r;
    std::ofstream out(path, std::ios::binary);
    if (!out) { r.message = "cannot open output path: " + path; return r; }
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    if (!out) { r.message = "cannot write output path: " + path; return r; }
    r.ok = true;
    r.bytes_sent = data.size();
    r.message = "wrote " + std::to_string(data.size()) + " bytes to " + path;
    return r;
}

} // namespace

const char* destination_kind_name(DestinationKind k) {
    switch (k) {
        case DestinationKind::File: return "file";
        case DestinationKind::Directory: return "directory";
        case DestinationKind::FtpStore: return "ftp";
        case DestinationKind::HttpPost: return "http-post";
        case DestinationKind::HttpPut: return "http-put";
        case DestinationKind::Sftp: return "sftp";
    }
    return "unknown";
}

bool parse_destination(const std::string& uri, Destination& out, std::string& error) {
    out = Destination{};
    auto starts = [&](const char* p) { return uri.rfind(p, 0) == 0; };

    // Local forms first.
    if (starts("file:")) { out.kind = DestinationKind::File; out.path = uri.substr(5); return true; }
    if (uri.find("://") == std::string::npos) { out.kind = DestinationKind::File; out.path = uri; return true; }

    std::string scheme = uri.substr(0, uri.find("://"));
    const std::string rest = uri.substr(scheme.size() + 3);
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), [](unsigned char c){ return std::tolower(c); });

    if (scheme == "ftp") out.kind = DestinationKind::FtpStore;
    else if (scheme == "sftp") out.kind = DestinationKind::Sftp;
    else if (scheme == "http" || scheme == "http+post") out.kind = DestinationKind::HttpPost;
    else if (scheme == "http+put") out.kind = DestinationKind::HttpPut;
    else { error = "unsupported destination scheme: " + scheme; return false; }

    // Split authority and path at the first '/'.
    const auto slash = rest.find('/');
    std::string authority = (slash == std::string::npos) ? rest : rest.substr(0, slash);
    out.path = (slash == std::string::npos) ? "/" : rest.substr(slash);

    // Optional user[:pass]@ credentials.
    const auto at = authority.rfind('@');
    if (at != std::string::npos) {
        const std::string cred = authority.substr(0, at);
        authority = authority.substr(at + 1);
        const auto colon = cred.find(':');
        if (colon == std::string::npos) out.username = cred;
        else { out.username = cred.substr(0, colon); out.password = cred.substr(colon + 1); }
    }

    // host[:port], with IPv6 literal in [brackets].
    if (!authority.empty() && authority.front() == '[') {   // [2001:db8::1]:port
        const auto rb = authority.find(']');
        if (rb == std::string::npos) { error = "malformed IPv6 authority: " + authority; return false; }
        out.host = authority.substr(1, rb - 1);
        out.ip_preference = IpPreference::IPv6;
        if (rb + 1 < authority.size() && authority[rb + 1] == ':')
            out.port = static_cast<std::uint16_t>(std::atoi(authority.c_str() + rb + 2));
    } else {
        const auto colon = authority.find(':');
        if (colon == std::string::npos) out.host = authority;
        else { out.host = authority.substr(0, colon); out.port = static_cast<std::uint16_t>(std::atoi(authority.c_str() + colon + 1)); }
    }
    if (out.host.empty()) { error = "destination host is empty"; return false; }
    return true;
}

DeliveryResult deliver(const Destination& d, const std::string& data) {
    switch (d.kind) {
        case DestinationKind::File:      return write_local(d.path, data);
        case DestinationKind::Directory: {
            std::string p = d.path;
            if (!p.empty() && p.back() != '/') p += '/';
            return write_local(p + (d.filename.empty() ? "output" : d.filename), data);
        }
        case DestinationKind::FtpStore:  return ftp_store(d, data);
        case DestinationKind::HttpPost:  return http_send(d, data, "POST");
        case DestinationKind::HttpPut:   return http_send(d, data, "PUT");
        case DestinationKind::Sftp:      return sftp_put(d, data);
    }
    DeliveryResult r; r.message = "unknown destination kind"; return r;
}

} // namespace sleela::decompiler
