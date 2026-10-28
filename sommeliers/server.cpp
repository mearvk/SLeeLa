#include "feedback.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <winsock2.h>
#  include <ws2tcpip.h>
using socket_length = int;
using socket_handle = SOCKET;
static const socket_handle invalid_socket = INVALID_SOCKET;
static void close_socket(socket_handle s) { closesocket(s); }
#else
#  include <arpa/inet.h>
#  include <netinet/in.h>
#  include <sys/socket.h>
#  include <unistd.h>
using socket_length = socklen_t;
using socket_handle = int;
static const socket_handle invalid_socket = -1;
static void close_socket(socket_handle s) { close(s); }
#endif

namespace {
constexpr unsigned short kPort = 4081;
constexpr std::size_t kMaxRequest = 8192;

bool send_all(socket_handle fd, const char *data, std::size_t size) {
    while (size > 0) {
#ifdef _WIN32
        int sent = send(fd, data, static_cast<int>(size), 0);
#else
        ssize_t sent = send(fd, data, size, 0);
#endif
        if (sent <= 0) return false;
        data += sent;
        size -= static_cast<std::size_t>(sent);
    }
    return true;
}

void reply(socket_handle fd, int status, const char *reason, const std::string &body) {
    char header[256];
    const int n = std::snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\nContent-Type: text/plain; charset=utf-8\r\n"
        "Content-Length: %zu\r\nConnection: close\r\n"
        "Cache-Control: no-store\r\nX-Content-Type-Options: nosniff\r\n\r\n",
        status, reason, body.size());
    if (n > 0 && static_cast<std::size_t>(n) < sizeof(header)) {
        send_all(fd, header, static_cast<std::size_t>(n));
        send_all(fd, body.data(), body.size());
    }
}

void handle_client(socket_handle client) {
    std::string request;
    char buffer[1024];
    std::size_t header_end;
    while (request.size() < kMaxRequest) {
#ifdef _WIN32
        int n = recv(client, buffer, sizeof(buffer), 0);
#else
        ssize_t n = recv(client, buffer, sizeof(buffer), 0);
#endif
        if (n <= 0) { close_socket(client); return; }
        request.append(buffer, static_cast<std::size_t>(n));
        header_end = request.find("\r\n\r\n");
        if (header_end != std::string::npos) break;
    }
    if (request.size() >= kMaxRequest || request.find("\r\n\r\n") == std::string::npos) {
        reply(client, 413, "Payload Too Large", "error\n");
        close_socket(client); return;
    }

    const std::size_t first_end = request.find("\r\n");
    if (first_end == std::string::npos) {
        reply(client, 400, "Bad Request", "error\n"); close_socket(client); return;
    }
    const std::string line = request.substr(0, first_end);
    const bool is_post = line.rfind("POST /feedback ", 0) == 0;
    const bool is_health = line.rfind("GET /health ", 0) == 0;
    if (is_health) {
        reply(client, 200, "OK", "ok\n");
        close_socket(client); return;
    }
    if (!is_post) {
        reply(client, 404, "Not Found", "use POST /feedback\n");
        close_socket(client); return;
    }

    std::size_t content_length = 0;
    std::size_t line_start = first_end + 2;
    while (line_start < header_end) {
        const std::size_t line_end = request.find("\r\n", line_start);
        if (line_end == std::string::npos || line_end > header_end) break;
        const std::string h = request.substr(line_start, line_end - line_start);
        if (h.size() >= 15 && h.compare(0, 15, "Content-Length:") == 0) {
            const char *p = h.c_str() + 15;
            while (*p == ' ' || *p == '\t') ++p;
            char *end = nullptr;
            unsigned long parsed = std::strtoul(p, &end, 10);
            if (end == p || *end != '\0' || parsed > 256) {
                reply(client, 400, "Bad Request", "input must be 1-256 bytes\n");
                close_socket(client); return;
            }
            content_length = static_cast<std::size_t>(parsed);
        }
        line_start = line_end + 2;
    }
    if (content_length == 0 || content_length > 256) {
        reply(client, 400, "Bad Request", "input must be 1-256 bytes\n");
        close_socket(client); return;
    }
    const std::size_t body_start = header_end + 4;
    while (request.size() - body_start < content_length && request.size() < kMaxRequest) {
#ifdef _WIN32
        int n = recv(client, buffer, sizeof(buffer), 0);
#else
        ssize_t n = recv(client, buffer, sizeof(buffer), 0);
#endif
        if (n <= 0) break;
        request.append(buffer, static_cast<std::size_t>(n));
    }
    if (request.size() - body_start < content_length) {
        reply(client, 400, "Bad Request", "incomplete body\n");
        close_socket(client); return;
    }
    std::string input = request.substr(body_start, content_length);
    while (!input.empty() && (input.back() == '\n' || input.back() == '\r')) input.pop_back();
    char result = '\0';
    if (sommeliers_feedback_symbol(input.c_str(), &result) != 0) {
        reply(client, 400, "Bad Request", "invalid input\n");
    } else {
        reply(client, 200, "OK", std::string(1, result) + "\n");
    }
    close_socket(client);
}
}

int main(int argc, char **argv) {
    unsigned long requested_port = kPort;
    if (argc > 1) {
        char *end = nullptr;
        requested_port = std::strtoul(argv[1], &end, 10);
        if (end == argv[1] || *end != '\0' || requested_port == 0 || requested_port > 65535) {
            std::fprintf(stderr, "usage: sommeliers-server [port 1..65535]\n");
            return 2;
        }
    }
#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        std::fprintf(stderr, "Winsock startup failed\n"); return 1;
    }
#endif
    socket_handle listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == invalid_socket) {
        std::fprintf(stderr, "socket creation failed\n");
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    int reuse = 1;
#ifdef _WIN32
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&reuse), sizeof(reuse));
#else
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
#endif
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(static_cast<unsigned short>(requested_port));
    /* Deliberately loopback-only: this is a local feedback service, not an
       unauthenticated public-facing server. */
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(listener, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != 0 ||
        listen(listener, 16) != 0) {
        std::fprintf(stderr, "cannot bind/listen on 127.0.0.1:%lu\n", requested_port);
        close_socket(listener);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    std::printf("Sommeliers feedback listening on http://127.0.0.1:%lu\n", requested_port);
    std::printf("POST /feedback with body 0, 1, s, or a string such as 08s1; GET /health\n");
    for (;;) {
        socket_handle client = accept(listener, nullptr, nullptr);
        if (client == invalid_socket) continue;
        handle_client(client);
    }
}
