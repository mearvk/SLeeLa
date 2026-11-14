#include "sleela_network.hpp"

#include <algorithm>
#include <climits>
#include <cstring>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
using native_socket_t = SOCKET;
using socket_length_t = int;
static constexpr native_socket_t invalid_socket_value = INVALID_SOCKET;
static int last_socket_error() { return WSAGetLastError(); }
static bool is_timeout_error(int e) { return e == WSAEWOULDBLOCK || e == WSAETIMEDOUT; }
static void close_native_socket(native_socket_t fd) { (void)closesocket(fd); }
#else
#include <cerrno>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
using native_socket_t = int;
using socket_length_t = socklen_t;
static constexpr native_socket_t invalid_socket_value = -1;
static int last_socket_error() { return errno; }
static bool is_timeout_error(int e) { return e == EAGAIN || e == EWOULDBLOCK || e == ETIMEDOUT; }
static void close_native_socket(native_socket_t fd) { (void)::close(fd); }
#endif

namespace sleela::network {
namespace {
int native_family(AddressFamily family) {
    switch (family) {
        case AddressFamily::IPv4: return AF_INET;
        case AddressFamily::IPv6: return AF_INET6;
        case AddressFamily::Any: default: return AF_UNSPEC;
    }
}

Status resolve(const Endpoint& endpoint, int type, int fallback_family,
               sockaddr_storage* address, socket_length_t* length, int* family) {
    addrinfo hints{};
    hints.ai_socktype = type;
    hints.ai_family = endpoint.family == AddressFamily::Any
        ? fallback_family : native_family(endpoint.family);
    hints.ai_flags = endpoint.host.empty() || endpoint.host == "*" ? AI_PASSIVE : 0;
    addrinfo* result = nullptr;
    const std::string service = std::to_string(endpoint.port);
    const char* host = (hints.ai_flags & AI_PASSIVE) ? nullptr : endpoint.host.c_str();
    if (::getaddrinfo(host, service.c_str(), &hints, &result) != 0 || !result)
        return Status::Error;
    if (result->ai_addrlen > sizeof(*address)) {
        ::freeaddrinfo(result);
        return Status::Error;
    }
    std::memcpy(address, result->ai_addr, result->ai_addrlen);
    *length = static_cast<socket_length_t>(result->ai_addrlen);
    *family = result->ai_family;
    ::freeaddrinfo(result);
    return Status::Ok;
}

native_socket_t native_handle(std::intptr_t value) {
    return static_cast<native_socket_t>(value);
}
} // namespace

Socket::Socket() = default;
Socket::~Socket() { (void)close(); }

Status Socket::open(Transport transport, AddressFamily family) {
    if (fd_ != -1) return Status::Invalid;
#ifdef _WIN32
    WSADATA data{};
    if (::WSAStartup(MAKEWORD(2, 2), &data) != 0) return Status::Error;
    winsock_started_ = true;
#endif
    transport_ = transport;
    family_ = family;
    const int af = family == AddressFamily::IPv6 ? AF_INET6 : AF_INET;
    const int type = transport == Transport::TCP ? SOCK_STREAM : SOCK_DGRAM;
    const int protocol = transport == Transport::TCP ? IPPROTO_TCP : IPPROTO_UDP;
    const native_socket_t fd = ::socket(af, type, protocol);
    if (fd == invalid_socket_value) {
#ifdef _WIN32
        (void)::WSACleanup();
        winsock_started_ = false;
#endif
        return Status::Error;
    }
    fd_ = static_cast<std::intptr_t>(fd);
    return Status::Ok;
}

Status Socket::bind(const Endpoint& endpoint) {
    if (fd_ == -1) return Status::Closed;
    sockaddr_storage address{};
    socket_length_t length = 0;
    int resolved_family = AF_UNSPEC;
    const int type = transport_ == Transport::TCP ? SOCK_STREAM : SOCK_DGRAM;
    if (resolve(endpoint, type, family_ == AddressFamily::IPv6 ? AF_INET6 : AF_INET,
                &address, &length, &resolved_family) != Status::Ok)
        return Status::Error;
    const native_socket_t fd = native_handle(fd_);
    if (resolved_family != (family_ == AddressFamily::IPv6 ? AF_INET6 : AF_INET))
        return Status::Error;
    int reuse = 1;
#ifdef _WIN32
    (void)::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                       reinterpret_cast<const char*>(&reuse), sizeof(reuse));
#else
    (void)::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
#endif
    return ::bind(fd, reinterpret_cast<sockaddr*>(&address), length) == 0
        ? Status::Ok : Status::Error;
}

Status Socket::connect(const Endpoint& endpoint) {
    if (fd_ == -1) return Status::Closed;
    sockaddr_storage address{};
    socket_length_t length = 0;
    int resolved_family = AF_UNSPEC;
    const int type = transport_ == Transport::TCP ? SOCK_STREAM : SOCK_DGRAM;
    if (resolve(endpoint, type, family_ == AddressFamily::IPv6 ? AF_INET6 : AF_INET,
                &address, &length, &resolved_family) != Status::Ok)
        return Status::Error;
    const native_socket_t fd = native_handle(fd_);
    if (resolved_family != (family_ == AddressFamily::IPv6 ? AF_INET6 : AF_INET))
        return Status::Error;
    if (::connect(fd, reinterpret_cast<sockaddr*>(&address), length) != 0)
        return Status::Error;
    connected_ = true;
    return Status::Ok;
}

Status Socket::send(const std::vector<std::uint8_t>& data) {
    if (fd_ == -1) return Status::Closed;
    if (data.size() > static_cast<std::size_t>(INT_MAX)) return Status::Invalid;
    const native_socket_t fd = native_handle(fd_);
#ifdef _WIN32
    const int sent = ::send(fd, reinterpret_cast<const char*>(data.data()),
                            static_cast<int>(data.size()), 0);
    return sent >= 0 && static_cast<std::size_t>(sent) == data.size()
        ? Status::Ok : Status::Error;
#else
    const ssize_t sent = ::send(fd, data.data(), data.size(), 0);
    return sent >= 0 && static_cast<std::size_t>(sent) == data.size()
        ? Status::Ok : Status::Error;
#endif
}

ReceiveResult Socket::receive(std::size_t max_bytes) {
    ReceiveResult result;
    if (fd_ == -1) {
        result.status = Status::Closed;
        return result;
    }
    const std::size_t capacity = std::min(max_bytes, static_cast<std::size_t>(INT_MAX));
    if (capacity == 0) {
        result.status = Status::Invalid;
        return result;
    }
    std::vector<std::uint8_t> buffer(capacity);
    const native_socket_t fd = native_handle(fd_);
#ifdef _WIN32
    const int received = ::recv(fd, reinterpret_cast<char*>(buffer.data()),
                                static_cast<int>(capacity), 0);
    if (received == SOCKET_ERROR) {
        result.status = is_timeout_error(last_socket_error()) ? Status::Timeout : Status::Error;
        return result;
    }
#else
    const ssize_t received = ::recv(fd, buffer.data(), capacity, 0);
    if (received < 0) {
        result.status = is_timeout_error(last_socket_error()) ? Status::Timeout : Status::Error;
        return result;
    }
#endif
    if (received == 0 && transport_ == Transport::TCP) {
        result.status = Status::Closed;
        return result;
    }
    buffer.resize(static_cast<std::size_t>(received));
    result.status = Status::Ok;
    result.data = std::move(buffer);
    return result;
}

Status Socket::close() {
    if (fd_ != -1) {
        close_native_socket(native_handle(fd_));
        fd_ = -1;
    }
    connected_ = false;
#ifdef _WIN32
    if (winsock_started_) {
        (void)::WSACleanup();
        winsock_started_ = false;
    }
#endif
    return Status::Ok;
}

bool Socket::openState() const noexcept { return fd_ != -1; }

TcpConnection::TcpConnection() : socket_(std::make_unique<Socket>()) {}
TcpConnection::~TcpConnection() { (void)close(); }
Status TcpConnection::connect(const Endpoint& endpoint) {
    const Status opened = socket_->open(Transport::TCP, endpoint.family);
    return opened == Status::Ok ? socket_->connect(endpoint) : opened;
}
Status TcpConnection::send(const std::vector<std::uint8_t>& data) {
    return socket_->send(data);
}
ReceiveResult TcpConnection::receive(std::size_t max_bytes) {
    return socket_->receive(max_bytes);
}
Status TcpConnection::close() { return socket_->close(); }
} // namespace sleela::network
