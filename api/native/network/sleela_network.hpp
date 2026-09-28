#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace sleela::network {
enum class AddressFamily { Any, IPv4, IPv6 };
enum class Transport { UDP, TCP };
enum class Status { Ok, Invalid, Timeout, Closed, Error };
struct Endpoint { std::string host; std::uint16_t port{}; AddressFamily family{AddressFamily::Any}; };
struct ReceiveResult { Status status{Status::Error}; std::vector<std::uint8_t> data; Endpoint peer; };
class Socket {
public:
 Socket(); ~Socket(); Socket(const Socket&)=delete; Socket& operator=(const Socket&)=delete;
 Status open(Transport, AddressFamily=AddressFamily::Any); Status bind(const Endpoint&); Status connect(const Endpoint&);
 Status send(const std::vector<std::uint8_t>&); ReceiveResult receive(std::size_t maxBytes=65536); Status close();
 bool openState() const noexcept;
private: int fd_{-1}; Transport transport_{Transport::UDP}; AddressFamily family_{AddressFamily::Any}; bool connected_{false};
};
class TcpConnection {
public:
 TcpConnection(); ~TcpConnection(); Status connect(const Endpoint&); Status send(const std::vector<std::uint8_t>&);
 ReceiveResult receive(std::size_t maxBytes=65536); Status close();
private: std::unique_ptr<Socket> socket_;
};
}