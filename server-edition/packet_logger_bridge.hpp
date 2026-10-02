#ifndef SLEELA_PACKET_LOGGER_BRIDGE_HPP
#define SLEELA_PACKET_LOGGER_BRIDGE_HPP
#include "../logger/sleela_logger.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
namespace sleela::server {
class PacketLoggerBridge {
public:
 explicit PacketLoggerBridge(const std::string& directory="logs");
 void set_logic(const sleela::LoggerLogic& logic);
 bool open();
 void close();
 bool record_received(const std::string& peer,const std::string& protocol,std::uint64_t stream,std::uint64_t request,std::uint64_t sequence,std::size_t bytes,const std::string& reason="raw_socket_receive");
 bool record_sent(const std::string& peer,const std::string& protocol,std::uint64_t stream,std::uint64_t request,std::uint64_t sequence,std::size_t bytes,const std::string& reason="raw_socket_send");
private:
 sleela::Logger logger_;
};
}
#endif
