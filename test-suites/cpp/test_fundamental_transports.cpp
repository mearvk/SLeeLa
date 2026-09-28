#include <cassert>
#include <cstdint>
#include "../../impl/fundamental/TcpTransport.hpp"
#include "../../impl/fundamental/UdpTransport.hpp"
int main() {
  sleela::fundamental::TcpTransport tcp;
  sleela::fundamental::UdpTransport udp;
  const std::uint8_t byte = 0;
  assert(!tcp.connected());
  assert(!udp.connected());
  assert(tcp.send(nullptr, 1) < 0);
  assert(tcp.receive(nullptr, 1) < 0);
  assert(udp.send(nullptr, 1) < 0);
  assert(udp.receive(nullptr, 1) < 0);
  return 0;
}
