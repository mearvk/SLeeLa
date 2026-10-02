#include "packet_logger_bridge.hpp"
namespace sleela::server {
PacketLoggerBridge::PacketLoggerBridge(const std::string& directory):logger_(directory){}
void PacketLoggerBridge::set_logic(const sleela::LoggerLogic& logic){logger_.set_logic(logic);}
bool PacketLoggerBridge::open(){return logger_.open();}
void PacketLoggerBridge::close(){logger_.close();}
bool PacketLoggerBridge::record_received(const std::string& peer,const std::string& protocol,std::uint64_t stream,std::uint64_t request,std::uint64_t sequence,std::size_t bytes,const std::string& reason){
 sleela::PacketRecord r; r.direction=sleela::LogDirection::Received; r.severity=sleela::LogSeverity::Info; r.admission=sleela::LogAdmission::Accepted; r.protocol=protocol; r.source=peer; r.destination="sleela-server"; r.stream_id=std::to_string(stream); r.request_id=std::to_string(request); r.sequence=sequence; r.packet_bytes=bytes; r.reason=reason; return logger_.record(r);
}
bool PacketLoggerBridge::record_sent(const std::string& peer,const std::string& protocol,std::uint64_t stream,std::uint64_t request,std::uint64_t sequence,std::size_t bytes,const std::string& reason){
 sleela::PacketRecord r; r.direction=sleela::LogDirection::Sent; r.severity=sleela::LogSeverity::Info; r.admission=sleela::LogAdmission::Accepted; r.protocol=protocol; r.source="sleela-server"; r.destination=peer; r.stream_id=std::to_string(stream); r.request_id=std::to_string(request); r.sequence=sequence; r.packet_bytes=bytes; r.reason=reason; return logger_.record(r);
}
}
