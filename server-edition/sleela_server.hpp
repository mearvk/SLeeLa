#ifndef SLEELA_SERVER_HPP
#define SLEELA_SERVER_HPP
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "packet_logger_bridge.hpp"
namespace sleela::server {
enum class Severity { DEBUG, INFO, NOTICE, WARN, ERROR, ALERT };
struct ServerConfig {
 std::string bind_address="0.0.0.0"; std::uint16_t port=19866;
 std::size_t max_connections=256, max_packet_bytes=16*1024*1024, max_header_bytes=64*1024, max_payload_bytes=16*1024*1024;
 std::uint32_t idle_timeout_seconds=60, max_packets_per_second=1000, max_failures_per_minute=60;
 bool require_known_grade=true, reject_malformed=true, log_payload=false;
 Severity minimum_log_level=Severity::INFO;
 std::unordered_set<std::string> allowed_services, denied_services, denied_operations;
 std::string log_file="server-edition/server.log";
};
struct Packet {
 std::uint8_t grade=0; std::uint16_t flags=0; std::uint64_t stream_id=0, request_id=0, sequence=0;
 std::string service_id, operation_id;
 std::unordered_map<std::string,std::string> fields;
 std::vector<std::uint8_t> payload;
};
struct Decision { bool accept=false; int status=400; Severity severity=Severity::WARN; std::string reason; double heuristic_score=0.0; };
class StructuredLogger {
 public: explicit StructuredLogger(const ServerConfig&); void write(Severity,const std::string&,const Packet*,const Decision*,const std::string& detail="");
 private: ServerConfig cfg_; std::mutex mutex_; std::string level_name(Severity) const;
};
class PacketInspector {
 public: explicit PacketInspector(const ServerConfig&); Decision inspect(const Packet&,std::size_t,std::uint64_t,std::uint64_t) const;
 static bool parse_headers(const std::string&,Packet&,std::string&);
 private: ServerConfig cfg_;
};
class SleelaServer {
 public: explicit SleelaServer(ServerConfig config={}); ~SleelaServer();
 bool start(); void run(); void stop(); bool running() const noexcept;
 private:
 ServerConfig config_; StructuredLogger logger_; PacketInspector inspector_; PacketLoggerBridge packet_logger_; int listen_fd_=-1; bool running_=false; std::mutex state_mutex_;
 void client_loop(int,std::string); bool read_exact(int,void*,std::size_t,std::uint32_t);
 bool write_status(int,int,const char*); bool read_packet(int,Packet&,std::size_t&,std::string&); void close_socket(int);
};
}
#endif
